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

#include "dmotFileMap.h"
#include "dmotFileMapEntry.h"
#include "../dmotools.h"
#include "../exceptions.h"



// Functors
/////////////

struct fEqualID{
	unsigned int pID;
	
	fEqualID( unsigned int id ){
		pID = id;
	}
	
	bool operator()( dmotFileMapEntry *entry ){
		return entry->GetID() == pID;
	}
};



// Class dmotFileMap
//////////////////////

// Constructor, destructor
////////////////////////////

dmotFileMap::dmotFileMap(){
}

dmotFileMap::~dmotFileMap(){
	RemoveAllEntries();
}



// Management
///////////////

int dmotFileMap::GetEntryCount() const{
	return pEntries.size();
}

bool dmotFileMap::HasEntryWithID( unsigned int id ) const{
	return std::find_if( pEntries.begin(), pEntries.end(), fEqualID( id ) ) != pEntries.end();
}

dmotFileMapEntry *dmotFileMap::GetEntryAt( int index ) const{
	if( index < 0 || index >= ( int )pEntries.size() ) THROW( dueInvalidParam );
	
	return pEntries.at( index );
}

dmotFileMapEntry *dmotFileMap::GetEntryWithID( unsigned int id ) const{
	std::vector<dmotFileMapEntry*>::const_iterator iter = std::find_if( pEntries.begin(), pEntries.end(), fEqualID( id ) );
	
	return ( iter == pEntries.end() ) ? NULL : *iter;
}

void dmotFileMap::AddEntry( dmotFileMapEntry *entry ){
	if( ! entry || HasEntryWithID( entry->GetID() ) ) THROW( dueInvalidParam );
	
	pEntries.push_back( entry );
}

void dmotFileMap::AddEntry( unsigned int id, unsigned int offset, unsigned int size ){
	if( HasEntryWithID( id ) ) THROW( dueInvalidParam );
	
	pEntries.push_back( new dmotFileMapEntry( id, offset, size ) );
}

void dmotFileMap::RemoveEntry( dmotFileMapEntry *entry ){
	if( ! entry ) THROW( dueInvalidParam );
	
	std::vector<dmotFileMapEntry*>::iterator iter = std::find( pEntries.begin(), pEntries.end(), entry );
	
	if( iter == pEntries.end() ) THROW( dueInvalidParam );
	pEntries.erase( iter );
	
	delete entry;
}

void dmotFileMap::RemoveAllEntries(){
	while( ! pEntries.empty() ){
		delete pEntries.back();
		pEntries.pop_back();
	}
}



void dmotFileMap::ReadFromFile( const char *path ){
	FILE *file = NULL;
	unsigned int i;
	sFileMapHeader header;
	sFileMapEntry entry;
	
	try{
		file = fopen( path, "rb" );
		if( ! file ) THROW( dueFileNotFound );
		
		if( fread( &header, 1, 8, file ) < 8 ) THROW( dueReadFile );
		
		if( header.check != 16 ){
			printf( "%s is not a valid file map (magic number is %i instead of 16).\n", path, header.check );
			THROW( dueInvalidFileFormat );
		}
		
		for( i=0; i<header.count; i++ ){
			if( fread( &entry, 1, 24, file ) < 24 ) THROW( dueReadFile );
			
			if( entry.v1 != 1 ){
				printf( "%s is not a valid file map (entry %i: first value is %i instead of 1).\n", path, i, entry.v1 );
				THROW( dueInvalidFileFormat );
			}
			
			if( entry.v6 != 0 ){
				printf( "%s is not a valid file map (entry %i: last value is %i instead of 0).\n", path, i, entry.v6 );
				THROW( dueInvalidFileFormat );
			}
			
			AddEntry( entry.v4, entry.v5, entry.v2 );
		}
		
		fclose( file );
		
	}catch( duException ){
		if( file ) fclose( file );
		throw;
	}
}
