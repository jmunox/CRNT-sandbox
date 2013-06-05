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

// Definition of the Socket class

#ifndef SOCKET_H
#define SOCKET_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

#include "Mutex.h"
#include "Condition.h"


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;


/**
 * \ingroup core
 * \brief Wrapper for TCP sockets.
 *
 */
class Socket
{
	static const int BUFSIZE1 = 50;

	public:
		Socket();
		Socket( const Socket &s );
		virtual ~Socket();
		
		// Server initialization
		bool create();
		bool bind( const int port );
		bool listen() const;
		bool accept( Socket& ) const;
		
		int close();
		
		// Client initialization
		bool connect( const std::string host, const int port );
		
		// Data Transimission
		bool send( const std::string ) const;
		void send( const unsigned char *buf, unsigned int len ) const;
		int recv( std::string& ) const;
		int recv( unsigned char *buf, unsigned int len ) const;
		
		// buffered read access
		char getChar();
		int readBuf( unsigned char *buf, int size );
		int readLine( unsigned char *buf, int size, char sep, bool keepSep = false );
		
		/**
		 * Set the TCP_NODELAY flag for reducing delay.
		 * \param flag TCP delay will be reduced when set to true.
		 */
		void setTcpNoDelay( bool flag );

		void set_non_blocking( const bool );
		
		bool is_valid() const { return m_sock != -1; }

		/**
		 * \brief This method gets the port to which the socket is bound to.
		 * 
		 * This method determines to which port the socket is actually bound to.
		 * As a socket will be bound to any free port when calling Socket::bind
		 * with parameter int port = 0, you have to call this method in order
		 * to get the actual port number to which the socket is bound to.
		 * In case the socket was bound to a fixed port, this method does not
 		 * yield any advantage over reusing the constant denoting the fixed
		 * 	port number.
		 */
		unsigned short getPortNumber();
		
	private:
		int m_sock;
		sockaddr_in m_addr;
		unsigned char _buf[BUFSIZE1];	///< Internal buffer.
		int _buf_pos;					///< Current position in internal buffer.
		int _buf_len;					///< Length of internal buffer.
		Mutex mutex;
		Condition condition;
		
		bool setOptions();
		
};


#endif //SOCKET_H
