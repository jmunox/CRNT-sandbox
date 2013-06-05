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

// OutPort.cpp
//

#include "OutPort.h"
#include "StreamTask.h"
#include <stdio.h>

using namespace std;


OutPort::OutPort()
: owner(NULL)
{
	receivers.clear();
}


OutPort::OutPort( const OutPort& p )
: receivers(p.receivers), owner(NULL)
{
}


OutPort::~OutPort()
{
}

int OutPort::getOutPortID() const { return outPortID; }
void OutPort::setOutPortID( StreamTask *t, int i )
{
	owner = t;
	if(t) {
		char num[16];
		sprintf( num, "%d", i );
		//TODO
		//id = owner->getId() + ":out" + num;
	}
	outPortID = i;
}


/**
 * Sends the data packet to all connected in-ports.
 * The packet will be cloned if more than one in-port
 * is connected such that every task will get separate copy.
 * 
 * @note The calling thread must not access the DataPacket p
 * anymore after calling this method. Therefore the pointer
 * p should be set to NULL immediately after the call.
 *
 * @param p The data packet to send.
 */
void OutPort::send( DataPacket *p )
{
	if( receivers.empty() ) {
		log( "send(): no receivers registered, discarding packet:" )
			<< "[outport " << outPortID << "]" << endl
			<< p << endl;
		delete p;
	}
	else {
		//process receivers backwards
		for( unsigned int i = receivers.size()-1; i > 0; i-- ) {
			receivers[i]->enqueue( p->clone() );
		}
		//first receiver gets original packet
		//--> no clone necessary if only one receiver
		receivers[0]->enqueue( p );
		p = NULL;
	}
}


/**
 * Connects an in-port to this object.
 * It's not secure to use this method after the toolbox
 * is started.
 *
 * @param port The in-port to connect.
 */
void OutPort::connect( InPort *port )
{
	//check if connection already exists
	for(unsigned i = 0; i < receivers.size(); i++) {
		if( receivers.at(i) == port ) {
			log( "WARNING: skipped adding identical connection." );
			return;
		}
	}
	
	receivers.push_back( port );
}


/**
 * Disonnects an in-port from this object.
 * It's not secure to use this method after the toolbox
 * is started.
 *
 * @param port The in-port to disconnect.
 */
void OutPort::disconnect( InPort *port )
{
	vector<InPort *>::iterator iter;
	for( iter = receivers.begin(); iter != receivers.end(); iter ++ ) {
		if( *iter == port ) {
			receivers.erase( iter );
			break;
		}
	}
}


