## @file
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
##

#include <AsmMacroIoLib.h>
#include <AutoGen.h>
#include <Base.h>
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>

.text
.align 2

// GIC Cpu interface
#define ARM_GIC_ICCICR  0x00  // CPU Interface Control Register
#define ARM_GIC_ICCPMR  0x04  // Interrupt Priority Mask Register
#define ARM_GIC_ICCBPR  0x08  // Binary Point Register
#define ARM_GIC_ICCIAR  0x0C  // Interrupt Acknowledge Register
#define ARM_GIC_ICCEIOR 0x10  // End Of Interrupt Register
#define ARM_GIC_ICCRPR  0x14  // Running Priority Register
#define ARM_GIC_ICCPIR  0x18  // Highest Pending Interrupt Register
#define ARM_GIC_ICCABPR 0x1C  // Aliased Binary Point Register
#define ARM_GIC_ICCIDR  0xFC  // Identification Register

// SRC (System Reset Controller) register offsets & masks
#define IMX6_SRC_SCR     0x0
#define IMX6_SRC_GPR1    0x20
#define IMX6_SRC_GPR2    0x24
#define IMX6_SRC_GPR3    0x28
#define IMX6_SRC_GPR4    0x2C
#define IMX6_SRC_GPR5    0x30
#define IMX6_SRC_GPR6    0x34
#define IMX6_SRC_GPR7    0x38
#define IMX6_SRC_GPR8    0x3C
#define IMX6_SRC_GPR9    0x40
#define IMX6_SRC_GPR10   0x44

GCC_ASM_EXPORT(ArmPlatformPeiBootAction)
GCC_ASM_EXPORT(ArmPlatformIsPrimaryCore)
GCC_ASM_EXPORT(ArmPlatformGetPrimaryCoreMpId)
GCC_ASM_EXPORT(ArmPlatformGetCorePosition)

//UINTN
//ArmPlatformGetPrimaryCoreMpId (
//  VOID
//  );
ASM_PFX(ArmPlatformGetPrimaryCoreMpId):
  MOV32 (r0, FixedPcdGet32 (PcdArmPrimaryCore))
  ldr   r0, [r0]
  bx    lr

//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_PFX(ArmPlatformIsPrimaryCore):
  mrc   p15,0,r0,c0,c0,5
  ands  r0,r0,#3
  moveq r0,#1
  movne r0,#0
  bx    lr

//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
ASM_PFX(ArmPlatformGetCorePosition):
  and   r0, r0, #ARM_CORE_MASK
  bx    lr

ASM_PFX(ArmPlatformPeiBootAction):
  // enable unaligned access
  mrc   p15, 0, r1, c1, c0, 0
  bic   r1, r1, #0x2
  mcr   p15, 0, r1, c1, c0, 0
  isb
  bx r14

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
