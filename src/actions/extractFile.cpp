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
#include <sys/types.h>

#include "../filemap/dmotFileMap.h"
#include "../filemap/dmotFileMapEntry.h"
#include "../exceptions.h"
#include "../dmotconfig.h"



int extractFile( const char *pathFileMap, const char *pathArchive, unsigned int fileID, const char *pathDestFile ){
	const dmotFileMapEntry *entry;
	dmotFileMap filemap;
	FILE *file = NULL;
	char *data = NULL;
	
	try{
		filemap.ReadFromFile( pathFileMap );
		
		entry = filemap.GetEntryWithID( fileID );
		if( entry ){
			file = fopen( pathArchive, "rb" );
			if( ! file ) THROW( dueFileNotFound );
			fseek( file, ( long int )entry->GetOffset(), SEEK_SET );
			data = new char[ entry->GetSize() ];
			if( ! data ) THROW( dueOutOfMemory );
			if( fread( data, 1, entry->GetSize(), file ) < entry->GetSize() ) THROW( dueReadFile );
			fclose( file );
			file = NULL;
			
			file = fopen( pathDestFile, "wb" );
			if( ! file ) THROW( dueWriteFile );
			if( fwrite( data, 1, entry->GetSize(), file ) < entry->GetSize() ) THROW( dueWriteFile );
			fclose( file );
			file = NULL;
			
			delete [] data;
			data = NULL;
			
			printf( "File extracted:\n" );
			printf( "- ID: %u\n", fileID );
			printf( "- Size: %u bytes\n", entry->GetSize() );
			printf( "- File: %s\n", pathDestFile );
			
		}else{
			printf( "File with ID %u not found in file map %s\n", fileID, pathFileMap );
			return 1;
		}
		
	}catch( duException e ){
		if( data ) delete [] data;
		if( file ) fclose( file );
		e.PrintError();
		return 1;
	}
	
	return 0;
}

int extractAllFiles( const char *pathFileMap, const char *pathArchive, const char *pathDestDir ){
	char *pathFile = NULL;
	dmotFileMap filemap;
	FILE *file = NULL;
	char *data = NULL;
	bool hasPathSep;
	int i, count;
	
	#ifdef OS_W32
	hasPathSep = ( pathDestDir[ strlen( pathDestDir ) - 1 ] == '\\' );
	#else
	hasPathSep = ( pathDestDir[ strlen( pathDestDir ) - 1 ] == '/' );
	#endif
	
	try{
		filemap.ReadFromFile( pathFileMap );
		count = filemap.GetEntryCount();
		
		pathFile = new char[ strlen( pathDestDir ) + 10 + 20 ];
		if( ! pathFile ) THROW( dueOutOfMemory );
		
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry = *filemap.GetEntryAt( i );
			
			#ifdef OS_W32
			if( hasPathSep ){
				sprintf( pathFile, "%sfile_%u.dat", pathDestDir, entry.GetID() );
			}else{
				sprintf( pathFile, "%s\\file_%u.dat", pathDestDir, entry.GetID() );
			}
			#else
			if( hasPathSep ){
				sprintf( pathFile, "%sfile_%u.dat", pathDestDir, entry.GetID() );
			}else{
				sprintf( pathFile, "%s/file_%u.dat", pathDestDir, entry.GetID() );
			}
			#endif
			
			file = fopen( pathArchive, "rb" );
			if( ! file ) THROW( dueFileNotFound );
			fseek( file, ( long int )entry.GetOffset(), SEEK_SET );
			data = new char[ entry.GetSize() ];
			if( ! data ) THROW( dueOutOfMemory );
			if( fread( data, 1, entry.GetSize(), file ) < entry.GetSize() ) THROW( dueReadFile );
			fclose( file );
			file = NULL;
			
			file = fopen( pathFile, "wb" );
			if( ! file ) THROW( dueWriteFile );
			if( fwrite( data, 1, entry.GetSize(), file ) < entry.GetSize() ) THROW( dueWriteFile );
			fclose( file );
			file = NULL;
			
			delete [] data;
			data = NULL;
			
			printf( "File extracted ( ID=%u Size=%u File=%s )\n", entry.GetID(), entry.GetSize(), pathFile );
		}
		
		delete [] pathFile;
		
	}catch( duException e ){
		if( pathFile ) delete [] pathFile;
		if( data ) delete [] data;
		if( file ) fclose( file );
		e.PrintError();
		return 1;
	}
	
	return 0;
}
