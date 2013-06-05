/*
 * Copyright (C) 2007 David Bannach, Embedded Systems Lab
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


#include "InPort.h"
#include "StreamTask.h"
#include <stdio.h>

using namespace std;



InPort::InPort() : mutex(), condition(), owner(NULL)
{
	maxQueueSize = 0; //infinite queue size
	lossless = false;
	droppedPackets = 0;
}


/**
 * Copy constructor.
 * @note The packet queue is not copied, i.e. it will be empty.
 */
InPort::InPort( const InPort& p )
{
	maxQueueSize = p.maxQueueSize;
	lossless = p.lossless;
	droppedPackets = p.droppedPackets;
	inPortID = -1;
	owner = NULL;
	// OAM REVISIT
	errQueueCounter = 0;
}


InPort::~InPort()
{
}

int InPort::getInPortID() const { return inPortID; }
unsigned int InPort::getMaxQueueSize() const { return maxQueueSize; }

void InPort::setInPortID( StreamTask *t, int i )
{
	owner = t;
	if(t) {
		char num[16];
		sprintf( num, "%d", i );
		//TODO
		//id = owner->getId() + ":in" + num;
	}
	inPortID = i;
}

void InPort::setMaxQueueSize( unsigned int size )
{
	mutex.lock();
	maxQueueSize = size;
	mutex.unlock();
}

void InPort::setSilent(bool silent)
{
	mutex.lock();
	this->silent = silent;
	mutex.unlock();
}

void InPort::setLossless( bool flag )
{
	mutex.lock();
	lossless = flag;
	mutex.unlock();
}


bool InPort::notEmpty()
{
	return !packetQueue.empty();
}


bool InPort::isEmpty()
{
	return packetQueue.empty();
}


DataPacket* InPort::receive( long timeout )
{
	mutex.lock();
	DataPacket *p = NULL;
	try{
		if( timeout > 0 && packetQueue.empty() ) {
			
			//create absolute timeout from relative time
			struct timeval tv;
			struct timespec ts;
			gettimeofday( &tv, NULL );
			ts.tv_sec = tv.tv_sec + timeout / 1000;;
			ts.tv_nsec = tv.tv_usec * 1000 + (timeout % 1000) * 1000000;
			if( ts.tv_nsec >= 1000000000 ) {
				ts.tv_sec++;
				ts.tv_nsec %= 1000000000;
			}
			
			condition.wait( &mutex, &ts );
		}
		else {
			while( packetQueue.empty() ) {
				condition.wait( &mutex );	//mutex will be unlocked while waiting
			}
		}
		
		if( !packetQueue.empty() ) {
			p = packetQueue.front();
			packetQueue.pop();
		}
	}
	catch( char const* msg ) {
		mutex.unlock();
		throw msg;
	}
	condition.signal();
	mutex.unlock();
	return p;
}

void InPort::cancel_receive()
{
	log("canceling..");
	mutex.lock();
	condition.cancel();
	mutex.unlock();
	log("canceled.")
		<< "\tpackets left in queue: " << packetQueue.size() << endl
		<< "\tdropped packets since start: " << droppedPackets << endl;
}


void InPort::enqueue( DataPacket *p )
{
	mutex.lock();
	
	if( lossless && maxQueueSize > 0 ) {
		while( !(packetQueue.size() < maxQueueSize) ) {
			condition.wait(&mutex);
		}
	}
	
	if( maxQueueSize == 0 || packetQueue.size() < maxQueueSize ) {
		packetQueue.push( p );
		// OAM REVISIT
		errQueueCounter = 0;
	}
	else {
		//discard packet
		if (!silent && !errQueueCounter) {
			log( "maximal queue size reached" );
		}
		else {
			if (!silent && errQueueCounter % maxQueueSize == 0) {
				log("Maximal queue size reached.") 
					<< "  packets discarded: " << errQueueCounter << endl << endl;
			}
		}
		
		// OAM REVISIT			
		errQueueCounter++;

// mk			
//			<< "[inport " << inPortID << "]"
//			<< ", queuesize= " << maxQueueSize
//			<< ", discarding packet:" << endl
//			<< p << endl;
			
		delete p;
		droppedPackets++;
	}
	condition.signal();
	mutex.unlock();
}



