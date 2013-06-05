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

#include "Timer.h"


Timer::Timer( struct timeval fireTimeVal, CallbackObj *callbackObject )
: callbackObject(callbackObject)
{
	fireTime.tv_sec = fireTimeVal.tv_sec;
	fireTime.tv_nsec = 1000 * fireTimeVal.tv_usec;
	timerIsSet = true;
	
	init();
}

Timer::~Timer() {}

std::string Timer::identify()
{
	return "a_timer_thread";
}

void Timer::set( struct timeval fireTimeVal )
{
	mutex.lock();
	fireTime.tv_sec = fireTimeVal.tv_sec;
	fireTime.tv_nsec = 1000 * fireTimeVal.tv_usec;
	timerIsSet = true;
	mutex.unlock();
}

void Timer::run()
{
	while( timerIsSet ) {
		mutex.lock();
		condition.wait( &mutex, &fireTime );
		timerIsSet = false;
		mutex.unlock();
		callbackObject->callback(this);
	}
}

