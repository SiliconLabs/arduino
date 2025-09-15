/**************************************************************************/ /**
 * @file
 * @brief OS abstraction layer primitives for platform/security on CMSIS RTOS2
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

#ifndef SLI_PSEC_OSAL_CMSIS_RTOS_H
#define SLI_PSEC_OSAL_CMSIS_RTOS_H

// -----------------------------------------------------------------------------
// Includes
#include "sl_common.h"
#include "sl_status.h"
#include "cmsis_os2.h"
#include "sl_core.h"
#include "sl_code_classification.h"

#if defined (SL_COMPONENT_CATALOG_PRESENT)
  #include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT)
  #include "FreeRTOSConfig.h"
  #if (configSUPPORT_STATIC_ALLOCATION == 1)
    #include "FreeRTOS.h"  // StaticSemaphore_t
    #include <string.h>
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Defines

/// In order to wait forever in blocking functions the user can pass the
/// following value.
#define SLI_PSEC_OSAL_WAIT_FOREVER  (osWaitForever)
/// In order to return immediately in blocking functions the user can pass the
/// following value.
#define SLI_PSEC_OSAL_NON_BLOCKING  (0)

// Checks if kernel is running
#define SLI_PSEC_OSAL_KERNEL_RUNNING (osKernelGetState() == osKernelRunning)

// Lock kernel (task scheduler) to enter critical section
#define SLI_PSEC_OSAL_KERNEL_CRITICAL_SECTION_START                        \
  int32_t kernel_lock_state = 0;                                           \
  osKernelState_t kernel_state = osKernelGetState();                       \
  if (kernel_state != osKernelInactive && kernel_state != osKernelReady) { \
    kernel_lock_state = osKernelLock();                                    \
    if (kernel_lock_state < 0) {                                           \
      return SL_STATUS_FAIL;                                               \
    }                                                                      \
  }

// Resume kernel to exit critical section
#define SLI_PSEC_OSAL_KERNEL_CRITICAL_SECTION_END                          \
  if (kernel_state != osKernelInactive && kernel_state != osKernelReady) { \
    if (osKernelRestoreLock(kernel_lock_state) < 0) {                      \
      return SL_STATUS_FAIL;                                               \
    }                                                                      \
  }

// -----------------------------------------------------------------------------
// Typedefs

/// Completion object used to wait for and signal end of an operation.
typedef struct sli_psec_osal_completion {
#if defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT) && (configSUPPORT_STATIC_ALLOCATION == 1)
  osSemaphoreAttr_t semaphore_attr;
  StaticSemaphore_t static_sem_object;
#endif
  osSemaphoreId_t   semaphore_ID;
} sli_psec_osal_completion_t;

/// SLI PSEC lock definition for CMSIS RTOS2.
typedef struct sli_psec_osal_lock {
#if defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT) && (configSUPPORT_STATIC_ALLOCATION == 1)
  StaticSemaphore_t static_sem_object;
#endif
  osMutexAttr_t mutex_attr;
  osMutexId_t   mutex_ID;
} sli_psec_osal_lock_t;

// -----------------------------------------------------------------------------
// Functions

/***************************************************************************//**
 * @brief          Set recursive attribute of lock
 *
 * @details        If recursive lock is needed, this function must be called
 *                 before calling sli_psec_osal_init_lock.
 *
 * @param lock     Pointer to the lock
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
__STATIC_INLINE
sl_status_t sli_psec_osal_set_recursive_lock(sli_psec_osal_lock_t *lock)
{
  if (lock == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  lock->mutex_attr.attr_bits |= osMutexRecursive;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief          Initialize a given lock
 *
 * @param lock     Pointer to the lock needing initialization
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
__STATIC_INLINE sl_status_t sli_psec_osal_init_lock(sli_psec_osal_lock_t *lock)
{
  if (lock == NULL) {
    return SL_STATUS_FAIL;
  }

#if defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT) && (configSUPPORT_STATIC_ALLOCATION == 1)
  // Zeroize all members of the lock attributes object and setup the static control block.
  lock->mutex_attr.cb_mem = &lock->static_sem_object;
  lock->mutex_attr.cb_size = sizeof(lock->static_sem_object);
#endif

  lock->mutex_ID = osMutexNew(&lock->mutex_attr);

  return (lock->mutex_ID == NULL ? SL_STATUS_FAIL : SL_STATUS_OK);
}

/***************************************************************************//**
 * @brief          Free a given lock
 *
 * @param lock     Pointer to the lock being freed
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
__STATIC_INLINE sl_status_t sli_psec_osal_free_lock(sli_psec_osal_lock_t *lock)
{
  if (lock == NULL) {
    return SL_STATUS_FAIL;
  }

  osStatus_t status = osMutexDelete(lock->mutex_ID);
  return (status == osOK ? SL_STATUS_OK : SL_STATUS_FAIL);
}

/***************************************************************************//**
 * @brief          Check if lock is open for calling thread
 *
 * @param lock     Pointer to the lock to be checked
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_PSEC_OSAL, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_psec_osal_lock_is_accessible(sli_psec_osal_lock_t *lock);

/***************************************************************************//**
 * @brief          Pend on a lock with timeout
 *
 * @param lock     Pointer to the lock being pended on
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_PSEC_OSAL, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_psec_osal_take_lock_timeout(sli_psec_osal_lock_t *lock,
                                            uint32_t timeout);

/***************************************************************************//**
 * @brief          Pend on a lock forever
 *
 * @param lock     Pointer to the lock being pended on
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
__STATIC_INLINE sl_status_t sli_psec_osal_take_lock(sli_psec_osal_lock_t *lock)
{
  return sli_psec_osal_take_lock_timeout(lock, SLI_PSEC_OSAL_WAIT_FOREVER);
}

/***************************************************************************//**
 * @brief          Try to acquire ownership of a lock without waiting.
 *
 * @param lock     Pointer to the lock being tested
 *
 * @return         SL_STATUS_OK on success (= lock successfully owned),
 *                 error code otherwise.
 *****************************************************************************/
__STATIC_INLINE
sl_status_t sli_psec_osal_take_lock_non_blocking(sli_psec_osal_lock_t *lock)
{
  return sli_psec_osal_take_lock_timeout(lock, SLI_PSEC_OSAL_NON_BLOCKING);
}

/***************************************************************************//**
 * @brief          Release a lock
 *
 * @param lock     Pointer to the lock being released
 *
 * @return         SL_STATUS_OK on success, error code otherwise.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_PSEC_OSAL, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_psec_osal_give_lock(sli_psec_osal_lock_t *lock);

/***************************************************************************//**
 * @brief          Initialize a completion object.
 *
 * @param p_comp   Pointer to an sli_psec_osal_completion_t object allocated
 *                 by the user.
 *
 * @return         Status code, @ref sl_status.h.
 *****************************************************************************/
__STATIC_INLINE sl_status_t
sli_psec_osal_init_completion(sli_psec_osal_completion_t *p_comp)
{
  if (p_comp == NULL) {
    return SL_STATUS_FAIL;
  }

#if defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT) && (configSUPPORT_STATIC_ALLOCATION == 1)
  // Zeroize all members of the semaphore attributes object and setup the static control block.
  memset(&p_comp->semaphore_attr, 0, sizeof(p_comp->semaphore_attr));
  p_comp->semaphore_attr.cb_mem = &p_comp->static_sem_object;
  p_comp->semaphore_attr.cb_size = sizeof(p_comp->static_sem_object);
  p_comp->semaphore_ID = osSemaphoreNew(1u, 0u, &p_comp->semaphore_attr);
#else
  p_comp->semaphore_ID = osSemaphoreNew(1u, 0u, NULL);
#endif

  return (p_comp->semaphore_ID == NULL ? SL_STATUS_FAIL : SL_STATUS_OK);
}

/***************************************************************************//**
 * @brief          Free a completion object.
 *
 * @param p_comp   Pointer to an sli_psec_osal_completion_t object.
 *
 * @return         Status code, @ref sl_status.h.
 *****************************************************************************/
__STATIC_INLINE sl_status_t
sli_psec_osal_free_completion(sli_psec_osal_completion_t *p_comp)
{
  if (p_comp == NULL) {
    return SL_STATUS_FAIL;
  }

  osStatus_t status = osSemaphoreDelete(p_comp->semaphore_ID);
  return (status == osOK ? SL_STATUS_OK : SL_STATUS_FAIL);
}

/***************************************************************************//**
 * @brief          Wait for completion event.
 *
 * @param p_comp   Pointer to completion object which must be initialized by
 *                 calling sli_psec_osal_completion_init before calling this
 *                 function.
 *
 * @param ticks    Ticks to wait for the completion.
 *                 Pass a value of SLI_PSEC_OSAL_WAIT_FOREVER in order to
 *                 wait forever.
 *                 Pass a value of SLI_PSEC_OSAL_NON_BLOCKING in order to
 *                 return immediately.
 *
 * @return         Status code, @ref sl_status.h. Typcally SL_STATUS_OK if success,
 *                 or SL_STATUS_TIMEOUT if no completion within the given ticks.
 *****************************************************************************/
__STATIC_INLINE sl_status_t
sli_psec_osal_wait_completion(sli_psec_osal_completion_t *p_comp, int ticks)
{
  if (p_comp == NULL) {
    return SL_STATUS_FAIL;
  }

  osStatus_t status = osOK;
  if (osKernelGetState() == osKernelRunning) {
    status = osSemaphoreAcquire(p_comp->semaphore_ID,
                                (uint32_t)ticks);
  }
  return (status == osOK ? SL_STATUS_OK : SL_STATUS_FAIL);
}

/***************************************************************************//**
 * @brief          Signal completion.
 *
 * @param p_comp   Pointer to completion object which must be initialized by
 *                 calling sli_psec_osal_completion_init before calling this
 *                 function.
 *
 * @return         Status code, @ref sl_status.h.
 *****************************************************************************/
__STATIC_INLINE
sl_status_t sli_psec_osal_complete(sli_psec_osal_completion_t* p_comp)
{
  if (p_comp == NULL) {
    return SL_STATUS_FAIL;
  }

  osStatus_t status = osOK;
  osKernelState_t state = osKernelGetState();
  if ((state == osKernelRunning) || (state == osKernelLocked)) {
    status = osSemaphoreRelease(p_comp->semaphore_ID);
  }
  return (status == osOK ? SL_STATUS_OK : SL_STATUS_FAIL);
}

/***************************************************************************//**
 * @brief          Lock the RTOS Kernel scheduler.
 *
 * @return         Status code, @ref cmsis_os2.h
 *****************************************************************************/
__STATIC_INLINE int32_t sli_psec_osal_kernel_lock(void)
{
  return osKernelLock();
}

/***************************************************************************//**
 * @brief          Restore the RTOS Kernel scheduler lock state.
 *
 * @return         Status code, @ref cmsis_os2.h
 *****************************************************************************/
__STATIC_INLINE int32_t sli_psec_osal_kernel_restore_lock(int32_t lock)
{
  return osKernelRestoreLock(lock);
}

/***************************************************************************//**
 * @brief          Get current RTOS kernel state.
 *
 * @return         Status code, @ref cmsis_os2.h
 *****************************************************************************/
__STATIC_INLINE osKernelState_t
sli_psec_osal_kernel_get_state(void)
{
  return osKernelGetState();
}

#ifdef __cplusplus
}
#endif

#endif // SLI_PSEC_OSAL_CMSIS_RTOS_H
