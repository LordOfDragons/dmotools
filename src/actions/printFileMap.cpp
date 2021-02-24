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

#include "../filemap/dmotFileMap.h"
#include "../filemap/dmotFileMapEntry.h"
#include "../exceptions.h"



int printFileMap( const char *filename ){
	dmotFileMap filemap;
	int i, count;
	
	try{
		filemap.ReadFromFile( filename );
		count = filemap.GetEntryCount();
		
		printf( "File Map %s: %i entries\n", filename, count );
		
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry = *filemap.GetEntryAt( i );
			
			printf( "- Entry %i: id=%u, offset=%u, size=%u\n", i, entry.GetID(), entry.GetOffset(), entry.GetSize() );
		}
		
	}catch( duException e ){
		e.PrintError();
		return 1;
	}
	
	return 0;
}

int printFileMapCSV( const char *filename ){
	dmotFileMap filemap;
	int i, count;
	
	try{
		filemap.ReadFromFile( filename );
		count = filemap.GetEntryCount();
		
		printf( "Entry,ID,Offset,Size\n" );
		
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry = *filemap.GetEntryAt( i );
			
			printf( "%i,%u,%u,%u\n", i, entry.GetID(), entry.GetOffset(), entry.GetSize() );
		}
		
	}catch( duException e ){
		e.PrintError();
		return 1;
	}
	
	return 0;
}
