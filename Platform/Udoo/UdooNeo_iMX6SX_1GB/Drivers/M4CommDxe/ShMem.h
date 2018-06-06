/** @file
*
*  Shared memory driver.
*
*  Copyright (c), Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef __SHMEM_H
#define __SHMEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------- */

typedef unsigned char UINT8;
typedef int check_uint8_bytes[(sizeof(UINT8) == 1) ? 1 : -1];
typedef unsigned int UINT32;
typedef int check_uint32_bytes[(sizeof(UINT32) == 4) ? 1 : -1];

/* ----------------------------------------------------------------------- */

#if 0
############################################################################################

|CS0|CS1|xxx|B0|B1|

CS0 = Control Structure 0
CS1 = Control Structure 1
xxx = (optional) platform-specific control space
B0  = Buffer 0
B1  = Buffer 1

Control Structure[ix]:
------------------
UINT32  Reader Ident
UINT32  Offset from CS to B[ix] Start
UINT32  Size of B[ix] in bytes
UINT32  Read Cursor offset from B[ix] Start
UINT32  Write Cursor offset from B[ix] Start
UINT32  Signal Locator
UINT32  Reserved[2]

Writers change Write Cursor value ONLY
Readers change Read Cursor value ONLY

Reader Ident - Platform specific value id of which entity is reading from this buffer

Signal Locator - Signal assertion denotes EDGE change from buffer-empty to buffer-not-empty.
                 (Only) Writers assert this signal.

############################################################################################
#endif

/* ----------------------------------------------------------------------- */

typedef struct _SHMEM_CONTROL
{
    UINT32  ReaderIdent;    
    UINT32  BufferOffset;     
    UINT32  BufferByteCount;      
    UINT32  ReadCursorOffset;
    UINT32  WriteCursorOffset;
    UINT32  SignalLocator;
    UINT32  Reserved1[2];   
} SHMEM_CONTROL;

typedef struct _SHMEM_CHANNEL
{
    SHMEM_CONTROL Control[2];
} SHMEM_CHANNEL;

/* ----------------------------------------------------------------------- *
    SETUP
 * ----------------------------------------------------------------------- */
UINT32
SHMEM_CalculateBytesNeededForChannel(
    UINT32  aExtraBytesAfterControls,
    UINT32  aBuffer0Bytes,
    UINT32  aBuffer1Bytes,
    UINT32  aAlignBuffers
    );
// Calculates linear span of bytes needed to create a channel with the
// specified sizes.

UINT32
SHMEM_CreateChannel(
    void *  apMemory,
    UINT32  aExtraBytesAfterControls,
    UINT32  aBuffer0Bytes,
    UINT32  aBuffer1Bytes,
    UINT32  aAlignBuffers
    );
// Initializes the fields in the SHMEM_CHANNEL in-place at *apMemory that 
// conforms to the sizes specified. No memory is allocated anywhere.

void
SHMEM_GetControls(
    SHMEM_CHANNEL *     apHdr,
    UINT32              aReaderIdent,
    SHMEM_CONTROL **    appRetReaderControl,
    SHMEM_CONTROL **    appRetWriterControl
    );
// find the reader identifier in the SHMEM_CHANNEL matching aReaderIdent
// and returns the appropriate reader and writer control for the reader
// matching that ident

/* ----------------------------------------------------------------------- *
    READER
 * ----------------------------------------------------------------------- */
UINT32 
SHMEM_Reader_GetBytesAvailable(
    SHMEM_CONTROL *apControl
    );

UINT32 
SHMEM_Reader_GetPtrs(
    SHMEM_CONTROL * apControl,
    void const **   appRetPtr1, 
    UINT32 *        apRetBytes1,
    void const **   appRetPtr2, 
    UINT32 *        apRetBytes2
    );

UINT32 
SHMEM_Reader_Consume(
    SHMEM_CONTROL * apControl,
    UINT32          aByteCount
    );
// returns the MINIMUM number of bytes remaining that can still be consumed
// after the operation.  if this == 0, then the buffer MAY BE empty, but if
// data was written during the consume there may be more that can be read.
// if too much data is requested then ((UINT32)-1) is returned.

/* ----------------------------------------------------------------------- *
    WRITER
 * ----------------------------------------------------------------------- */
UINT32
SHMEM_Writer_GetBytesFree(
    SHMEM_CONTROL *apControl
    );

UINT32 
SHMEM_Writer_GetPtrs(
    SHMEM_CONTROL * apControl,
    void **         appRetPtr1, 
    UINT32 *        apRetBytes1,
    void **         appRetPtr2, 
    UINT32 *        apRetBytes2
    );

UINT32 
SHMEM_Writer_Produce(
    SHMEM_CONTROL * apControl,
    UINT32          aByteCount
    );
// returns the MAXIMUM number of bytes that are availble in the buffer that
// can be consumed after the operation.  If this is == aByteCount, then 
// the buffer was empty before the operation.
// if too much data is specified then ((UINT32)-1) is returned.

/* ----------------------------------------------------------------------- *
    HELPERS
 * ----------------------------------------------------------------------- */
UINT32
SHMEM_Reader_CopyOutAndConsume(
    SHMEM_CONTROL * apControl,
    void *          apTargetBuffer,
    UINT32          aByteCount
    );

UINT32 
SHMEM_Writer_CopyInAndProduce(
    SHMEM_CONTROL * apControl,
    void const *    apSourceBuffer,
    UINT32          aByteCount
    );

UINT32
SHMEM_Read(
    SHMEM_CHANNEL * apChannel,
    UINT32          aMyIdent,
    void *          apTargetBuffer,
    UINT32          aByteCount
    );

UINT32
SHMEM_Write(
    SHMEM_CHANNEL * apChannel,
    UINT32          aMyIdent,
    void const *    apSourceBuffer,
    UINT32          aByteCount
    );

/* ----------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif // __SHMEM_H
