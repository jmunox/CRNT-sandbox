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

// Value.h

#ifndef VALUE_H
#define VALUE_H

#include "TBObject.h"

//workaround for lowercase macro definitions found in cygwin
#ifdef log2
#define LOG2_WORKAROUND(x) log2(x)
#undef log2
double log2( double x );
#endif

typedef unsigned int   uint;	/**< An unsigned integer of at least 32 bits */ 
typedef signed int     int32;	/**< An 32 bit signed integer (2s complement) */
typedef unsigned int   uint32;	/**< An 32 bit unsigned integer */

/** 
    A type representing a 32bit fixed-point number. 
    The binary point lies between bits 15 and 16. 
*/ 
typedef int32 fix; 

/** 
    A type representing a 32bit unsigned fixed-point number. 
    The binary point lies between bits 15 and 16. 
*/ 
typedef uint32 ufix; 




/**
 * \ingroup core
 * \brief Abstract class for values.
 *
 * The abstract (super-) class for values of different types.
 */
class Value: public TBObject
{

	private:
		bool valid; //=true;

	public:
		Value( bool valid = true );
		virtual ~Value() {};

		/// Clone this object
		virtual Value* clone() const = 0;
		
		virtual bool isValid() const;	///< check if this value is valid
		virtual void setValid( bool valid );	///< set the valid state
		virtual void invalidate();		///< set this value invalid

		//access (read)
		virtual int getInt() const = 0;
		virtual float getFloat() const = 0;
		virtual fix getFix() const = 0;
		/// Print object to stream o.
		virtual void toString( std::ostream &o ) const = 0;

		//access (write)
		virtual void setVal( int k ) = 0;
		virtual void setVal( float f ) = 0;
		
		//math ops
		virtual Value& operator+=( const Value& v ) = 0;
		virtual Value& operator-=( const Value& v ) = 0;
		virtual Value& operator*=( const Value& v ) = 0;
		virtual Value& operator/=( const Value& v ) = 0;
		virtual Value& operator+=( const int v ) = 0;
		virtual Value& operator-=( const int v ) = 0;
		virtual Value& operator*=( const int v ) = 0;
		virtual Value& operator/=( const int v ) = 0;
		
		/// Square root
		/**
		 * Sets this value to the square root of itself.
		 * @returns Reference to itself.
		 */
		virtual Value& sqrt() = 0;
		virtual Value& log2() = 0;	///< Logarithm to base 2.
		virtual Value& exp2() = 0;	///< Raise 2 to-the-power of this value.
		virtual Value& exp() = 0;	///< Raise e to-the-power of thie value.
		virtual Value& sin() = 0;	///< Sine
		virtual Value& cos() = 0;	///< Cosine
		virtual Value& tan() = 0;	///< Tangent
		virtual Value& asin() = 0;	///< Arc-Sine
		virtual Value& acos() = 0;	///< Arc-Cosine
		virtual Value& atan() = 0;	///< Arc-Tangent
		virtual Value& abs() = 0;	///< ABS
		
		//comparison ops
		virtual bool operator<( const Value& v ) const = 0;
		virtual bool operator>( const Value& v ) const = 0;
		virtual bool operator==( const Value& v ) const = 0;
		virtual bool operator!=( const Value& v ) const = 0;
		virtual bool operator<=( const Value& v ) const = 0;
		virtual bool operator>=( const Value& v ) const = 0;
};


/// the c++ style "toString()" operator for Values
std::ostream& operator<<( std::ostream& o, const Value* v );
//ostream& operator<<( ostream& o, const Value& v );



#endif	//VALUE_H
