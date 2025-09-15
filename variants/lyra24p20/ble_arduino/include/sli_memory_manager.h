/***************************************************************************//**
 * @file
 * @brief Memory Manager Driver API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_MEMORY_MANAGER_H_
#define SLI_MEMORY_MANAGER_H_

#include "sl_memory_manager.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

// Memory Manager integration to SystemView is enabled on GCC builds of
// applications that include the SystemView component
#if defined(SL_CATALOG_SYSTEMVIEW_TRACE_PRESENT) && defined(__GNUC__)
#define SLI_MEMORY_MANAGER_ENABLE_SYSTEMVIEW
#endif

// Minimum block alignment in bytes. 8 bytes is the minimum alignment to account for largest CPU data type
// that can be used in some block allocation scenarios. 64-bit data type may be used to manipulate the
// allocated block. The ARM processor ABI defines data types and byte alignment, and 8-byte alignment
// can be seen for the largest data object type.
#define SLI_BLOCK_ALLOC_MIN_ALIGN   SL_MEMORY_BLOCK_ALIGN_8_BYTES

// Minimum block allocation size to avoid creating a block too small while splitting up an allocated block.
// Minimum size is formed from (metadata + payload) size. Size expressed in bytes.
#define SLI_BLOCK_ALLOCATION_MIN_SIZE   (SLI_BLOCK_METADATA_SIZE_BYTE + SL_MEMORY_MANAGER_BLOCK_ALLOCATION_MIN_SIZE)

// Minimum block reservation size to avoid creating a block too small while splitting up a reserved block.
// Contrary to block allocations, reservations don't have metadata.
#define SLI_BLOCK_RESERVATION_MIN_SIZE_BYTE   SL_MEMORY_MANAGER_BLOCK_ALLOCATION_MIN_SIZE
#define SLI_BLOCK_RESERVATION_MIN_SIZE_DWORD  SLI_BLOCK_LEN_BYTE_TO_DWORD(SL_MEMORY_MANAGER_BLOCK_ALLOCATION_MIN_SIZE)

// 64-bit word size (in octets).
#define SLI_WORD_SIZE_64                8u
// 32-bit word size (in octets).
#define SLI_WORD_SIZE_32                4u
#define SLI_DEF_INT_32_NBR_BITS         32u

// 1-byte size (in bits).
#define SLI_DEF_INT_08_NBR_BITS         8u

// Size of block metadata area in different units.
#define SLI_BLOCK_METADATA_SIZE_BYTE    sizeof(sli_block_metadata_t)
#define SLI_BLOCK_METADATA_SIZE_DWORD   SLI_BLOCK_LEN_BYTE_TO_DWORD(SLI_BLOCK_METADATA_SIZE_BYTE)

// Size of reservation handle area in different units.
#define SLI_RESERVATION_HANDLE_SIZE_BYTE    sizeof(sl_memory_reservation_t)
#define SLI_RESERVATION_HANDLE_SIZE_DWORD   SLI_BLOCK_LEN_BYTE_TO_DWORD(SLI_RESERVATION_HANDLE_SIZE_BYTE)

// Size of pool handle area in different units.
#define SLI_POOL_HANDLE_SIZE_BYTE    sizeof(sl_memory_pool_t)
#define SLI_POOL_HANDLE_SIZE_DWORD   SLI_BLOCK_LEN_BYTE_TO_DWORD(SLI_POOL_HANDLE_SIZE_BYTE)

#ifdef SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES
#define SLI_MAX_RESERVATION_COUNT 32
#endif

/*******************************************************************************
 **********************************   MACROS   *********************************
 ******************************************************************************/

// Macros to align a value to the nearest value multiple of the specified alignment
// (rounded up or down). These macros are used for memory addresses requiring an alignment.
#define SLI_ALIGN_ROUND_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))
#define SLI_ALIGN_ROUND_DOWN(num, align) ((num) & ~((align) - 1))

// Macros to convert block length in different units (bytes, double words).
// Byte to word will round up to account for extra bytes.
#define SLI_BLOCK_LEN_DWORD_TO_BYTE(len) ((len) * SLI_WORD_SIZE_64)
#define SLI_BLOCK_LEN_BYTE_TO_DWORD(len) ((len + SLI_WORD_SIZE_64 - 1) / SLI_WORD_SIZE_64)

// Macro to test address given a specified data alignment.
#define SLI_ADDR_IS_ALIGNED(ptr, align_byte)   (((uintptr_t)(const void *)(ptr)) % (align_byte) == 0)

// Macro to convert from bits to byte.
#define SLI_POOL_BITS_TO_BYTE(bits) (((bits) + 7u) / SLI_DEF_INT_08_NBR_BITS)

/*******************************************************************************
 *********************************   TYPEDEF   *********************************
 ******************************************************************************/

// Block metadata containing information about allocated block.
// This metadata allows to implement explicit free blocks list.
// NOTE: The metadata size should ideally be a multiple of 8 bytes (see description of
// SLI_BLOCK_ALLOC_MIN_ALIGN for other details) or at least multiple of CPU data size
// (e.g. 4 bytes for 32-bit CPU).
// 'length' is expressed in double words unit. It can described a block up to 512 KB (65535 * 8 bytes).
typedef struct {
  uint16_t block_in_use : 1;        // Flag indicating if block allocated or not.
  uint16_t heap_start_align : 1;    // Flag indicating if first block at heap start undergone a data payload adjustment.
#if defined(SLI_MEMORY_MANAGER_ENABLE_SYSTEMVIEW)
  uint16_t block_type : 1;          // Block type (LT or ST).
  uint16_t reserved : 13;           // Unallocated for future usage.
#else
  uint16_t reserved : 14;           // Unallocated for future usage.
#endif
  uint16_t length;                  // Block size (metadata not included just data payload), in double words (64 bit).
  uint16_t offset_neighbour_prev;   // Offset to previous neighbor, in double words. It includes metadata/payload sizes.
  uint16_t offset_neighbour_next;   // Offset to next neighbor, in double words.
} sli_block_metadata_t;

/*******************************************************************************
 ****************************   GLOBAL VARIABLES   *****************************
 ******************************************************************************/

extern sli_block_metadata_t *sli_free_st_list_head;
extern sli_block_metadata_t *sli_free_lt_list_head;
extern uint32_t sli_free_blocks_number;
#if defined(DEBUG_EFM) || defined(DEBUG_EFM_USER)
extern bool reserve_no_retention_first;
#endif

#ifdef SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES
extern sl_memory_reservation_t* sli_reservation_handle_ptr_table[];
extern uint32_t sli_reservation_alignment_table[];

extern sl_memory_reservation_t sli_reservation_no_retention_table[];
extern uint32_t sli_reservation_no_retention_alignment_table[];
#endif

// The heap name is also used as the Memory Profiler tracker handle for the heap
// pool managed by the Memory Manager
extern const char sli_mm_heap_name[];
extern const char sli_mm_heap_reservation_name[];

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * Initializes a memory block metadata to some reset values.
 *
 * @param[in] block_metadata  Pointer to block metadata.
 ******************************************************************************/
void sli_memory_metadata_init(sli_block_metadata_t *block_metadata);

/***************************************************************************//**
 * Gets pointer to the first free block of adequate size.
 *
 * @param[in]  size               Size of the block, in bytes.
 * @param[in]  align              Required alignment for the block, in bytes.
 * @param[in]  type               Type of block (long-term or short term).
 *                                  BLOCK_TYPE_LONG_TERM
 *                                  BLOCK_TYPE_SHORT_TERM
 * @param[in]  block_reservation  Indicates if the free block is for a dynamic
 *                                reservation.
 * @param[out] block              Pointer to variable that will receive the
 *                                start address of the free block.
 *
 * @return    Size of the block adjusted with the alignment.
 ******************************************************************************/
size_t sli_memory_find_free_block(size_t size,
                                  size_t align,
                                  sl_memory_block_type_t type,
                                  bool block_reservation,
                                  sli_block_metadata_t **block);

/***************************************************************************//**
 * Finds the next free block that will become the long-term or short-term head
 * pointer.
 *
 * @param[in]  type  Type of block (long-term or short term).
 *                     BLOCK_TYPE_LONG_TERM
 *                     BLOCK_TYPE_SHORT_TERM
 *
 * @param[in]  block_start_from  Pointer to block where to start searching.
 *                               NULL pointer means start from one of heap
 *                               ends according to the block type.
 *
 * @return     Pointer to the new free block.
 ******************************************************************************/
sli_block_metadata_t *sli_memory_find_head_free_block(sl_memory_block_type_t type,
                                                      sli_block_metadata_t *block_start_from);

/***************************************************************************//**
 * Gets long-term head pointer to the first free block.
 *
 * @return    Pointer to first free long-term block.
 ******************************************************************************/
void *sli_memory_get_longterm_head_ptr(void);

/***************************************************************************//**
 * Gets short-term head pointer to the first free block.
 *
 * @return    Pointer to first free short-term block.
 ******************************************************************************/
void *sli_memory_get_shortterm_head_ptr(void);

/***************************************************************************//**
 * Update free lists heads (short and long terms)
 *
 * @param[in]  free_head  Block from where to start searching or next free block.
 *
 * @param[in]  condition_block  Block condition to check if update is necessary
 *             or not.
 *
 * @param[in]  search  Boolean condition to check if searching the heap for a free
 *                     block is necessary.
 ******************************************************************************/
void sli_update_free_list_heads(sli_block_metadata_t *free_head,
                                const sli_block_metadata_t *condition_block,
                                bool search);

#ifdef SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES
/***************************************************************************//**
 * Gets the pointer to sl_memory_reservation_t{} by block address.
 *
 * @param[in]  addr  Pointer to the block reservation.
 *
 * @return    Pointer to reservation handle.
 ******************************************************************************/
sl_memory_reservation_t *sli_memory_get_reservation_handle_by_addr(void *addr);

/***************************************************************************//**
 * Gets the size of a reservation by block address.
 *
 * @param[in]  addr  Pointer to the block reservation.
 *
 * @return    Size of the reservation in bytes.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_size_by_addr(void *addr);

/***************************************************************************//**
 * Get the alignment of a reservation by block address.
 *
 * @param[in]  addr  Pointer to the block reservation.
 *
 * @return    Alignment of the reservation in bytes.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_align_by_addr(void *addr);

/***************************************************************************//**
 * Bookkeeps a reservation for profiling purposes.
 *
 * @param[in]  reservation_handle_ptr  Pointer to the reservation handle.
 * @param[in]  align                   Alignment of the reservation.
 *
 * @return    SL_STATUS_FULL if record is full.
 ******************************************************************************/
sl_status_t sli_memory_save_reservation_handle(sl_memory_reservation_t *reservation_handle_ptr,
                                               uint32_t align);

/***************************************************************************//**
 * Removes a reservation from records.
 *
 * @param[in]  reservation_handle_ptr  Pointer to the reservation handle.
 *
 * @return    SL_STATUS_NOT_FOUND if reservation is does not exist in records.
 ******************************************************************************/
sl_status_t sli_memory_remove_reservation_handle(sl_memory_reservation_t *reservation_handle_ptr);

/***************************************************************************//**
 * Bookkeeps a reservation (no retention) for profiling purposes.
 *
 * @param[in]  block_address  Pointer to the block reservation.
 * @param[in]  block_size  Size of the reservation.
 * @param[in]  align  Alignment of the reservation.
 *
 * @return    SL_STATUS_NOT_FOUND if reservation is does not exist in records.
 ******************************************************************************/
sl_status_t sli_memory_save_reservation_no_retention(void * block_address, uint32_t block_size, uint32_t align);

/***************************************************************************//**
 * Gets the size of a reservation (no retention) by block address.
 *
 * @param[in]  addr  Pointer to the block reservation.
 *
 * @return    Size of the reservation (no retention) in bytes.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_no_retention_size(void * addr);

/***************************************************************************//**
 * Gets the alignment of a reservation (no retention) by block address.
 *
 * @param[in]  addr  Pointer to the block reservation.
 *
 * @return    Alignment of the reservation in bytes.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_no_retention_align(void * addr);

/***************************************************************************//**
 * Does a heap integrity check forwards from sli_free_lt_list_head and return
 * the pointer to the corrupted sli_block_metadata_t{} (if applicable).
 * This could go past reservations so there are checks.
 *
 * @return    Pointer to the corrupted sli_block_metadata_t{}.
 ******************************************************************************/
sli_block_metadata_t * sli_memory_check_heap_integrity_forwards(void);

/***************************************************************************//**
 * Does a heap integrity check backwards from sli_free_st_list_head and return
 * the pointer to the corrupted sli_block_metadata_t{} (if applicable).
 * This should not go past any reservations, hence there are no checks.
 *
 * @return    Pointer to the corrupted sli_block_metadata_t{}.
 ******************************************************************************/
sli_block_metadata_t *sli_memory_check_heap_integrity_backwards(void);
#endif /* SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES */

#ifdef __cplusplus
}
#endif

#endif /* SLI_MEMORY_MANAGER_H_ */
