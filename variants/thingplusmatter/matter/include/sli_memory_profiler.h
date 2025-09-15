/***************************************************************************//**
 * @file
 * @brief Macros and functions for memory profiling
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

#ifndef SLI_MEMORY_PROFILER_H
#define SLI_MEMORY_PROFILER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "sl_status.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)

// The component catalog is present, so we're in an application build and can
// check if the Memory Profiler is present
#include "sl_component_catalog.h"
#if defined(SL_CATALOG_MEMORY_PROFILER_PRESENT)
// The Memory Profiler is present. Use its configuration and enable profiling.
#include "sli_memory_profiler_config.h"
#define SLI_MEMORY_PROFILER_ENABLE_PROFILING 1
#else
// The Memory Profiler is not present. Disable profiling.
#define SLI_MEMORY_PROFILER_ENABLE_PROFILING 0
#endif // defined(SL_CATALOG_MEMORY_PROFILER_PRESENT)

#else // defined(SL_COMPONENT_CATALOG_PRESENT)

// The component catalog is not present, so we're in a library build. The build
// environment of the library must specify the configuration defines that affect
// the macros in this header if it wants a non-default configuration. We default
// to enabling basic profiling but disabling ownership tracking.
#if !defined(SLI_MEMORY_PROFILER_ENABLE_PROFILING)
#define SLI_MEMORY_PROFILER_ENABLE_PROFILING 1
#endif

#if !defined(SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING)
#define SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING 0
#endif

#endif // defined(SL_COMPONENT_CATALOG_PRESENT)

#ifdef __cplusplus
extern "C" {
#endif

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
/***************************************************************************//**
 * @addtogroup memory_profiler Memory Profiler
 * @{
 *
 * @brief Memory profiler for tracking and reporting system RAM usage
 *
 * This memory profiler component provides a tool for tracking and reporting of
 * system RAM usage. It is implemented as a combination of a simple component
 * running in the Device Under Test (DUT) and a Python script running on a
 * computer. The on-DUT component is a relatively thin layer that uses J-Link
 * RTT (Real Time Transfer) to output events about memory allocation on the
 * device. The Python script on the computer receives these events and maintains
 * the memory usage bookkeeping. The script is available in
 * `platform/service/memory_manager/profiler/scripts/memory_profiler_platform.py`.
 *
 * The memory profiler represents memory in a hierarchical structure with the
 * physical RAM as the root of the tree. The physical RAM is split into
 * allocations such as stack and heap, and the heap is further split into
 * allocations for different uses of memory.
 *
 * Each node in the tree is represented by one instance of a memory tracker.
 * Each tracker has a short description and records the current and peak number
 * of bytes allocated. Leaves in the tree represent concrete uses of RAM, for
 * example allocations of a particular object types in a wireless stack.
 * Intermediate nodes typically correspond to memory allocation abstractions,
 * such as memory pools or helper functions that are used by the concrete users
 * of memory.
 *
 * The trackers can represent two different types of allocation behavior:
 *
 * 1. Pool trackers represent memory abstractions that make one contiguous
 *    allocation from their parent memory to allocate a pool. Portions of that
 *    pool are then given to callers that allocate memory from the pool
 *    abstraction. The Memory Manager (MM) heap is one example of such a
 *    pool. The MM heap reserves some portion of the physical RAM at
 *    initialization time and gives portions of that pool to its callers.
 *    Another example of a pool tracker are the individual pools that can be
 *    created and deleted at runtime using the MM Pool API. Each pool is
 *    associated with a pool tracker to track the allocation and freeing of the
 *    pool items during the lifetime of the pool.
 *
 * 2. Trackers that are not pools are used to collect the allocation statistics
 *    of a certain type of allocation or use case so that the memory use of that
 *    particular type can be easily distinguished from all the other users of
 *    memory. For example, the Memory Manager creates a separate tracker
 *    for long-term heap allocations, short-term heap allocations, and heap
 *    reservations, so that these can be counted individually. Components such
 *    as wireless stacks may create their own trackers for tracking things like
 *    the total sum of allocations made by a particular component, or more
 *    granularly the allocations made for a particular type of a resource.
 *
 * The Memory Manager is integrated to the Memory Profiler and all
 * allocations from the heap are automatically tracked by the top-level trackers
 * for the heap. This provides the usage tracking for the heap as a whole, i.e.
 * how many bytes of heap are used in total. If a component or an application
 * wants to track how many bytes it has allocated from the heap, it can create a
 * new memory profiler tracker. Pool memory trackers are created with a call to
 * `sli_memory_profiler_create_pool_tracker()` and other trackers are created
 * with `sli_memory_profiler_create_tracker()`.
 *
 * Both types of trackers are identified by a handle of type
 * `sli_memory_tracker_handle_t`, which is just a void pointer that is provided
 * by the code that creates the tracker. Code that creates a tracker can use any
 * void pointer that is guaranteed to be unique among all trackers for the
 * duration of the tracker lifetime, i.e. until the tracker is deleted with a
 * call to `sli_memory_profiler_delete_tracker()`. A good approach is to use a
 * pointer that is tightly associated with the tracked object or use case. For
 * example, a tracker for a memory pool can use the pool handle as the tracker
 * handle. A function that wants to track the allocations it makes can use its
 * own function pointer as the tracker handle.
 *
 * Top-level trackers for the heap are created at platform initialization time
 * and are never deleted. Other trackers have the same lifetime as the entity or
 * component that is being tracked. For example, when a memory pool is created,
 * a tracker is created for the pool. When the pool is deleted, the tracker is
 * also deleted. Similarly if a component creates a tracker for its memory use
 * when the component is initialized or started, the component would typically
 * delete the tracker for its memory when the component is de-initialized or
 * stopped. Trackers are deleted with a call to
 * `sli_memory_profiler_delete_tracker()`.
 *
 * The memory profiler implementation aims to minimize the impact of the memory
 * profiler calls on both RAM and CPU usage when the profiling is not enabled by
 * including the component in the SW configuration. To keep the calling code
 * clean of conditional compilation but still allow completely removing the
 * tracking calls when the profiler is not used, the calling code is encouraged
 * to do the tracking via the macros such as @ref
 * SLI_MEMORY_PROFILER_TRACK_ALLOC() and @ref SLI_MEMORY_PROFILER_TRACK_FREE().
 * These macros will automatically expand to empty when the profiling is
 * disabled at build time.
 *
 * If a component is compiled into a library (as opposed to being compiled at
 * application build time), the library may contain calls to the Memory Profiler
 * API even if tracking will be disabled in the application. In this case the SW
 * is linked with a stub Memory Profiler API implementation that provides dummy
 * functions. The stub implementation does not use any RAM or RTT, so the
 * overhead is minimized.
 *
 * Trackers use @ref SLI_MEMORY_PROFILER_TRACK_ALLOC() to track the allocation of
 * memory blocks and include the bytes of RAM to the counts in the trackers. To
 * make sure that any arbitrary pointer can be correctly mapped to the
 * allocation it belongs to, every tracker must accurately track the pointer and
 * size of the memory block that they are operating with. As an example, when an
 * allocation from the MM heap is made, the MM heap implementation tracks the
 * full allocation RAM block including any extra metadata or size padding that
 * is used around the block returned to the caller. The caller will track the
 * allocation using the size it requested from the heap (which is smaller than
 * the full block needed to satisfy the allocation), and the pointer it received
 * from the heap (which is within the full block allocated from the RAM).
 *
 * Trackers use @ref SLI_MEMORY_PROFILER_TRACK_FREE() to mark the freeing of a
 * memory block. Here too the callers must accurately identify the freed block
 * by providing the same pointer they provided in the corresponding call to
 * track the allocation. The tracking of the free does not need to be fully
 * symmetric, though. As an example, assume that an app has created the tracker
 * `app_mallocs` and is using @ref SLI_MEMORY_PROFILER_TRACK_ALLOC() to track on
 * that tracker every block it received from @ref sl_malloc(). When the
 * application calls @ef sl_free() to free a memory block, the application does
 * not need to call @ref SLI_MEMORY_PROFILER_TRACK_FREE() to mark the freeing.
 * The MM heap will track the freeing in `sl_free()` and the freeing of the
 * memory is automatically counted in the `app_mallocs` tracker as well.
 *
 ******************************************************************************/

/**
 * @brief Memory tracker handle type
 */
typedef const void* sli_memory_tracker_handle_t;

/**
 * @brief Value used to indicate an invalid memory tracker handle
 */
#define SLI_INVALID_MEMORY_TRACKER_HANDLE ((sli_memory_tracker_handle_t) NULL)

/**
 * @brief Initialize the memory profiler
 *
 * Memory Profiler initialization is handled internally by the Platform
 * initialization. Applications do not need to and should not call this function
 * directly.
 */
void sli_memory_profiler_init();

/**
 * @brief Get the current program counter
 *
 * This helper can be used to obtain the current program counter when invoking
 * @ref SLI_MEMORY_PROFILER_TRACK_OWNERSHIP() with the intention of assigning
 * the ownership to the location that is invoking the tracking macro.
 *
 * @return Current program counter
 */
#if defined(__GNUC__)
__attribute__( (always_inline) ) static inline void * sli_memory_profiler_get_pc(void)
{
  void *pc;
  __asm volatile ("MOV %0, PC" : "=r" (pc));
  return pc;
}
#elif defined(__IAR_SYSTEMS_ICC__)
_Pragma("inline=forced") static inline void * sli_memory_profiler_get_pc(void)
{
  void *pc;
  __asm volatile ("MOV %0, PC" : "=r" (pc));
  return pc;
}
#else
static inline void * sli_memory_profiler_get_pc(void)
{
  // Memory Profiler supports ownership tracking only with the GCC or IAR compiler
  return NULL;
}
#endif

/**
 * @brief Get the return address of the current function
 *
 * This helper can be used to obtain the return address of the current function
 * when invoking @ref SLI_MEMORY_PROFILER_TRACK_OWNERSHIP() with the intention
 * of assigning the ownership to the location that made the call to the function
 * that is invoking the tracking macro.
 *
 * Note that the IAR compiler does not provide a mechanism to reliably obtain
 * the return address, so we must emulate that by taking the content of the link
 * register. This is never completely reliable, but is guaranteed to fail if
 * `sli_memory_profiler_get_return_address()` is used in a function after calls
 * to other functions have already been made. Therefore this function should
 * always be called right at the beginning of the function that wants to know
 * the return address.
 *
 * @return The return address of the current function
 */
#if defined(__GNUC__)
__attribute__( (always_inline) ) static inline void * sli_memory_profiler_get_return_address(void)
{
  return __builtin_extract_return_addr(__builtin_return_address(0));
}
#elif defined(__IAR_SYSTEMS_ICC__)
_Pragma("inline=forced") static inline void * sli_memory_profiler_get_return_address(void)
{
  uint32_t lr;
  lr = __get_LR();
  return (void *)lr;
}
#else
static inline void * sli_memory_profiler_get_return_address(void)
{
  // Memory Profiler supports ownership tracking only with the GCC or IAR compiler
  return NULL;
}
#endif

/**
 * @brief Create a pool memory tracker
 *
 * This function creates a memory tracker for a memory use case that allocates
 * one fixed-size pool from some allocator and then allocates portions of the
 * pool to its own clients.
 *
 * The memory block specified by @p pointer and @p size must match a memory
 * allocation that the pool implementation has successfully obtained from some
 * memory allocator that has tracked the allocation. If the pool implementation
 * later frees the pool memory block (for example when the component that uses
 * the pool is de-initialized), the pool implementation must delete the pool
 * tracker with a call to @ref sli_memory_profiler_delete_tracker before it
 * frees the memory block that the pool was using.
 *
 * @param[in] tracker_handle The handle to identify the tracker. The handle must
 *   remain unique among all trackers until the tracker is deleted with @ref
 *   sli_memory_profiler_delete_tracker.
 * @param[in] description Short description of the usage of the tracker memory,
 *   or NULL to omit the description. The description can be set or updated
 *   later with a call to @ref sli_memory_profiler_describe_tracker.
 * @param[in] ptr Pointer to the pool block allocated from the parent memory
 * @param[in] size The size of the pool block allocated from the parent memory
 *
 * @return SL_STATUS_OK if a tracker was created, SL_STATUS_NOT_AVAILABLE if the
 *   Memory Profiler is not included in the application.
 */
sl_status_t sli_memory_profiler_create_pool_tracker(sli_memory_tracker_handle_t tracker_handle,
                                                    const char *description,
                                                    void* ptr,
                                                    size_t size);

/**
 * @brief Create a memory tracker
 *
 * This function creates a memory tracker for a use case that allocates blocks
 * from tracked parent allocator.
 *
 * @param[in] tracker_handle The handle to identify the tracker. The handle must
 *   remain unique among all trackers until the tracker is deleted with @ref
 *   sli_memory_profiler_delete_tracker.
 * @param[in] description Short description of the usage of the tracker memory,
 *   or NULL to omit the description. The description can be set or updated
 *   later with a call to @ref sli_memory_profiler_describe_tracker.
 *
 * @return SL_STATUS_OK if a tracker was created, SL_STATUS_NOT_AVAILABLE if the
 *   Memory Profiler is not included in the application.
 */
sl_status_t sli_memory_profiler_create_tracker(sli_memory_tracker_handle_t tracker_handle,
                                               const char *description);

/**
 * @brief Add or update a description to a previously created memory tracker
 *
 * This function is typically used to assign a description to a tracker that has
 * been created by a lower layer that does not know the use case of the memory.
 * For example, when the Memory Manager creates a memory pool object, it
 * creates a Memory Profiler tracker with the same handle as the pool handle but
 * cannot assign a descriptive name, as it cannot know what the pool is used
 * for. The caller that created the pool can then use this function with the
 * pool handle to assign a description for the pool memory.
 *
 * @param[in] tracker_handle The handle of the memory tracker
 * @param[in] description Short description of the usage of the tracker memory,
 *   or NULL to clear the description.
 */
void sli_memory_profiler_describe_tracker(sli_memory_tracker_handle_t tracker_handle,
                                          const char *description);

/**
 * @brief Delete a memory tracker
 *
 * This function deletes a previously created memory tracker.
 *
 * @param[in] tracker_handle The handle of the memory tracker
 */
void sli_memory_profiler_delete_tracker(sli_memory_tracker_handle_t tracker_handle);

/**
 * @brief Track the allocation of a memory block
 *
 * NOTE: This function is intended to be called via the @ref
 * SLI_MEMORY_PROFILER_TRACK_ALLOC() macro.
 *
 * @param[in] tracker_handle The handle of the memory tracker
 * @param[in] ptr Pointer to the allocated memory or NULL if allocation failed
 * @param[in] size The number of bytes allocated, or attempted to allocate
 */
void sli_memory_profiler_track_alloc(sli_memory_tracker_handle_t tracker_handle,
                                     void * ptr,
                                     size_t size);

/**
 * @brief Track the reallocation of a previously allocated memory block
 *
 * NOTE: This function is intended to be called via the @ref
 * SLI_MEMORY_PROFILER_TRACK_REALLOC() macro.
 *
 * NOTE: Reallocation is a special operation that is intended to be tracked only
 * by the underlying heap allocator in the Memory Manager. Any
 * higher-level allocations that are tracked within the reallocated block are
 * automatically moved and resized without any tracking calls from the
 * higher-level trackers.
 *
 * If the realloc operation involves allocating a new block and freeing the
 * previous block, the Memory Manager heap must track the realloc when
 * the allocation of the new memory has been tracked but the old memory block
 * has not been freed yet. This is needed to guarantee that both @p ptr and @p
 * realloced_ptr are valid and owned by the calling thread when the realloc is
 * tracked.
 *
 * @param[in] tracker_handle The handle of the lowest-layer heap memory tracker
 * @param[in] ptr Pointer to the original memory block
 * @param[in] realloced_ptr Pointer to the resized or allocated memory
 * @param[in] size The size that the block was reallocated to
 */
void sli_memory_profiler_track_realloc(sli_memory_tracker_handle_t tracker_handle,
                                       void * ptr,
                                       void * realloced_ptr,
                                       size_t size);

/**
 * @brief Track the allocation of a memory block and record ownership
 *
 * NOTE: This function is intended to be called via the @ref
 * SLI_MEMORY_PROFILER_TRACK_ALLOC_WITH_OWNERSHIP() macro.
 *
 * @param[in] tracker_handle The handle of the memory tracker
 * @param[in] ptr Pointer to the allocated memory or NULL if allocation failed
 * @param[in] size The number of bytes allocated, or attempted to allocate
 * @param[in] pc The program counter at the location of the allocation
 */
void sli_memory_profiler_track_alloc_with_ownership(sli_memory_tracker_handle_t tracker_handle,
                                                    void * ptr,
                                                    size_t size,
                                                    void * pc);

/**
 * @brief Track the freeing of a memory block
 *
 * NOTE: This function is intended to be called via the
 * @ref SLI_MEMORY_PROFILER_TRACK_FREE() macro.
 *
 * @param[in] tracker_handle The handle of the memory tracker
 * @param[in] ptr Pointer to the allocated memory
 */
void sli_memory_profiler_track_free(sli_memory_tracker_handle_t tracker_handle,
                                    void * ptr);

/**
 * @brief Track the transfer of memory allocation ownership
 *
 * NOTE: This function is intended to be called via @ref
 * SLI_MEMORY_PROFILER_TRACK_OWNERSHIP() or @ref
 * SLI_MEMORY_PROFILER_TRACK_OWNERSHIP_ON_TRACKER() macros.
 *
 * @param[in] tracker_handle Handle of the tracker level at which the ownership
 *   is taken. This is used to disambiguate in cases where nested allocations
 *   start at the same memory location, and the caller is specifically taking
 *   ownership of one of the outer blocks that may contain smaller allocations
 *   that have their own (more detailed) owners. If set to
 *   `SLI_INVALID_MEMORY_TRACKER_HANDLE`, the ownership of the innermost
 *   allocation is taken.
 * @param[in] ptr Pointer to the allocated memory for which ownership is taken.
 *   The caller may pass a NULL pointer to indicate that the location pointer to
 *   be @p pc has failed to obtain a valid pointer, for example because a memory
 *   allocation that was meant to provide the pointer has failed.
 * @param[in] pc The program counter at the location that took ownership
 */
void sli_memory_profiler_track_ownership(sli_memory_tracker_handle_t tracker_handle,
                                         void * ptr,
                                         void * pc);

/**
 * @brief Trigger the creation of a snapshot of the current state
 *
 * This function can be used by the device to trigger the analysis software on
 * the PC or Mac to take a snapshot of the current state of the allocation
 * bookkeeping. This would typically be used by test cases that communicate with
 * the device under test and want to synchronously record the state of the
 * allocations at a known point in the test sequence.
 *
 * @param[in] name Short name for the snapshot that is being created. The name
 *   is immediately sent in the RTT event to the analysis software and does not
 *   need to be retained in the device.
 */
void sli_memory_profiler_take_snapshot(const char *name);

/**
 * @brief Output a generic log event in the RTT event stream
 *
 * This function is meant to be used for temporary debugging purposes only. When
 * debugging a memory leak requires visibility to software actions other than
 * memory allocation or free, calls to this generic logging mechanism can be
 * added so that the sequence of software events can be seen with respect to the
 * allocation and free events that are visible in the normal Memory Profiler
 * events.
 *
 * @param[in] log_id Numeric identifier of this log event. This is passed in the
 *   RTT event to the analysis tool and the ID appears in the log produced by
 *   the analyzer but the value is not otherwise used by the Memory Profiler. It
 *   is the responsibility of the caller to use values that are sufficiently
 *   unique that the developer can identify the logs.
 *
 * @param[in] arg1 Arbitrary 32-bit argument that's relevant for the developer
 *
 * @param[in] arg2 Arbitrary 32-bit argument that's relevant for the developer
 *
 * @param[in] arg3 Arbitrary 32-bit argument that's relevant for the developer
 *
 * @param[in] pc The program counter at the location of the log call
 */
void sli_memory_profiler_log(uint32_t log_id,
                             uint32_t arg1,
                             uint32_t arg2,
                             uint32_t arg3,
                             void * pc);

// The macros expand to their full content only when profiling is included
#if SLI_MEMORY_PROFILER_ENABLE_PROFILING

/**
 * @brief Macro to wrap calls to @ref sli_memory_profiler_track_alloc
 */
#define SLI_MEMORY_PROFILER_TRACK_ALLOC(tracker_handle, ptr, size)    \
  do {                                                                \
    sli_memory_profiler_track_alloc((tracker_handle), (ptr), (size)); \
  } while (0)

/**
 * @brief Macro to wrap calls to @ref sli_memory_profiler_track_alloc_with_ownership
 *
 * If ownership tracking is disabled at build time, the macro reduces to normal
 * tracking without ownership.
 */
#if SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING
#define SLI_MEMORY_PROFILER_TRACK_ALLOC_WITH_OWNERSHIP(tracker_handle, ptr, size, pc)     \
  do {                                                                                    \
    void * volatile _pc = (pc);                                                           \
    sli_memory_profiler_track_alloc_with_ownership((tracker_handle), (ptr), (size), _pc); \
  } while (0)
#else // SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING
#define SLI_MEMORY_PROFILER_TRACK_ALLOC_WITH_OWNERSHIP(tracker_handle, ptr, size, pc) \
  do {                                                                                \
    (void) (pc);                                                                      \
    sli_memory_profiler_track_alloc((tracker_handle), (ptr), (size));                 \
  } while (0)
#endif // SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING

/**
 * @brief Macro to wrap calls to @ref sli_memory_profiler_track_realloc
 */
#define SLI_MEMORY_PROFILER_TRACK_REALLOC(tracker_handle, ptr, realloced_ptr, size)      \
  do {                                                                                   \
    sli_memory_profiler_track_realloc((tracker_handle), (ptr), (realloced_ptr), (size)); \
  } while (0)

/**
 * @brief Macro to wrap calls to @ref sli_memory_profiler_track_free
 */
#define SLI_MEMORY_PROFILER_TRACK_FREE(tracker_handle, ptr)  \
  do {                                                       \
    sli_memory_profiler_track_free((tracker_handle), (ptr)); \
  } while (0)

#else // SLI_MEMORY_PROFILER_ENABLE_PROFILING

// Empty implementation of tracking macros when memory profiling calls are
// excluded at build time
#define SLI_MEMORY_PROFILER_TRACK_ALLOC(tracker_handle, ptr, size) \
  do {                                                             \
    (void) (tracker_handle);                                       \
    (void) (ptr);                                                  \
    (void) (size);                                                 \
  } while (0)

#define SLI_MEMORY_PROFILER_TRACK_ALLOC_WITH_OWNERSHIP(tracker_handle, ptr, size, pc) \
  do {                                                                                \
    (void) (tracker_handle);                                                          \
    (void) (ptr);                                                                     \
    (void) (size);                                                                    \
    (void) (pc);                                                                      \
  } while (0)

#define SLI_MEMORY_PROFILER_TRACK_REALLOC(tracker_handle, ptr, realloced_ptr, size) \
  do {                                                                              \
    (void) (tracker_handle);                                                        \
    (void) (ptr);                                                                   \
    (void) (realloced_ptr);                                                         \
    (void) (size);                                                                  \
  } while (0)

#define SLI_MEMORY_PROFILER_TRACK_FREE(tracker_handle, ptr) \
  do {                                                      \
    (void) (tracker_handle);                                \
    (void) (ptr);                                           \
  } while (0)

#endif // SLI_MEMORY_PROFILER_ENABLE_PROFILING

// Ownership tracking calls are included based on dedicated configuration
#if SLI_MEMORY_PROFILER_ENABLE_PROFILING && SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING

/**
 * @brief Macro to wrap calls to @ref sli_memory_profiler_track_ownership
 */
#define SLI_MEMORY_PROFILER_TRACK_OWNERSHIP(ptr, pc)                                    \
  do {                                                                                  \
    void * volatile _pc = (pc);                                                         \
    sli_memory_profiler_track_ownership(SLI_INVALID_MEMORY_TRACKER_HANDLE, (ptr), _pc); \
  } while (0)

/**
 * @brief Macro to wrap calls to @ref sli_memory_profiler_track_ownership
 */
#define SLI_MEMORY_PROFILER_TRACK_OWNERSHIP_ON_TRACKER(tracker_handle, ptr, pc) \
  do {                                                                          \
    void * volatile _pc = (pc);                                                 \
    sli_memory_profiler_track_ownership((tracker_handle), (ptr), _pc);          \
  } while (0)

#else // SLI_MEMORY_PROFILER_ENABLE_PROFILING && SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING

#define SLI_MEMORY_PROFILER_TRACK_OWNERSHIP(ptr, pc) \
  do {                                               \
    (void) (ptr);                                    \
    (void) (pc);                                     \
  } while (0)

#define SLI_MEMORY_PROFILER_TRACK_OWNERSHIP_ON_TRACKER(tracker_handle, ptr, pc) \
  do {                                                                          \
    (void) (tracker_handle);                                                    \
    (void) (ptr);                                                               \
    (void) (pc);                                                                \
  } while (0)

#endif // SLI_MEMORY_PROFILER_ENABLE_PROFILING && SLI_MEMORY_PROFILER_ENABLE_OWNERSHIP_TRACKING

/** @} end memory_profiler */
/// @endcond

#ifdef __cplusplus
}
#endif

#endif // SLI_MEMORY_PROFILER_H
