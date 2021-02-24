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

#include "dmotools.h"



int printFileMap( const char *filename );
int printFileMapCSV( const char *filename );
int extractFile( const char *pathFileMap, const char *pathArchive, unsigned int fileID, const char *pathDestFile );
int extractAllFiles( const char *pathFileMap, const char *pathArchive, const char *pathDestDir );
int diffFileMap( const char *pathFileMap1, const char *pathFileMap2 );
int extractDiff( const char *pathFileMap1, const char *pathFileMap2, const char *pathArchive2, const char *pathDestDir );
int compactArchive( const char *pathFileMap, const char *pathArchive, const char *pathDestFileMap, const char *pathDestArchive );
int updateArchive( const char *pathFileMap, const char *pathArchive, int argc, char **args );

void printSyntax(){
	printf( "dmotools.exe <action> <params>\n" );
	printf( "\n" );
	printf( "dmotools.exe filemap <filename.hf>\n" );
	printf( "dmotools.exe filemapCSV <filename.hf>\n" );
	printf( "   print file map (normal or as CSV)\n" );
	printf( "\n" );
	printf( "dmotools.exe extractFile <filename.hf> <filename.pf> <fileID> <destFile>\n" );
	printf( "   extracts a file from the archives into a destination file\n" );
	printf( "\n" );
	printf( "dmotools.exe extractAllFiles <filename.hf> <filename.pf> <destDir>\n" );
	printf( "   extracts all files from the archives into files file_<ID>.dat in destDir\n" );
	printf( "\n" );
	printf( "dmotools.exe diffFileMap <filemap1.hf> <filemap2.hf>\n" );
	printf( "   show differences between two filemaps\n" );
	printf( "\n" );
	printf( "dmotools.exe extractDiff <filemap1.hf> <filemap2.hf> <filemap2.pf> <destDir>\n" );
	printf( "   extract differences between two filemaps into files <change>_<ID>.dat in\n" );
	printf( "   destDir with change being 'changed', 'added' or 'removed'\n" );
	printf( "\n" );
	printf( "dmotools.exe compactArchive <filemap.hf> <archive.pf> <destFilemap.hf> <destArchive.pf>\n" );
	printf( "   create copy of pack files by copying file data but removing unneeded padding\n" );
	printf( "\n" );
	printf( "dmotools.exe updateArchive <filemap.hf> <archive.pf> <fileID1> <pathFileData1> ...\n" );
	printf( "   update files inside pack files. can update more than one file inside a pack file in\n" );
	printf( "   one go by specificing more than on pair of fileID/pathFileData.\n" );
}

int main( int argc, char **args ){
	if( argc == 1 ){
		printSyntax();
		
	}else if( strcmp( args[ 1 ], "filemap" ) == 0 ){
		if( argc == 3 ){
			return printFileMap( args[ 2 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "filemapCSV" ) == 0 ){
		if( argc == 3 ){
			return printFileMapCSV( args[ 2 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "extractFile" ) == 0 ){
		if( argc == 6 ){
			return extractFile( args[ 2 ], args[ 3 ], ( unsigned int )atoll( args[ 4 ] ), args[ 5 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "extractAllFiles" ) == 0 ){
		if( argc == 5 ){
			return extractAllFiles( args[ 2 ], args[ 3 ], args[ 4 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "diffFileMap" ) == 0 ){
		if( argc == 4 ){
			return diffFileMap( args[ 2 ], args[ 3 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "extractDiff" ) == 0 ){
		if( argc == 6 ){
			return extractDiff( args[ 2 ], args[ 3 ], args[ 4 ], args[ 5 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "compactArchive" ) == 0 ){
		if( argc == 6 ){
			return compactArchive( args[ 2 ], args[ 3 ], args[ 4 ], args[ 5 ] );
			
		}else{
			printSyntax();
		}
		
	}else if( strcmp( args[ 1 ], "updateArchive" ) == 0 ){
		if( argc >= 6 && ( argc - 4 ) % 2 == 0 ){
			return updateArchive( args[ 2 ], args[ 3 ], argc - 4, args + 4 );
			
		}else{
			printSyntax();
		}
		
	}else{
		printSyntax();
	}
	
	return 0;
}
