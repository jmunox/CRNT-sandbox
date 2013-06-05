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

#ifndef THREAD_H
#define THREAD_H

extern "C" {
#include <pthread.h>
#include <semaphore.h>
}

#include "TBObject.h"


/**
 * \ingroup core
 * \brief Wrapper class for POSIX threads.
 *
 * An abstract wrapper class for POSIX threads.
 * Subclasses must implement the run() method to
 * be non-abstract.
 */
class Thread : public TBObject
{
	public:
		Thread();
		virtual ~Thread();

		/// Initializes and starts the thread.
		virtual void init();
  
		/// Is initialized, ie. became a proper thread.
		bool isInitialized() { return _initialized; }

		/// Exit the thread.
		virtual void exitNow() { _exit = true; }
		bool exiting() { return _exit; }
		
		virtual void cancel();

		/// The thread's main method.
		/** Subclasses must override this method. */
		virtual void run() = 0;
		
		/**
		 * Used to get the identification of a Thread object.
		 * \return identification string
		 */
		virtual std::string identify() = 0;

		/// Blocks until the thread has finished.
		int joinMe();

	private:
		pthread_t thread;

		bool _initialized;
		bool _exit;

		static void* __starter(void* th);
};


#endif // THREAD_H
