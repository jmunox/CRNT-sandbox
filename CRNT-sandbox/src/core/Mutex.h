/*
 * MUTEX_H_
 */

//Unit Test for Mutex.h - a wrapper for posix-thread mutex
//

#ifndef MUTEX_H_
#define MUTEX_H_

using namespace std;


#include <pthread.h>


/**
 * Mutex_H_
 * \ingroup core
 * \brief Wrapper for POSIX mutex.
 *
 * A wrapper class for the POSIX mutex.
 * Used for ensuring mutual exclusion on
 * shared data.
 */
class Mutex
{
	public:
	Mutex();
		~Mutex();

		void lock();						///< Lock this object.
		void unlock();						///< Unlock this object.
		pthread_mutex_t* getPosixMutex();	///< Get the POSIX structure.

		static Mutex GLOBAL_MUTEX;

	private:
		pthread_mutex_t mutex;
};



#endif	//Mutex_H_
