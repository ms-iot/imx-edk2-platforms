/** @file
*
*  Copyright (c) 2019 Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/
#ifndef _PROVISIONING_H_
#define _PROVISIONING_H_

#define TPMREADPUBLIC_SZ 0xE
#define TPMCREATEPRIMARY_SZ 0x177
#define TPMEVICTEK_SZ 0x37
#define TPMOUT_SZ 1000

#define SEND_REQUEST_TO_HOST(msg) SerialPortWrite ((UINT8*)msg, AsciiStrLen (msg))

extern EFI_GUID giMXPlatformProvisioningGuid;

#pragma pack(1)

typedef struct {
  TPM2_COMMAND_HEADER       Header;
  TPMI_DH_OBJECT            ObjectHandle;
} TPM2_READ_PUBLIC_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER       Header;
  TPM2B_PUBLIC               OutPublic;
  TPM2B_NAME                 Name;
  TPM2B_NAME                 QualifiedName;
} TPM2_READ_PUBLIC_RESPONSE;

#pragma pack()


#endif // _PROVISIONING_H_
