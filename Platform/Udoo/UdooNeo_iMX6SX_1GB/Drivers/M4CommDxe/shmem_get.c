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

void
SHMEM_GetControls(
    SHMEM_CHANNEL *     apHdr,
    UINT32              aReaderIdent,
    SHMEM_CONTROL **    appRetReaderControl,
    SHMEM_CONTROL **    appRetWriterControl
    )
{
    if (appRetReaderControl != NULL)
    {
        if (apHdr->Control[0].ReaderIdent == aReaderIdent)
            *appRetReaderControl = &apHdr->Control[0];
        else
            *appRetReaderControl = &apHdr->Control[1];
    }

    if (appRetWriterControl != NULL)
    {
        if (apHdr->Control[0].ReaderIdent == aReaderIdent)
            *appRetWriterControl = &apHdr->Control[1];
        else
            *appRetWriterControl = &apHdr->Control[0];
    }
}

