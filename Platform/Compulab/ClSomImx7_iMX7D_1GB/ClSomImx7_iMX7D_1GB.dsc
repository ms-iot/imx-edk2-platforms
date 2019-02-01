#/** @file
# Compulab iMX7 Dual SoM board description
#
# The board is iMX7 with 1GB DRAM
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
  DEFINE BOARD_NAME     = ClSomImx7_iMX7D_1GB
  DEFINE IMX_FAMILY     = IMX7D
  BOARD_DIR             = Platform/Compulab/$(BOARD_NAME)
  FLASH_DEFINITION      = $(BOARD_DIR)/$(BOARD_NAME).fdf

################################################################################
#
# Platform Description
#
################################################################################
!include Silicon/NXP/iMX7Pkg/iMX7CommonDsc.inc

################################################################################
#
# Board PCD Sections
#
################################################################################

########################
#
# iMX7Pkg PCDs
#
########################
[PcdsFixedAtBuild.common]

  # System memory size (1GB)
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x40000000

  # uSDHCx | CL_SOM_IMX7D Connections
  #-------------------------------------
  # uSDHC1 | SD Card slot
  # uSDHC2 | WL1801MOD wifi (optional)
  # uSDHC3 | eMMC
  # uSDHC4 | N/A
  #
  giMXPlatformTokenSpaceGuid.PcdSdhc1Enable|TRUE
  giMXPlatformTokenSpaceGuid.PcdSdhc1CardDetectSignal|0xFF00
  giMXPlatformTokenSpaceGuid.PcdSdhc1WriteProtectSignal|0xFF00

  giMXPlatformTokenSpaceGuid.PcdSdhc3Enable|TRUE
  giMXPlatformTokenSpaceGuid.PcdSdhc3CardDetectSignal|0xFF00
  giMXPlatformTokenSpaceGuid.PcdSdhc3WriteProtectSignal|0xFF00

  #
  # USB EHCI Controller
  #
  giMX7TokenSpaceGuid.PcdUsbOTGBase|0x30B20000
  giMX7TokenSpaceGuid.PcdUsbEHCILength|0x4000
  giMX7TokenSpaceGuid.PcdIsUsbPortOTG|TRUE

  #
  # Default to watchdog 1
  #
  giMX7TokenSpaceGuid.PcdWatchdogResetIndex|1

  #
  # UART1 for console and kernel debugger
  #
  giMXPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x30860000
  giMXPlatformTokenSpaceGuid.PcdKdUartInstance|1
