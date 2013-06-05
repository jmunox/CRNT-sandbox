/*
 * Copyright (C) 2007-2010 David Bannach, Embedded Systems Lab
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


#include "Thread.h"
#include "StreamTask.h"
#include "SocketException.h"
#include <iostream>
#include <string.h>
#include <exception>

using namespace std;


void* Thread::__starter(void* th) {
	Thread *thread = static_cast<Thread *>(th);

	try{
		thread->run();
	}
	catch( char const* msg ) {
		cerr << "::: ERROR: the thread '" << thread->getId()
			 << "' did not catch the exception \""
			 << msg << "\"" << endl;
		if( strcmp( msg, "condition canceled" ) == 0 ) {
			cerr << "::: The 'Thread' class is catching it for you"
				 << " but you might fail restarting this task." << endl;
			cerr << "::: It is recommended to catch this exception"
				 << " directly in the class '" << thread->getType() << "'." << endl;
		}
		else {
			throw msg;
			thread->_initialized = false;
			pthread_exit(0);
		}
	}
	catch( SocketException &e ) {
		cerr << "::: ERROR: the thread '" << thread->getId()
			 << "' did not catch the socket exception \""
			 << e.description() << "\"" << endl;
		
		thread->_initialized = false;
		pthread_exit(0);
	}
	catch( exception &e ) {
		cerr << "::: ERROR: an exception of type '" << e.what() << "' occured in the thread '"
			 << thread->getId() << "'" << endl;
		thread->_initialized = false;
		pthread_exit(0);
	}
	catch( ... ) {
		if (!thread->isInitialized()) {
			// Assuming thread cancellation - must rethrow.
			cerr << "::: The thread '" << thread->getId() << "' has been canceled." << endl;
			throw;
		}
		else {
			cerr << "::: ERROR: an unknown exception occured in the thread '"
				<< thread->getId() << "'" << endl;
		}
	}
	
	thread->_initialized = false;
	pthread_exit(0);
	return 0;
}


Thread::Thread() :
	_initialized(false), _exit(false)
{
}

Thread::~Thread()
{
	if( _initialized ) {
		pthread_join(this->thread, NULL);
	}
}

void Thread::init()
{
	if( _initialized ) {
		cerr << "Tread already initialized." << endl;
		return;
	}
	if( !pthread_create( &(this->thread), NULL, Thread::__starter, static_cast<void*>(this))) {
		//cerr << "Thread created." << endl;
		_initialized = true;
	}
	else {
		cerr << "Thread creation failed." << endl;
	}

}

int Thread::joinMe()
{
	if( _initialized ) {
		_initialized = false;
		void *v;
		return pthread_join(thread, &v);
	}
	else {
		return 0;
	}
}

void Thread::cancel()
{
	if( _initialized ) {
		_initialized = false;
		pthread_cancel( thread );
	}
}

