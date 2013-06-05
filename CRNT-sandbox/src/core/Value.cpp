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

// Value.cpp

#include "Value.h"

//part of lowercase-macro-workaround (see Value.h)
#ifdef LOG2_WORKAROUND
#warning "log2" is defined as macro in your system!! enabling workaround.
double log2( double x ) { return LOG2_WORKAROUND( x ); }
#endif //LOG2_WORKAROUND
 
Value::Value( bool valid ) : valid( valid ) {}

bool Value::isValid() const { return valid; }
void Value::setValid( bool v ) { valid = v; }
void Value::invalidate() { valid = false; }


std::ostream& operator<<( std::ostream& o, const Value* v )
{
	v->toString( o );
	return o;
}

