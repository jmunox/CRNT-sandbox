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

#ifndef STREAMTASK_H
#define STREAMTASK_H

#include "Thread.h"
#include "InPort.h"
#include "OutPort.h"
#include "DataPacket.h"
#include "StreamTaskContainer.h"

#include <vector>




/**
 * \ingroup core
 * \brief Base class for streaming tasks.
 *
 * Every task in the toolbox is a subclass of this abstract class.
 * They must implement the run() method declared by the Thread class.
 */
class StreamTask : public Thread
{
	public:
		StreamTask( unsigned int inports = 0, unsigned int outports = 0 );
		StreamTask( const StreamTask& s );
		virtual ~StreamTask();

		virtual void start();			///<Starts this thread.
		virtual void stop();			///<Stops this thread.
		const std::vector<InPort *>& getInPorts();	///< Returns in-port vector.
		const std::vector<OutPort *>& getOutPorts();	///< Returns out-port vector.
        //void setParams( Json::Value &jval, bool update = false );
        //virtual void setParamsFromTop( Json::Value &jval, bool update = false );
		virtual std::string identify();
		
		/**
		 * \brief Check if thread has stopped.
		 * \return \c true, if thread is not in run() method.
		 */
		virtual bool isStopped();

		/**
		 * \brief returns the URL of the description of the stream task
		 */
		virtual std::string getDescriptionURL();

		/**
		 * \brief returns the description of the stream task
		 */
		virtual std::string readDescription();

		virtual void setParent(StreamTaskContainer *parent);
	
	protected:
		/**
		 * Cancel all calls that might be blocking this task in the run() method.
		 * It allows a task to stop execution. This method is called from the stop() method.
		 *
		 * Important: Overwrite this method in subclasses that use blocking functions
		 * (except inPort.receive()). In-ports are canceled automatically.
		 */
		virtual void cancelAllBlockingCalls() {};
		
		void paramsChanged();
		
		int addInPorts( unsigned int n );
		int addOutPorts( unsigned int n );
		void setInPortNum( unsigned int n );
		void setOutPortNum( unsigned int n );
		
		std::vector<InPort *> inPorts;	///<The task's in-ports.
		std::vector<OutPort *> outPorts;	///<The task's out-ports.

		/**
		 * \brief Maximal queue size for each in-port.
		 *
		 * Zero will allow infinite queue size.
		 */
		unsigned int inPortBufferSize; //(9999);

		/**
		 * \brief Lossless mode to use for each in-port.
		 * 
		 * A lossless in-port will block senders as long as the queue is full.
		 * \see InPort::setLossless()
		 */
		bool inPortLossless; //(false);

		/**
		 * \brief Silent mode to use for each in-port.
		 * 
		 * A silent in-port will print no warning when dropping packets.
		 * \see InPort::setSilent()
		 */
		bool inPortSilent; //(false);

		/**
		 * \brief Do not start task if 'true'.
		 */
		bool disabled; // (false);

		/**
		 * \brief URL that points to a file describing this stream task
		 */
		std::string descriptionURL; //("");
		
		/// Whether this task is running.
		/**
		 * The run() method must return if running is set to false
		 * in order to ensure a proper exit of the thread.
		 */
		bool running;

	protected:
		void initPorts();
		
		StreamTaskContainer *parent;
};



#endif	//STREAMTASK_H
