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
SHMEM_Writer_CopyInAndProduce(
    SHMEM_CONTROL * apControl,
    void const *    apSourceBuffer,
    UINT32          aByteCount
    )
{
    void *pBuf1;
    void *pBuf2;
    UINT32 sizeBuf1;
    UINT32 sizeBuf2;
    UINT32 amount;

    if (aByteCount == 0)
        return 0;

    amount = SHMEM_Writer_GetPtrs(apControl, &pBuf1, &sizeBuf1, &pBuf2, &sizeBuf2);
    if (amount < aByteCount)
        return 0;

    if (sizeBuf1 < aByteCount)
        amount = sizeBuf1;
    else
        amount = aByteCount;

    memcpy(pBuf1, apSourceBuffer, amount);

    if (amount != aByteCount)
        memcpy(pBuf2, ((UINT8 *)apSourceBuffer) + amount, aByteCount - amount);

    return SHMEM_Writer_Produce(apControl, aByteCount);
}

UINT32
SHMEM_Write(
    SHMEM_CHANNEL * apChannel,
    UINT32          aMyIdent,
    void const *    apSourceBuffer,
    UINT32          aByteCount
    )
{
    SHMEM_CONTROL * pWriterControl;

    SHMEM_GetControls(apChannel, aMyIdent, NULL, &pWriterControl);

    return SHMEM_Writer_CopyInAndProduce(pWriterControl, apSourceBuffer, aByteCount);
}

