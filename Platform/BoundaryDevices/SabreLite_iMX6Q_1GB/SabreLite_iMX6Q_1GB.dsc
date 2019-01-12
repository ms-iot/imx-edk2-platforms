#
# iMX6Q SabreLite board description
# The board is iMX6Q with 1GB DRAM
#
#  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

################################################################################
#
# Board [Defines] Section
#
################################################################################

[Defines]
  DEFINE BOARD_NAME     = SabreLite_iMX6Q_1GB
  DEFINE IMX_FAMILY     = IMX6Q
  DEFINE DRAM_SIZE      = DRAM_1GB
  BOARD_DIR             = Platform/BoundaryDevices/$(BOARD_NAME)
  FLASH_DEFINITION      = $(BOARD_DIR)/$(BOARD_NAME).fdf

  DEFINE CONFIG_NOT_SECURE_UEFI = 1

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
gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"BoundaryDevices"
gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|0x00000001 # FirmwareRevision 0.1

# SMBIOS Type1 Strings
giMX6TokenSpaceGuid.PcdSystemFamily|L"SabreLite"
giMX6TokenSpaceGuid.PcdSystemManufacturer|L"BoundaryDevices"
giMX6TokenSpaceGuid.PcdSystemProductName|L"SabreLite i.MX 6Quad"
giMX6TokenSpaceGuid.PcdSystemSkuNumber|L"BD-SL-i.MX6"
giMX6TokenSpaceGuid.PcdSystemVersionNumber|L"1.0"
giMX6TokenSpaceGuid.PcdSystemUuid|{0xd3,0x27,0x4a,0x32,0xcc,0x29,0x48,0x06,0x82,0xa7,0x3a,0x13,0x6c,0xf3,0xff,0x7d}

# SMBIOS Type2 Strings
giMX6TokenSpaceGuid.PcdBoardAssetTag|L"0"
giMX6TokenSpaceGuid.PcdBoardLocationInChassis|L"Open Board"
giMX6TokenSpaceGuid.PcdBoardManufacturer|L"BoundaryDevices"
giMX6TokenSpaceGuid.PcdBoardProductName|L"SabreLite i.MX 6Quad"
giMX6TokenSpaceGuid.PcdBoardVersionNumber|L"3.0"

# SMBIOS Type3 Strings
giMX6TokenSpaceGuid.PcdChassisAssetTag|L"0"
giMX6TokenSpaceGuid.PcdChassisManufacturer|L"BoundaryDevices"
giMX6TokenSpaceGuid.PcdChassisVersionNumber|L"1.0"

# SMBIOS Type4 Strings
giMX6TokenSpaceGuid.PcdProcessorAssetTag|L"0"
giMX6TokenSpaceGuid.PcdProcessorManufacturer|L"NXP"
giMX6TokenSpaceGuid.PcdProcessorPartNumber|L"i.MX 6Quad"
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
# giMX6TokenSpaceGuid.PcdEHCIBase|0x02184200


#
# uSDHC Controllers as connected on iMX6DQ SabreLite
#
# uSDHCx | SabreLite On-board Connection | Physical Base
#-----------------------------------------------------
# uSDHC1 | Not Connected               | 0x02190000
# uSDHC2 | Not Connected               | 0x02194000
# uSDHC3 | SD Card Socket              | 0x02198000
# uSDHC4 | uSD Card Socket             | 0x0219C000
#
giMXPlatformTokenSpaceGuid.PcdSdhc1Base|0x02190000
giMXPlatformTokenSpaceGuid.PcdSdhc2Base|0x02194000
giMXPlatformTokenSpaceGuid.PcdSdhc3Base|0x02198000
giMXPlatformTokenSpaceGuid.PcdSdhc4Base|0x0219C000
  
#
# SD Card Slot (uSDHC3)
#---------------------------------------------------
# Card Detect Signal   | SD3_DAT5 | ALT5 | GPIO7_IO0
# Write Protect Signal | SD3_DAT4 | ALT5 | GPIO7_IO1
#
# MSB byte is GPIO bank number, and LSB byte is IO number
#
giMXPlatformTokenSpaceGuid.PcdSdhc3Enable|TRUE
giMXPlatformTokenSpaceGuid.PcdSdhc3CardDetectSignal|0x0700
#giMXPlatformTokenSpaceGuid.PcdSdhc3WriteProtectSignal|0x0701

#
# uSD CARD Slot (uSDHC4)
#---------------------------------------------------
# Card Detect Signal   | NANDF_D6 | ALT5 | GPIO2_IO6
# Write Protect Signal | NANDF_D7 | ALT5 | GPIO2_IO7
#
# MSB byte is GPIO bank number, and LSB byte is IO number
#
giMXPlatformTokenSpaceGuid.PcdSdhc4Enable|TRUE
giMXPlatformTokenSpaceGuid.PcdSdhc4CardDetectSignal|0x0206
#giMXPlatformTokenSpaceGuid.PcdSdhc4WriteProtectSignal|0x0207

#
# GPIO reset pin
#
giMX6TokenSpaceGuid.PcdPcieResetGpio|FALSE
giMX6TokenSpaceGuid.PcdPcieResetGpioBankNumber|0
giMX6TokenSpaceGuid.PcdPcieResetGpioIoNumber|0

#
# UART initialization required
#
giMXPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x02020000   # UART1
giMXPlatformTokenSpaceGuid.PcdKdUartInstance|1                # UART1
#giMXPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x021E8000   # UART2
#giMXPlatformTokenSpaceGuid.PcdKdUartInstance|1                # UART2

