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

// 
// DataPacket.cpp
// mk : added super packet support
// 

#include "DataPacket.h"
#include <math.h>
#include <stdio.h>
#include <typeinfo>

using namespace std;

/// Operator for printing DataPackets to an output stream.
ostream& operator<<( ostream &o, DataPacket *p ){
  
  o << "DataPacket:" << endl;
  p->toString( o );
	
  return o;
}


/// Static counter variable. It is incremented in the constructor.
/// @todo Might not be therad-save. Mutex needed?
int DataPacket::counter = 0;

Mutex DataPacket::mutex;
vector<Value> DataPacket::streamDescs;


/// Default constructor.
DataPacket::DataPacket( int streamId ) : streamId(streamId)
{

  number = counter++;
  seqNr = 0;
  endOfStream = false;
	
  if( gettimeofday( &timestamp, NULL ) < 0 ) {
    log( "ERROR: gettimeofday() failed." );
  }
}



/// Copy Constructor.
/**
 * Creates a copy of an existing DataPacket
 */
 // mk added super packet support
DataPacket::DataPacket( const DataPacket &p ){
	
	number = p.number;
  	seqNr = p.seqNr;
  	timestamp.tv_sec = p.timestamp.tv_sec;
  	timestamp.tv_usec = p.timestamp.tv_usec;
  	streamId = p.streamId;
	endOfStream = p.endOfStream;
  
	packetVector.clear();
    
    // copy segment data
    for( uint32 i=0; i<p.packetVector.size(); i++ ){
    	packetVector.push_back( p.packetVector[i]->clone() );
    }  
  
  	dataVector.clear();
    
    // copy the Value objects
    for( uint32 i=0; i<p.dataVector.size(); i++ ) {
    	dataVector.push_back( p.dataVector[i]->clone() );
    }
}



/// Special copy Constructor.
/**
 * Creates a copy of an existing DataPacket but only
 * containing the channels listed in \a channels.
 * @param p DataPacket to copy.
 * @param channels Indexes of channels to copy.
 */
 // mk added super packet support
DataPacket::DataPacket( DataPacket &p, vector<unsigned> channels ){ 

  	number = p.number;
  	seqNr = p.seqNr;
  	timestamp.tv_sec = p.timestamp.tv_sec;
  	timestamp.tv_usec = p.timestamp.tv_usec;
   	streamId = p.streamId;
	endOfStream = p.endOfStream;
 
 	packetVector.clear();
    
    //copy the segment data
    for( uint32 i=0; i<p.packetVector.size(); i++ ) {
      	packetVector.push_back( p.packetVector[i]->clone(channels) );
    }
  
    dataVector.clear();
    
    //copy the Value objects
    for( uint32 i=0; i<channels.size(); i++ ) {
		Value *v = p.dataVector.at( channels.at(i) );
      	dataVector.push_back( v->clone() );
    }
}


// implemented by mk
DataPacket::~DataPacket() {
	
//	log("destructor");
	
#if 1
	// delete segment data (1 level recursion)
	for ( uint i=0; i<packetVector.size(); i++ ) {
  		DataPacket* p = packetVector.at(i);
  		delete p;	
	}
#endif
	// delete ordinary data
  	for( uint32 i=0; i<dataVector.size(); i++ ) {
   		delete dataVector.at(i);
  	}
}



Value *DataPacket::getChannel( unsigned int i ) const
{
	Value *val = NULL;
	
	try{
		val = dataVector.at( i );
	}
	catch( ... ) {
		log( "WARNING: channel does not exist:" ) << i << endl;
	}
	
	return val;
}

void DataPacket::setChannel( unsigned int i, Value *val )
{
	try{
		Value *old = dataVector.at( i );
		dataVector.at( i ) = val;
		delete old;
	}
	catch( ... ) {
		log( "WARNING: channel does not exist:" ) << i << endl;
	}
}

unsigned long int DataPacket::size() const
{
	return dataVector.size();
}

void DataPacket::setStreamId( int id )
{
	if( id < 0 ) {
		log( "ERROR: invalid stream identifier: " ) << id << endl;
		return;
	}
	
//	if( streamId < 0 ) {
		streamId = id;
//	}
//	else {
//		log( "ERROR: stream identifier may be set only once" );
//		return;
//	}
}

int DataPacket::getStreamId() const
{
	return streamId;
}


double DataPacket::getTimestampSec() const
{
	double seconds = timestamp.tv_sec;
	seconds += timestamp.tv_usec / 1000000.0;
	return seconds;
}


void DataPacket::setTimestampSec(double seconds)
{
	double integral;
	double fractional = modf(seconds, &integral);
	timestamp.tv_sec = integral;
	timestamp.tv_usec = fractional * 1000000;
}


int DataPacket::get_number() {
	return number;	
}



/**
 * This method is called by the << operator defined in the
 * file DataPacket.cpp.
 * Overwriting methods must call this implementation first
 * (supercall) in order to print members of this class.
 * @see DataPacket.cpp for operator<<().
 */
void DataPacket::toString( ostream &o ){
  
  o << "\taddr              : " << (long)this << endl;
  o << "\tnumber            : " << number << endl;
  o << "\tseqNr             : " << seqNr << endl;
  o << "\tendOfStream       : " << (endOfStream ? "true" : "false") << endl;
  o << "\tstreamId          : " << streamId << endl;
  o << "\tpacketVector.size : " << packetVector.size() << endl; // mk
  o << "\tdataVector.size   : " << dataVector.size() << endl;   // mk
  o << "\ttimestamp         : " << timestamp.tv_sec << " sec, ";
  o << timestamp.tv_usec << " usec";
  o << ", addr " << &timestamp;
  o << endl;

  // mk
  // OAM REVISIT
  
  /*
  o << "\tDataSegment:" << endl;
  uint32 tsteps = packetVector.size();
  if ( tsteps ){
    uint32 channels = packetVector[0]->dataVector.size();
    for ( uint32 i=0; i<tsteps; i++ ){
      
      o << "\tpacketVector[" << i << "] : ";
      DataPacket *p = packetVector[i];
      ValueVect v = p->dataVector;

      for( uint32 j=0; j<channels; j++ ){
	o << v.at(j) << " ";
      }
      o << endl;
    }
  }
  o << endl;
*/

  o << "\tdataVector: ";
  for( unsigned int i = 0; i < dataVector.size(); i++ ) {
    o << dataVector[i] << " ";
  }
  o << endl;
  
}


// mk // debug
void DataPacket::dump_packet(char * filename) {

	FILE *fp = fopen(filename,"a");
	if ( !fp ) {
		printf("error opening dump file %s!",filename);
	}
	
	fprintf(fp,"\nSUPER PACKET\n");
	fprintf(fp,"\taddr       		: %ld\n",(long)this);
	fprintf(fp,"\tnumber     		: %d\n",number);
	fprintf(fp,"\tseqNr      		: %llu\n",seqNr);
	fprintf(fp,"\tendOfStream  		: %s\n", (endOfStream ? "true" : "false"));
	fprintf(fp,"\tpacketVector.size : %d\n",(int)packetVector.size());
	fprintf(fp,"\tdataVector.size   : %d\n",(int)dataVector.size());
	fprintf(fp,"\ttimestamp  		: %d sec\n",(int)timestamp.tv_sec);
	fprintf(fp,"\ttimestamp  		: %d usec\n",(int)timestamp.tv_usec);

	fprintf(fp,"\tDataSegment:\n");
  
	for ( uint32 i=0; i<packetVector.size(); i++ ) {
    	
   		fprintf(fp,"\tDataPacket[%d]\n",i);
	      	
     	DataPacket *p = packetVector.at( i );
   	  	ValueVect v   = p->dataVector;
   	  	
		fprintf(fp,"\t\tpacketVector.size : %d\n",(int)p->packetVector.size());
		fprintf(fp,"\t\tdataVector.size   : %d\n",(int)p->dataVector.size());
		
    	for( uint32 j=0; j<v.size(); j++ ){
			fprintf(fp,"\t\tDataVector[%d] = %f\n",j,v.at( j )->getFloat());
      	}
    }  
    
    for ( uint32 i=0; i<dataVector.size(); i++ ) {
    	fprintf(fp,"\tDataVector[%d] = %f\n",i,dataVector.at( i )->getFloat());
    }
    fclose( fp );
}



DataPacket *DataPacket::clone(){
  return new DataPacket( *this );
}



DataPacket *DataPacket::clone( vector<unsigned> channels ){
  return new DataPacket( *this, channels );
}



/**
 * All Values from packet p are moved (and appended) to this packet.
 * @note Packet p is empty after calling this method.
 * @note If the packet contains segmented data, the packet p must contain
 * segmented data of equal length in order to successfully merge the data
 * @param[in] p DataPacket to move Values from.
 */
void DataPacket::appendContentOf( DataPacket *p ){
  
  // how to merge SuperPackets ??
  // check for equal size of packetVector
  if ( packetVector.size() ){

    if ( packetVector.size() == p->packetVector.size() ){

      for ( uint32 i=0; i<packetVector.size(); i++ ){
	packetVector[i]->appendContentOf(p->packetVector[i]);
	p->packetVector[i]->dataVector.clear();
      }
      p->packetVector.clear();
    }
    else{
      // no sync!
    }
  }
  else{
    for( uint32 i=0; i<p->dataVector.size(); i++ ) {
      dataVector.push_back( p->dataVector.at(i) );
    }
    p->dataVector.clear();
  }
}



// keep selected channels only
void DataPacket::narrowData( vector<unsigned> channels ){
   
  // segment data
  if ( packetVector.size() ){
    for ( uint32 i=0; i<packetVector.size() ; i++ ){
      packetVector[i]->narrowData(channels);
    }
  }
  // normal data
  else{
    vector<Value*> keep;
    for( uint32 i = 0; i < channels.size(); i++ ) {
      uint32 ch = channels.at( i );
      Value *v = dataVector.at( ch );
      keep.push_back( v );
      dataVector[ch] = NULL;
    }
    
    for( uint32 i = 0; i < dataVector.size(); i++ ) {
      delete dataVector[i];
    }
    dataVector.clear();  
    dataVector.swap( keep );
  }  
}



// delete all values and clear \a dataVector
void DataPacket::empty(){

  for ( uint32 i=0; i<dataVector.size(); i++ ){
    delete dataVector[i];
  }
  dataVector.clear();
  
  // segment data
  for ( uint32 i=0; i<packetVector.size(); i++ ){
    packetVector[i]->empty();
  }
  packetVector.clear();
}



// invalidates all values in this packet
void DataPacket::invalidateValues(){

  for( uint32 i=0; i < dataVector.size(); i++ ) {
    dataVector.at( i )->invalidate();
  }

  // segment data
  for ( uint32 i=0; i<packetVector.size(); i++ ){
    packetVector[i]->invalidateValues();
  }
}



/*
// static methods

Value DataPacket::createStreamDescription(const DataPacket *p) {
	Value desc;
	desc["source"] = "io.ColumnFileParser";
	Value channels;
	for (int i = 0; i < p->dataVector.size(); i++) {
		Value channel;
		channel["type"] = typeid( p->dataVector.at(i) ).name();
		channel["name"] = "channel" + i;
		channels.append(channel);
	}
	desc["channels"] = channels;
	return desc;
}

Json::Value DataPacket::createStreamDescription(vector<string> channelNames, string type) {
	Value desc;
	desc["source"] = "io.ColumnFileParser";
	Value channels;
	for (int i = 0; i < channelNames.size(); i++) {
		Value channel;
		channel["type"] = type;
		channel["name"] = channelNames.at(i);
		channels.append(channel);
	}
	desc["channels"] = channels;
	return desc;
}

int DataPacket::addStreamDescription( const Value desc ) {
	mutex.lock();
	streamDescs.push_back(desc);
	int id = streamDescs.size() - 1;
	mutex.unlock();
	//clog << "added stream description " << id << ": " << desc.toStyledString() << endl;
	return id;
}


Value DataPacket::getStreamDescription( int streamId ) {
	mutex.lock();
	Value desc = streamDescs.at(streamId);
	mutex.unlock();
	return desc;
}*/

