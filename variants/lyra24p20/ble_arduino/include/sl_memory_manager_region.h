/***************************************************************************//**
 * @file
 * @brief Getters for Heap and stack.
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

#ifndef SL_MEMORY_MANAGER_REGION_H_
#define SL_MEMORY_MANAGER_REGION_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup memory_manager Memory Manager
 * @{
 ******************************************************************************/

// ----------------------------------------------------------------------------
// DATA TYPES

/// @brief Memory region structure.
typedef struct sl_memory_region_t {
  void * addr; ///< Pointer to the beginning of the memory region. Can be NULL.
  size_t size; ///< Size of this memory region.
} sl_memory_region_t;

// ----------------------------------------------------------------------------
// PROTOTYPES

/***************************************************************************//**
 * Gets size and location of the stack.
 *
 * @return  description of the region reserved for the C stack.
 ******************************************************************************/
sl_memory_region_t sl_memory_get_stack_region(void);

/***************************************************************************//**
 * Gets size and location of the heap.
 *
 * @return  description of the region reserved for the C heap.
 ******************************************************************************/
sl_memory_region_t sl_memory_get_heap_region(void);

/** @} end addtogroup memory_manager) */

#ifdef __cplusplus
}
#endif

#endif /* SL_MEMORY_MANAGER_REGION_H_ */
