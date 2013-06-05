/*
 * SerialDevice.cpp
 *
 *  Created on: Jan 27, 2012
 *      Author: jmunoza
 */

#include "SerialDevice.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// OAM REVISIT: Added
#include <stropts.h>
#include <sys/ioctl.h>

#include <iostream>

#include <string.h>


using namespace std;



Mutex SerialDevice::mutex;


/**
 * Creates a SerialDevice object without opening it.
 * Call open() before using this object.
 *
 * @param devname Name of the serial device (e.g. /dev/ttyS0).
 * @param speed Speed setting of the serial line. Must be one of the
 * constants defined in termios.h (e.g. B115200). Default is B115200.
 * @param vtime Read timeout (default is 5).
 */
SerialDevice::SerialDevice( const char *devname, speed_t speed, int vtime )
: readonly( false ), devSpeed( speed )
{
	devName = new string( devname );
	read_timeout = vtime;
	valid = false;
	_buf_pos = 0;
	_buf_len = 0;
}


/// Copy constructor.
SerialDevice::SerialDevice( const SerialDevice &d )
{
	readonly = d.readonly;
	devSpeed = d.devSpeed;
	devName = new string( *d.devName );
	read_timeout = d.read_timeout;
	valid = false;
	_buf_pos = d._buf_pos;
	_buf_len = d._buf_len;
}



SerialDevice::~SerialDevice()
{
	if( valid ) {
		std::cout << "closing serial device:"  << *devName << endl;
		::close( fd );
		valid = false;
	}
	delete devName;
}


void SerialDevice::open( bool readonly_ )
{
	if( valid ) {
		std::cout << "ERROR: already open()ed." ;
		return;
	}

	readonly = readonly_;

	mutex.lock();

	struct termios options;

	if( readonly ) {
		fd = ::open( devName->c_str(), O_RDONLY | O_NOCTTY /*| O_NONBLOCK*/ );
	}
	else {
		fd = ::open( devName->c_str(), O_RDWR | O_NOCTTY /*| O_NONBLOCK*/ );
	}

    if (fd == -1) {
    	std::cout << "open(): Cannot open serial device:" << *devName << ", " << fd << ", " << strerror(errno) << endl;
		mutex.unlock();
	    throw IOError( fd, errno );
    }
    else {
    	std::cout << "open(): serial device opened:" << *devName << ", fd=" << fd << endl;
    }

    //clear options struct
    bzero(&options, sizeof(options));

    //baudrate
    cfsetispeed(&options, devSpeed);
    cfsetospeed(&options, devSpeed);

    //c_cflag: 8 data bits, no parity bit, one stop bit, no hardware-based flow control, CLOCAL, CREAD
    options.c_cflag |= (CS8 | CLOCAL | CREAD);

    //c_lflag: raw input mode
    options.c_lflag = 0;

    //c_iflag: raw mode
    options.c_iflag &= ~(IGNBRK | IGNCR | INLCR | ICRNL | IUCLC |
		    IXANY | IXON | IXOFF | INPCK | ISTRIP);
    options.c_iflag |= (BRKINT | IGNPAR);

    //c_oflag raw output
    options.c_oflag = 0;

    //c_cc options
    options.c_cc[VEOF] = 4;
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = read_timeout;

    //set options
    tcsetattr( fd, TCSANOW, &options );

	valid = true;
	mutex.unlock();
}


void SerialDevice::close()
{
	if( !valid ) {
		std::cout << "WARNING: Cannot close. SerialDevice is not open.";
		return;
	}

	mutex.lock();
	::close( fd );
	valid = false;
	mutex.unlock();
}


bool SerialDevice::isOpen()
{
	bool isopen;
	mutex.lock();	//maybe overkill but better be sure
	isopen = valid;
	mutex.unlock();
	return isopen;
}


//char SerialDevice::readChar()
//{
//	char c;
//	int ret = read( fd, &c, 1 );
//	if( ret != 1 ) {
//		throw IOError( ret, errno );
//	}
//	return c;
//}


/**
 * Returns one character from the internal buffer.
 * A read operation on the serial device is only
 * performed if the internal buffer is empty.
 * @return Next character in stream.
 */
char SerialDevice::getChar()
{
	if( _buf_pos < _buf_len ) {
		//buffer is not empty
	}
	else {
		//buffer is empty, try to refill it!
		_buf_pos = 0;
		_buf_len = read( fd, _buf, _BUFSIZE );
		if( _buf_len < 1 ) {
			throw IOError( _buf_len, errno );
		}
	}

	return _buf[_buf_pos++];
}

// OAM REVISIT: Added
/**
 * Check whether data from the serial device is
 * available.
 * @return True=Data available, false=no data.
 */
bool SerialDevice::isChar()
{
	int bytes;

	if( _buf_pos < _buf_len ) {
		// internal buffer is not empty
		return true;
	}
	else {
		// internal buffer is empty, test device
		ioctl( fd,  FIONREAD, &bytes);
	}

	return (bytes?true:false);
}

/**
 * Clear serial read queue by reading all pending
 * characters until serial port is empty.
 *
 * @param[in] timeout, optional time [sec] to wait for silence
 * @return True=No data for some time, false=still data coming in
 */
bool SerialDevice::consumeChars(int timeout)
{
	bool done = false;
	time_t starttime = time(NULL);

	if ( !isOpen() ) return false;

	while ( !done ) {
		while ( isChar() ) {
			getChar();

			// check if timed out
			if ( difftime(time(NULL), starttime) > timeout ) {
				done = true;
				break;
			}
		}

		if (!done) {
			usleep( 200*1000 ); // wait for 200ms
			done = ~isChar();
		}
	}

	return done;
}


/**
 * Reads \p size bytes from the serial device.
 * The internal buffer is read first before performing
 * any read operation on the serial device.
 *
 * @param[out] buf The buffer for the received bytes. Must be at least
 * \p size bytes long.
 * @param[in] size Number of bytes to read.
 * @return Actual number of bytes read.
 */
int SerialDevice::readBuf( unsigned char *buf, int size )
{
	int pos = 0;
	int ret = 0;

	//get bytes from internal buffer first
	if( _buf_pos < _buf_len ) {
		pos = _buf_len - _buf_pos;
		if( size < pos ) {
			pos = size;
		}
		memcpy( buf, &_buf[_buf_pos], pos );
		_buf_pos += pos;
	}
	std::cout << "Entering while. " << std::endl;
	while( pos < size ) {
		ret = read( fd, &buf[pos], size - pos );
		if( ret > 0 ) {
			pos += ret;
		}
		else {
			throw IOError( ret, errno );
		}
	}

	return pos;
}


/**
 * Reads all characters until a newline character, but
 * maximal \p size-1 bytes. The newline character will be
 * omitted. A string termination character (0x0) will
 * be inserted after the last character.
 * The internal buffer is used to reduce read operations on
 * the serial device.
 *
 * @param[out] buf The buffer for the received characters.
 * Must be at least \p size bytes long.
 * @param[in] size Number of bytes to read.
 * @param[in] sep Line separator character.
 * @return Actual number of bytes read.
 */
int SerialDevice::readLine( unsigned char *buf, int size, char sep )
{
	int pos = 0;
	unsigned char ch = 0;

	while( (pos < size-1) && ((ch = getChar()) != sep) ) {
		buf[pos++] = ch;
	}
	buf[pos] = '\0';

	return pos;
}


/**
 * Writes \p size bytes to the serial device.
 *
 * @param[in] buf The buffer to be sent. Must be at least \p size bytes long.
 * @param[in] size Number of bytes to write.
 * @return Actual number of bytes written.
 */
int SerialDevice::writeBuf( const unsigned char *buf, int size )
{
	int pos = 0;
	int ret = 0;

	while( !readonly && (pos < size) ) {
		ret = write( fd, &buf[0], size - pos );
		if( ret > 0 ) {
			pos += ret;
		}
		else {
			throw IOError( ret, errno );
		}
	}

	return pos;
}




