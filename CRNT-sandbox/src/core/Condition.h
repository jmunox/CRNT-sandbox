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

 
#ifndef CONDITION_H
#define CONDITION_H

#include "TBObject.h"
#include "Mutex.h"
#include <pthread.h>


/**
 * \ingroup core
 * \brief Wrapper for POSIX conditions.
 *
 * A wrapper class for POSIX conditions.
 * Used in conjunction with Mutex objects
 * to synchronize threads.
 */
class Condition : TBObject
{
	public:
		Condition();
		~Condition();
		
		/**
		 * \brief Wait for the condition to be signalized.
		 * 
		 * Lets a thread wait until this condition is signal()ed.
		 * The Mutex m must be locked before calling wait().
		 * It will be unlocked automatically before setting the
		 * thread to sleep state and it will be re-locked before
		 * returning from this method.
		 * If \p timeout is specified this method will not block
		 * longer than \p timeout (absolute time).
		 *
		 * \param timeout Absolute timestamp.
		 * \see See manpage for "pthread_cond_wait" for details.
		 */
		void wait( Mutex *m, const struct timespec *timeout = NULL );
	
		/**
		 * \brief Signalize the condition.
		 * 
		 * Awakens a thread waiting for this condition in the
		 * wait() method.
		 * For proper operation the Mutex used in the corresponding
		 * wait() method must be unlocked after calling signal().
		 * This ipmlies that the Mutex must be locked before the call.
		 * 
		 * \see See manpage for "pthread_cond_signal" for details.
		 */
		void signal();

		/**
		 * \brief Cancel a wait().
		 * 
		 * The thread that is blocked in the wait() method will
		 * return with the exception "condition canceled".
		 */
		void cancel();

	private:
		bool canceling;
		pthread_cond_t condition;
};



#endif	//CONDITION_H
