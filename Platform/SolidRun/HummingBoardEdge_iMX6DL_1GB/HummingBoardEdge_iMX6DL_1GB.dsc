#/** @file
# iMX6Dual Light SoM board description
# The board is iMX6DL with 1GB DRAM
#
#  Copyright (c) Microsoft Corporation. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#**/

################################################################################
#
# Board [Defines] Section
#
################################################################################

[Defines]
  DEFINE BOARD_NAME      = HummingBoardEdge_iMX6DL_1GB
  DEFINE IMX_FAMILY      = IMX6SDL
  DEFINE IMX_CHIP_TYPE   = DUALLITE
  DEFINE DRAM_SIZE       = DRAM_1GB
  BOARD_DIR             = Platform/SolidRun/$(BOARD_NAME)
  FLASH_DEFINITION      = $(BOARD_DIR)/$(BOARD_NAME).fdf

################################################################################
#
# [BuildOptions] Section
#
################################################################################
[BuildOptions]
  GCC:*_*_*_CC_FLAGS = -D$(BOARD_NAME) -DCPU_$(IMX_FAMILY) -DSOCTYPE_$(IMX_CHIP_TYPE) -Wno-unused-local-typedefs

[BuildOptions.common.EDKII.DXE_CORE,BuildOptions.common.EDKII.DXE_DRIVER,BuildOptions.common.EDKII.UEFI_DRIVER,BuildOptions.common.EDKII.UEFI_APPLICATION]
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000

[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  GCC:*_*_ARM_DLINK_FLAGS = -z common-page-size=0x1000

################################################################################
#
# Platform Description
#
################################################################################
!include Silicon/NXP/iMX6Pkg/iMX6CommonDsc.inc

[LibraryClasses.common]
  ArmPlatformLib|$(BOARD_DIR)/Library/iMX6BoardLib/iMX6BoardLib.inf

[Components.common]
  # Display Support
!if $(CONFIG_HEADLESS) == FALSE
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  !if $(IMX_FAMILY) == IMX6SX
    #
    # Use board-specific GOP for SoloX
    #
    $(BOARD_DIR)/Drivers/GraphicsOutputDxe/GraphicsOutputDxe.inf
  !else
    Silicon/NXP/iMX6Pkg/Drivers/GopDxe/GopDxe.inf
  !endif
!endif

  # ACPI Support
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  $(BOARD_DIR)/AcpiTables/AcpiTables.inf

  # SMBIOS Support
  Silicon/NXP/iMX6Pkg/Drivers/PlatformSmbiosDxe/PlatformSmbiosDxe.inf
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf

################################################################################
#
# Board PCD Sections
#
################################################################################

########################
#
# iMX6Pkg PCDs
#
########################
[PcdsFixedAtBuild.common]

# SMBIOS Type0 Strings
gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"SolidRun"
gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|0x00000001 # FirmwareRevision 0.1

# SMBIOS Type1 Strings
giMX6TokenSpaceGuid.PcdSystemFamily|L"HummingBoard"
giMX6TokenSpaceGuid.PcdSystemManufacturer|L"SolidRun"
giMX6TokenSpaceGuid.PcdSystemProductName|L"HummingBoard-Edge i4Pro"
giMX6TokenSpaceGuid.PcdSystemSkuNumber|L"SRMX6DLWT1D01GE008E00CH"
giMX6TokenSpaceGuid.PcdSystemVersionNumber|L"1.4"
giMX6TokenSpaceGuid.PcdSystemUuid|{0x18,0x00,0x5b,0x2d,0x66,0x43,0x4e,0xb4,0xaa,0x0d,0x49,0x81,0x97,0xa7,0x54,0xfd}

# SMBIOS Type2 Strings
giMX6TokenSpaceGuid.PcdBoardAssetTag|L"0"
giMX6TokenSpaceGuid.PcdBoardLocationInChassis|L"Open Board"
giMX6TokenSpaceGuid.PcdBoardManufacturer|L"SolidRun"
giMX6TokenSpaceGuid.PcdBoardProductName|L"HummingBoard-Edge i4Pro"
giMX6TokenSpaceGuid.PcdBoardVersionNumber|L"1.4"

# SMBIOS Type3 Strings
giMX6TokenSpaceGuid.PcdChassisAssetTag|L"0"
giMX6TokenSpaceGuid.PcdChassisManufacturer|L"SolidRun"
giMX6TokenSpaceGuid.PcdChassisVersionNumber|L"1.4"

# SMBIOS Type4 Strings
giMX6TokenSpaceGuid.PcdProcessorAssetTag|L"0"
giMX6TokenSpaceGuid.PcdProcessorManufacturer|L"NXP"
giMX6TokenSpaceGuid.PcdProcessorPartNumber|L"i.MX 6DualLite"
giMX6TokenSpaceGuid.PcdProcessorSocketDesignation|L"FCPBGA"
giMX6TokenSpaceGuid.PcdProcessorVersionNumber|L"1.0"

# SMBIOS Type16
giMX6TokenSpaceGuid.PcdPhysicalMemoryMaximumCapacity|0x100000 # 1GB

# SMBIOS Type17
giMX6TokenSpaceGuid.PcdMemoryBankLocation|L"Bank 0"
giMX6TokenSpaceGuid.PcdMemoryDeviceLocation|L"On SoM"

#
# USB EHCI Controller (USB_UH1)
#
giMX6TokenSpaceGuid.PcdEHCIBase|0x02184200

#
# Enable uSDHC2 and uSDHC3 Controllers.
# On HummingBoardEdge, SDCard slot is attached to uSDHC2
# with CD wired to SD2_CD_B, eMMC is attached to uSDHC3
#
giMXPlatformTokenSpaceGuid.PcdSdhc2Enable|TRUE
giMXPlatformTokenSpaceGuid.PcdSdhc2CardDetectSignal|0xFFFF # Use uSDHC internal CD circuit
giMXPlatformTokenSpaceGuid.PcdSdhc2WriteProtectSignal|0xFF00
giMXPlatformTokenSpaceGuid.PcdSdhc3Enable|FALSE

#
# UART initialization required
#
giMXPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x02020000   # UART1
giMXPlatformTokenSpaceGuid.PcdKdUartInstance|1                # UART1

#
# GPIO reset pin (PERST)
#
giMX6TokenSpaceGuid.PcdPcieResetGpio|TRUE
giMX6TokenSpaceGuid.PcdPcieResetGpioBankNumber|2
giMX6TokenSpaceGuid.PcdPcieResetGpioIoNumber|11
