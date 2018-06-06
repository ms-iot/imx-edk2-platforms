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

#define STRUCTURE_FIELD_OFFSET(type, field)   ((UINT32)(UINT32 *)&(((type *)0)->field))

UINT32
SHMEM_CreateChannel(
    void *  apMemory,
    UINT32  aExtraBytesAfterControls,
    UINT32  aBuffer0Bytes,
    UINT32  aBuffer1Bytes,
    UINT32  aAlignBuffers
    )
{
    SHMEM_CHANNEL * pHdr;
    UINT32          offset;

    pHdr = (SHMEM_CHANNEL *)apMemory;

	memset(pHdr, 0, sizeof(SHMEM_CHANNEL));
    
    offset = sizeof(SHMEM_CHANNEL);
    offset += aExtraBytesAfterControls;
    offset = (offset + 3) & ~3;
    if (aAlignBuffers > 0)
        offset = (((offset + (aAlignBuffers - 1)) / aAlignBuffers) * aAlignBuffers);

    pHdr->Control[0].BufferOffset = offset;
    pHdr->Control[0].BufferByteCount = aBuffer0Bytes;

    offset += aBuffer0Bytes;
    offset = (offset + 3) & ~3;
    if (aAlignBuffers > 0)
        offset = (((offset + (aAlignBuffers - 1)) / aAlignBuffers) * aAlignBuffers);

    pHdr->Control[1].BufferOffset = offset - STRUCTURE_FIELD_OFFSET(SHMEM_CHANNEL, Control[1]);
    pHdr->Control[1].BufferByteCount = aBuffer1Bytes;

    return 1;
}

