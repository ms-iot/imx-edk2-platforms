/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
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

#ifndef _IMX_IO_MUX_H_
#define _IMX_IO_MUX_H_

#define _IMX_PAD(CtlRegOffset, MuxRegOffset) \
  ((((CtlRegOffset) & 0xffff) << 16) | ((MuxRegOffset) & 0xffff))

#define _IMX_PAD_CTL_OFFSET(ImxPadVal) ((ImxPadVal) >> 16)
#define _IMX_PAD_MUX_OFFSET(ImxPadVal) ((ImxPadVal) & 0xffff)

#endif // _IMX_IO_MUX_H_
