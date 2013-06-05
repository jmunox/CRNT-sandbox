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

// IntValue.h

#ifndef INTVALUE_H
#define INTVALUE_H

#include "Value.h"



/**
 * \ingroup core
 * \brief The toolbox's integer.
 *
 *
 */
class IntValue: public Value
{

	public:
		IntValue();
		IntValue( int k, bool valid = true );
		virtual ~IntValue();

		/// Clone this object
		virtual Value* clone() const;

		//access (read)
		virtual int getInt() const;
		virtual float getFloat() const;
		virtual fix getFix() const;
		/// Print object to stream o.
		virtual void toString( std::ostream &o ) const;

		//access (write)
		virtual void setVal( int k );
		virtual void setVal( float f );
		
		//math ops
		virtual Value& operator+=( const Value& v );
		virtual Value& operator-=( const Value& v );
		virtual Value& operator*=( const Value& v );
		virtual Value& operator/=( const Value& v );
		virtual Value& operator+=( const int v );
		virtual Value& operator-=( const int v );
		virtual Value& operator*=( const int v );
		virtual Value& operator/=( const int v );
		
		virtual Value& sqrt();	///< square root
		virtual Value& log2();	///< Logarithm to base 2.
		virtual Value& exp2();	///< Raise 2 to-the-power of this value.
		virtual Value& exp();	///< Raise e to-the-power of thie value.
		virtual Value& sin();	///< Sine
		virtual Value& cos();	///< Cosine
		virtual Value& tan();	///< Tangent
		virtual Value& asin();	///< Arc-Sine
		virtual Value& acos();	///< Arc-Cosine
		virtual Value& atan();	///< Arc-Tangent
		virtual Value& abs();	///< ABS
		
		//comparison ops
		virtual bool operator<( const Value& v ) const;
		virtual bool operator>( const Value& v ) const;
		virtual bool operator==( const Value& v ) const;
		virtual bool operator!=( const Value& v ) const;
		virtual bool operator<=( const Value& v ) const;
		virtual bool operator>=( const Value& v ) const;

		private:
			int val;
};


#endif	//INTVALUE_H
