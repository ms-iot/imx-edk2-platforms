/** @file
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
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
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>

#include <iMX7.h>

#define MEMORY_ATTRIBUTES_PCDCACHEENABLE    -1

ARM_MEMORY_REGION_DESCRIPTOR iMX7MemoryDescriptor[] =
{
    {
        FixedPcdGet64(PcdSystemMemoryBase),
        FixedPcdGet64(PcdSystemMemoryBase),
        FixedPcdGet64(PcdSystemMemorySize),
        MEMORY_ATTRIBUTES_PCDCACHEENABLE,
    },
    {
        ARM_PERIPHERALS_REGISTERS_PHYSICAL,
        ARM_PERIPHERALS_REGISTERS_PHYSICAL,
        ARM_PERIPHERALS_REGISTERS_LENGTH,
        ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
    },
    {
        ARM_IP_BUS_REGISTERS_PHYSICAL,
        ARM_IP_BUS_REGISTERS_PHYSICAL,
        ARM_IP_BUS_REGISTERS_LENGTH,
        ARM_MEMORY_REGION_ATTRIBUTE_DEVICE
    },
    {
        PCIE_REG_REGISTER_PHYSICAL,
        PCIE_REG_REGISTER_PHYSICAL,
        PCIE_REG_REGISTER_LENGTH,
        ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
    },
    {
        //
        // End of table
        //
        0,
        0,
        0,
        0,
    },
};

#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS (sizeof(iMX7MemoryDescriptor) / sizeof(iMX7MemoryDescriptor[0]))

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR describing a Physical-to-
                                    Virtual Memory mapping. This array must be ended by a zero-filled
                                    entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
    IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
)
{
    ARM_MEMORY_REGION_ATTRIBUTES cacheAttributes;
    UINTN index;
    ARM_MEMORY_REGION_DESCRIPTOR *virtualMemoryTable;

    ASSERT(VirtualMemoryMap != NULL);

    DEBUG((EFI_D_VERBOSE, "Enter: ArmPlatformGetVirtualMemoryMap\n"));

    virtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)AllocatePages(EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR) * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
    if (virtualMemoryTable == NULL) {
        return;
    }

    cacheAttributes = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    DEBUG((EFI_D_VERBOSE, "cacheAttributes=0x%d\n", cacheAttributes));

    for (index = 0; index < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS; index++) {

        virtualMemoryTable[index].PhysicalBase = iMX7MemoryDescriptor[index].PhysicalBase;
        virtualMemoryTable[index].VirtualBase = iMX7MemoryDescriptor[index].VirtualBase;
        virtualMemoryTable[index].Length = iMX7MemoryDescriptor[index].Length;

        if(iMX7MemoryDescriptor[index].Attributes == MEMORY_ATTRIBUTES_PCDCACHEENABLE) {
            virtualMemoryTable[index].Attributes = cacheAttributes;
        } else {
            virtualMemoryTable[index].Attributes = iMX7MemoryDescriptor[index].Attributes;
        }
    }

    ASSERT((index) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

    // Reserve OP-TEE private memory
    BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        EFI_RESOURCE_ATTRIBUTE_PRESENT |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        EFI_RESOURCE_ATTRIBUTE_TESTED,
        FixedPcdGet32(PcdTrustZonePrivateMemoryBase),
        FixedPcdGet32(PcdTrustZonePrivateMemorySize)
    );

    // Reserve TPM2 Control Area
    BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        EFI_RESOURCE_ATTRIBUTE_PRESENT |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        EFI_RESOURCE_ATTRIBUTE_TESTED,
        FixedPcdGet32(PcdTpm2AcpiBufferBase),
        FixedPcdGet32(PcdTpm2AcpiBufferSize)
    );

    // Reserve Global Data area
    BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        EFI_RESOURCE_ATTRIBUTE_PRESENT |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        EFI_RESOURCE_ATTRIBUTE_TESTED,
        FixedPcdGet32(PcdGlobalDataBaseAddress),
        FixedPcdGet32(PcdGlobalDataSize)
    );

    *VirtualMemoryMap = virtualMemoryTable;
}

