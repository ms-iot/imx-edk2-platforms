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

#include "shmemp.h"

UINT32 
SHMEM_Reader_GetBytesAvailable(
    SHMEM_CONTROL *apControl
    )
{
    UINT32 ret;
    UINT32 snapshotWrite;
    UINT32 readOffset;

    snapshotWrite = apControl->WriteCursorOffset;
    MEMORY_READ_BARRIER;

    readOffset = apControl->ReadCursorOffset;

    if (snapshotWrite >= readOffset)
        ret = snapshotWrite - readOffset;
    else
        ret = (apControl->BufferByteCount - readOffset) + snapshotWrite;
    
    return ret;
}

UINT32 
SHMEM_Reader_GetPtrs(
    SHMEM_CONTROL * apControl,
    void const **   appRetPtr1, 
    UINT32 *        apRetBytes1,
    void const **   appRetPtr2, 
    UINT32 *        apRetBytes2
    )
{
    UINT32  available;
    UINT32  snapshotWrite;
    UINT32  readOffset;
    UINT8 * pBuffer;

    snapshotWrite = apControl->WriteCursorOffset;
    MEMORY_READ_BARRIER;

    readOffset = apControl->ReadCursorOffset;

    if (snapshotWrite >= readOffset)
        available = snapshotWrite - readOffset;
    else
        available = (apControl->BufferByteCount- readOffset) + snapshotWrite;

    if (available == 0)
    {
        *appRetPtr1 = NULL;
        *apRetBytes1 = 0;
        *appRetPtr2 = NULL;
        *apRetBytes2 = 0;
        return 0;
    }

    pBuffer = ((UINT8 *)apControl) + apControl->BufferOffset;

    *appRetPtr1 =  pBuffer + readOffset;
    if (readOffset < snapshotWrite)
    {
        *apRetBytes1 = snapshotWrite - readOffset;
        *appRetPtr2 = NULL;
        *apRetBytes2 = 0;
    }
    else
    {
        *apRetBytes1 = apControl->BufferByteCount - readOffset;
        *appRetPtr2 = pBuffer;
        *apRetBytes2 = snapshotWrite;
    }

    return available;
}

UINT32 
SHMEM_Reader_Consume(
    SHMEM_CONTROL * apControl,
    UINT32          aByteCount
    )
{
    UINT32 tailData;
    UINT32 available;
    UINT32 readOffset;
    UINT32 snapshotWrite;

    snapshotWrite = apControl->WriteCursorOffset;
    MEMORY_READ_BARRIER;

    readOffset = apControl->ReadCursorOffset;

    if (snapshotWrite >= readOffset)
        available = snapshotWrite - readOffset;
    else
        available = (apControl->BufferByteCount - readOffset) + snapshotWrite;

    if (aByteCount > available)
        return (UINT32)-1;

    if (readOffset >= snapshotWrite)
    {
        tailData = apControl->BufferByteCount - readOffset;

        if (aByteCount >= tailData)
        {
            readOffset = 0;
            aByteCount -= tailData;
            available -= tailData;
        }
    }

    //
    // Consumer moves read pointer
    //
    MEMORY_WRITE_BARRIER;
    apControl->ReadCursorOffset = readOffset + aByteCount;
    MEMORY_WRITE_BARRIER;

    return available - aByteCount;
}

