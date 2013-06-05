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

#ifndef TBOBJECT_H
#define TBOBJECT_H

#include <stdlib.h>
#include <string>
#include <iostream>

/**
 * \ingroup core
 * \brief Base class for all Toolbox objects.
 *
 * This class is the base class for all Toolbox objects.
 */
class TBObject
{

	public:
		TBObject();
		virtual ~TBObject();
		virtual void setId( std::string id );	///< Set the id string.
		virtual std::string getId() const;		///< Get the id string.
		virtual std::string getType() const;		///< Get the name of the actual type.
//		void setParams( Json::Value &jval ) { GIBN::setParams( jval ); }

	protected:
		virtual std::ostream& log( const char *msg ) const;
		//CRITICAL TODO
		std::string id;
		void paramsChanged(){return;};
};
	


#endif	//TBOBJECT_H
