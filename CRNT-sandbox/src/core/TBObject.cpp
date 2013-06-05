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

#include <typeinfo>
#include "TBObject.h"
#include "Mutex.h"

using namespace std;


TBObject::TBObject()
{
}

TBObject::~TBObject()
{
}


string TBObject::getId() const { return id; }
void TBObject::setId( string id ) { this->id = id; paramsChanged(); }

string TBObject::getType() const
{
	string crypticName = typeid(*this).name();
	size_t startPos = crypticName.find_first_not_of("0123456789");
	return crypticName.substr(startPos);
}


/**
 * Writes the Message msg to the standard error output.
 * A reference to the used output-stream is returned.
 * @param msg The log message.
 * @return The ostream used for the output.
 */
ostream& TBObject::log( const char *msg ) const
{
	time_t rawtime;
	time( &rawtime );
	char timestring[20];
	
	//need mutex because localtime() is not thread-safe
	//also nice to sync log messages (at least the first line)
	Mutex::GLOBAL_MUTEX.lock();
		struct tm *timeinfo = localtime( &rawtime );
		strftime( timestring, 20, "%b %d %X", timeinfo );
		clog << timestring << " [" << id << "] ::: "
			 << this->getType()
			 << " @ " << this << ": " << msg << endl;
	Mutex::GLOBAL_MUTEX.unlock();
	
	return clog;
}

