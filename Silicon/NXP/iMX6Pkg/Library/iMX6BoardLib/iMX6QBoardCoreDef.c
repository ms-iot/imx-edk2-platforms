/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
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

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include <Ppi/ArmMpCoreInfo.h>

ARM_CORE_INFO iMX6Ppi[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value.
    // Not used with i.MX6, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },

#if FixedPcdGet32(PcdCoreCount) > 1
  {
    // Cluster 0, Core 1
    0x0, 0x1,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX6, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
#endif // FixedPcdGet32(PcdCoreCount) > 1

#if FixedPcdGet32(PcdCoreCount) > 2
  {
    // Cluster 0, Core 2
    0x0, 0x2,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX6, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },

  {
    // Cluster 0, Core 3
    0x0, 0x3,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX6, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  }
#endif // FixedPcdGet32(PcdCoreCount) > 2
};

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  // Only support one cluster
  *CoreCount    = sizeof (iMX6Ppi) / sizeof (ARM_CORE_INFO);
  ASSERT (*CoreCount == FixedPcdGet32 (PcdCoreCount));
  *ArmCoreTable = iMX6Ppi;
  return EFI_SUCCESS;
}

EFI_GUID mArmMpCoreInfoPpiGuid = ARM_MP_CORE_INFO_PPI_GUID;
ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };
EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &mArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}
