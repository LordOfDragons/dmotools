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



int diffFileMap( const char *pathFileMap1, const char *pathFileMap2 ){
	const dmotFileMapEntry *entry2;
	dmotFileMap filemap1;
	dmotFileMap filemap2;
	int i, count;
	
	try{
		filemap1.ReadFromFile( pathFileMap1 );
		filemap2.ReadFromFile( pathFileMap2 );
		
		printf( "ChangeType,ID\n" );
		count = filemap1.GetEntryCount();
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry1 = *filemap1.GetEntryAt( i );
			
			entry2 = filemap2.GetEntryWithID( entry1.GetID() );
			if( entry2 ){
				if( entry1.GetSize() != entry2->GetSize() ){
					printf( "Changed,%u\n", entry1.GetID() );
				}
				
			}else{
				printf( "Removed,%u\n", entry1.GetID() );
			}
		}
		
		count = filemap2.GetEntryCount();
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry1 = *filemap2.GetEntryAt( i );
			
			entry2 = filemap1.GetEntryWithID( entry1.GetID() );
			if( ! entry2 ){
				printf( "Added,%u\n", entry1.GetID() );
			}
		}
		
	}catch( duException e ){
		e.PrintError();
		return 1;
	}
	
	return 0;
}

int extractDiff( const char *pathFileMap1, const char *pathFileMap2, const char *pathArchive2, const char *pathDestDir ){
	const dmotFileMapEntry *entry2;
	char *pathFile = NULL;
	dmotFileMap filemap1;
	dmotFileMap filemap2;
	FILE *fileArchive = NULL;
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
		filemap1.ReadFromFile( pathFileMap1 );
		filemap2.ReadFromFile( pathFileMap2 );
		
		pathFile = new char[ strlen( pathDestDir ) + 13 + 20 ];
		if( ! pathFile ) THROW( dueOutOfMemory );
		
		fileArchive = fopen( pathArchive2, "rb" );
		if( ! fileArchive ) THROW( dueFileNotFound );
		
		count = filemap1.GetEntryCount();
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry1 = *filemap1.GetEntryAt( i );
			
			entry2 = filemap2.GetEntryWithID( entry1.GetID() );
			if( entry2 ){
				if( entry1.GetSize() != entry2->GetSize() ){
					#ifdef OS_W32
					if( hasPathSep ){
						sprintf( pathFile, "%schanged_%u.dat", pathDestDir, entry1.GetID() );
					}else{
						sprintf( pathFile, "%s\\changed_%u.dat", pathDestDir, entry1.GetID() );
					}
					#else
					if( hasPathSep ){
						sprintf( pathFile, "%schanged_%u.dat", pathDestDir, entry1.GetID() );
					}else{
						sprintf( pathFile, "%s/changed_%u.dat", pathDestDir, entry1.GetID() );
					}
					#endif
			
					fseek( fileArchive, ( long int )entry2->GetOffset(), SEEK_SET );
					data = new char[ entry2->GetSize() ];
					if( ! data ) THROW( dueOutOfMemory );
					if( fread( data, 1, entry2->GetSize(), fileArchive ) < entry2->GetSize() ) THROW( dueReadFile );
					
					file = fopen( pathFile, "wb" );
					if( ! file ) THROW( dueWriteFile );
					if( fwrite( data, 1, entry2->GetSize(), file ) < entry2->GetSize() ) THROW( dueWriteFile );
					fclose( file );
					file = NULL;
					
					delete [] data;
					data = NULL;
					
					printf( "Written %s\n", pathFile );
				}
				
			}else{
				#ifdef OS_W32
				if( hasPathSep ){
					sprintf( pathFile, "%sremoved_%u.dat", pathDestDir, entry1.GetID() );
				}else{
					sprintf( pathFile, "%s\\removed_%u.dat", pathDestDir, entry1.GetID() );
				}
				#else
				if( hasPathSep ){
					sprintf( pathFile, "%sremoved_%u.dat", pathDestDir, entry1.GetID() );
				}else{
					sprintf( pathFile, "%s/removed_%u.dat", pathDestDir, entry1.GetID() );
				}
				#endif
				
				file = fopen( pathFile, "wb" );
				if( ! file ) THROW( dueWriteFile );
				fclose( file );
				file = NULL;
				
				printf( "Written %s\n", pathFile );
			}
		}
		
		count = filemap2.GetEntryCount();
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry1 = *filemap2.GetEntryAt( i );
			
			entry2 = filemap1.GetEntryWithID( entry1.GetID() );
			if( ! entry2 ){
				#ifdef OS_W32
				if( hasPathSep ){
					sprintf( pathFile, "%sadded_%u.dat", pathDestDir, entry1.GetID() );
				}else{
					sprintf( pathFile, "%s\\added_%u.dat", pathDestDir, entry1.GetID() );
				}
				#else
				if( hasPathSep ){
					sprintf( pathFile, "%sadded_%u.dat", pathDestDir, entry1.GetID() );
				}else{
					sprintf( pathFile, "%s/added_%u.dat", pathDestDir, entry1.GetID() );
				}
				#endif
		
				fseek( fileArchive, ( long int )entry1.GetOffset(), SEEK_SET );
				data = new char[ entry1.GetSize() ];
				if( ! data ) THROW( dueOutOfMemory );
				if( fread( data, 1, entry1.GetSize(), fileArchive ) < entry1.GetSize() ) THROW( dueReadFile );
				
				file = fopen( pathFile, "wb" );
				if( ! file ) THROW( dueWriteFile );
				if( fwrite( data, 1, entry1.GetSize(), file ) < entry1.GetSize() ) THROW( dueWriteFile );
				fclose( file );
				file = NULL;
				
				delete [] data;
				data = NULL;
				
				printf( "Written %s\n", pathFile );
			}
		}
		
		fclose( fileArchive );
		fileArchive = NULL;
		
		delete [] pathFile;
		
	}catch( duException e ){
		if( pathFile ) delete [] pathFile;
		if( fileArchive ) delete [] fileArchive;
		if( data ) delete [] data;
		if( file ) fclose( file );
		e.PrintError();
		return 1;
	}
	
	return 0;
}
