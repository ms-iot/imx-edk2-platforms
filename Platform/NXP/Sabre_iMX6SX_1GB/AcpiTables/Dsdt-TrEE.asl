/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*  Copyright 2019 NXP
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

Device (TREE)
{
  Name (_HID, "NXP010F")
  Name (_CID, "TrEECSMP")
  Name (_UID, 0)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    // OP-TEE Shared memory area.
    // Needs to match: PcdTrustZoneSharedMemoryBase, PcdTrustZoneSharedMemorySize
    MEMORY32FIXED (ReadWrite, 0x81E00000, 0x00200000, )
  })
}
