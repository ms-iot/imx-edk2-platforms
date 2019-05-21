#/** @file
# iMX6SX SabreSD board description
# The board is iMX6SX with 1GB DRAM
#
#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Copyright 2019 NXP
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
  DEFINE BOARD_NAME               = Sabre_iMX6SX_1GB
  DEFINE IMX_FAMILY               = IMX6SX
  DEFINE DRAM_SIZE                = DRAM_1GB
  DEFINE CONFIG_DUMP_SYMBOL_INFO  = TRUE
  DEFINE CONFIG_HEADLESS          = FALSE
  DEFINE CONFIG_NOT_SECURE_UEFI   = TRUE
  DEFINE CONFIG_PCIE              = TRUE
  DEFINE BUILD_TARGETS            = DEBUG
  BOARD_DIR                       = Platform/NXP/$(BOARD_NAME)
  FLASH_DEFINITION                = $(BOARD_DIR)/$(BOARD_NAME).fdf

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
  $(BOARD_DIR)/Drivers/GraphicsOutputDxe/GraphicsOutputDxe.inf
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

#
# USB EHCI Controller USB OTG2
#
giMX6TokenSpaceGuid.PcdEHCIBase|0x02184200

  #
  # uSDHC Controllers as connected on iMX6SX SabreSD
  #
  # uSDHCx | On-board Connection         | Physical Base
  #-----------------------------------------------------
  # uSDHC1 | Not Connected               | 0x02190000
  # uSDHC2 | J5 SD2 Card Socket (SDIO)   | 0x02194000
  # uSDHC3 | J3 SD3 Card Socket (8-bit)  | 0x02198000
  # uSDHC4 | J4 SD4 Card Socket (Boot)   | 0x0219C000
  #
  giMXPlatformTokenSpaceGuid.PcdSdhc1Base|0x02190000
  giMXPlatformTokenSpaceGuid.PcdSdhc2Base|0x02194000
  giMXPlatformTokenSpaceGuid.PcdSdhc3Base|0x02198000
  giMXPlatformTokenSpaceGuid.PcdSdhc4Base|0x0219C000

  #
  # Enable uSDHC2 Controller.
  #---------------------------------------------------
  giMXPlatformTokenSpaceGuid.PcdSdhc2Enable|TRUE
  giMXPlatformTokenSpaceGuid.PcdSdhc2CardDetectSignal|0xFF00
  giMXPlatformTokenSpaceGuid.PcdSdhc2WriteProtectSignal|0xFF00

  #
  # Enable uSDHC3 Controller.
  #---------------------------------------------------
  giMXPlatformTokenSpaceGuid.PcdSdhc3Enable|TRUE
  giMXPlatformTokenSpaceGuid.PcdSdhc3CardDetectSignal|0x020A
  giMXPlatformTokenSpaceGuid.PcdSdhc3WriteProtectSignal|0xFFFF

  #
  # Enable uSDHC4 Controller.
  #---------------------------------------------------
  giMXPlatformTokenSpaceGuid.PcdSdhc4Enable|TRUE
  giMXPlatformTokenSpaceGuid.PcdSdhc4CardDetectSignal|0xFFFF
  giMXPlatformTokenSpaceGuid.PcdSdhc4WriteProtectSignal|0xFFFF

  #
  # No UART initialization required leveraging the first boot loader
  #
  giMXPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x02020000   # UART1
  giMXPlatformTokenSpaceGuid.PcdKdUartInstance|1                # UART1

  #
  # PCIE
  #
  # PCI Host config space is fixed at 0x08FFC000. The memory range from
  # 0x08000000 - 0x08FFBFFF is assigned for PCIe. The memory layout defined
  # by the boot loader is as below
  #
  # PCIe Device Config Space : 0x08F00000 - 0x08F80000
  # PCIe IO (unsupported)    : 0x08F80000 - 0x08F8FFFF
  # PCIe Memory              : 0x08000000 - 0x08EFFFFF
  #
  giMX6TokenSpaceGuid.PcdPcieDeviceConfigBase|0x08F00000
  giMX6TokenSpaceGuid.PcdPcieDeviceConfigSize|0x00080000
  giMX6TokenSpaceGuid.PcdPcieHostConfigBase|0x08FFC000
  giMX6TokenSpaceGuid.PcdPcieIOBase|0x00000000
  giMX6TokenSpaceGuid.PcdPcieIOSize|0x00000000
  giMX6TokenSpaceGuid.PcdPciMemoryBase|0x08000000
  giMX6TokenSpaceGuid.PcdPciMemorySize|0x00F00000
  giMX6TokenSpaceGuid.PcdPciPrefetchMemoryBase|0x00000000
  giMX6TokenSpaceGuid.PcdPciPrefetchMemorySize|0x00000000
  giMX6TokenSpaceGuid.PcdPcieResetGpio|TRUE
  giMX6TokenSpaceGuid.PcdPcieResetGpioBankNumber|2
  giMX6TokenSpaceGuid.PcdPcieResetGpioIoNumber|0
