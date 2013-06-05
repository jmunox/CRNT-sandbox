#include "Mutex.h"

using namespace std;

Mutex Mutex::GLOBAL_MUTEX = Mutex();

Mutex::Mutex()
{
	pthread_mutex_init( &mutex, NULL );
//	log( "initialized." );
}


Mutex::~Mutex()
{
	pthread_mutex_destroy( &mutex );
}


/**
 * Locks this object for the calling thread.
 * This method blocks until no other thread has
 * locked this object.
 * @see See the manpage of "pthread_mutex_lock" for details.
 */
void Mutex::lock()
{
	pthread_mutex_lock( &mutex );
//	log( "locked." );
}



/**
 * Releases the lock for this object.
 * @see See the manpage of "pthread_mutex_unlock" for details.
 */
void  Mutex::unlock()
{
	pthread_mutex_unlock( &mutex );
//	log( "unlocked." );
}


/**
 * Returns a pointer to the POSIX structure of the mutex.
 * Used in the method Condition::wait(Mutex).
 */
pthread_mutex_t* Mutex::getPosixMutex()
{
	return &mutex;
}

