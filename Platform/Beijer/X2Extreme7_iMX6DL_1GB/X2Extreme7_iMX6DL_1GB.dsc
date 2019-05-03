#/** @file
# iMX6Dual Lite SoM board description
# The board is iMX6DL with 1GB DRAM
#
#  Copyright (c) 2019 Microsoft Corporation. All rights reserved.
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
  DEFINE BOARD_NAME      = X2Extreme7_iMX6DL_1GB
  DEFINE IMX_FAMILY      = IMX6DL
  DEFINE DRAM_SIZE       = DRAM_1GB
  BOARD_DIR              = Platform/Beijer/$(BOARD_NAME)
  FLASH_DEFINITION       = $(BOARD_DIR)/$(BOARD_NAME).fdf

################################################################################
#
# [BuildOptions] Section
#
################################################################################
[BuildOptions]
  GCC:*_*_*_CC_FLAGS = -D$(BOARD_NAME) -DCPU_$(IMX_FAMILY) -Wno-unused-local-typedefs

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
gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Beijer"
gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|0x00000001 # FirmwareRevision 0.1

# SMBIOS Type1 Strings
giMX6TokenSpaceGuid.PcdSystemFamily|L"X2"
giMX6TokenSpaceGuid.PcdSystemManufacturer|L"Beijer"
giMX6TokenSpaceGuid.PcdSystemProductName|L"X2 Extreme 7"
giMX6TokenSpaceGuid.PcdSystemSkuNumber|L"640013905"
giMX6TokenSpaceGuid.PcdSystemVersionNumber|L"1"
giMX6TokenSpaceGuid.PcdSystemUuid|{0xee,0x25,0x1b,0x24,0xf0,0xac,0x44,0x44,0xb6,0xe5,0x4a,0xa6,0x2b,0x22,0x80,0x78}

# SMBIOS Type2 Strings
giMX6TokenSpaceGuid.PcdBoardAssetTag|L"0"
giMX6TokenSpaceGuid.PcdBoardLocationInChassis|L"Open Board"
giMX6TokenSpaceGuid.PcdBoardManufacturer|L"Beijer"
giMX6TokenSpaceGuid.PcdBoardProductName|L"X2 Extreme 7"
giMX6TokenSpaceGuid.PcdBoardVersionNumber|L"1.0"

# SMBIOS Type3 Strings
giMX6TokenSpaceGuid.PcdChassisAssetTag|L"0"
giMX6TokenSpaceGuid.PcdChassisManufacturer|L"Beijer"
giMX6TokenSpaceGuid.PcdChassisVersionNumber|L"1.0"

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
# Enable uSDHC2 Controllers.
# SDCard slot is attached to uSDHC2
#
giMXPlatformTokenSpaceGuid.PcdSdhc2Enable|TRUE
giMXPlatformTokenSpaceGuid.PcdSdhc2CardDetectSignal|0xFFFF # Use uSDHC internal CD circuit
giMXPlatformTokenSpaceGuid.PcdSdhc2WriteProtectSignal|0xFF00

#
# UART initialization required
#
giMXPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x021E8000   # UART2
giMXPlatformTokenSpaceGuid.PcdKdUartInstance|2                # UART2

#
# GPIO reset pin (PERST)
#
giMX6TokenSpaceGuid.PcdPcieResetGpio|TRUE
giMX6TokenSpaceGuid.PcdPcieResetGpioBankNumber|2
giMX6TokenSpaceGuid.PcdPcieResetGpioIoNumber|11

[PcdsFeatureFlag.common]
giMX6TokenSpaceGuid.PcdLvdsEnable|TRUE
giMX6TokenSpaceGuid.PcdUseDefaultX2LvdsPanel|TRUE
giMX6TokenSpaceGuid.PcdGopHorizontalFlip|TRUE
giMX6TokenSpaceGuid.PcdGopVerticalFlip|TRUE
