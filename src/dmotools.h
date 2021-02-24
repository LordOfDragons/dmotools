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

#ifndef _DMOTOOLS_H_
#define _DMOTOOLS_H_

struct sFileMapHeader{
	unsigned int check;
	unsigned int count;
};

struct sFileMapEntry{
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
	unsigned int v4;
	unsigned int v5;
	unsigned int v6;
};

struct sFileArchiveHeader{
	unsigned int check;
	unsigned int count;
};

#endif
