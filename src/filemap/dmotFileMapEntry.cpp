/* 
 * DMO Tools
 *
 * Copyright (C) 2010, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <functional>

#include "dmotFileMapEntry.h"
#include "../exceptions.h"



// Class dmotFileMapEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

dmotFileMapEntry::dmotFileMapEntry( unsigned int id ){
	pID = id;
	pOffset = 0;
	pSize = 0;
}

dmotFileMapEntry::dmotFileMapEntry( unsigned int id, unsigned int offset, unsigned int size ){
	pID = id;
	pOffset = offset;
	pSize = size;
}

dmotFileMapEntry::~dmotFileMapEntry(){
}



// Management
///////////////

void dmotFileMapEntry::SetOffset( unsigned int offset ){
	pOffset = offset;
}

void dmotFileMapEntry::SetSize( unsigned int size ){
	pSize = size;
}
