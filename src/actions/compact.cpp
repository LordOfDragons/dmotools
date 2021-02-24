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
#include "../dmotools.h"



int compactArchive( const char *pathFileMap, const char *pathArchive, const char *pathDestFileMap, const char *pathDestArchive ){
	bool copyPadding = false;
	int paddingLen = 268;
	FILE *fileArchive = NULL;
	FILE *fileDestFileMap = NULL;
	FILE *fileDestArchive = NULL;
	dmotFileMap filemap;
	sFileMapHeader fheader;
	sFileMapEntry fentry;
	sFileArchiveHeader fheaderArchive;
	unsigned int offset;
	unsigned int size;
	char *padding = NULL;
	char *data = NULL;
	int i, count;
	
	try{
		filemap.ReadFromFile( pathFileMap );
		count = filemap.GetEntryCount();
		
		fileArchive = fopen( pathArchive, "rb" );
		if( ! fileArchive ) THROW( dueFileNotFound );
		
		fileDestFileMap = fopen( pathDestFileMap, "wb" );
		if( ! fileDestFileMap ) THROW( dueWriteFile );
		
		fileDestArchive = fopen( pathDestArchive, "wb" );
		if( ! fileDestArchive ) THROW( dueWriteFile );
		
		fheader.check = 16;
		fheader.count = count;
		if( fwrite( &fheader, 1, 8, fileDestFileMap ) < 8 ) THROW( dueWriteFile );
		
		fheaderArchive.check = 16;
		fheaderArchive.count = 0;
		if( fwrite( &fheaderArchive, 1, 8, fileDestArchive ) < 8 ) THROW( dueWriteFile );
		
		if( copyPadding ){
			for( i=0; i<count; i++ ){
				const dmotFileMapEntry &entry = *filemap.GetEntryAt( i );
				
				offset = ( unsigned int )( ftell( fileDestArchive ) + paddingLen );
				size = ( unsigned int )paddingLen + ( unsigned int )entry.GetSize();
				
				fseek( fileArchive, ( long int )entry.GetOffset() - paddingLen, SEEK_SET );
				
				data = new char[ size ];
				if( ! data ) THROW( dueOutOfMemory );
				
				if( fread( data, 1, size, fileArchive ) < size ) THROW( dueReadFile );
				if( fwrite( data, 1, size, fileDestArchive ) < size ) THROW( dueWriteFile );
				
				delete [] data;
				data = NULL;
				
				fentry.v1 = 1;
				fentry.v2 = entry.GetSize();
				fentry.v3 = entry.GetSize();
				fentry.v4 = entry.GetID();
				fentry.v5 = offset;
				fentry.v6 = 0;
				if( fwrite( &fentry, 1, 24, fileDestFileMap ) < 24 ) THROW( dueWriteFile );
			}
			
		}else{
			padding = new char[ paddingLen ];
			if( ! padding ) THROW( dueOutOfMemory );
			memset( padding, ' ', paddingLen );
			
			for( i=0; i<count; i++ ){
				const dmotFileMapEntry &entry = *filemap.GetEntryAt( i );
				
				if( fwrite( padding, 1, paddingLen, fileDestArchive ) < ( unsigned int )paddingLen ) THROW( dueWriteFile );
				
				offset = ftell( fileDestArchive );
				
				fseek( fileArchive, entry.GetOffset(), SEEK_SET );
				
				data = new char[ entry.GetSize() ];
				if( ! data ) THROW( dueOutOfMemory );
				
				if( fread( data, 1, entry.GetSize(), fileArchive ) < entry.GetSize() ) THROW( dueReadFile );
				if( fwrite( data, 1, entry.GetSize(), fileDestArchive ) < entry.GetSize() ) THROW( dueWriteFile );
				
				delete [] data;
				data = NULL;
				
				fentry.v1 = 1;
				fentry.v2 = entry.GetSize();
				fentry.v3 = entry.GetSize();
				fentry.v4 = entry.GetID();
				fentry.v5 = offset;
				fentry.v6 = 0;
				if( fwrite( &fentry, 1, 24, fileDestFileMap ) < 24 ) THROW( dueWriteFile );
			}
			
			delete [] padding;
			padding = NULL;
		}
		
		fclose( fileDestArchive );
		fileDestArchive = NULL;
		
		fclose( fileDestFileMap );
		fileDestFileMap = NULL;
		
		fclose( fileArchive );
		fileArchive = NULL;
		
		printf( "Created archive %s and file map %s\n", pathDestArchive, pathDestFileMap );
		
	}catch( duException e ){
		if( data ) delete [] data;
		if( padding ) delete [] padding;
		if( fileDestArchive ) fclose( fileDestArchive );
		if( fileDestFileMap ) fclose( fileDestFileMap );
		if( fileArchive ) fclose( fileArchive );
		e.PrintError();
		return 1;
	}
	
	return 0;
}
