#
# iMX6 Quad and Dual SOM platforms common description
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
# [Defines] Section
#
################################################################################
[Defines]
  PLATFORM_NAME                  = iMX6
  PLATFORM_GUID                  = F01954EE-8AC4-45BC-80E5-511EAE64C38C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(BOARD_NAME)
  SUPPORTED_ARCHITECTURES        = ARM
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

!include Silicon/NXP/iMX6Pkg/iMX6ConfigDsc.inc

################################################################################
#
# [LibraryClasses] Sections
#
# When parsing the DSC file, the precedence rules will followed.
#  1. If a Library Class Instance (INF) is specified in the EDK II [Components] section
#     (INF files <LibraryClasses> sub-section,) then it will be used.
#  2. If not specified in the [Components] section, then the Library Class Instance that is
#     defined in the [LibraryClasses.$(ARCH).$(MODULE_TYPE)] section will be used.
#  3. If not specified in the [LibraryClasses.$(ARCH).$(MODULE_TYPE)] section, then
#     the Library Class Instance that is defined in the
#     [LibraryClasses.Common.$(MODULE_TYPE)] section will be used.
#  4. If not specified in the [LibraryClasses.Common.$(MODULE_TYPE)] section, then
#     the Library Class Instance that is defined in the [LibraryClasses.$(ARCH)] section
#     will be used.
#  5. If not specified in the [LibraryClasses.$(ARCH)] section, then the Library Class
#     Instance that is defined in the [LibraryClasses] Section or
#     [LibraryClasses.Common] section will be used.
#  6. It is an error if it has not been specified in one of the above sections
#
################################################################################

[LibraryClasses.common]
  #
  # CPU and Timing
  #
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmMmuLib|ArmPkg/Library/ArmMmuLib/ArmMmuBaseLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  ArmSmcLib|ArmPkg/Library/ArmSmcLib/ArmSmcLib.inf
  CacheMaintenanceLib|ArmPkg/Library/ArmCacheMaintenanceLib/ArmCacheMaintenanceLib.inf
  ArmDisassemblerLib|ArmPkg/Library/ArmDisassemblerLib/ArmDisassemblerLib.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  RealTimeClockLib|Silicon/NXP/iMXPlatformPkg/Library/VirtualRealTimeClockLib/VirtualRealTimeClockLib.inf
  DefaultExceptionHandlerLib|ArmPkg/Library/DefaultExceptionHandlerLib/DefaultExceptionHandlerLib.inf
  CpuExceptionHandlerLib|ArmPkg/Library/ArmExceptionLib/ArmExceptionLib.inf
  TimerLib|Silicon/NXP/iMX6Pkg/Library/TimerLib/TimerLib.inf

  #
  # Memory
  #
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  MemoryInitPeiLib|ArmPlatformPkg/MemoryInitPei/MemoryInitPeiLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DmaLib|EmbeddedPkg/Library/NonCoherentDmaLib/NonCoherentDmaLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf

  #
  # Platform
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf
  ArmPlatformStackLib|ArmPlatformPkg/Library/ArmPlatformStackLib/ArmPlatformStackLib.inf
  EfiResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf

  #
  # Peripherals
  #
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf

  #
  # UEFI Services and Runtime
  #
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  ReportStatusCodeLib|IntelFrameworkModulePkg/Library/DxeReportStatusCodeLibFramework/DxeReportStatusCodeLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  NonDiscoverableDeviceRegistrationLib|MdeModulePkg/Library/NonDiscoverableDeviceRegistrationLib/NonDiscoverableDeviceRegistrationLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf

  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf

  #
  # Human Interface Infrastructure (HII)
  #
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf

  #
  # BDS
  #
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  PlatformBootManagerLib|ArmPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  FdtLib|EmbeddedPkg/Library/FdtLib/FdtLib.inf
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf

  #
  # Shell
  #
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

  #
  # Serial, Console and Debugging
  #
  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
  SerialPortLib|Silicon/NXP/iMXPlatformPkg/Library/UartSerialPortLib/UartSerialPortLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf

  # Enable debug prints in both release and debug builds
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  #
  # The Standard IO window in the debugger will show load and unload events for symbols.
  #
!if $(CONFIG_DUMP_SYMBOL_INFO) == TRUE
  PeCoffExtraActionLib|Platform/Microsoft/Library/LauterbachPeCoffExtraActionLib/LauterbachPeCoffExtraActionLib.inf
!else
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
!endif

  #
  # Display
  #
  iMXDisplayLib|Silicon/NXP/iMXPlatformPkg/Library/iMXDisplayLib/iMXDisplayLib.inf

  #
  # Security
  #
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  Tcg2PhysicalPresenceLib|SecurityPkg/Library/DxeTcg2PhysicalPresenceLib/DxeTcg2PhysicalPresenceLib.inf
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf

  #
  # iMX6 Specifics
  #
  iMXIoMuxLib|Silicon/NXP/iMX6Pkg/Library/iMX6IoMuxLib/iMX6IoMuxLib.inf
  iMX6AudioLib|Silicon/NXP/iMX6Pkg/Library/iMX6AudioLib/iMX6AudioLib.inf
!if $(IMX_FAMILY) == IMX6SX
  iMX6ClkPwrLib|Silicon/NXP/iMX6Pkg/Library/iMX6SxClkPwrLib/iMX6SxClkPwrLib.inf
!else
  iMX6ClkPwrLib|Silicon/NXP/iMX6Pkg/Library/iMX6ClkPwrLib/iMX6ClkPwrLib.inf
!endif
  iMX6UsbPhyLib|Silicon/NXP/iMX6Pkg/Library/iMX6UsbPhyLib/iMX6UsbPhyLib.inf

  # VariableRuntimeDxe Requirements
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf

[LibraryClasses.ARM]
  #
  # It is not possible to prevent the ARM compiler for generic intrinsic functions.
  # This library provides the instrinsic functions generate by a given compiler.
  # [LibraryClasses.ARM] and NULL mean link this library into all ARM images.
  #
  NULL|ArmPkg/Library/CompilerIntrinsicsLib/CompilerIntrinsicsLib.inf
  NULL|MdePkg/Library/BaseStackCheckLib/BaseStackCheckLib.inf

[LibraryClasses.common.SEC]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicLib.inf
  ArmGicArchLib|ArmPkg/Library/ArmGicArchSecLib/ArmGicArchSecLib.inf
  ArmPlatformStackLib|ArmPlatformPkg/Library/ArmPlatformStackLib/ArmPlatformStackLib.inf
  ExtractGuidedSectionLib|EmbeddedPkg/Library/PrePiExtractGuidedSectionLib/PrePiExtractGuidedSectionLib.inf
  HobLib|EmbeddedPkg/Library/PrePiHobLib/PrePiHobLib.inf
  LzmaDecompressLib|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  MemoryAllocationLib|EmbeddedPkg/Library/PrePiMemoryAllocationLib/PrePiMemoryAllocationLib.inf
  MemoryInitPeiLib|ArmPlatformPkg/MemoryInitPei/MemoryInitPeiLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  PlatformPeiLib|ArmPlatformPkg/PlatformPei/PlatformPeiLib.inf
  PrePiHobListPointerLib|ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf

[LibraryClasses.common.PEI_CORE]
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf

[LibraryClasses.common.DXE_CORE]
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  PerformanceLib|MdeModulePkg/Library/DxeCorePerformanceLib/DxeCorePerformanceLib.inf

[LibraryClasses.common.DXE_DRIVER]
  ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicLib.inf
  ArmGicArchLib|ArmPkg/Library/ArmGicArchLib/ArmGicArchLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  DmaLib|EmbeddedPkg/Library/NonCoherentDmaLib/NonCoherentDmaLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  PlatformBootManagerLib|ArmPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  ArmSoftFloatLib|ArmPkg/Library/ArmSoftFloatLib/ArmSoftFloatLib.inf
  NonDiscoverableDeviceRegistrationLib|MdeModulePkg/Library/NonDiscoverableDeviceRegistrationLib/NonDiscoverableDeviceRegistrationLib.inf

!if $(CONFIG_OPTEE) == TRUE
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  OpteeClientApiLib|Platform/Microsoft/OpteeClientPkg/Library/OpteeClientApiLib/OpteeClientApiLib.inf
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterDxe.inf
  Tpm2DeviceLibOptee|Platform/Microsoft/OpteeClientPkg/Library/Tpm2DeviceLibOptee/Tpm2DeviceLibOptee.inf
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
!endif

[LibraryClasses.common.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  ReportStatusCodeLib|IntelFrameworkModulePkg/Library/DxeReportStatusCodeLibFramework/DxeReportStatusCodeLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  ArmSoftFloatLib|ArmPkg/Library/ArmSoftFloatLib/ArmSoftFloatLib.inf

!if $(CONFIG_OPTEE) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  OpteeClientApiLib|Platform/Microsoft/OpteeClientPkg/Library/OpteeClientApiLib/OpteeClientApiLib.inf
!endif

[LibraryClasses.common.UEFI_DRIVER]
  ReportStatusCodeLib|IntelFrameworkModulePkg/Library/DxeReportStatusCodeLibFramework/DxeReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|IntelFrameworkModulePkg/Library/DxeReportStatusCodeLibFramework/DxeReportStatusCodeLib.inf
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf

!if $(CONFIG_OPTEE) == TRUE
  OpteeClientApiLib|Platform/Microsoft/OpteeClientPkg/Library/OpteeClientApiLib/OpteeClientApiLib.inf
!if $(CONFIG_MEASURED_BOOT) == TRUE
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
!endif
!endif

################################################################################
#
# PCD Sections
#
################################################################################

########################
#
# ArmPkg PCDs
#
########################
[PcdsFeatureFlag.common]
  #
  # Use the Vector Table location in CpuDxe. We will not copy the Vector Table at
  # PcdCpuVectorBaseAddress
  #
  gArmTokenSpaceGuid.PcdRelocateVectorTable|FALSE

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdTrustzoneSupport|FALSE
  gArmTokenSpaceGuid.PcdVFPEnabled|1

  #
  # ARM General Interrupt Controller settings
  #
!if $(IMX_FAMILY) == IMX6ULL
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x00A01000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x00A02000
  gArmTokenSpaceGuid.PcdArmPrimaryCore|0x000
!else
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x00A01000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x00A00100
  gArmTokenSpaceGuid.PcdArmPrimaryCore|0x000

  #
  # ARM L2x0 PCDs
  #
  #gArmTokenSpaceGuid.PcdL2x0ControllerBase|0x00A02000
!endif

  #
  # GPT Timer Frequency
  # Set to 1KHz, which equals to 1us resolution
  #
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|1000000

########################
#
# ArmPlatformPkg PCDs
#
########################
[PcdsFixedAtBuild.common]
  # Size of the region used by UEFI in permanent memory (Reserved 16MB)
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x01000000
!if $(IMX_FAMILY) == IMX6ULL
  #
  # Refer to the detailed memory map at OpteeClientPkg PCDs section below
  #
  giMX6TokenSpaceGuid.PcdMemoryStartAddress|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x82A00000
  !if $(DRAM_SIZE) == DRAM_512MB
  #
  # Board DDR starts at 0x80000000 and ends 0x9FFF0000 (512MB)
  # SystemMemorySize is 0x9DFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0x9FFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x1B5F0000
  !endif
!elseif $(IMX_FAMILY) == IMX6SX
  #
  # Refer to the detailed memory map at OpteeClientPkg PCDs section below
  #
  giMX6TokenSpaceGuid.PcdMemoryStartAddress|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x829D4000
  !if $(DRAM_SIZE) == DRAM_512MB
  #
  # Board DDR starts at 0x80000000 and ends 0x9FFF0000 (512MB)
  # SystemMemorySize is 0x9FFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0x9FFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x1D61C000
  !elseif $(DRAM_SIZE) == DRAM_1GB
  #
  # Board DDR starts at 0x80000000 and ends 0xBFFF0000 (1GB)
  # SystemMemorySize is 0xBFFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0xBFFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x3D61C000
  !else
  #
  # Board DDR starts at 0x80000000 and ends 0xFFFF0000 (2GB)
  # SystemMemorySize is 0xFFFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0xFFFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x7D61C000
  !endif
!else
  giMX6TokenSpaceGuid.PcdMemoryStartAddress|0x10000000
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x12A00000

  !if $(DRAM_SIZE) == DRAM_512MB
  #
  # Board DDR starts at 0x10000000 and ends 0x2FFF0000 (512MB)
  # SystemMemorySize is 0x2FFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0x2FFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x1D5F0000
  !elseif $(DRAM_SIZE) == DRAM_1GB
  #
  # Board DDR starts at 0x10000000 and ends 0x4FFF0000 (1GB)
  # SystemMemorySize is 0x4FFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0x4FFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x3D5F0000
  !else
  #
  # Board DDR starts at 0x10000000 and ends 0x8FFF0000 (2GB)
  # SystemMemorySize is 0x8FFF0000 - PcdSystemMemoryBase
  #
  giMX6TokenSpaceGuid.PcdMemoryEndAddress|0x8FFF0000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x7D5F0000
  !endif
!endif

!if $(CONFIG_MPCORE) == TRUE
  !if (($(IMX_FAMILY) == IMX6Q) || ($(IMX_FAMILY) == IMX6QP))
  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  !elseif (($(IMX_FAMILY) == IMX6D) || ($(IMX_FAMILY) == IMX6DL) || ($(IMX_FAMILY) == IMX6DP))
  gArmPlatformTokenSpaceGuid.PcdCoreCount|2
  !else
  gArmPlatformTokenSpaceGuid.PcdCoreCount|1
  !endif
!else
  gArmPlatformTokenSpaceGuid.PcdCoreCount|1
!endif

########################
#
# MdePkg PCDs
#
########################
[PcdsFeatureFlag.common]
  gEfiMdePkgTokenSpaceGuid.PcdComponentNameDisable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnosticsDisable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdComponentName2Disable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnostics2Disable|TRUE

[PcdsFixedAtBuild.common]
  gEfiMdePkgTokenSpaceGuid.PcdMaximumUnicodeStringLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdMaximumAsciiStringLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdSpinLockTimeout|10000000
  gEfiMdePkgTokenSpaceGuid.PcdDebugClearMemoryValue|0xAF
  gEfiMdePkgTokenSpaceGuid.PcdPostCodePropertyMask|0
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|320
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1

  #
  # DEBUG_ASSERT_ENABLED       0x01
  # DEBUG_PRINT_ENABLED        0x02
  # DEBUG_CODE_ENABLED         0x04
  # CLEAR_MEMORY_ENABLED       0x08
  # ASSERT_BREAKPOINT_ENABLED  0x10
  # ASSERT_DEADLOOP_ENABLED    0x20
  #
  !if $(TARGET) == RELEASE
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x23
  !else
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2f
  !endif

  #
  # Debug Flags as defined in DebugLib.h
  #
  # DEBUG_INIT      0x00000001  // Initialization
  # DEBUG_WARN      0x00000002  // Warnings
  # DEBUG_LOAD      0x00000004  // Load events
  # DEBUG_FS        0x00000008  // EFI File system
  # DEBUG_POOL      0x00000010  // Alloc & Free (pool)
  # DEBUG_PAGE      0x00000020  // Alloc & Free (page)
  # DEBUG_INFO      0x00000040  // Informational debug messages
  # DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
  # DEBUG_VARIABLE  0x00000100  // Variable
  # DEBUG_BM        0x00000400  // Boot Manager
  # DEBUG_BLKIO     0x00001000  // BlkIo Driver
  # DEBUG_NET       0x00004000  // Network Io Driver
  # DEBUG_UNDI      0x00010000  // UNDI Driver
  # DEBUG_LOADFILE  0x00020000  // LoadFile
  # DEBUG_TEEC      0x00040000  // TEE Client Tracings
  # DEBUG_EVENT     0x00080000  // Event messages
  # DEBUG_GCD       0x00100000  // Global Coherency Database changes
  # DEBUG_CACHE     0x00200000  // Memory range cachability changes
  # DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may
  #                             // significantly impact boot performance
  # DEBUG_ERROR     0x80000000  // Error
  #
  # By default only enable init, error, warning and load debug prints to minimize
  # debug messages. Use "0x8FEF4DCF" for detailed debugging purposes.
  #
!if $(TARGET) == RELEASE
  #
  # In release builds enable error, warning, and load (base and entry point)
  #
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel| 0x80000006
!else
  #
  # In debug builds enable error, cache, info, load, warn, and init
  #
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel| 0x80000047
!endif
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|3

########################
#
# MdeModulePkg PCDs
#
########################
[PcdsFeatureFlag.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdTurnOffUsbLegacySupport|TRUE

  ## If TRUE, Graphics Output Protocol will be installed on virtual handle created by ConsplitterDxe.
  #  It could be set FALSE to save size.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE

[PcdsFixedAtBuild.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|4
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  # GUID of the UI app
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  #
  # Make VariableRuntimeDxe work at emulated non-volatile variable mode.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE

[PcdsPatchableInModule]
  # Console Resolution
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1920
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1080

########################
#
# EmbeddedPkg PCDs
#
########################
[PcdsFeatureFlag.common]
  gEmbeddedTokenSpaceGuid.PcdPrePiProduceMemoryTypeInformationHob|TRUE

[PcdsFixedAtBuild.common]
  #
  # Optional feature to help prevent EFI memory map fragments
  # Turned on and off via: PcdPrePiProduceMemoryTypeInformationHob
  # Values are in EFI Pages (4K). DXE Core will make sure that
  # at least this much of each type of memory can be allocated
  # from a single memory range. This way you only end up with
  # maximum of two fragements for each type in the memory map
  # (the memory used, and the free memory that was prereserved
  # but not used).
  #
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiACPIReclaimMemory|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiACPIMemoryNVS|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiReservedMemoryType|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiRuntimeServicesData|80
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiRuntimeServicesCode|40
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiBootServicesCode|400
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiBootServicesData|3000
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiLoaderCode|10
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiLoaderData|0

  #
  # Timer Configuration
  #
  # The timer period is configured in units of 100ns. We configure it for 10ms
  # timer ticks which will be used mainly by DXE Core to keep track of system
  # time and hence keep track of all events.
  #
  gEmbeddedTokenSpaceGuid.PcdTimerPeriod|100000

########################
#
# IntelFrameworkModulePkg PCDs
#
########################
[PcdsFixedAtBuild.common]
  # GUID of the UEFI Shell
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile|{ 0x83, 0xA5, 0x04, 0x7C, 0x3E, 0x9E, 0x1C, 0x4F, 0xAD, 0x65, 0xE0, 0x52, 0x68, 0xD0, 0xB4, 0xD1 }

########################
#
# OpteeClientPkg PCDs
#
########################
[PcdsFixedAtBuild.common]
!if $(IMX_FAMILY) == IMX6ULL
  #
  # Reserved for TPM2 ACPI
  # 0x80814000 - 0x80816FFF
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferBase|0x80814000
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferSize|0x3000

  #
  # iMX Plaform Memory Layout
  #
  # +-------------------+===> (0x80000000) PcdFrameBufferBase (DDR Start Address)
  # |Frame Buffer       |  ^
  # |                   |  |  (0x00800000) PcdFrameBufferSize 8MB
  # |                   |  v
  # +-------------------+===
  # |                   |     Gap
  # +-------------------|===> (0x80814000) PcdTpm2AcpiBufferBase
  # |TPM2 Control Area  |  ^
  # |                   |  |  (0x3000) 12KB
  # |                   |  v
  # +-------------------+===> (0x80817000) PcdGlobalDataBaseAddress
  # | Global Data       |  ^
  # |                   |  |  (0x1000) PcdGlobalDataSize 4KB
  # |                   |  v
  # +-------------------+===
  # | Gap               |
  # +-------------------+===> (0x80820000) UEFI Load Address
  # | UEFI              |  ^
  # |                   |  |  (0x1D0000) 1920KB
  # |                   |  v
  # +-------------------+===> (0x80A00000) PcdTrustZonePrivateMemoryBase (OPTEE image base address)
  # | TZ Private Memory |  ^
  # | (OPTEE)           |  |  (0x1E00000) 30MB
  # |                   |  v
  # +-------------------+===> (0x82800000) PcdTrustZoneSharedMemoryBase (includes mobj bookkeeping page)
  # | TZ Shared Memory  |  ^
  # |                   |  |  (0x200000) 2MB
  # |                   |  v
  # +-------------------+===> (0x82A00000) PcdSystemMemoryBase
  # | Operating System  |  ^
  # | Memory            |  |  
  # |                   |  |  (0x1B5F0000) 437MB for 512MB DRAM
  # |                   |  |  
  # |                   |  v  
  # +------------------ +===> 0x8DFFFFFF/0x9DFFFFFF
  #
  # Reserved for Optee private memory (31M)
  # 0x80A00000 - 0x827FFFFF
  #
  # TrustZone private memory (?Mb OPTEE + ?Mb PSCI)
  # 0x8E000000 - 8FFFFFFF  (256MB)          0x9E000000 - 9FFFFFFF    (512MB)
  # This memory is managed privately by the OpTEE OS.
  #    CFG_DDR_TEETZ_RESERVED_START & CFG_DDR_TEETZ_RESERVED_START
  #

  DEFINE GLOBAL_DATA_BASE_ADDRESS = 0x80817000

  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemoryBase|0x80A00000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemorySize|0x01E00000

  #
  # TrustZone shared memory (2Mb)
  # 0x12800000 - 0x129FFFFF
  # This memory is managed by the normal world but shared with the OpTEE OS.
  # It must match OpTEE optee_os/core/arch/arm/plat-imx/platform_config.h:
  #    CFG_SHMEM_START & CFG_SHMEM_SIZE
  # NOTE: The first page of the SHMEM is owned by OPTEE for mobj bookkeeping
  # and we should not touch it. We will skip the first 4K of SHMEM and take that
  # into account for SHMEM size in PcdTrustZoneSharedMemorySize.
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemoryBase|0x82800000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemorySize|0x00200000
  giMX6TokenSpaceGuid.PcdFrameBufferBase|0x80000000
!elseif $(IMX_FAMILY) == IMX6SX
  #
  # Reserved for TPM2 ACPI
  # 0x82000000 - 0x82002FFF
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferBase|0x82000000
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferSize|0x3000

  #
  # iMX Plaform Memory Layout
  #
  # +-------------------+===> (0x80000000) PcdTrustZonePrivateMemoryBase (OPTEE image base address)
  # | TZ Private Memory |  ^
  # | (OPTEE)           |  |  (0x01E00000) PcdTrustZonePrivateMemorySize 30MB
  # |                   |  v
  # +-------------------+===> (0x81E00000) PcdTrustZoneSharedMemoryBase (includes mobj bookkeeping page)
  # | TZ Shared Memory  |  ^
  # |                   |  |  (0x00200000) PcdTrustZoneSharedMemorySize 2MB
  # |                   |  v
  # +-------------------|===> (0x82000000) PcdTpm2AcpiBufferBase
  # | TPM2 Control Area |  ^
  # |                   |  |  (0x00003000) PcdTpm2AcpiBufferSize 12KB
  # |                   |  v
  # +-------------------+===> (0x82003000) PcdGlobalDataBaseAddress
  # | Global Data       |  ^
  # |                   |  |  (0x00001000) PcdGlobalDataSize 4KB
  # |                   |  v
  # +-------------------+===> (0x82004000) PcdFdBaseAddress (UEFI Load Address)
  # | UEFI              |  ^
  # |                   |  |  (0x001D0000) PcdFdSize 1856KB
  # |                   |  v
  # +-------------------+===> (0x821D4000) PcdFrameBufferBase
  # | FrameBuffer       |  ^
  # |                   |  |  (0x00800000) PcdFrameBufferSize 8MB
  # |                   |  v
  # +-------------------+===> (0x829D4000) PcdSystemMemoryBase
  # | Operating System  |  ^
  # | Memory            |  |
  # |                   |  |
  # |                   |  v
  # +------------------ +===>

  DEFINE GLOBAL_DATA_BASE_ADDRESS = 0x82003000

  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemoryBase|0x80000000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemorySize|0x01E00000

  #
  # TrustZone shared memory (2Mb)
  # This memory is managed by the normal world but shared with the OpTEE OS.
  # It must match OpTEE optee_os/core/arch/arm/plat-imx/platform_config.h:
  #    CFG_SHMEM_START & CFG_SHMEM_SIZE
  # NOTE: The first page of the SHMEM is owned by OPTEE for mobj bookkeeping
  # and we should not touch it. We will skip the first 4K of SHMEM and take that
  # into account for SHMEM size in PcdTrustZoneSharedMemorySize.
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemoryBase|0x81E00000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemorySize|0x00200000

  giMX6TokenSpaceGuid.PcdFrameBufferBase|0x821D4000

!else
  #
  # Reserved for TPM2 ACPI
  # 0x10814000 - 0x10816FFF
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferBase|0x10814000
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferSize|0x3000

  #
  # iMX Plaform Memory Layout
  #
  # +-------------------+===> (0x10000000) PcdFrameBufferBase (DDR Start Address)
  # |Frame Buffer       |  ^
  # |                   |  |  (0x00800000) PcdFrameBufferSize 8MB
  # |                   |  v
  # +-------------------+===
  # |                   |     Gap
  # +-------------------|===> (0x10814000) PcdTpm2AcpiBufferBase
  # |TPM2 Control Area  |  ^
  # |                   |  |  (0x3000) 12KB
  # |                   |  v
  # +-------------------+===> (0x10817000) PcdGlobalDataBaseAddress
  # | Global Data       |  ^
  # |                   |  |  (0x1000) PcdGlobalDataSize 4KB
  # |                   |  v
  # +-------------------+===
  # | Gap               |
  # +-------------------+===> (0x10820000) UEFI Load Address
  # | UEFI              |  ^
  # |                   |  |  (0x1D0000) 1920KB
  # |                   |  v
  # +-------------------+===> (0x10A00000) PcdTrustZonePrivateMemoryBase (OPTEE image base address)
  # | TZ Private Memory |  ^
  # | (OPTEE)           |  |  (0x1E00000) 30MB
  # |                   |  v
  # +-------------------+===> (0x12800000) PcdTrustZoneSharedMemoryBase (includes mobj bookkeeping page)
  # | TZ Shared Memory  |  ^
  # |                   |  |  (0x200000) 2MB
  # |                   |  v
  # +-------------------+===> (0x12A00000) PcdSystemMemoryBase
  # | Operating System  |  ^
  # | Memory            |  |  (0x3D5F0000) 982MB for 1GB DRAM OR
  # |                   |  |  (0x7D5F0000) 2005MB for 2GB DRAM
  # |                   |  v
  # +------------------ +===
  #
  # Reserved for Optee private memory (31M)
  # 0x10A00000 - 0x127FFFFF
  #
  # TrustZone private memory (?Mb OPTEE + ?Mb PSCI)
  # 0x8E000000 - 8FEFFFFF
  # This memory is managed privately by the OpTEE OS.
  #    CFG_DDR_TEETZ_RESERVED_START & CFG_DDR_TEETZ_RESERVED_START
  #

  DEFINE GLOBAL_DATA_BASE_ADDRESS = 0x10817000

  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemoryBase|0x10A00000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemorySize|0x01E00000

  #
  # TrustZone shared memory (2Mb)
  # 0x12800000 - 0x129FFFFF
  # This memory is managed by the normal world but shared with the OpTEE OS.
  # It must match OpTEE optee_os/core/arch/arm/plat-imx/platform_config.h:
  #    CFG_SHMEM_START & CFG_SHMEM_SIZE
  # NOTE: The first page of the SHMEM is owned by OPTEE for mobj bookkeeping
  # and we should not touch it. We will skip the first 4K of SHMEM and take that
  # into account for SHMEM size in PcdTrustZoneSharedMemorySize.
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemoryBase|0x12800000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemorySize|0x00200000

  giMX6TokenSpaceGuid.PcdFrameBufferBase|0x10000000

!endif
########################
#
# SecurityPkg PCDs
#
########################
[PcdsFixedAtBuild.common]
  #
  # override the default values from SecurityPkg to ensure images from all sources are verified in secure boot
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x04
  gEfiSecurityPkgTokenSpaceGuid.PcdFixedMediaImageVerificationPolicy|0x04
  gEfiSecurityPkgTokenSpaceGuid.PcdRemovableMediaImageVerificationPolicy|0x04

  #
  # PcdTpmInstanceGuid tells the Tcg2 Driver which TPM version is being used.
  # The below GUID is TPMv2 GUID.
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{ 0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17 }

  ## Sets the default Physical Presence flags that are consumed by the PP TrEE library.
  #
  # Options are defined in TrEEPhysicalPresenceData.h
  # Current setting:
  #   TREE_FLAG_NO_PPI_CLEAR
  #
  #gEfiSecurityPkgTokenSpaceGuid.PcdPPDefaultTrEEFlags|0x02

  ## Indicates whether or not the Tcg2Dxe should measure CRTM instead of PEI.
  # TRUE means the Tcg2Dxe will measure CRTM.
  #
  #gEfiSecurityPkgTokenSpaceGuid.PcdLateMeasureCRTM|TRUE

[PcdsDynamicExDefault.common.DEFAULT]
  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2HashAlgorithmBitmap|0x0

########################
#
# MsPkg PCDs
#
########################
[PcdsFixedAtBuild.common]

  #
  # SD first FAT partition device path:
  # VenHw(AAFB8DAA-7340-43AC-8D49-0CCE14812489,03000000)/SD(0x0)/HD(1,MBR,0xAE420040,0x1000,0x20000)
  #
  # eMMC first FAT partition device path:
  # VenHw(AAFB8DAA-7340-43AC-8D49-0CCE14812489,04000000)/eMMC(0x0)/HD(1,MBR,0xAE420040,0x1000,0x20000)
  #
  # Use the SDCard first fat partition for logging and misc storage.
  #
  gMsPkgTokenSpaceGuid.PcdStorageMediaPartitionDevicePath|L"VenHw(AAFB8DAA-7340-43AC-8D49-0CCE14812489,03000000)/SD(0x0)/HD(1,MBR,0xAE420040,0x1000,0x20000)"

!if $(CONFIG_SECURE_BOOT) == TRUE
  gMsPkgTokenSpaceGuid.PcdSecureBootEnable|TRUE
!else
  gMsPkgTokenSpaceGuid.PcdSecureBootEnable|FALSE
!endif

########################
#
# iMXPlatformPkg PCDs
#
########################
[PcdsFixedAtBuild.common]

  # Global data area
  giMXPlatformTokenSpaceGuid.PcdGlobalDataBaseAddress|$(GLOBAL_DATA_BASE_ADDRESS)
  giMXPlatformTokenSpaceGuid.PcdGlobalDataSize|0x1000

################################################################################
#
# [Components] Section
#
################################################################################
[Components.common]

  #
  # SEC (PrePi)
  #
  ArmPlatformPkg/PrePi/PeiMPCore.inf

  #
  # DXE Core
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
  }
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  #
  # DXE required Architectural Protocols as defined by UEFI 2.6 specs
  #
  # BDS Arch Protocol
  # CPU Arch Protocol
  # Metronome Arch Protocol
  # Real Time Clock Arch Protocol
  # Reset Arch Protocol
  # Runtime Arch Protocol
  # Security Arch Protocol
  # Timer Arch Protocol
  # Variable Arch Protocol
  # Variable Write Arch Protocol
  # EFI Capsule Arch Protocol
  # Watchdog Timer Arch Protocol
  #

  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  EmbeddedPkg/ResetRuntimeDxe/ResetRuntimeDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
      NULL|SecurityPkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
  }
  Silicon/NXP/iMX6Pkg/Drivers/TimerDxe/TimerDxe.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf

  #
  # Serial Console
  #
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf
  EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf

  #
  # Filesystem Stack
  #
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
  Platform/Microsoft/Drivers/SdMmcDxe/SdMmcDxe.inf
  Silicon/NXP/iMXPlatformPkg/Drivers/SdhcDxe/SdhcDxe.inf

  #
  # USB Stack
  #
!if $(CONFIG_USB) == TRUE
  Silicon/NXP/iMX6Pkg/Drivers/PciEmulation/PciEmulation.inf
  MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
!endif

!if $(CONFIG_PCIE) == TRUE
  # PCIe support
  Silicon/NXP/iMX6Pkg/Drivers/PciExpress/iMX6PciExpress.inf
!endif

  #
  # Generic Bds (Not Intel's)
  #
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }
  #
  # Shell
  #
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }

!if $(CONFIG_MEASURED_BOOT) == TRUE
  #
  # FTPM
  #
  SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      Tpm2DeviceLib|Platform/Microsoft/OpteeClientPkg/Library/Tpm2DeviceLibOptee/Tpm2DeviceLibOptee.inf

    <PcdsFixedAtBuild>
      !if $(CONFIG_OPTEE_PROFILE) == TRUE
        gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
      !else
        gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0
      !endif
  }
!endif

MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  # Applications
  MdeModulePkg/Application/HelloWorld/HelloWorld.inf
  Silicon/NXP/iMX6Pkg/Application/ImxClkPwr/ImxClkPwr.inf

!if $(CONFIG_EDK_TESTS) == TRUE
  Platform/Microsoft/Application/StorageTest/StorageTest.inf
  Platform/Microsoft/Application/AuthVarTest/AuthVarTest.inf
!endif

!if $(CONFIG_OPTEE) == TRUE
  Platform/Microsoft/OpteeClientPkg/Application/OpteeClientApiTest/OpteeClientApiTest.inf
  #SecurityPkg/Application/VariableAuthTestDxe/VariableAuthTestDxe.inf
!endif
#
# Boilerplate for compiling UEFI applications and UEFI drivers with the
# standard library.
#
!include StdLib/StdLib.inc
