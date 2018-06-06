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
#

#include <AsmMacroIoLib.h>
#include <Library/ArmLib.h>

.text
.align 2

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

  //
  // mrc - move arm register from coprocessor
  // mrc coproc name, opcode 1, ARM register, coproc Crn, coproc CRm, opcode 2
  //
  mrc   p15,0,r0,c0,c0,5
  ands  r0,r0,#3
  moveq r0,#1
  movne r0,#0
  bx 	lr

//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
ASM_PFX(ArmPlatformGetCorePosition):
  and   r0, r0, #ARM_CORE_MASK
  bx    lr

ASM_PFX(ArmPlatformPeiBootAction):
  // enable un aligned access
  mrc   p15, 0, r1, c1, c0, 0
  bic   r1, r1, #0x2
  mcr   p15, 0, r1, c1, c0, 0
  isb
  
  bx r14

ASM_FUNCTION_REMOVE_IF_UNREFERENCED

