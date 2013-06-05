/*
 * Copyright (C) 2008 David Bannach, Embedded Systems Lab
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

#ifndef TIMER_H_
#define TIMER_H_

#include "Thread.h"
#include "Condition.h"
#include "Mutex.h"

/**
 * \ingroup core
 * \brief Timer thread.
 *
 * A timer thread which calls a user defined function when fired.
 */
class Timer: private Thread
{
	public:
		class CallbackObj {
			public: virtual void callback(Timer *caller) = 0;
		};

		Timer( struct timeval fireTime, CallbackObj *callbackObject );
		virtual ~Timer();
		
		/**
		 * Set a (new) fire time for the timer. Do not call this method
		 * if the timer is set but not fired yet. It is save to use it in
		 * the callback method.
		 */
		virtual void set( struct timeval fireTime );
		
	private:
		struct timespec fireTime;
		CallbackObj *callbackObject;
		Condition condition;
		Mutex mutex;
		bool timerIsSet;
		
		void run();
		std::string identify();
};

#endif /*TIMER_H_*/
