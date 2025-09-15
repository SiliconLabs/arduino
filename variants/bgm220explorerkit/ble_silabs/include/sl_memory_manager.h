/***************************************************************************//**
 * @file
 * @brief Memory Manager Driver API definition.
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

#ifndef SL_MEMORY_MANAGER_H_
#define SL_MEMORY_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "sl_status.h"
#include "sl_core.h"
#include "sl_memory_manager_region.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @addtogroup memory_manager Memory Manager
 *
 * @details
 * ## Overview
 *
 * The Memory Manager is a platform-level software module that provides different
 * ways to perform runtime allocations, either one shot or dynamic.
 * The Memory Manager complements the toolchain linker by managing the RAM memory
 * not allocated/partitioned by the linker. It offers different constructs that
 * will help the different Silicon Labs SDK software modules and your application
 * to build, as much as possible, an efficient and optimized RAM layout. The main
 * Memory Manager constructs will be:
 *   - A dynamic allocation API
 *   - A memory pool API
 *   - A dynamic reservation API
 *
 * The Memory Manager can be used in an RTOS context as it is thread-safe by
 * protecting adequately its internal shared resources.
 *
 * ## Initialization
 *
 * The initialization part includes the following configuration files:
 *   - \b sl_memory_manager_region_config.h
 *   - \b sl_memory_manager_config.h
 *
 * These header files offer a few configurations for the Memory Manager.
 * They use the CMSIS Configuration Wizard Annotations that can be rendered
 * by Simplicity Studio to set graphically the configuration settings value.
 *
 * The API function sl_memory_init() is used to initialize the Memory Manager
 *  module. This function must be called early during your initialization sequence.
 *  If the SL System component (@ref system) is used by your application, the
 *  sl_memory_init() call will be added automatically to your initialization
 *  sequence.
 *
 * \a sl_memory_manager_region_config.h allows to configure the stack size for
 * the application. The default value of 4096 bytes for SL_STACK_SIZE will
 * be used by the linker to allocate a stack zone in the RAM. In a baremetal
 * application, the stack size is bound to the value set by SL_STACK_SIZE.
 * So you should carefully size the stack in that case.
 * In an RTOS application, the stack size SL_STACK_SIZE will serve mainly for the
 * code running in the main() context until the kernel is launched. Once the kernel is
 * started, the different tasks' stacks, created upon tasks' creation, will allow
 * to save the different contexts (that is task, function, ISR contexts). The main
 * stack will be less active while the application's tasks are running.
 *
 * @note It is not possible to specify a minimum heap size via a configuration value
 *       in \a sl_memory_manager_region_config.h. The GCC and IAR linker files define
 *       a heap section in RAM that will be the last zone of the RAM partitioned by
 *       the toolchain linker. The size of this heap zone will be the remaining
 *       space of the RAM. If you need to perform some checks on the heap size, you
 *       should do it at runtime using the Memory Manager \ref subsubsection-statistics
 *       "statistics API". You cannot do it during the toolchain preprocessor time.
 *
 * ## Functionalities
 *
 * The Memory Manager offers different functionalities such as:
 *   - Dynamically allocating and freeing blocks.
 *   - Creating and deleting memory pools. Allocating and freeing fixed-size
 * blocks from a given pool.
 *   - Reserving and releasing blocks.
 *   - Getting statistics about the heap usage and the stack.
 *   - Retargeting the standard C library memory functions malloc()/free()/
 * calloc()/realloc() to the Memory Manager ones.
 *   - Overloading the C++ standard new/delete operators to the Memory Manager
 * malloc()/free()
 *
 * \subsubsection subsubsection-dynamic-allocation Dynamic Allocation
 * The dynamic allocation API allows to dynamically allocate and free memory blocks
 * of various sizes. The API supports the classic signatures of memory functions
 * malloc()/free()/calloc()/realloc() while also offering variants of the same
 * functions.
 *
 * <table>
 * <tr><th>Operation<th>Standard-Like Function<th>Variant Function
 * <tr><td>Allocating a block<td>sl_malloc()<td>
 * <ul>
 *     <li>sl_memory_alloc()
 *     <li>sl_memory_alloc_advanced()
 * </ul>
 * <tr><td>Freeing a block<td>sl_free()<td>sl_memory_free()
 * <tr><td>Allocating a block whose content is zero'ed<td>sl_calloc()<td>sl_memory_calloc()
 * <tr><td>Re-allocating a block<td>sl_realloc()<td>sl_memory_realloc()
 * </table>
 *
 * The variants functions \a sl_memory_xxxx() differs from the standard-like functions
 * with the following:
 *   - They return an error code of type \a sl_status_t. You may want
 * to process any returned error code different from \a SL_STATUS_OK.
 *   - They allow to specify a block alignment requirement in bytes. The alignment
 * can be any power-of-two values between 1 and 512 bytes inclusively. The default
 * block alignment the Memory Manager will use is 8 bytes to maximize CPU accesses
 * to allocated memory blocks.
 *   - They allow to specify a block type as long-term or short-term (further explained
 * below). The Memory Manager allows to allocate a block from different ends of the heap
 * to limit the fragmentation.
 *
 * Allocating a block can be done by specifying your requested size with the simple
 * sl_malloc(). If you have a special alignment requirement, the function
 * sl_memory_alloc_advanced() is the one to use.
 * The Memory Manager will use a first fit algorithm to find the block fitting
 * the requested size. If the found block is too large, the allocator tries to split it
 * to create a new free block from the unwanted portion of the found block. The block
 * internal split operation helps to limit the internal fragmentation.
 *
 * The dynamic allocation API allows to specify the block type as long-term
 * (BLOCK_TYPE_LONG_TERM) or short-term (BLOCK_TYPE_SHORT_TERM) with the
 * functions sl_memory_alloc() or sl_memory_alloc_advanced().
 * The long-term (LT) allocations are allocated from the heap start,
 * while short-term (ST) ones are allocated from the heap end. LT/ST allocations
 * relate to the expected lifetime of the block allocation. LT blocks are used for
 * the full duration of the application or for something that is expected
 * to last a long time. For instance, a control data structure enabling the proper
 * functioning of a stack's layer, a driver, a part of the application layer.
 * ST blocks are used for something that is expected to be freed quite quickly.
 * For example, a received buffer that needs to be processed and once processed
 * will be freed.
 * Grouping your allocations as LT blocks and/or ST blocks can help to limit the
 * heap fragmentation.
 * Certain functions does not allow to indicate the block type. In that case,
 * a default type is selected by the allocator.
 *
 * <table>
 * <tr><th>Function<th>Block type
 * <tr><td>sl_malloc()<td>Long-term by default<td>
 * <tr><td>sl_memory_alloc()<td>Long-term or short-term<td>
 * <tr><td>sl_memory_alloc_advanced()<td>Long-term or short-term<td>
 * <tr><td>sl_calloc()<td>Long-term by default<td>
 * <tr><td>sl_memory_calloc()<td>Long-term or short-term<td>
 * <tr><td>sl_realloc()<td>Long-term by default<td>
 * <tr><td>sl_memory_realloc()<td>Long-term by default<td>
 * </table>
 *
 * Freeing a block is done by calling sl_free() or sl_memory_free(). sl_memory_free()
 * returns an error code of type sl_status_t that you may want to test. Passing a NULL
 * pointer to sl_free() or sl_memory_free() results in a neutral situation where the
 * free() function will do nothing. If the same block is freed twice, the function
 * sl_memory_free() will return an error. During the free operation, the function will
 * try to merge adjacent blocks to the block that is being freed in order to limit
 * the internal fragmentation. The adjacent blocks must, of course, not be in use
 * to be merged.
 *
 * If you want to get a block from the heap whose content has been initialized to zero
 * to avoid any garbage values, the function sl_calloc() or sl_memory_calloc() can be
 * called.
 *
 * If you need to reallocate a block, the function sl_realloc() or sl_memory_realloc()
 * should be called. Both versions allow to:
 *   - Extend the block with the requested size greater than the original size.
 *   - Reduce the block with the requested size smaller than the original size.
 *   - Extend a different block with the requested size greater than the original size.
 *
 * The block can be moved elsewhere in the heap if it is impossible to extend it in its
 * current memory space. A reduced block will always stay in the original block space as the
 * allocator does not need to provide a different block.
 * The content of the reallocated memory block is preserved up to the lesser of the
 * new and old sizes, even if the block is moved to a new location. If the new size
 * is larger, the value of the newly allocated portion is indeterminate.
 * Some combinations of input parameters when calling sl_realloc() or sl_memory_realloc()
 * will lead to the same behavior as sl_malloc(), sl_memory_alloc() or sl_free(),
 * sl_memory_free() (cf. the sl_realloc() or sl_memory_realloc() function description
 * for more details about those combinations).
 *
 * The following code snippet shows a basic block allocation and
 * deallocation using the standard-like functions:
 * @code{.c}
 * uint8_t *ptr8;
 *
 * ptr8 = (uint8_t *)sl_malloc(200);
 * memset(ptr8, 0xAA, 100);
 * sl_free(ptr8);
 * @endcode
 *
 * This other code snippet shows the same basic block allocation and
 * deallocation using the variant functions:
 * @code{.c}
 * uint8_t *ptr8;
 * sl_status_t status;
 *
 * status = sl_memory_alloc(100, BLOCK_TYPE_LONG_TERM, (void **)&ptr8);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * memset(ptr8, 0xBB, 100);
 *
 * status = sl_memory_free(ptr8);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 * @endcode
 *
 * ### Memory Pool
 *
 * The memory pool API allows to:
 *   - Create a pool composed of N number of fixed-size blocks: sl_memory_create_pool().
 *   - Delete a pool: sl_memory_delete_pool().
 *   - Get a block from the pool: sl_memory_pool_alloc().
 *   - Free a pool's block: sl_memory_pool_free().
 *
 * Memory pools are convenient if you want to ensure a sort of guaranteed quotas
 * for some memory allocations situations. It is also more robust to unexpected
 * allocations errors as opposed to the dynamic allocation API in which a block
 * allocation can fail randomly if there is no free block to satisfy the requested
 * size.
 *
 * The memory pool API uses a pool handle. This handle is initialized when the pool
 * is created with sl_memory_create_pool(). Then this handle is passed as an input
 * parameter of the other functions. The handle can be allocated statically or
 * dynamically. A static pool handle means the handle of type
 * @ref sl_memory_pool_t "sl_memory_pool_t{}" is a global variable for example.
 * A dynamic pool handle means the handle is obtained from the heap itself by
 * calling the function sl_memory_pool_handle_alloc().The dynamic pool handle will
 * be freed with a call to sl_memory_pool_handle_free().
 *
 * The following code snippet shows a typical memory pool API sequence using
 * a static pool handle:
 * @code{.c}
 * uint8_t *ptr8;
 * sl_status_t status;
 * sl_memory_pool_t pool1_handle = { 0 };
 *
 * // Create a pool of 15 blocks whose size is 100 bytes for each block.
 * status = sl_memory_create_pool(100, 15, &pool1_handle);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * status = sl_memory_pool_alloc(&pool1_handle, (void **)&ptr8);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * memset(ptr8, 0xCC, 100);
 *
 * status = sl_memory_pool_free(&pool1_handle, ptr8);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * status = sl_memory_delete_pool(&pool1_handle);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 * @endcode
 *
 * This other code snippet presents the previous typical memory pool API sequence
 * using a dynamic pool handle:
 * @code{.c}
 * sl_status_t status;
 * sl_memory_pool_t *pool1_handle = NULL;
 *
 * status = sl_memory_pool_handle_alloc(&pool1_handle);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * // Create a pool of 15 blocks of 100 bytes in size.
 * status = sl_memory_create_pool(100, 15, &pool1_handle);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * // Get blocks from the pool, use them and free them once done.
 * ...
 *
 * status = sl_memory_delete_pool(&pool1_handle);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * status = sl_memory_pool_handle_free(pool1_handle);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 * @endcode
 *
 * ### Dynamic Reservation
 *
 * The dynamic reservation is a special construct allowing to reserve a block
 * of a given size with sl_memory_reserve_block() and to release it with
 * sl_memory_release_block(). The reserved block can then be used to any
 * application purposes. The reserved block will be taken from the
 * short-term section at the end of the heap.
 * Please note that the dynamic reservation API is not meant
 * to be used in the same way as the \ref subsubsection-dynamic-allocation
 * "dynamic allocation API".
 *
 * The dynamic reservation API uses a reservation handle. This handle is initialized
 * when the block is reserved with sl_memory_reserve_block(). Then this handle
 * is passed as an input parameter to the other functions. The handle can be
 * allocated statically or dynamically. A static reservation handle means the handle
 * of type @ref sl_memory_reservation_t "sl_memory_reservation_t{}" is a global
 * variable for example. A dynamic reservation handle means the handle is
 * obtained from the heap itself by calling the function
 * sl_memory_reservation_handle_alloc(). The dynamic reservaiton handle will be
 * freed with a call to sl_memory_reservation_handle_free().
 *
 * The following code snippet shows a typical dynamic reservation API sequence
 * using a static reservation handle:
 * @code{.c}
 * uint8_t *ptr8;
 * sl_status_t status;
 * sl_memory_reservation_t reservation_handle1 = { 0 };
 *
 * status = sl_memory_reserve_block(1024,
 *                                  SL_MEMORY_BLOCK_ALIGN_8_BYTES,
 *                                  reservation_handle1,
 *                                  (void **)&ptr8);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * memset(ptr8, 0xDD, 1024);
 *
 * status = sl_memory_release_block(&reservation_handle1);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 * @endcode
 *
 * This other code snippet demonstrates the previous typical dynamic reservation API
 * sequence using a dynamic reservation handle:
 * @code{.c}
 * uint8_t *ptr8;
 * sl_status_t status;
 * sl_memory_reservation_t *reservation_handle1;
 *
 * status = sl_memory_reservation_handle_alloc(&reservation_handle1);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * status = sl_memory_reserve_block(1024,
 *                                  SL_MEMORY_BLOCK_ALIGN_8_BYTES,
 *                                  reservation_handle1,
 *                                  (void **)&ptr8);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * memset(ptr8, 0xEE, 1024);
 *
 * status = sl_memory_release_block(&reservation_handle1);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 *
 * status = sl_memory_reservation_handle_free(reservation_handle1);
 * if (status != SL_STATUS_OK) {
 *   // Process the error condition.
 * }
 * @endcode
 *
 * \subsubsection subsubsection-statistics Statistics
 *
 * As your code is allocating and freeing blocks, you may want to know at a certain
 * instant what the current state of the heap is. Some heap statistics queries at
 * runtime can help to understand the current usage of the heap. By using the
 * following statistics functions, you may be able to perform some asynchronous
 * runtime heap checks:
 * - Total heap size: sl_memory_get_total_heap_size().
 * - Current free heap size: sl_memory_get_free_heap_size().
 * - Current used heap size: sl_memory_get_used_heap_size().
 * - Highest accumulated heap size usage: sl_memory_get_heap_high_watermark().
 *   - You can reset the high heap usage watermark with
 * sl_memory_reset_heap_high_watermark().
 *
 * Besides a few functions each dedicated to a specific statistic, the function
 * sl_memory_get_heap_info() allows to get a general heap information structure
 * of type @ref sl_memory_heap_info_t "sl_memory_heap_info_t{}" with several heap
 *  statistics. Some of them overlap the statistics returned by the dedicated
 * functions while the others complements statistics returned by the dedicated
 * functions. Refer to the description of @ref sl_memory_heap_info_t
 * "sl_memory_heap_info_t{}" for more information of each field.
 *
 * If you want to know the start address and the total size of the program's
 * stack and/or heap, simply call respectively the function sl_memory_get_stack_region()
 * and/or sl_memory_get_heap_region().
 *
 * ### C/C++ Toolchains Standard Memory Functions Retarget/Overload
 *
 * A program can perform dynamic memory allocations and deallocations using the
 * standard memory functions whose implementation is provided by the C or C++
 * toolchain libraries.
 * - C toolchain for the classic malloc()/free()/calloc()/realloc()
 * - C++ toolchain for the new/delete operators
 *
 * The Memory Manager supports the C standard memory functions retarget and the C++
 * new/delete overload.
 *
 * When the \b memory_manager component is installed, the C standard memory
 * functions are automatically retargeted to the Memory Manager ones:
 *   - GCC: coupled to the linker option "--wrap", the functions retargeted are
 *     - standard _malloc_r()  -> sl_malloc()
 *     - standard _free_r()    -> sl_free()
 *     - standard _calloc_r()  -> sl_calloc()
 *     - standard _realloc_r() -> sl_realloc()
 *   - IAR: it has three separate heap memory handlers (the basic, the advanced,
 * and the no-free heap handlers). IAR generally auto-selects one of the handlers.
 *     - Basic heap
 *       - standard __basic_malloc()  -> sl_malloc()
 *       - standard __basic_free()    -> sl_free()
 *       - standard __basic_calloc()  -> sl_calloc()
 *       - standard __basic_realloc() -> sl_realloc()
 *     - Advanced heap
 *       - standard __iar_dl_malloc()  -> sl_malloc()
 *       - standard __iar_dl_free()    -> sl_free()
 *       - standard __iar_dl_calloc()  -> sl_calloc()
 *       - standard __iar_dl_realloc() -> sl_realloc()
 *     - No Free heap
 *       - standard __no_free_malloc()  -> sl_malloc()
 *       - standard __no_free_calloc()  -> sl_calloc()
 *
 * If you need the C++ new/delete global overload calling sl_memory_alloc() and
 * sl_memory_free(), please install the additional component
 * \b memory_manager_cpp.
 * This global overload of new/delete operators will also apply to any C++
 * standard containers (for example vector, string, list).
 *
 * @note The Silicon Labs SDK generates a GCC or IAR linker script with Simplicity
 * Studio. A typical toolchain linker script will define a section called "heap"
 * or "HEAP". Usually, the C memory standard functions will assume a linker-defined
 * "heap" section exists. If the memory_manager component is present, the
 * toolchain linker script will define a new heap section named "memory_manager_heap"
 * or "MEMORY_MANAGER_HEAP". Since the Memory Manager retargets the standard
 * function malloc()/free()/calloc()/realloc() to the Memory Manager ones, there
 * should not be any issues in your program. If an unlikely situation occurs where
 * the toolchain standard memory functions retarget does not work, your application
 * might end up calling a standard malloc() implementation from the
 * toolchain instead of the Memory Manager one. In that case, a runtime
 * error can occur and it is expected. You should then review the project settings
 * to detect why the Memory Manager retarget did not work properly.
 *
 * ## Hints
 * ### Memory Allocations from ISR
 *
 * In general, ISR must be kept short. Allocating and freeing blocks from an ISR is
 * possible but you should be careful. Nothing really prevents you from calling the
 * dynamic allocation API functions such as sl_malloc() and sl_free(). But keep in
 * mind a few things with the dynamic allocation API:
 *   - The dynamic allocation API functions protect their internal resources
 * such as global lists managing the heap metadata by using critical sections.
 * So when in your ISR, you will disable interrupts for a certain period of time,
 * preventing other interrupts to be processed in time if your application has hard
 * real-time constraints. This increases the overall interrupt latency of your
 * system if this ISR executes very often to perform a dynamic memory operation
 *   - They can introduce non-deterministic behavior which is undesirable if your
 * application requires crucial precise timing
 *   - A function such as sl_malloc() can fail if there is no block to satisfy
 * your requested size allocation. Implementing the proper error handling in the
 * ISR may increase the time spent in the ISR.
 *
 * In the end, it really depends of your ISR processing context doing memory
 * allocations/deallocations. If you really need to perform dynamic allocation from
 * ISR, it may be better at least to use a memory pool. Getting and releasing a block
 * from a  pool is an operation more deterministic. And if you have properly
 * sized your pool with a number of available blocks, you are less likely to
 * encounter an allocation error.
 *
 * @{
 *****************************************************************************/

// ----------------------------------------------------------------------------
// DEFINES

/// Special value to indicate the default block alignment to the Memory Manager
/// allocator. 8 bytes is the minimum alignment to account for largest CPU data
/// type that can be used in some block allocation scenarios.
#define SL_MEMORY_BLOCK_ALIGN_DEFAULT     0xFFFFFFFFU

/// Pre-defined values for block alignment managed by the Memory Manager allocator.
#define SL_MEMORY_BLOCK_ALIGN_8_BYTES     8U      ///< 8 bytes alignment.
#define SL_MEMORY_BLOCK_ALIGN_16_BYTES    16U     ///< 16 bytes alignment.
#define SL_MEMORY_BLOCK_ALIGN_32_BYTES    32U     ///< 32 bytes alignment.
#define SL_MEMORY_BLOCK_ALIGN_64_BYTES    64U     ///< 64 bytes alignment.
#define SL_MEMORY_BLOCK_ALIGN_128_BYTES   128U    ///< 128 bytes alignment.
#define SL_MEMORY_BLOCK_ALIGN_256_BYTES   256U    ///< 256 bytes alignment.
#define SL_MEMORY_BLOCK_ALIGN_512_BYTES   512U    ///< 512 bytes alignment.

// ----------------------------------------------------------------------------
// DATA TYPES

/// @brief Block type.
typedef enum {
  BLOCK_TYPE_LONG_TERM = 0,   ///< Long-term block type.
  BLOCK_TYPE_SHORT_TERM = 1   ///< Short-term block type.
} sl_memory_block_type_t;

/// @brief General purpose heap information.
typedef struct {
  uint32_t base_addr;               ///< Heap base address.
  size_t used_size;                 ///< Used size (in bytes), independently of alignment.
  size_t free_size;                 ///< Free size (in bytes), independently of alignment.
  size_t total_size;                ///< Total heap size (in bytes).
  size_t free_block_count;          ///< Number of free blocks.
  size_t free_block_largest_size;   ///< Largest free block size (in bytes).
  size_t free_block_smallest_size;  ///< Smallest free block size (in bytes).
  size_t used_block_count;          ///< Number of used blocks.
  size_t used_block_largest_size;   ///< Largest used block size (in bytes).
  size_t used_block_smallest_size;  ///< Smallest used block size (in bytes).
} sl_memory_heap_info_t;

/// @brief Memory block reservation handle.
typedef struct {
  void *block_address;                 ///< Reserved block base address.
  size_t block_size;                   ///< Reserved block size (in bytes).
} sl_memory_reservation_t;

/// @brief Memory pool handle.
typedef struct {
#if defined(SL_MEMORY_POOL_POWER_AWARE)
  sl_memory_reservation_t *reservation; ///< Pointer to reservation handle.
#else
  void *block_address;                 ///< Reserved block base address.
#endif
  uint32_t *block_free;                 ///< Pointer to pool's free blocks list.
  size_t block_count;                   ///< Max quantity of blocks in the pool.
  size_t block_size;                    ///< Size of each block.
} sl_memory_pool_t;

// ----------------------------------------------------------------------------
// PROTOTYPES

/***************************************************************************//**
 * Initializes the memory manager.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function should only be called once.
 ******************************************************************************/
sl_status_t sl_memory_init(void);

/***************************************************************************//**
 * Reserves a memory block that will never need retention in EM2.
 *
 * @param[in]  size   Size of the block, in bytes.
 * @param[in]  align  Required alignment for the block, in bytes.
 * @param[out] block  Pointer to variable that will receive the start address
 *                    of the allocated block. NULL in case of error condition.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Required alignment of memory block (in bytes) MUST be a power of 2
 *        and can range from 1 to 512 bytes.
 *        The define SL_MEMORY_BLOCK_ALIGN_DEFAULT can be specified to select
 *        the default alignment.
 ******************************************************************************/
sl_status_t sl_memory_reserve_no_retention(size_t size,
                                           size_t align,
                                           void **block);

/***************************************************************************//**
 * Allocates a memory block of at least requested size from the heap. Simple
 * version.
 *
 * @param[in] size  Size of the block, in bytes.
 *
 * @return    Pointer to allocated block if successful. Null pointer if
 *            allocation failed.
 *
 * @note Requesting a block of 0 byte will return a null pointer.
 *
 * @note All allocated blocks using this function will be considered long-term
 *       allocations.
 ******************************************************************************/
void *sl_malloc(size_t size);

/***************************************************************************//**
 * Dynamically allocates a block of memory.
 *
 * @param[in]  size   Size of the block, in bytes.
 * @param[in]  type   Type of block (long-term or short-term).
 *                      BLOCK_TYPE_LONG_TERM
 *                      BLOCK_TYPE_SHORT_TERM
 * @param[out] block  Pointer to variable that will receive the start address
 *                    of the allocated block. NULL in case of error condition.
 *
 * @return     SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_alloc(size_t size,
                            sl_memory_block_type_t type,
                            void **block);

/***************************************************************************//**
 * Dynamically allocates a block of memory. Advanced version that allows to
 * specify alignment.
 *
 * @param[in]  size     Size of the block, in bytes.
 * @param[in]  align    Required alignment for the block, in bytes.
 * @param[in]  type     Type of block (long-term or short term).
 *                        BLOCK_TYPE_LONG_TERM
 *                        BLOCK_TYPE_SHORT_TERM
 * @param[out] block    Pointer to variable that will receive the start address
 *                      of the allocated block. NULL in case of error condition.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Required alignment of memory block (in bytes) MUST be a power of 2
 *        and can range from 1 to 512 bytes.
 *        The define SL_MEMORY_BLOCK_ALIGN_DEFAULT can be specified to select
 *        the default alignment.
 ******************************************************************************/
sl_status_t sl_memory_alloc_advanced(size_t size,
                                     size_t align,
                                     sl_memory_block_type_t type,
                                     void **block);

/***************************************************************************//**
 * Frees a previously allocated block back into the heap. Simple version.
 *
 * @param[in] ptr   Pointer to memory block to be freed.
 *
 * @note Passing a null pointer does nothing.
 ******************************************************************************/
void sl_free(void *ptr);

/***************************************************************************//**
 * Frees a dynamically allocated block of memory.
 *
 * @param[in] block   Pointer to the block that must be freed.
 *
 * @return    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_free(void *block);

/***************************************************************************//**
 * Dynamically allocates a block of memory cleared to 0. Simple version.
 *
 * @param[in] item_count  Number of elements to be allocated.
 * @param[in] size        Size of each elements, in bytes.
 *
 * @return Pointer to allocated block if successful. Null pointer if
 *         allocation failed.
 *
 * @note All allocated blocks using this function will be considered long-term
 *       allocations.
 ******************************************************************************/
void *sl_calloc(size_t item_count,
                size_t size);

/***************************************************************************//**
 * Dynamically allocates a block of memory cleared to 0.
 *
 * @param[in]  item_count   Number of elements to be allocated.
 * @param[in]  size         Size of each elements, in bytes.
 * @param[in]  type         Type of block (long-term or short-term).
 *                            BLOCK_TYPE_LONG_TERM
 *                            BLOCK_TYPE_SHORT_TERM
 * @param[out] block        Pointer to variable that will receive the start
 *                          address of the allocated block. NULL in case of
 *                          error condition.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_calloc(size_t item_count,
                             size_t size,
                             sl_memory_block_type_t type,
                             void **block);

/***************************************************************************//**
 * Resizes a previously allocated memory block. Simple version.
 *
 * @param[in] ptr   Pointer to the allocation to resize. If NULL, behavior
 *                  is same as sl_malloc(), sl_memory_alloc().
 * @param[in] size  New size of the block, in bytes. If 0, behavior is same as
 *                  sl_free(), sl_memory_free().
 *
 * @return Pointer to newly allocated block, if successful. Null pointer if
 *         re-allocation failed.
 *
 * @note All re-allocated blocks using this function will be considered
 *       long-term allocations.
 *
 * @note 'ptr' NULL and 'size' of 0 bytes is an incorrect parameters
 *       combination. No reallocation will be done by the function as it is
 *       an error condition.
 *
 * @note If the new 'size' is the same as the old, the function changes nothing
 *       and returns the same provided address 'ptr'.
 ******************************************************************************/
void *sl_realloc(void *ptr,
                 size_t size);

/***************************************************************************//**
 * Resizes a previously allocated memory block.
 *
 * @param[in]  ptr   Pointer to the allocation to resize. If NULL, behavior
 *                   is same as sl_malloc(), sl_memory_alloc().
 * @param[in]  size  New size of the block, in bytes. If 0, behavior is same as
 *                   sl_free(), sl_memory_free().
 * @param[out] block Pointer to variable that will receive the start address of
 *                   the new allocated memory. NULL in case of error condition.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note All re-allocated blocks using this function will be considered
 *       long-term allocations.
 *
 * @note 'ptr' NULL and 'size' of 0 bytes is an incorrect parameters
 *       combination. No reallocation will be done by the function as it is
 *       an error condition.
 *
 * @note If the new 'size' is the same as the old, the function changes nothing
 *       and returns the same provided address 'ptr'.
 ******************************************************************************/
sl_status_t sl_memory_realloc(void *ptr,
                              size_t size,
                              void **block);

/***************************************************************************//**
 * Dynamically reserves a block of memory.
 *
 * @param[in]  size    Size of the block, in bytes.
 * @param[in]  align   Required alignment for the block, in bytes.
 * @param[in]  handle  Handle to the reserved block.
 * @param[out] block   Pointer to variable that will receive the start address
 *                     of the allocated block. NULL in case of error condition.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Required alignment of memory block (in bytes) MUST be a power of 2
 *        and can range from 1 to 512 bytes.
 *        The define SL_MEMORY_BLOCK_ALIGN_DEFAULT can be specified to select
 *        the default alignment.
 ******************************************************************************/
sl_status_t sl_memory_reserve_block(size_t size,
                                    size_t align,
                                    sl_memory_reservation_t *handle,
                                    void **block);

/***************************************************************************//**
 * Frees a dynamically reserved block of memory.
 *
 * @param[in] handle  Handle to the reserved block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_release_block(sl_memory_reservation_t *handle);

/***************************************************************************//**
 * Dynamically allocates a block reservation handle.
 *
 * @param[out] handle  Handle to the reserved block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_reservation_handle_alloc(sl_memory_reservation_t **handle);

/***************************************************************************//**
 * Frees a dynamically allocated block reservation handle.
 *
 * @param[in] handle  Handle to the reserved block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_reservation_handle_free(sl_memory_reservation_t *handle);

/***************************************************************************//**
 * Gets the size of the memory reservation handle structure.
 *
 * @return  Memory reservation handle structure's size in bytes.
 ******************************************************************************/
uint32_t sl_memory_reservation_handle_get_size(void);

/***************************************************************************//**
 * Creates a memory pool.
 *
 * @param[in] block_size    Size of each block, in bytes.
 * @param[in] block_count   Number of blocks in the pool.
 * @param[in] pool_handle   Handle to the memory pool.
 *
 * @note  This function assumes the 'pool_handle' is provided by the caller:
 *        - either statically (e.g. as a global variable)
 *        - or dynamically by calling sl_memory_pool_handle_alloc().
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_create_pool(size_t block_size,
                                  uint32_t block_count,
                                  sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Deletes a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note All pool allocations need to be freed by calling sl_memory_pool_free()
 *       on each block before calling sl_memory_delete_pool().
 *
 * @note The pool_handle provided is neither freed or invalidated. It can be
 *       reused in a new call to sl_memory_create_pool() to create another pool.
 ******************************************************************************/
sl_status_t sl_memory_delete_pool(sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Allocates a block from a memory pool.
 *
 * @param[in]  pool_handle Handle to the memory pool.
 * @param[out] block       Pointer to a variable that will receive the address
 *                         of the allocated block. NULL in case of error
 *                         condition.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_alloc(sl_memory_pool_t *pool_handle,
                                 void **block);

/***************************************************************************//**
 * Frees a block from a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 * @param[in] block       Pointer to the block to free.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_free(sl_memory_pool_t *pool_handle,
                                void *block);

/***************************************************************************//**
 * Dynamically allocates a memory pool handle.
 *
 * @param[out] pool_handle Handle to the memory pool.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_handle_alloc(sl_memory_pool_t **pool_handle);

/***************************************************************************//**
 * Frees a dynamically allocated memory pool handle.
 *
 * @param[in] pool_handle Handle to the memory pool.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_handle_free(sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Gets the size of the memory pool handle structure.
 *
 * @return  Memory pool handle structure's size.
 ******************************************************************************/
uint32_t sl_memory_pool_handle_get_size(void);

/***************************************************************************//**
 * Gets the total count of blocks in a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 *
 * @return  Total number of blocks.
 ******************************************************************************/
uint32_t sl_memory_pool_get_total_block_count(const sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Gets the count of free blocks in a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 *
 * @return  Number of free blocks.
 ******************************************************************************/
uint32_t sl_memory_pool_get_free_block_count(const sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Gets the count of used blocks in a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 *
 * @return  Number of used blocks.
 ******************************************************************************/
uint32_t sl_memory_pool_get_used_block_count(const sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Populates an sl_memory_heap_info_t{} structure with the current status of
 * the heap.
 *
 * @param[in] heap_info Pointer to structure that will receive further heap
 *                      information data.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_get_heap_info(sl_memory_heap_info_t *heap_info);

/***************************************************************************//**
 * Gets the total size of the heap.
 *
 * @return  Heap's size in bytes.
 ******************************************************************************/
size_t sl_memory_get_total_heap_size(void);

/***************************************************************************//**
 * Gets the current free heap size.
 *
 * @return  Free heap size in bytes.
 ******************************************************************************/
size_t sl_memory_get_free_heap_size(void);

/***************************************************************************//**
 * Gets the current used heap size.
 *
 * @return  Used heap size in bytes.
 ******************************************************************************/
size_t sl_memory_get_used_heap_size(void);

/***************************************************************************//**
 * Gets heap high watermark.
 *
 * @return  Highest heap usage in bytes recorded.
 ******************************************************************************/
size_t sl_memory_get_heap_high_watermark(void);

/***************************************************************************//**
 * Reset heap high watermark to the current heap used.
 ******************************************************************************/
void sl_memory_reset_heap_high_watermark(void);

/** @} (end addtogroup memory_manager) */

#ifdef __cplusplus
}
#endif

#endif /* SL_MEMORY_MANAGER_H_ */
