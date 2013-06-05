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

// FloatValue.cpp

#include "FloatValue.h"

#include <cmath>



FloatValue::FloatValue() : val( 0 ) {}
FloatValue::FloatValue( float k, bool valid ) : Value( valid ), val( k ) {}
//IntValue::IntValue( int k, bool valid ) : Value( valid ), val( k ) {}
FloatValue::~FloatValue() {}

Value* FloatValue::clone() const { return new FloatValue( *this ); }
		
//access (read)
int FloatValue::getInt() const
{
	//log( "getInt() WARNING: conversion float->int" );
	return (int)val;
}
float FloatValue::getFloat() const { return val; }
fix FloatValue::getFix() const
{
	log( "getFix() WARNING: conversion float->fix" );
	return (int)(val*65536);
}
void FloatValue::toString( std::ostream &o ) const { o << std::scientific << val; }

//access (write)
void FloatValue::setVal( int k )
{ 
	//log( "setVal(int) WARNING: conversion int->float" );
	val = (float)k;
}
void FloatValue::setVal( float f ) { val = f; }

//math ops
Value& FloatValue::operator+=( const Value& v ) { val += v.getFloat(); return *this; }
Value& FloatValue::operator-=( const Value& v ) { val -= v.getFloat(); return *this; }
Value& FloatValue::operator*=( const Value& v ) { val *= v.getFloat(); return *this; }
Value& FloatValue::operator/=( const Value& v ) { val /= v.getFloat(); return *this; }
Value& FloatValue::operator+=( const int v ) { val += v; return *this; }
Value& FloatValue::operator-=( const int v ) { val -= v; return *this; }
Value& FloatValue::operator*=( const int v ) { val *= v; return *this; }
Value& FloatValue::operator/=( const int v ) { val /= v; return *this; }
		
Value& FloatValue::sqrt() { val = (float) ::sqrt(val); return *this; }
Value& FloatValue::log2() { val = (float) ::log2(val); return *this; }
Value& FloatValue::exp2() { val = (float) ::exp2(val); return *this; }
Value& FloatValue::exp() { val = (float) ::exp(val); return *this; }
Value& FloatValue::sin() { val = (float) ::sin(val); return *this; }
Value& FloatValue::cos() { val = (float) ::cos(val); return *this; }
Value& FloatValue::tan() { val = (float) ::tan(val); return *this; }
Value& FloatValue::asin() { val = (float) ::asin(val); return *this; }
Value& FloatValue::acos() { val = (float) ::acos(val); return *this; }
Value& FloatValue::atan() { val = (float) ::atan(val); return *this; }
Value& FloatValue::abs() { val = ::fabs(val); return *this; }

//comparison ops
bool FloatValue::operator<( const Value& v ) const { return val < v.getFloat(); }
bool FloatValue::operator>( const Value& v ) const { return val > v.getFloat(); }
bool FloatValue::operator==( const Value& v ) const { return val == v.getFloat(); }
bool FloatValue::operator!=( const Value& v ) const { return val != v.getFloat(); }
bool FloatValue::operator<=( const Value& v ) const { return val <= v.getFloat(); }
bool FloatValue::operator>=( const Value& v ) const { return val >= v.getFloat(); }

		


