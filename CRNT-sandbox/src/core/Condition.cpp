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

 
#include "Condition.h"


Condition::Condition() : canceling(false)
{
	if( pthread_cond_init( &condition, NULL ) == 0 ) {
		//log( "Condition initialized." );
	}
	else {
		log( "ERROR: condition variable not initialized properly." );
	}
}


Condition::~Condition()
{
}


void Condition::wait( Mutex *m, const struct timespec *timeout )
{
	if( timeout ) {
		int ret = pthread_cond_timedwait( &condition, m->getPosixMutex(), timeout );
	}
	else {
		pthread_cond_wait( &condition, m->getPosixMutex() );
	}
	
	if( canceling ) {
		canceling = false;
		throw "condition canceled";
	}
}


void  Condition::signal()
{
	pthread_cond_signal( &condition );
}


void Condition::cancel()
{
	canceling = true;
	signal();
}
