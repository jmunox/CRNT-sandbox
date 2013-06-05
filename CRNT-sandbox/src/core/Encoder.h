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

// Encoder.h

 
#ifndef ENCODER_H
#define ENCODER_H

#include "DataPacket.h"
#include "TBObject.h"


/**
 * \ingroup core
 * \brief Abstract encoder interface.
 *
 * An abstract interface for encoder plugins used by writers (e.g. TCPWriter)
 * for serializing DataPackets to external streams.
 */
class Encoder : public TBObject
{
	public:
		virtual ~Encoder() {};
		
		/// Clone this object.
		virtual Encoder* clone() const = 0;
		
		/// Initialize the Encoder.
		/**
		 * This method must be called before using the Encoder.
		 * @param[in] p Data packet that can be used to initialize the Encoder.
		 */
		virtual void init( const DataPacket *p ) = 0;
		
		/// Get the header bytes.
		/**
		 * Fills the buffer 'buf' with header bytes that must be sent
		 * before any encoded data packets.
		 * @param[out] buf Buffer to be filled with the header bytes.
		 * @param[in] size Size of the buffer.
		 * @returns Length of the header in the buffer.
		 * @note init() must be called first.
		 */
		virtual int get_header( unsigned char *buf, unsigned int size ) = 0;
		
		/// Encode a data packet.
		/**
		 * Writes the encoded data packet 'p' into the buffer 'buf'.
		 * @param[in] p Data packet to be encoded.
		 * @param[out] buf Buffer to be filled with the encoded data packet.
		 * @param[in] size Size of the buffer.
		 * @returns Length of the encoded data packet in the buffer.
		 * @note init() must be called first.
		 */
		virtual int encode( const DataPacket *p, unsigned char *buf, unsigned int size ) = 0;
		
		/// Get the footer bytes.
		/**
		 * Fills the buffer 'buf' with footer bytes that must be sent
		 * after the last encoded data packet.
		 * @param[out] buf Buffer to be filled with the footer bytes.
		 * @param[in] size Size of the buffer.
		 * @returns Length of the footer in the buffer.
		 * @note init() must be called first.
		 */
		virtual int get_footer( unsigned char *buf, unsigned int size ) = 0;
};
	


#endif	//ENCODER_H
