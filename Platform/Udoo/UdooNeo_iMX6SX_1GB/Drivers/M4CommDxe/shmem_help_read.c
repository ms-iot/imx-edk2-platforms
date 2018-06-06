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
SHMEM_Reader_CopyOutAndConsume(
    SHMEM_CONTROL * apControl,
    void *          apTargetBuffer,
    UINT32          aByteCount
    )
{
    void const *pBuf1;
    void const *pBuf2;
    UINT32 sizeBuf1;
    UINT32 sizeBuf2;
    UINT32 amount;

    if (aByteCount == 0)
        return 0;

    amount = SHMEM_Reader_GetPtrs(apControl, &pBuf1, &sizeBuf1, &pBuf2, &sizeBuf2);
    if (amount < aByteCount)
        return (UINT32)-1;

    if (sizeBuf1 < aByteCount)
        amount = sizeBuf1;
    else
        amount = aByteCount;

    memcpy(apTargetBuffer, pBuf1, amount);

    if (amount != aByteCount)
        memcpy(((UINT8 *)apTargetBuffer) + amount, pBuf2, aByteCount - amount);

    return SHMEM_Reader_Consume(apControl, aByteCount);
}

UINT32
SHMEM_Read(
    SHMEM_CHANNEL * apChannel,
    UINT32          aMyIdent,
    void *          apTargetBuffer,
    UINT32          aByteCount
    )
{
    SHMEM_CONTROL * pReaderControl;

    SHMEM_GetControls(apChannel, aMyIdent, &pReaderControl, NULL);

    return SHMEM_Reader_CopyOutAndConsume(pReaderControl, apTargetBuffer, aByteCount);
}

