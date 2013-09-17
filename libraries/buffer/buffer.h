//
// buffer.h : Multipurpose byte buffer structure and methods
//
//	This byte-buffer structure provides an easy and efficient way to store
//	and process a stream of bytes. You can create as many buffers as you
//	like (within memory limits), and then use this common set of functions to
//	access each buffer. The buffers are designed for FIFO operation (first
//	in, first out). This means that the first byte you put in the buffer
//	will be the first one you get when you read out the buffer. Supported
//	functions include buffer initialize, get byte from front of buffer, add
//	byte to end of buffer, check if buffer is full, and flush buffer. The
//	buffer uses a circular design so no copying of data is ever necessary.
//	This buffer is not dynamically allocated, it has a user-defined fixed
//	maximum size. This buffer is used in many places in the avrlib code.
//
// Copyright (c) 2001-2002 Pascal Stang
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id$

#ifndef BUFFER_H
#define BUFFER_H

// structure/typdefs
#ifdef __cplusplus
extern "C"{
#endif
//! cBuffer structure
typedef struct struct_cBuffer
{
	unsigned char *dataptr;			///< the physical memory address where the buffer is stored
	unsigned short size;			///< the allocated size of the buffer
	unsigned short datalength;		///< the length of the data currently in the buffer
	unsigned short dataindex;		///< the index into the buffer where the data starts
} cBuffer;

// function prototypes

//! initialize a buffer to start at a given address and have given size
void bufferInit(cBuffer* buffer, unsigned char *start, unsigned short size);

//! get the first byte from the front of the buffer
unsigned char	bufferGetFromFront(cBuffer* buffer);

//! dump (discard) the first numbytes from the front of the buffer
void bufferDumpFromFront(cBuffer* buffer, unsigned short numbytes);

//! get a byte at the specified index in the buffer (kind of like array access)
// ** note: this does not remove the byte that was read from the buffer
unsigned char	bufferGetAtIndex(cBuffer* buffer, unsigned short index);

//! add a byte to the end of the buffer
unsigned char	bufferAddToEnd(cBuffer* buffer, unsigned char data);

//! check if the buffer is full/not full (returns zero value if full)
unsigned short	bufferIsNotFull(cBuffer* buffer);

//! flush (clear) the contents of the buffer
void			bufferFlush(cBuffer* buffer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

