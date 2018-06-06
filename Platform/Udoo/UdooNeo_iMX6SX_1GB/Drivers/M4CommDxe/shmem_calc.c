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
SHMEM_CalculateBytesNeededForChannel(
    UINT32  aExtraBytesAfterControls,
    UINT32  aBuffer0Bytes,
    UINT32  aBuffer1Bytes,
    UINT32  aAlignBuffers
    )
{
    UINT32 count;

    count = sizeof(SHMEM_CHANNEL);

    count += aExtraBytesAfterControls;
    count = (count + 3) & ~3;
    if (aAlignBuffers > 0)
        count = (((count + (aAlignBuffers - 1)) / aAlignBuffers) * aAlignBuffers);

    count += aBuffer0Bytes;
    count = (count + 3) & ~3;
    if (aAlignBuffers > 0)
        count = (((count + (aAlignBuffers - 1)) / aAlignBuffers) * aAlignBuffers);

    count += aBuffer1Bytes;
    count = (count + 3) & ~3;
    if (aAlignBuffers > 0)
        count = (((count + (aAlignBuffers - 1)) / aAlignBuffers) * aAlignBuffers);

    return count;
}

