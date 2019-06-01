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

#ifndef _IMX_VAR_STORE_H_
#define _IMX_VAR_STORE_H_

#define MAX_VARIABLE_SIZE 1024

#define IMX_VARIABLE_SMBIOS_SIGNATURE           L"Signature"
#define IMX_VARIABLE_SMBIOS_SIGNATURE_VALUE     L"SmbiosOverrideConfigurationTable"
#define IMX_VARIABLE_SMBIOS_REVISION            L"Revision"
#define IMX_VARIABLE_SMBIOS_MAX_REVISION        1

#define IMX_VARIABLE_SMBIOS_MANUFACTURER        L"System Manufacturer"
#define IMX_VARIABLE_SMBIOS_PRODUCT_NAME        L"System Product Name"
#define IMX_VARIABLE_SMBIOS_SKU                 L"System SKU"
#define IMX_VARIABLE_SMBIOS_SERIAL              L"System Serial Number"
#define IMX_VARIABLE_SMBIOS_FAMILY              L"System Family"
#define IMX_VARIABLE_SMBIOS_BASEBOARD_PRODUCT   L"Baseboard Product"

#endif // _IMX_VAR_STORE_H_
