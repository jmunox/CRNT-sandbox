/*
 * Copyright (C) 2007-2009 David Bannach, Embedded Systems Lab
 * 
 * This file is part of the CRN Toolbox.
 * The CRN Toolbox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * The CRN Toolbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with the CRN Toolbox; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "StreamTask.h"

#include <iostream>
#include <fstream>

using namespace std;

StreamTask::StreamTask( unsigned int inports, unsigned int outports ) : running(false), parent(NULL)
{
	inPortBufferSize= 9999;
	inPortLossless= false;
	inPortSilent = false;
	disabled = false;
	std::string descriptionURL ="";


//	log( "constructor" )
//		<< "\tinports: " << inports 
//		<< ", outports: " << outports << endl;
	
	for( unsigned int i = 0; i < inports; i++ ) {
		inPorts.push_back( new InPort() );
	}
	for( unsigned int i = 0; i < outports; i++ ) {
		outPorts.push_back( new OutPort() );
	}
}


/// Copy constructor.
StreamTask::StreamTask( const StreamTask& s )
{
	unsigned int inports = s.inPorts.size();
	unsigned int outports = s.outPorts.size();

	log( "copy constructor" )
		<< "\tinports: " << inports 
		<< ", outports: " << outports << endl;
	
	for( unsigned int i = 0; i < inports; i++ ) {
		inPorts.push_back( new InPort() );
	}
	for( unsigned int i = 0; i < outports; i++ ) {
		outPorts.push_back( new OutPort() );
	}

	running = false;
}


/**
 * All objects in the in-port and out-port vectors are
 * deleted here.
 */
StreamTask::~StreamTask()
{
	log( "destructor" );

	for( unsigned int i = 0; i < inPorts.size(); i++ ) {
		delete inPorts[i];
	}

	for( unsigned int i = 0; i < outPorts.size(); i++ ) {
		delete outPorts[i];
	}

}

void StreamTask::paramsChanged()
{
	initPorts();
}


void StreamTask::initPorts()
{
	for( unsigned int i = 0; i < inPorts.size(); i++ ) {
		inPorts[i]->setMaxQueueSize( inPortBufferSize );
		inPorts[i]->setLossless( inPortLossless );
		inPorts[i]->setSilent( inPortSilent );
		inPorts[i]->setInPortID( this, i );
	}
	for( unsigned int i = 0; i < outPorts.size(); i++ ) {
		outPorts[i]->setOutPortID( this, i );
	}
}


/**
 * \brief Add in-ports to this task.
 *
 * \param n Number of ports to add.
 * \returns Total number of in-ports.
 */
int StreamTask::addInPorts( unsigned int n )
{
	for( unsigned int i = 0; i < n; i++ ) {
		inPorts.push_back( new InPort() );
	}
	initPorts();
	return inPorts.size();
}


/**
 * \brief Add out-ports to this task.
 *
 * \param n Number of ports to add.
 * \returns Total number of out-ports.
 */
int StreamTask::addOutPorts( unsigned int n )
{
	for( unsigned int i = 0; i < n; i++ ) {
		outPorts.push_back( new OutPort() );
	}
	initPorts();
	return outPorts.size();
}


/**
 * \brief Set number of in-ports.
 *
 * \param n Number of in-ports.
 */
void StreamTask::setInPortNum( unsigned int n )
{
	log( "setting number of in-ports to:" ) << n << endl;
	for( unsigned int i = 0; i < inPorts.size(); i++ ) {
		delete inPorts[i];
	}
	inPorts.clear();

	for( unsigned int i = 0; i < n; i++ ) {
		inPorts.push_back( new InPort() );
	}
	initPorts();
}


/**
 * \brief Set number of out-ports.
 *
 * \param n Number of out-ports.
 */
void StreamTask::setOutPortNum( unsigned int n )
{
	log( "setting number of out-ports to:" ) << n << endl;
	for( unsigned int i = 0; i < outPorts.size(); i++ ) {
		delete outPorts[i];
	}
	outPorts.clear();

	for( unsigned int i = 0; i < n; i++ ) {
		outPorts.push_back( new OutPort() );
	}
	initPorts();
}


/**
 * this method is called by the ToolBox that owns this object.
 * do not call this method anywhere else.
 */
void StreamTask::start()
{
	initPorts();

/*	if( !paramsInitialized ) {
		log( "parameters not initialized yet. cannot start." );
		throw 1;
	}*/
	
	if( disabled ) {
		log( "task is disabled. not starting." );
		return;
	}
	
	if( !running ) {
		running = true;
		init();
		log( "start(): started." );
	}
	else {
		log( "start(): running already." );
	}
}

/**
 * this method is called by the ToolBox that owns this object.
 * do not call this method anywhere else.
 */
void StreamTask::stop()
{
	if( !running ) {
		log( "task is not running" );
		return;
	}
	
	exitNow();
	running = false;
	log( "stop(): stopping..." );
	
	// cancel inports
	for( unsigned int i = 0; i < inPorts.size(); i++ ) {
		inPorts.at(i)->cancel_receive();
	}
	
	//this->cancel();	//maybe overkill, but works fine (on some systems only!)
	//
	// Dave Butenhof:
	// "Ultimately, if you want portability, don't mix pthreads and C++ at all.
	// In particular, though, avoid cancellation and C++ exceptions in any
	// program where you do mix them. It'll work smoothly on a very few
	// platforms, inconsistently and awkwardly on a few others, and not at all
	// on most."
	//
	// David Schwartz:
	// "You cannot use cancellation unless all the code that a thread might be
	// running when it is cancelled knows how to handle being cancelled and
	// sets appropriate modes and handlers. In all probability, your C++
	// standard library does not know how to handle being cancelled, so you
	// must not ever cancel a thread that is or might be running C++ standard
	// library code (unless you know that your C++ standard library is
	// cancellation safe)."
	
	// seems we have to cancel blocking calls manually :-(
	this->cancelAllBlockingCalls();
	
	this->joinMe();
	log( "stop(): done." );
}


const vector<InPort *>& StreamTask::getInPorts()
{
	return inPorts;
}


const vector<OutPort *>& StreamTask::getOutPorts()
{
	return outPorts;
}


string StreamTask::identify()
{
	return getId();
}


bool StreamTask::isStopped()
{
	return !isInitialized();
}

string StreamTask::getDescriptionURL()
{
	return descriptionURL;
}

string StreamTask::readDescription()
{
	if(descriptionURL.length() > 0)
	{
		string description = "";

		ifstream descriptionFile(descriptionURL.c_str());

		string line;

		if(descriptionFile.is_open())
		{
			while(!descriptionFile.eof())
			{
				getline(descriptionFile, line);
				description = description + line;
			}
			descriptionFile.close();
		}
		return description;
	}
	// return the empty description
	else
		return "{}";
}

void StreamTask::setParent(StreamTaskContainer *parent) {
	this->parent = parent;
}
