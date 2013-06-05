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

#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "TBObject.h"
#include "Value.h"
#include "Mutex.h"
#include "OutOfMemoryException.h"
//TODO
/*
#include "../jsoncpp/json_reader.h"
#include "../jsoncpp/json_value.h"
#include "../jsoncpp/json_writer.h"

*/

#include <vector>
#include <sys/time.h>



typedef std::vector<Value*> ValueVect; // mk::2oo6o6o6



/**
 * \ingroup core
 * \brief Data packet.
 * 
 * These are the data packets that are passed between StreamTasks. Each
 * data packet contains a data vector (the channels), a time stamp, and
 * a sequence number.
 * 
 * The channels in the payload may be accessed by the getChannel()
 * and setChannel() methods
 */
class DataPacket: public TBObject
{
	public:
		DataPacket( int streamId = -1 );
		DataPacket( const DataPacket &p );
		DataPacket( DataPacket &p, std::vector<unsigned> channels );
		virtual ~DataPacket();

		/**
		 * @brief Time of creation.
		 */
		struct timeval timestamp;
		
		/**
		 * @brief Sequence number.
		 */
		unsigned long long seqNr;
		
		/**
		 * @brief Denoting end of stream.
		 */
		bool endOfStream;
		
		/**
		 * @brief Payload of the packet. The "channels".
		 */
		std::vector< Value* > dataVector;
		
		/** 
		 * @brief Payload of a "super packet".
		 */
		std::vector< DataPacket* > packetVector; // super packet // tm, mk
		
		
		
		/**
		 * @brief Access channel @a i of this data packet.
		 * @note A warning message will be printed to the log if
		 * channel @a i does not exist.
		 * @param i channel index.
		 * @returns A pointer to the Value on chanel @a i
		 * or a NULL pointer if channel @a i does not exist.
		 */
		Value *getChannel( unsigned int i ) const;
		
		/**
		 * @brief Set channel @a i of this data packet to the value @a val.
		 * @note A warning message will be printed to the log if
		 * channel @a i does not exist.
		 * @warning This will DELETE the previous Value. Be sure the previous
		 * Value is not referenced anywhere anymore. Otherwise use getChannel()
		 * to retrieve a pointer to the Value object and then change its value.
		 * @param i channel index.
		 * @param val Value to set.
		 */
		void setChannel( unsigned int i, Value *val );
		
		/**
		 * @brief Get the number of channels contained by this data packet.
		 * @return Size of dataVector (number of channels).
		 */
		unsigned long int size() const;
		
		/**
		 * \brief Set the ID of the stream this packet is belonging to.
		 * \param id Stream-identifier.
		 * \note This method may be called only once.
		 */
		void setStreamId( int id );
		
		/**
		 * \brief Get the ID of the stream this packet is belonging to.
		 * \return Stream-identifier.
		 */
		int getStreamId() const;
		
		/**
		 * \brief Get the timestamp in decimal seconds.
		 * \note You may loose precision using this method.
		 * \note For more precise results read the timestamp member variable directly.
		 * \return Timestamp in seconds.
		 */
		double getTimestampSec() const;
		
		/**
		 * \brief Set the timestamp from a decimal value.
		 * \note For more precise timestamps set the timestamp member variable directly.
		 * \param seconds Timestamp in seconds.
		 */
		void setTimestampSec(double seconds);
		
		
		virtual void toString( std::ostream &o );		///< Print object to stream o.
//		virtual void toString( std::string *str );	///< Prints the data to a string.

		/// Get a copy of the object.
		virtual DataPacket *clone();
		/// Get a copy of the object containing selected channels only.
		virtual DataPacket *clone( std::vector<unsigned> channels );
		/// Append (move) Values from packet \a p.
		virtual void appendContentOf( DataPacket *p );
		/// Only keep selected channels (Values), delete others.
		virtual void narrowData( std::vector<unsigned> channels );
		/// delete all channels.
		virtual void empty();
		/// invalidate all values.
		virtual void invalidateValues();
		
		// mk // debug
		void dump_packet(char * filename);
		
		// mk quick hack
		int get_number();
		
		
		/**
		 * Add a stream description to the global list and
		 * get a streamId (for use in DataPackets) in return.
		 * @param desc Stream description to add.
		 * @returns A valid streamId.
		 */
	//	static int addStreamDescription( const Json::Value desc );
		
		/**
		 * Get a stream description from the global list.
		 * @param streamId StreamId for wich to get the description.
		 * @returns Stream description.
		 */
	//	static Json::Value getStreamDescription( int streamId );
		
		/**
		 * Create a stream description with the given channel names and type.
		 * @param channelNames Names of the channels.
		 * @param type Data type of the channel (use "LONG" or "DOUBLE")
		 * @returns A stream description as json object.
		 */
	//	static Json::Value createStreamDescription(std::vector<std::string> channelNames, std::string type);
		
		/**
		 * Automatically generate a stream description for a packet.
		 * @param p Data packet to generate a stream description for.
		 * @returns A stream description as json object.
		 */
	//	static Json::Value createStreamDescription(const DataPacket *p);
	
	
	private:
		/**
		 * @brief Static counter variable counting the number of data packets that have
		 * been instantiated.
		 */
		static int counter;
		
		/**
		 * @brief Global packet number. Cloned/copied packets have the same number.
		 */
		int number;
		
		/**
		 * \brief Stream identifier.
		 */
		int streamId;
		
		static Mutex mutex;
		static std::vector<Value> streamDescs;
};


// the c++ style "toString()" operator
std::ostream& operator<<( std::ostream &o, DataPacket *p );



#endif	//DATAPACKET_H
