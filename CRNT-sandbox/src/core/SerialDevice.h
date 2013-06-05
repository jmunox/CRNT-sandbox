/*
 * SerialDevice.h
 *
 *  Created on: Jan 27, 2012
 *      Author: jmunoza
 */

#ifndef SERIALDEVICE_H_
#define SERIALDEVICE_H_

#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "Mutex.h"

using namespace std;




//#define _BUFSIZE 50


/**
 * \ingroup core
 * \brief I/O error.
 *
 */
class IOError
{
	public:
		IOError( int ret, int err ) { retVal = ret; errNo = err; }

		int retVal;
		int errNo;
};


class SerialDevice {
	static const int _BUFSIZE = 50;

public:
	SerialDevice( const char *devname, speed_t speed_t = B115200, int vtime = 5 );
	SerialDevice(const SerialDevice &d);
	virtual ~SerialDevice();



		/// Open the serial device;
		virtual void open( bool readonly = false );

		/// Close the serial device;
		virtual void close();

		/// Read one char.
		virtual char getChar(); //buffered access

		/// Check if char available
		virtual bool isChar();

		// Clear serial read queue by reading all pending
		bool consumeChars(int timeout=2);

		/// Read a buffer of chars.
		virtual int readBuf( unsigned char *buf, int size );

		/// Read one line of chars.
		virtual int readLine( unsigned char *buf, int size, char sep = '\n' );

		/// Write a buffer of chars.
		virtual int writeBuf( const unsigned char *buf, int size );

		/// Check if serial device is opened
		virtual bool isOpen();

	protected:
		static Mutex mutex;
		bool valid;
		bool readonly;
		std::string *devName;
		speed_t devSpeed;
		int fd;							///< File descriptor.
		int read_timeout;				///< Serial port read timeout in seconds.
		unsigned char _buf[_BUFSIZE];	///< Internal buffer.
		int _buf_pos;					///< Current position in internal buffer.
		int _buf_len;					///< Length of internal buffer.

};

//}

#endif /* SerialDevice_H_ */
