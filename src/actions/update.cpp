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

#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "../filemap/dmotFileMap.h"
#include "../filemap/dmotFileMapEntry.h"
#include "../exceptions.h"
#include "../dmotconfig.h"
#include "../dmotools.h"



struct sReplaceFile{
	unsigned int id;
	std::string filename;
	sReplaceFile( unsigned int aID, const char *aFilename ) : id( aID ), filename( aFilename ){}
};

struct fEqualID{
	unsigned int pID;
	
	fEqualID( unsigned int id ){
		pID = id;
	}
	
	bool operator()( sReplaceFile &entry ){
		return entry.id == pID;
	}
};



int updateArchive( const char *pathFileMap, const char *pathArchive, int argc, char **args ){
	bool copyPadding = false;
	std::vector<sReplaceFile> replaceFiles;
	std::vector<sReplaceFile>::const_iterator iterReplace;
	dmotFileMapEntry *updateEntry;
	int paddingLen = 268;
	FILE *fileData = NULL;
	FILE *fileArchive = NULL;
	FILE *fileTempFileMap = NULL;
	FILE *fileTempArchive = NULL;
	dmotFileMap filemap;
	sFileMapHeader fheader;
	sFileMapEntry fentry;
	sFileArchiveHeader fheaderArchive;
	unsigned int writeSize;
	unsigned int offset;
	unsigned int size;
	unsigned int id;
	unsigned int j;
	char *padding = NULL;
	char *data = NULL;
	int i, count;
	
	try{
		filemap.ReadFromFile( pathFileMap );
		count = filemap.GetEntryCount();
		
		// fill replace file list
		for( i=0; i<argc; i+=2 ){
			id = ( unsigned int )atoll( args[ i ] );
			
			updateEntry = filemap.GetEntryWithID( id );
			
			if( updateEntry ){
				printf( "Replace File with ID %u with content from file %s.\n", id, args[ i + 1 ] );
				replaceFiles.push_back( sReplaceFile( id, args[ i + 1 ] ) );
				
			}else{
				printf( "File with ID %u not found in the file map, ignored.\n", id );
			}
		}
		
		// update archive into temporary files
		fileArchive = fopen( pathArchive, "rb" );
		if( ! fileArchive ) THROW( dueFileNotFound );
		
		fileTempFileMap = tmpfile();
		if( ! fileTempFileMap ) THROW( dueWriteFile );
		
		fileTempArchive = tmpfile();
		if( ! fileTempArchive ) THROW( dueWriteFile );
		
		fheader.check = 16;
		fheader.count = count;
		if( fwrite( &fheader, 1, 8, fileTempFileMap ) < 8 ) THROW( dueWriteFile );
		
		fheaderArchive.check = 16;
		fheaderArchive.count = 0;
		if( fwrite( &fheaderArchive, 1, 8, fileTempArchive ) < 8 ) THROW( dueWriteFile );
		
		if( ! copyPadding ){
			padding = new char[ paddingLen ];
			if( ! padding ) THROW( dueOutOfMemory );
			memset( padding, ' ', paddingLen );
		}
		
		for( i=0; i<count; i++ ){
			const dmotFileMapEntry &entry = *filemap.GetEntryAt( i );
			
			if( copyPadding ){
				fseek( fileArchive, ( long int )entry.GetOffset() - paddingLen, SEEK_SET );
				
				data = new char[ paddingLen ];
				if( ! data ) THROW( dueOutOfMemory );
				
				if( fread( data, 1, paddingLen, fileArchive ) < ( unsigned int )paddingLen ) THROW( dueReadFile );
				if( fwrite( data, 1, paddingLen, fileTempArchive ) < ( unsigned int )paddingLen ) THROW( dueWriteFile );
				
				delete [] data;
				data = NULL;
				
			}else{
				if( fwrite( padding, 1, paddingLen, fileTempArchive ) < ( unsigned int )paddingLen ) THROW( dueWriteFile );
			}
			
			offset = ftell( fileTempArchive );
			
			iterReplace = std::find_if( replaceFiles.begin(), replaceFiles.end(), fEqualID( entry.GetID() ) );
			
			if( iterReplace == replaceFiles.end() ){
				fseek( fileArchive, entry.GetOffset(), SEEK_SET );
				
				data = new char[ entry.GetSize() ];
				if( ! data ) THROW( dueOutOfMemory );
				
				if( fread( data, 1, entry.GetSize(), fileArchive ) < entry.GetSize() ) THROW( dueReadFile );
				if( fwrite( data, 1, entry.GetSize(), fileTempArchive ) < entry.GetSize() ) THROW( dueWriteFile );
				
				delete [] data;
				data = NULL;
				
				writeSize = entry.GetSize();
				
			}else{
				const char *filename = ( *iterReplace ).filename.c_str();
				
				fileData = fopen( filename, "rb" );
				if( ! fileData ) THROW( dueFileNotFound );
				
				fseek( fileData, 0, SEEK_END );
				size = ( unsigned int )ftell( fileData );
				fseek( fileData, 0, SEEK_SET );
				
				data = new char[ size ];
				if( ! data ) THROW( dueOutOfMemory );
				
				if( fread( data, 1, size, fileData ) < size ) THROW( dueReadFile );
				if( fwrite( data, 1, size, fileTempArchive ) < size ) THROW( dueWriteFile );
				
				delete [] data;
				data = NULL;
				
				fclose( fileData );
				fileData = NULL;
				
				writeSize = size;
			}
			
			fentry.v1 = 1;
			fentry.v2 = writeSize;
			fentry.v3 = writeSize;
			fentry.v4 = entry.GetID();
			fentry.v5 = offset;
			fentry.v6 = 0;
			if( fwrite( &fentry, 1, 24, fileTempFileMap ) < 24 ) THROW( dueWriteFile );
		}
		
		if( padding ){
			delete [] padding;
			padding = NULL;
		}
		
		fclose( fileArchive );
		fileArchive = NULL;
		
		// copy temporary files back to the original files
		data = new char[ 1024 ];
		if( ! data ) THROW( dueOutOfMemory );
		
		fileData = fopen( pathFileMap, "wb" );
		if( ! fileData ) THROW( dueWriteFile );
		
		size = ( unsigned int )ftell( fileTempFileMap );
		fseek( fileTempFileMap, 0, SEEK_SET );
		
		for( j=0; j<size; j+=1024 ){
			if( j + 1024 > size ){
				if( fread( data, 1, size - j, fileTempFileMap ) < ( size - j ) ) THROW( dueReadFile );
				if( fwrite( data, 1, size - j, fileData ) < ( size - j ) ) THROW( dueWriteFile );
				
			}else{
				if( fread( data, 1, 1024, fileTempFileMap ) < 1024 ) THROW( dueReadFile );
				if( fwrite( data, 1, 1024, fileData ) < 1024 ) THROW( dueWriteFile );
			}
		}
		
		fclose( fileData );
		fileData = NULL;
		
		
		
		fileData = fopen( pathArchive, "wb" );
		if( ! fileData ) THROW( dueWriteFile );
		
		size = ( unsigned int )ftell( fileTempArchive );
		fseek( fileTempArchive, 0, SEEK_SET );
		
		for( j=0; j<size; j+=1024 ){
			if( j + 1024 > size ){
				if( fread( data, 1, size - j, fileTempArchive ) < ( size - j ) ) THROW( dueReadFile );
				if( fwrite( data, 1, size - j, fileData ) < ( size - j ) ) THROW( dueWriteFile );
				
			}else{
				if( fread( data, 1, 1024, fileTempArchive ) < 1024 ) THROW( dueReadFile );
				if( fwrite( data, 1, 1024, fileData ) < 1024 ) THROW( dueWriteFile );
			}
		}
		
		fclose( fileData );
		fileData = NULL;
		
		delete [] data;
		data = NULL;
		
		// clean up
		fclose( fileTempArchive );
		fileTempArchive = NULL;
		
		fclose( fileTempFileMap );
		fileTempFileMap = NULL;
		
		printf( "Updated archive %s and file map %s\n", pathArchive, pathFileMap );
		
	}catch( duException e ){
		if( data ) delete [] data;
		if( padding ) delete [] padding;
		if( fileTempArchive ) fclose( fileTempArchive );
		if( fileTempFileMap ) fclose( fileTempFileMap );
		if( fileArchive ) fclose( fileArchive );
		if( fileData ) fclose( fileData );
		e.PrintError();
		return 1;
	}
	
	return 0;
}
