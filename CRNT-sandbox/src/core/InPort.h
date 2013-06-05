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

// InPort.h - The Input-Port Object
//

#ifndef INPORT_H
#define INPORT_H

#include "TBObject.h"
#include "DataPacket.h"
#include "Mutex.h"
#include "Condition.h"
//#include "StreamTask.h"

#include <queue>

//StreamTask-dummy
class StreamTask;

/**
 * \ingroup core
 * \brief In-Port for stream tasks.
 *
 * The input port for StreamTask objects.
 * Can be connected to OutPort objects.
 */
class InPort: public TBObject
{
	public:
		InPort();
		InPort( const InPort& p );
		virtual ~InPort();

		/**
		 * \brief Pop a packet from the receive queue.
		 * 
		 * Receives a data packet from connected OutPort object(s).
		 * This method blocks if no packet is available.
		 * If \p timeout is greater than zero the method will block at most for
		 * \p timeout milliseconds.
		 * 
		 * \param timeout Maximum time in milliseconds to wait for a packet.
		 * \return The first data packet in queue or NULL if a timeout occured.
		 * \throws "condition canceled" if the method cancel_receive() was called.
		 */
		virtual DataPacket* receive( long timeout = 0 );

		/**
		 * \brief Cancel a call of the receive() method.
		 * 
		 * A thread that is blocked in the receve() method will
		 * return from the receive() method with the exception
		 * "condition canceled".
		 */
		virtual void cancel_receive();

		/**
		 * \brief Push a packet to the receive queue.
		 * 
		 * Enqueues a data packet in this objects receive queue.
		 * The packet will be discarded if the queue is full.
		 *
		 * This method is called from the OutPort::send(p) method.
		 * Don't call it anywhere else.
		 *
		 * \param p The data packet to be enqueued.
		 */
		virtual void enqueue( DataPacket *p );
		
		/**
		 * \brief Check if queue is not empty.
		 * 
		 * Use this method to find out if a call of receive() will block or not.
		 * \return 'true' if the receive queue contains data packets.
		 */
		virtual bool notEmpty();

		/**
		 * \brief Check if queue is empty.
		 * 
		 * Use this method to find out if a call of receive() could block.
		 * \return 'true' if the receive queue contains no data packet.
		 */
		virtual bool isEmpty();
		
		/**
		 * \brief Set the identifier string of this in-port.
		 * \param t The StreamTask which this in-port belongs to.
		 * \param i The index of this in-port.
		 */
		virtual void setInPortID( StreamTask *t, int i );

		/**
		 * \brief Get the id of this in-port.
		 * \return The index (id) of this in-port.
		 */
		virtual int getInPortID() const;

		/**
		 * \brief Set the maximal number of packets the in-port queue allows.
		 *
		 * A value of 0 allows infinite queue size (constrained by system memory).
		 * \param size Maximal number of packets to store. Zero will allow
		 * infinite queue size.
		 */
		virtual void setMaxQueueSize( unsigned int size );

		/**
		 * \brief Get the maximal allowed queue size of this in-port.
		 * \return The maximal allowed queue size of this in-port.
		 */
		virtual unsigned int getMaxQueueSize() const;
		
		/**
		 * \brief No message when packets are dropped.
		 */
		virtual void setSilent(bool silent);

		/**
		 * \brief Set lossless mode.
		 *
		 * In lossless mode the enqueue() method will block as long as
		 * the queue is full, to ensure that no packet will be discarded.
		 * (default: disabled)
		 * \param flag desired state of lossless mode (enable/disable)
		 * \see StreamTask::inPortLossless()
		 */
		virtual void setLossless( bool flag );

	protected:
		Mutex mutex;
		Condition condition;
		std::queue<DataPacket *> packetQueue;
		unsigned int maxQueueSize;
		unsigned int droppedPackets;
		bool lossless;
		bool silent;
		StreamTask *owner;
		int inPortID;
		// OAM REVISIT
		int errQueueCounter;
};



#endif	//INPORT_H
