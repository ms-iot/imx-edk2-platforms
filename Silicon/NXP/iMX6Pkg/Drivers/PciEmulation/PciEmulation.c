//
// Copyright (C) Microsoft. All rights reserved
//
/** @file

  Copyright (c) 2008-2009, Apple Inc. All rights reserved.

  All rights reserved. This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/NonDiscoverableDeviceRegistrationLib.h>

EFI_STATUS
EFIAPI
PciEmulationEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  return RegisterNonDiscoverableMmioDevice (
           NonDiscoverableDeviceTypeEhci,
           NonDiscoverableDeviceDmaTypeNonCoherent,
           NULL,
           NULL,
           1,
           FixedPcdGet32(PcdEHCIBase) + 0x100, FixedPcdGet32(PcdEHCILength) - 0x100
           );
}
