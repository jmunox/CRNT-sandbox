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

// OutPort.h - The Input-Port Object
// 
 
#ifndef OUTPORT_H
#define OUTPORT_H

#include "TBObject.h"
#include "InPort.h"
#include "DataPacket.h"

#include <vector>

//StreamTask-dummy
class StreamTask;


/**
 * \ingroup core
 * \brief Out-Port for stream tasks.
 *
 * The output port for StreamTask objects.
 * InPort objects can be connected to it.
 */
class OutPort: public TBObject
{
	public:
		OutPort();
		OutPort( const OutPort& p );
		virtual ~OutPort();
		
		virtual void setOutPortID( StreamTask *t, int i );		///< Set the outport-id.
		virtual int getOutPortID() const;		///< Get the outport-id.

		/// Send packet to connected in-ports.
		virtual void send( DataPacket *p );
		
		/// Connect an in-port.
		virtual void connect( InPort *port );
		
		/// Disconnect an in-port.
		virtual void disconnect( InPort *port );
	
	protected:
		std::vector<InPort *> receivers;
		StreamTask *owner;
		int outPortID;
};
	


#endif	//OUTPORT_H
