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

// Implementation of the Socket class.


#include "Socket.h"
#include "SocketException.h"
#include <string.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>



Socket::Socket() : m_sock ( -1 ), mutex(), condition()
{
	_buf_pos = 0;
	_buf_len = 0;
	memset( &m_addr, 0, sizeof( m_addr ) );
}


Socket::Socket( const Socket &s ) : m_sock ( -1 ), mutex(), condition()
{
	_buf_pos = 0;
	_buf_len = 0;
	memset( &m_addr, 0, sizeof( m_addr ) );
}


Socket::~Socket()
{
	if( is_valid() ) {
		::close( m_sock );
	}
}

bool Socket::setOptions()
{
	// TIME_WAIT - argh
	int on = 1;
	if( setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof( on ) ) == -1 ) {
		return false;
	}
	
	return true;
}

bool Socket::create()
{
	m_sock = socket( AF_INET, SOCK_STREAM, 0 );

	if( !is_valid() ) {
		return false;
	}
		
	return setOptions();
}



bool Socket::bind( const int port )
{
	if( !is_valid() ) {
		return false;
	}
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons( port );
	
	int bind_return = ::bind( m_sock, (struct sockaddr *)&m_addr, sizeof( m_addr ) );
	if( bind_return == -1 ) {
		return false;
	}
	
	return true;
}

unsigned short Socket::getPortNumber()
{
	if( !is_valid() ) {
		return 0;
	}
	
	struct sockaddr_in sa;

	int iLen = sizeof(sa);

	// When the socket is bound to the passed port "0", it actually is bound to any free port.
	// However, this is done asynchronously, presumably in bind(), so it might happend that a calling sequence like ...
	// 	- Socket::create();
	// 	- Socket::bind(0);
	//	- Socket::getPortNumber();
	// ... returns a wrong actual port number. Due to that, we have to wait for getsockname to return a valid result.
	while (getsockname(m_sock, (struct sockaddr *)&sa, (socklen_t*)&iLen) == -1)
	{
		perror("Socket::getPortNumber(): getting port failed. retrying..");
		sleep(1);
	}

	return ntohs(sa.sin_port);
}

bool Socket::listen() const
{
	if( !is_valid() ) {
		return false;
	}
	
	int listen_return = ::listen( m_sock, MAXCONNECTIONS );
	
	if( listen_return == -1 ) {
		return false;
	}
	
	return true;
}


bool Socket::accept( Socket& new_socket ) const
{
	int addr_length = sizeof( m_addr );
	new_socket.m_sock = ::accept( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );
	new_socket.setOptions();
	
	if ( new_socket.m_sock <= 0 ) {
		return false;
	}
	else {
		return true;
	}
}


int Socket::close() {
	//reset internal buffer
	_buf_pos = 0;
	_buf_len = 0;
	
	if( is_valid() ) {
		return ::close( m_sock );
	}
	else {
		return -1;
	}
}


bool Socket::send( const std::string s ) const
{
	int status = ::send( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
	if ( status == -1 ) {
		return false;
	}
	else {
		return true;
	}
}


void Socket::send( const unsigned char *buf, unsigned int len ) const
{
	int status = ::send( m_sock, buf, len, MSG_NOSIGNAL );
	if ( status == -1 ) {
		throw SocketException( "Could not write to socket." );
	}
}


/**
 * @warning This is an unbuffered access method. It does not use
 * the internal buffer and, therefore, it may not be used together
 * with the buffered access methods.
 */
int Socket::recv( unsigned char *buf, unsigned int len ) const
{
	int numBytes = ::recv( m_sock, buf, len, 0 );
	
	if( numBytes >= 0 ) {
		return numBytes;
	}
	else {
		std::cout << "Socket::recv(): ERROR, received " << numBytes << " bytes.  errno= " << errno << std::endl;
		return 0;
	}
}



/**
 * @warning This is an unbuffered access method. It does not use
 * the internal buffer and, therefore, it may not be used together
 * with the buffered access methods.
 */
int Socket::recv( std::string& s ) const
{
	char buf[ MAXRECV + 1 ];
	
	s = "";
	
	memset( buf, 0, MAXRECV + 1 );
	
	int status = ::recv( m_sock, buf, MAXRECV, 0 );
	
	if( status == -1 ) {
		std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
		return 0;
	}
	else if( status == 0 ) {
		return 0;
	}
	else {
		s = buf;
		return status;
	}
}



bool Socket::connect( const std::string host, const int port )
{
	if( !is_valid() ) {
		return false;
	}
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons( port );
	
	int status = inet_pton( AF_INET, host.c_str(), &m_addr.sin_addr );
	
	if( errno == EAFNOSUPPORT ) {
		return false;
	}
	
	status = ::connect( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );
	
	if( status == 0 ) {
		return true;
	}
	else {
		return false;
	}
}


void Socket::setTcpNoDelay( bool flag )
{
	int ret = setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, &flag, 4 ); 
}


void Socket::set_non_blocking( const bool b )
{
	int opts;
	
	opts = fcntl( m_sock, F_GETFL );
	
	if ( opts < 0 ) {
		return;
	}
	
	if( b ) {
		opts = ( opts | O_NONBLOCK );
	}
	else {
		opts = ( opts & ~O_NONBLOCK );
	}
	
	fcntl( m_sock, F_SETFL,opts );
}



/**
 * Returns one character from the internal buffer.
 * A read operation on the socket is only
 * performed if the internal buffer is empty.
 * @return Next character in stream.
 */
inline char Socket::getChar()
{
	if( _buf_pos < _buf_len ) {
		//buffer is not empty
	}
	else {
		//buffer is empty, try to refill it!
		_buf_pos = 0;
		_buf_len = ::recv( m_sock, _buf, BUFSIZE1, 0 );
		if( _buf_len < 1 ) {
			//std::cerr << "Socket::getChar(): ERROR: " << errno << std::endl;
			throw SocketException("error while reading from socket");
		}
	}

	return _buf[_buf_pos++];
}


/**
 * Reads \p size bytes from the socket.
 * The internal buffer is read first before performing
 * any read operation on the socket itself.
 *
 * @param[out] buf The buffer for the received bytes. Must be at least
 * \p size bytes long.
 * @param[in] size Number of bytes to read.
 * @return Actual number of bytes read.
 */
int Socket::readBuf( unsigned char *buf, int size )
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
	
	while( pos < size ) {
		ret = ::recv( m_sock, &buf[pos], size - pos, 0 );
		if( ret > 0 ) {
			pos += ret;
		}
		else {
			//std::cerr << "Socket::readBuf(): ERROR: " << errno << std::endl;
			throw SocketException("error while receiving");
		}
	}
	
	return pos;
}


/**
 * Reads all characters until the separator character \p sep, but
 * maximal \p size-1 bytes. The separator character can be
 * omitted (default). A string termination character (0x0) will
 * be inserted after the last character.
 * The internal buffer is used to reduce read operations on
 * the socket.
 *
 * @param[out] buf The buffer for the received characters.
 * Must be at least \p size bytes long.
 * @param[in] size Number of bytes to read.
 * @param[in] sep Line separator character.
 * @param[in] keepSep Include separator character if \true (default = \a false).
 * @return Actual number of bytes read.
 */
int Socket::readLine( unsigned char *buf, int size, char sep, bool keepSep )
{
	int pos = 0;
	unsigned char ch = 0;
	
	while( (pos < size-1) && ((ch = getChar()) != sep) ) {
		buf[pos++] = ch;
	}
	if( keepSep && (ch == sep) ) {
		buf[pos++] = ch;
	}
	buf[pos] = '\0';
	
	return pos;
}


