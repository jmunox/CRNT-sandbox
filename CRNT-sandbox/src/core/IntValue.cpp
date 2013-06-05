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

// IntValue.cpp

#include "IntValue.h"



IntValue::IntValue() : val( 0 ) {}
IntValue::IntValue( int k, bool valid ) : Value( valid ), val( k ) {}
IntValue::~IntValue() {}

Value* IntValue::clone() const { return new IntValue( *this ); }
		
//access (read)
int IntValue::getInt() const { return val; }
float IntValue::getFloat() const
{
	//log( "getFloat() WARNING: conversion int->float" );
	return (float)val;
}
fix IntValue::getFix() const
{
	log( "getFix() WARNING: conversion int->fix" );
	return val<<16;
}
void IntValue::toString( std::ostream &o ) const { o << val; }

//access (write)
void IntValue::setVal( int k ) { val = k; }
void IntValue::setVal( float f )
{ 
	//log( "set(float) WARNING: conversion float->int" );
	val = (int)f;
}

//math ops
Value& IntValue::operator+=( const Value& v ) { val += v.getInt(); return *this; }
Value& IntValue::operator-=( const Value& v ) { val -= v.getInt(); return *this; }
Value& IntValue::operator*=( const Value& v ) { val *= v.getInt(); return *this; }
Value& IntValue::operator/=( const Value& v ) { val /= v.getInt(); return *this; }
Value& IntValue::operator+=( const int v ) { val += v; return *this; }
Value& IntValue::operator-=( const int v ) { val -= v; return *this; }
Value& IntValue::operator*=( const int v ) { val *= v; return *this; }
Value& IntValue::operator/=( const int v ) { val /= v; return *this; }

Value& IntValue::sqrt()
{
	if( val < 0 ) {
		//imaginary result
		log( "sqrt() ERROR: trying to take square root of negative value" );
		//better throw an exception here!
		val = 0;
		return *this;
	}
	else if( val == 0 ) {
		val = 0;
		return *this;
	}

	long y = (long) val;

	// the following code is adapted from
	// http://www.codecodex.com/wiki/index.php?title=Calculate_an_integer_square_root
	//-------------------------------------------------------------------------

	long    x_old, x_new;
	long    testy;
	int     nbits;
	int     i;
	
	/* select a good starting value using binary logarithms: */
	nbits = (sizeof(y) * 8) - 1;    /* subtract 1 for sign bit */
	for (i = 4, testy = 16L;; i += 2, testy <<= 2L) {
		if (i >= nbits || y <= testy) {
			x_old = (1L << (i / 2L));       /* x_old = sqrt(testy) */
			break;
		}
	}
	/* x_old >= sqrt(y) */
	/* use the Babylonian method to arrive at the integer square root: */
	for (;;) {
		x_new = (y / x_old + x_old) / 2L;
		if (x_old <= x_new)
			break;
		x_old = x_new;
	}
	/* make sure that the answer is right: */
	if ((long long) x_old * x_old > y || ((long long) x_old + 1) * ((long long) x_old + 1) <= y) {
		log( "Error in isqrt algorithm." );
		//better throw an exception here!
		val = 0;
		return *this;
	}
	val = x_old;

	//-------------------------------------------------------------------------
	// end of adapted code.

	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::log2()
{
	log( "log2(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::exp2()
{
	log( "exp2(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::exp()
{
	log( "exp(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::sin()
{
	log( "sin(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::cos()
{
	log( "cos(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::tan()
{
	log( "tan(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::asin()
{
	log( "asin(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::acos()
{
	log( "acos(): ERROR, not implemented yet" );
	return *this;
}


/**
 * @todo Implement this method.
 */
Value& IntValue::atan()
{
	log( "atan(): ERROR, not implemented yet" );
	return *this;
}


Value& IntValue::abs() {
	val = ::abs( val );
	return *this;
}



//comparison ops
bool IntValue::operator<( const Value& v ) const { return val < v.getInt(); }
bool IntValue::operator>( const Value& v ) const { return val > v.getInt(); }
bool IntValue::operator==( const Value& v ) const { return val == v.getInt(); }
bool IntValue::operator!=( const Value& v ) const { return val != v.getInt(); }
bool IntValue::operator<=( const Value& v ) const { return val <= v.getInt(); }
bool IntValue::operator>=( const Value& v ) const { return val >= v.getInt(); }

		


