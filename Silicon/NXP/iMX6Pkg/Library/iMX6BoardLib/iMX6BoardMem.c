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
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

#include "iMX6.h"

#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS  8

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR describing
                                    a Physical-to-Virtual Memory mapping. This
                                    array must be ended by a zero-filled entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR **VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes;
  UINTN                         Index = 0;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;

  ASSERT (VirtualMemoryMap != NULL);

  DEBUG ((DEBUG_VERBOSE, "Enter: ArmPlatformGetVirtualMemoryMap\n"));

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR *)AllocatePages (
                         EFI_SIZE_TO_PAGES (sizeof (ARM_MEMORY_REGION_DESCRIPTOR) *
                             MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
    return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;
  DEBUG ((DEBUG_VERBOSE, "CacheAttributes=0x%d\n", CacheAttributes));
#if defined(CPU_IMX6ULL)
  VirtualMemoryTable[Index].PhysicalBase   = SOC_REGISTERS_PHYSICAL_BASE1;
  VirtualMemoryTable[Index].VirtualBase    = SOC_REGISTERS_PHYSICAL_BASE1;
  VirtualMemoryTable[Index].Length         = SOC_REGISTERS_PHYSICAL_LENGTH1;
  VirtualMemoryTable[Index].Attributes     = SOC_REGISTERS_ATTRIBUTES;

  VirtualMemoryTable[++Index].PhysicalBase = APBH_DMA_REGISTERS_PHYSICAL_BASE;
  VirtualMemoryTable[Index].VirtualBase    = APBH_DMA_REGISTERS_PHYSICAL_BASE;
  VirtualMemoryTable[Index].Length         = APBH_DMA_REGISTERS_PHYSICAL_LENGTH;
  VirtualMemoryTable[Index].Attributes     = SOC_REGISTERS_ATTRIBUTES;
  VirtualMemoryTable[++Index].PhysicalBase = GIC_REGISTERS_PHYSICAL_BASE;
  VirtualMemoryTable[Index].VirtualBase    = GIC_REGISTERS_PHYSICAL_BASE;
  VirtualMemoryTable[Index].Length         = GIC_REGISTERS_PHYSICAL_LENGTH;
  VirtualMemoryTable[Index].Attributes     = SOC_REGISTERS_ATTRIBUTES;
#else
  // SOC registers region 1 (0x00100000 size 0x00C00000)
  VirtualMemoryTable[Index].PhysicalBase   = SOC_REGISTERS_PHYSICAL_BASE1;
  VirtualMemoryTable[Index].VirtualBase    = SOC_REGISTERS_PHYSICAL_BASE1;
  VirtualMemoryTable[Index].Length         = SOC_REGISTERS_PHYSICAL_LENGTH1;
  VirtualMemoryTable[Index].Attributes     = SOC_REGISTERS_ATTRIBUTES;

  // PCIE Registers (0x01000000 size 0x01000000)
  VirtualMemoryTable[++Index].PhysicalBase = PCIE_REGISTERS_PHYSICAL_BASE;
  VirtualMemoryTable[Index].VirtualBase    = PCIE_REGISTERS_PHYSICAL_BASE;
  VirtualMemoryTable[Index].Length         = PCIE_REGISTERS_PHYSICAL_LENGTH;
  VirtualMemoryTable[Index].Attributes     = SOC_REGISTERS_ATTRIBUTES;

  // SOC registers region 2 (excluding EIM) (0x02000000 size 0x00A00000)
  VirtualMemoryTable[++Index].PhysicalBase = SOC_REGISTERS_PHYSICAL_BASE2;
  VirtualMemoryTable[Index].VirtualBase    = SOC_REGISTERS_PHYSICAL_BASE2;
  VirtualMemoryTable[Index].Length         = SOC_REGISTERS_PHYSICAL_LENGTH2;
  VirtualMemoryTable[Index].Attributes     = SOC_REGISTERS_ATTRIBUTES;
#endif

  // Framebuffer
  VirtualMemoryTable[++Index].PhysicalBase = FixedPcdGet32 (PcdFrameBufferBase);
  VirtualMemoryTable[Index].VirtualBase    = FixedPcdGet32 (PcdFrameBufferBase);
  VirtualMemoryTable[Index].Length         = FixedPcdGet32 (PcdFrameBufferSize);
  VirtualMemoryTable[Index].Attributes     = DDR_ATTRIBUTES_UNCACHED;

  // Boot (UEFI) DRAM region (kernel.img & boot working DRAM) (0x10800000 size 0x001D0000)
  VirtualMemoryTable[++Index].PhysicalBase   = BOOT_IMAGE_PHYSICAL_BASE;
  VirtualMemoryTable[Index].VirtualBase      = BOOT_IMAGE_PHYSICAL_BASE;
  VirtualMemoryTable[Index].Length           = BOOT_IMAGE_PHYSICAL_LENGTH;
  VirtualMemoryTable[Index].Attributes       = BOOT_IMAGE_ATTRIBUTES;

  // TrustZone Shared Memory
  VirtualMemoryTable[++Index].PhysicalBase = FixedPcdGet64 (PcdTrustZoneSharedMemoryBase);
  VirtualMemoryTable[Index].VirtualBase    = FixedPcdGet64 (PcdTrustZoneSharedMemoryBase);
  VirtualMemoryTable[Index].Length         = FixedPcdGet64 (PcdTrustZoneSharedMemorySize);
  VirtualMemoryTable[Index].Attributes     = CacheAttributes;

  // Base SDRAM
  VirtualMemoryTable[++Index].PhysicalBase = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase    = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].Length         = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes     = CacheAttributes;

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase = 0;
  VirtualMemoryTable[Index].VirtualBase    = 0;
  VirtualMemoryTable[Index].Length         = 0;
  VirtualMemoryTable[Index].Attributes     = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT ((Index + 1) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap = VirtualMemoryTable;
}
