#
# iMX6 Common Configurations Definition
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

[Defines]

  # Display surface will not be drawn to.
!ifndef CONFIG_HEADLESS
  DEFINE CONFIG_HEADLESS = FALSE
!endif

  # Useful if working with Lauterbach JTAG. Will dump to console the complete
  # command to copy and past into the Lauterbach command line to load symbols
  # for each module.
  # Use in conjution with Informational debug messages:
  # DEBUG_INFO      0x00000040
!ifndef CONFIG_DUMP_SYMBOL_INFO
  DEFINE CONFIG_DUMP_SYMBOL_INFO = TRUE
!endif

  # Enable if PSCI is implemented.
!ifndef CONFIG_MPCORE
  DEFINE CONFIG_MPCORE = TRUE
!endif

  # Enable USB by default
!ifndef CONFIG_USB
  DEFINE CONFIG_USB = TRUE
!endif

  # Disable the PCIexpress stack by default. Enable on demand.
!ifndef CONFIG_PCIE
  DEFINE CONFIG_PCIE = FALSE
!endif

  # States whether OPTEE boot flow is in effect or not. This has the following
  # implications:
  # - OPTEE owns the SecureWorld and UEFI has to run in NormalWorld.
  # - Specific memory layout that is defined in the platform .dsc file.
  # - Temp: Running in single-core until PSCI is enabled.
  # WARNING: Don't set this to FALSE unless you know what you are doing. Running
  # UEFI in SecureWorld will most likely result in boot failure due to lack of
  # proper CPU sec configuration. That is by UEFI design.
!ifndef CONFIG_OPTEE
  DEFINE CONFIG_OPTEE = TRUE
!endif

  # Allow collecting performance tracing from OPTEE hot code paths
  # Performance summary results can be inspected with the shell app Dp.efi
!ifndef CONFIG_OPTEE_PROFILE
  DEFINE CONFIG_OPTEE_PROFILE = FALSE
!endif

  # Call build with -D CONFIG_NOT_SECURE_UEFI=1 to disable the following security features
!if $(CONFIG_NOT_SECURE_UEFI) == TRUE

!ifndef CONFIG_AUTH_VAR
  DEFINE CONFIG_AUTH_VAR = FALSE
!endif

!ifndef CONFIG_MEASURED_BOOT
  DEFINE CONFIG_MEASURED_BOOT = FALSE
!endif

!ifndef CONFIG_SECURE_BOOT
  DEFINE CONFIG_SECURE_BOOT = FALSE
!endif

!else

  # UEFI authenticated variable runtime services
!ifndef CONFIG_AUTH_VAR
  DEFINE CONFIG_AUTH_VAR = TRUE
!endif

  # TPM stack for the fTPM
!ifndef CONFIG_MEASURED_BOOT
  DEFINE CONFIG_MEASURED_BOOT = TRUE
!endif

  # If Platform Key (PK) is provisioned, then SecureBoot variables will get
  # updated to reflect that SecureBoot is enabled. Otherwise, SecureBoot
  # variables will indicate that the feature is disabled, and the OS will
  # not boot apply SecureBoot policies
!ifndef CONFIG_SECURE_BOOT
  DEFINE CONFIG_SECURE_BOOT = TRUE
!endif

  # Enable building various EFI test apps. Disabled by default. Define as TRUE
  # in your board DSC file.
!ifndef CONFIG_EDK_TESTS
  DEFINE CONFIG_EDK_TESTS = FALSE
!endif

!endif

[BuildOptions]
#
# For each enabled CONFIG_* flag in the dsc files, define that flag in C code.
# Description of each CONFIG_* flag exist in iMX6DefaultConfigDsc.inc
# Note: The = sign in BuildOptions section appends to the existing macro, while
# == can be used to replace its value. See the EDK2 DSC specs.
#
!if $(CONFIG_HEADLESS) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_HEADLESS=1
!endif

!if $(CONFIG_DUMP_SYMBOL_INFO) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_DUMP_SYMBOL_INFO=1
!endif

!if $(CONFIG_MPCORE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_MPCORE=1
!endif

!if $(CONFIG_USB) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_USB=1
!endif

!if $(CONFIG_PCIE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_PCIE=1
!endif

!if $(CONFIG_OPTEE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_OPTEE=1
!endif

!if $(CONFIG_OPTEE_PROFILE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DOPTEE_PROFILE=1
!endif

!if $(CONFIG_AUTH_VAR) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_AUTH_VAR=1
!endif

!if $(CONFIG_MEASURED_BOOT) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_MEASURED_BOOT=1
!endif

!if $(CONFIG_SECURE_BOOT) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_SECURE_BOOT=1
!endif

!if $(CONFIG_EDK_TESTS) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_EDK_TESTS=1
!endif
