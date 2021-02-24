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

#ifndef _DMOTFILEMAPENTRY_H_
#define _DMOTFILEMAPENTRY_H_



/**
 * @brief File Map Entry.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotFileMapEntry{
private:
	unsigned int pID;
	unsigned int pOffset;
	unsigned int pSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new file map entry. */
	dmotFileMapEntry( unsigned int id );
	/** Creates a new file map entry. */
	dmotFileMapEntry( unsigned int id, unsigned int offset, unsigned int size );
	/** Cleans up the file map entry. */
	~dmotFileMapEntry();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the id. */
	inline unsigned int GetID() const{ return pID; }
	/** Retrieves the offset. */
	inline unsigned int GetOffset() const{ return pOffset; }
	/** Sets the offset. */
	void SetOffset( unsigned int offset );
	/** Retrieves the size. */
	inline unsigned int GetSize() const{ return pSize; }
	/** Sets the size. */
	void SetSize( unsigned int size );
	/*@}*/
};

#endif
