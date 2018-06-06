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
SHMEM_Writer_GetBytesFree(
    SHMEM_CONTROL *apControl
    )
{
    UINT32 ret;
    UINT32 snapshotRead;
    UINT32 writeOffset;

    snapshotRead = apControl->ReadCursorOffset;
    MEMORY_READ_BARRIER;

    writeOffset = apControl->WriteCursorOffset;

    if (snapshotRead <= writeOffset)
        ret = (apControl->BufferByteCount - writeOffset) + snapshotRead;
    else
        ret = snapshotRead - writeOffset;

    return ret - 1;
}

UINT32 
SHMEM_Writer_GetPtrs(
    SHMEM_CONTROL * apControl,
    void **         appRetPtr1, 
    UINT32 *        apRetBytes1,
    void **         appRetPtr2, 
    UINT32 *        apRetBytes2
    )
{
    UINT32  space;
    UINT32  snapshotRead;
    UINT32  writeOffset;
    UINT8 * pBuffer;

    snapshotRead = apControl->ReadCursorOffset;
    MEMORY_READ_BARRIER;

    writeOffset = apControl->WriteCursorOffset;

    if (snapshotRead <= writeOffset)
        space = (apControl->BufferByteCount - writeOffset) + snapshotRead;
    else
        space = snapshotRead - writeOffset;

    if (space <= 1)
    {
        *appRetPtr1 = NULL;
        *apRetBytes1 = 0;
        *appRetPtr2 = NULL;
        *apRetBytes2 = 0;
        return 0;
    }
    --space;

    pBuffer = ((UINT8 *)apControl) + apControl->BufferOffset;

    *appRetPtr1 = pBuffer + writeOffset;
    if (snapshotRead > writeOffset)
    {
        *apRetBytes1 = (snapshotRead - writeOffset) - 1;
        *appRetPtr2 = NULL;
        *apRetBytes2 = 0;
    }
    else
    {
        *apRetBytes1 = (apControl->BufferByteCount - writeOffset);
        if (snapshotRead == 0)
        {
            (*apRetBytes1)--;
            *appRetPtr2 = NULL;
			*apRetBytes2 = 0;
        }
        else
        {
            *appRetPtr2 = pBuffer;
            *apRetBytes2 = snapshotRead - 1;
        }
    }

    return space;
}

UINT32 
SHMEM_Writer_Produce(
    SHMEM_CONTROL * apControl,
    UINT32          aByteCount
    )
{
    UINT32 tailSpace;
    UINT32 space;
    UINT32 snapshotRead;
    UINT32 writeOffset;
    UINT32 dataInBuffer;

    snapshotRead = apControl->ReadCursorOffset;
    MEMORY_READ_BARRIER;

    writeOffset = apControl->WriteCursorOffset;

    if (snapshotRead <= writeOffset)
        space = (apControl->BufferByteCount - writeOffset) + snapshotRead;
    else
        space = snapshotRead - writeOffset;

    dataInBuffer = apControl->BufferByteCount - space;

    space--;

    if (aByteCount > space)
        return (UINT32)-1;

    tailSpace = (apControl->BufferByteCount - writeOffset);

    if (aByteCount >= tailSpace)
    {
        writeOffset = 0;
        aByteCount -= tailSpace;
        dataInBuffer += tailSpace;
    }

    //
    // Producer moves write pointer
    //
    MEMORY_WRITE_BARRIER;
    apControl->WriteCursorOffset = writeOffset + aByteCount;
    MEMORY_WRITE_BARRIER;

    return dataInBuffer + aByteCount;
}

