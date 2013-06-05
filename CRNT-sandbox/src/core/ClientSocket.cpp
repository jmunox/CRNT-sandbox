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

// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"


ClientSocket::ClientSocket( std::string host, int port )
{
	if( !Socket::create() ) {
		throw SocketException( "Could not create client socket." );
	}
	
	if( !Socket::connect( host, port ) ) {
		throw SocketException( "Could not bind to port." );
	}
}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
	if( !Socket::send( s ) ) {
		throw SocketException( "Could not write to socket." );
	}
	
	return *this;
}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
	if( !Socket::recv ( s ) ) {
		throw SocketException( "Could not read from socket." );
	}
	
	return *this;
}
