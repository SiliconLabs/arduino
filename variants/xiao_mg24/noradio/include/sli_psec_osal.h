/**************************************************************************/ /**
 * @file
 * @brief OS abstraction layer primitives for the platform/security components.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SLI_PSEC_OSAL_H
#define SLI_PSEC_OSAL_H

// -----------------------------------------------------------------------------
// Includes

#if defined(SLI_PSEC_CONFIG_FILE)
  #include SLI_PSEC_CONFIG_FILE
#endif

#if defined (SL_COMPONENT_CATALOG_PRESENT)
  #include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_MICRIUMOS_KERNEL_PRESENT) || defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT)
// Include CMSIS RTOS2 kernel abstraction layer:
  #include "sli_psec_osal_cmsis_rtos2.h"
  #define SLI_PSEC_THREADING
#else
// Include bare metal abstraction layer:
  #include "sli_psec_osal_baremetal.h"
#endif

#endif // SLI_PSEC_OSAL_H
