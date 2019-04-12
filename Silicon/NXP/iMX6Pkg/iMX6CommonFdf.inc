#
# FLASH layout file for iMX6 Quad and Dual SOM boards.
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

# Continuation of [FV.FvMain]
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  #
  # PI DXE Drivers producing Architectural Protocols (EFI Services)
  #
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  # Timers
  INF Silicon/NXP/iMX6Pkg/Drivers/TimerDxe/TimerDxe.inf

  INF MdeModulePkg/Universal/SerialDxe/SerialDxe.inf
  INF MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  INF EmbeddedPkg/ResetRuntimeDxe/ResetRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  #
  # Filesystem Stack
  #
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF Platform/Microsoft/Drivers/SdMmcDxe/SdMmcDxe.inf
  INF Silicon/NXP/iMXPlatformPkg/Drivers/SdhcDxe/SdhcDxe.inf

  #
  # Provisioning
  #
  INF Silicon/NXP/iMXPlatformPkg/Drivers/Provisioning/Provisioning.inf

  #
  # USB Stack
  #
!if $(CONFIG_USB) == TRUE
  INF Silicon/NXP/iMX6Pkg/Drivers/PciEmulation/PciEmulation.inf
  INF MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf
  INF MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
!endif

!if $(CONFIG_PCIE) == TRUE
  #
  # PCIe
  #
  INF Silicon/NXP/iMX6Pkg/Drivers/PciExpress/iMX6PciExpress.inf
!endif

  #
  # Bds
  #
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Application/UiApp/UiApp.inf

  #
  # UEFI application (Shell Embedded Boot Loader)
  #
  INF ShellPkg/Application/Shell/Shell.inf

  #
  # Authenticated Variables and OPTEE TAs
  #

!if $(CONFIG_MEASURED_BOOT) == TRUE
  #
  # fTPM
  #
  INF SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf
!endif

  # Hello World TA UUID in UEFI format: 8aaaf200-2450-11e4-abe2-0002a5d5c51b
  FILE FREEFORM = 8AAAF200-2450-11E4-ABE2-0002A5D5C51B {
   SECTION RAW = Platform/Microsoft/OpteeClientPkg/Bin/HelloWorldTa/Arm/Test/8aaaf200-2450-11e4-abe2-0002a5d5c51b.ta
  }

!if $(CONFIG_MEASURED_BOOT) == TRUE
  # fTPM Service TA UUID in UEFI format: bc50d971-d4c9-42c4-82cb-343fb7f37896
  FILE FREEFORM = BC50D971-D4C9-42C4-82CB-343FB7F37896 {
    SECTION RAW = Platform/Microsoft/OpteeClientPkg/Bin/fTpmTa/Arm/Test/bc50d971-d4c9-42c4-82cb-343fb7f37896.ta
  }
!endif

!if $(CONFIG_EDK_TESTS) == TRUE
  FILE FREEFORM = 33630B18-7705-468B-92BD-FEBE27799EDB {
    SECTION RAW = Platform/Microsoft/Application/AuthVarTest/Test_KEK.bin
  }

  FILE FREEFORM = 27A9C9E7-4B7A-4C7E-A8F5-77DB38D7100A {
    SECTION RAW = Platform/Microsoft/Application/AuthVarTest/Test_db.bin
  }
!endif

INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

[FV.FVMAIN_COMPACT]
FvAlignment        = 8
ERASE_POLARITY     = 1
MEMORY_MAPPED      = TRUE
STICKY_WRITE       = TRUE
LOCK_CAP           = TRUE
LOCK_STATUS        = TRUE
WRITE_DISABLED_CAP = TRUE
WRITE_ENABLED_CAP  = TRUE
WRITE_STATUS       = TRUE
WRITE_LOCK_CAP     = TRUE
WRITE_LOCK_STATUS  = TRUE
READ_DISABLED_CAP  = TRUE
READ_ENABLED_CAP   = TRUE
READ_STATUS        = TRUE
READ_LOCK_CAP      = TRUE
READ_LOCK_STATUS   = TRUE

  INF ArmPlatformPkg/PrePi/PeiMPCore.inf

  FILE FV_IMAGE = 9E21FD93-9C72-4c15-8C4B-E77F1DB2D792 {
    SECTION GUIDED EE4E5898-3914-4259-9D6E-DC7BD79403CF PROCESSING_REQUIRED = TRUE {
      SECTION FV_IMAGE = FVMAIN
    }
  }


################################################################################
#
# Rules are use with the [FV] section's module INF type to define
# how an FFS file is created for a given INF file. The following Rule are the default
# rules for the different module type. User can add the customized rules to define the
# content of the FFS file.
#
################################################################################


############################################################################
# Example of a DXE_DRIVER FFS file with a Checksum encapsulation section   #
############################################################################
#
#[Rule.Common.DXE_DRIVER]
#  FILE DRIVER = $(NAMED_GUID) {
#    DXE_DEPEX    DXE_DEPEX               Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
#    COMPRESS PI_STD {
#      GUIDED {
#        PE32     PE32                    $(INF_OUTPUT)/$(MODULE_NAME).efi
#        UI       STRING="$(MODULE_NAME)" Optional
#        VERSION  STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
#      }
#    }
#  }
#
############################################################################
!if 0
[Rule.Common.SEC]
  FILE SEC = $(NAMED_GUID) RELOCS_STRIPPED {
    TE  TE    Align = 32                $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.Common.PEI_CORE]
  FILE PEI_CORE = $(NAMED_GUID) {
    TE     TE                           $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI     STRING ="$(MODULE_NAME)" Optional
  }

[Rule.Common.PEIM]
  FILE PEIM = $(NAMED_GUID) {
    PEI_DEPEX PEI_DEPEX Optional       $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32      PE32                     $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI       STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.PEIM.TIANOCOMPRESSED]
  FILE PEIM = $(NAMED_GUID) DEBUG_MYTOOLS_IA32 {
    PEI_DEPEX PEI_DEPEX Optional        $(INF_OUTPUT)/$(MODULE_NAME).depex
    GUIDED A31280AD-481E-41B6-95E8-127F4C984779 PROCESSING_REQUIRED = TRUE {
      PE32      PE32                    $(INF_OUTPUT)/$(MODULE_NAME).efi
      UI        STRING="$(MODULE_NAME)" Optional
    }
  }

[Rule.Common.DXE_CORE]
  FILE DXE_CORE = $(NAMED_GUID) {
    PE32     PE32                       $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI       STRING="$(MODULE_NAME)" Optional
  }


[Rule.Common.UEFI_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_RUNTIME_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.UEFI_APPLICATION]
  FILE APPLICATION = $(NAMED_GUID) {
    UI     STRING ="$(MODULE_NAME)" Optional
    PE32   PE32                         $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.Common.UEFI_DRIVER.BINARY]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX DXE_DEPEX Optional      |.depex
    PE32      PE32                    |.efi
    UI        STRING="$(MODULE_NAME)" Optional
    VERSION   STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
  }

[Rule.Common.UEFI_APPLICATION.BINARY]
  FILE APPLICATION = $(NAMED_GUID) {
    PE32      PE32                    |.efi
    UI        STRING="$(MODULE_NAME)" Optional
    VERSION   STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
  }
!endif

[Rule.Common.SEC]
  FILE SEC = $(NAMED_GUID) RELOCS_STRIPPED {
    TE  TE    Align = 32                $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.Common.PEI_CORE]
  FILE PEI_CORE = $(NAMED_GUID) {
    TE     TE                           $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI     STRING ="$(MODULE_NAME)" Optional
  }

[Rule.Common.PEIM]
  FILE PEIM = $(NAMED_GUID) {
     PEI_DEPEX PEI_DEPEX Optional       $(INF_OUTPUT)/$(MODULE_NAME).depex
     PE32      PE32                     $(INF_OUTPUT)/$(MODULE_NAME).efi
     UI       STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.PEIM.TIANOCOMPRESSED]
  FILE PEIM = $(NAMED_GUID) DEBUG_MYTOOLS_IA32 {
    PEI_DEPEX PEI_DEPEX Optional        $(INF_OUTPUT)/$(MODULE_NAME).depex
    GUIDED A31280AD-481E-41B6-95E8-127F4C984779 PROCESSING_REQUIRED = TRUE {
      PE32      PE32                    $(INF_OUTPUT)/$(MODULE_NAME).efi
      UI        STRING="$(MODULE_NAME)" Optional
    }
  }

[Rule.Common.DXE_CORE]
  FILE DXE_CORE = $(NAMED_GUID) {
    PE32     PE32                       $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI       STRING="$(MODULE_NAME)" Optional
  }


[Rule.Common.UEFI_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_RUNTIME_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.UEFI_APPLICATION]
  FILE APPLICATION = $(NAMED_GUID) {
    UI     STRING ="$(MODULE_NAME)" Optional
    PE32   PE32                         $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.Common.USER_DEFINED.ACPITABLE]
  FILE FREEFORM = $(NAMED_GUID) {
    RAW          ACPI                 |.acpi
    RAW          ASL Optional         |.aml
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.UEFI_DRIVER.BINARY]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX DXE_DEPEX Optional      |.depex
    PE32      PE32                    |.efi
    UI        STRING="$(MODULE_NAME)" Optional
    VERSION   STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
  }

[Rule.Common.UEFI_APPLICATION.BINARY]
  FILE APPLICATION = $(NAMED_GUID) {
    PE32      PE32                    |.efi
    UI        STRING="$(MODULE_NAME)" Optional
    VERSION   STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
  }
