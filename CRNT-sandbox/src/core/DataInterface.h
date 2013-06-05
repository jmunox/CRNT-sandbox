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

// DataInterface.h

 
#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include "TBObject.h"
#include "DataPacket.h"


/**
 * \ingroup core
 * \brief Base class for data interfaces.
 *
 * An abstract base class for DataPacket interfaces. A DataInterface
 * allows to access the content of a DataPacket with getter/setter methods.
 * It defines names for fields/chanels of the data packet.
 */
class DataInterface : public TBObject
{
	//GIBN_MAKE_AVAILABLE_ABSTRACT( DataInterface, TBObject )
	public:
		virtual ~DataInterface();
		
		/**
		 * \brief Get the Json description of a data packet.
		 * \param p The data packet to convert.
		 * \returns Json description of the data packet p.
		 */
		//virtual Json::Value toJson( const DataPacket* p ) = 0;
};
	


#endif	//DATAINTERFACE_H
