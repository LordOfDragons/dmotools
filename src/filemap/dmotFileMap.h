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

#ifndef _DMOTFILEMAP_H_
#define _DMOTFILEMAP_H_

#include <vector>

class dmotFileMapEntry;



/**
 * @brief File Map.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotFileMap{
private:
	std::vector<dmotFileMapEntry*> pEntries;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new filemap. */
	dmotFileMap();
	/** Cleans up the filemap. */
	~dmotFileMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of entries. */
	int GetEntryCount() const;
	/** Determines if an entry with the given ID exists. */
	bool HasEntryWithID( unsigned int id ) const;
	/** Retrieves the entry at the given position. */
	dmotFileMapEntry *GetEntryAt( int index ) const;
	/** Retrieves the entry with the given ID or null if not found. */
	dmotFileMapEntry *GetEntryWithID( unsigned int id ) const;
	/** Adds an entry. */
	void AddEntry( dmotFileMapEntry *entry );
	/** Adds an entry. */
	void AddEntry( unsigned int id, unsigned int offset, unsigned int size );
	/** Removes an entry. */
	void RemoveEntry( dmotFileMapEntry *entry );
	/** Removes all entries. */
	void RemoveAllEntries();
	
	/** Read file map. */
	void ReadFromFile( const char *path );
	/*@}*/
};

#endif
