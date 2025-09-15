/**************************************************************************/ /**
 * @file
 * @brief Threading primitive implementation for mbed TLS
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef THREADING_ALT_H
#define THREADING_ALT_H

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
/***************************************************************************//**
 * \addtogroup sl_mbedtls_plugins
 * \{
 ******************************************************************************/

/***************************************************************************//**
 * \addtogroup sl_mbedtls_plugins_threading Threading Primitives
 * \brief Threading primitive implementation for mbed TLS
 *
 * This module provides a threading implementation, based on CMSIS RTOS2, that
 * can be used by Mbed TLS when threading is required.
 *
 * \note These plugins are automatically enabled when creating an SLC project
 * with Micrium OS or FreeRTOS with Mbed TLS.
 *
 * \{
 ******************************************************************************/

#include <stdbool.h>

#if defined(MBEDTLS_THREADING_ALT) && defined(MBEDTLS_THREADING_C)

#if defined(SL_COMPONENT_CATALOG_PRESENT)
  #include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_MICRIUMOS_KERNEL_PRESENT) || defined(SL_CATALOG_FREERTOS_KERNEL_PRESENT)

#include "sli_psec_osal.h"
#include "sl_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SL_THREADING_ALT

#define MUTEX_INIT  = { 0 }

/// Mbed TLS mutexes maps to SLI PSEC OSAL locks.
typedef sli_psec_osal_lock_t mbedtls_threading_mutex_t;

typedef struct mbedtls_test_thread_t {
  osThreadAttr_t thread_attr;
  osThreadId_t thread_ID;
} mbedtls_test_thread_t;

#include "mbedtls/threading.h"

#ifndef MBEDTLS_ERR_THREADING_THREAD_ERROR
#define MBEDTLS_ERR_THREADING_THREAD_ERROR                 -0x001F
#endif
/**
 * \brief          Set mutex recursive
 *
 * \param mutex    Pointer to the mutex
 */
static inline void THREADING_SetRecursive(mbedtls_threading_mutex_t *mutex)
{
  sl_status_t sl_status = sli_psec_osal_set_recursive_lock((sli_psec_osal_lock_t*)mutex);
  EFM_ASSERT(sl_status == SL_STATUS_OK);
}

/**
 * \brief          Initialize a given mutex
 *
 * \param mutex    Pointer to the mutex needing initialization
 */
static inline void THREADING_InitMutex(mbedtls_threading_mutex_t *mutex)
{
  sl_status_t sl_status = sli_psec_osal_init_lock(mutex);
  EFM_ASSERT(sl_status == SL_STATUS_OK);
}

/**
 * \brief          Free a given mutex
 *
 * \param mutex    Pointer to the mutex being freed
 */
static inline void THREADING_FreeMutex(mbedtls_threading_mutex_t *mutex)
{
  sl_status_t sl_status = sli_psec_osal_free_lock(mutex);
  EFM_ASSERT(sl_status == SL_STATUS_OK);
}

/**
 * \brief          Pend on a mutex
 *
 * \param mutex    Pointer to the mutex being pended on
 *
 * \return         RTOS_ERR_NONE on success, error code otherwise.
 */
static inline int THREADING_TakeMutexBlocking(mbedtls_threading_mutex_t *mutex)
{
  if (mutex == NULL) {
    return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
  }
  sl_status_t sl_status = sli_psec_osal_take_lock(mutex);
  return (sl_status == SL_STATUS_OK ? 0 : MBEDTLS_ERR_THREADING_MUTEX_ERROR);
}

/**
 * \brief          Try to own a mutex without waiting
 *
 * \param mutex    Pointer to the mutex being tested
 *
 * \return         RTOS_ERR_NONE on success (= mutex successfully owned), error code otherwise.
 */
static inline int THREADING_TakeMutexNonBlocking(mbedtls_threading_mutex_t *mutex)
{
  if (mutex == NULL) {
    return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
  }
  sl_status_t sl_status = sli_psec_osal_take_lock_non_blocking(mutex);
  return (sl_status == SL_STATUS_OK ? 0 : MBEDTLS_ERR_THREADING_MUTEX_ERROR);
}

/**
 * \brief          Release a mutex
 *
 * \param mutex    Pointer to the mutex being released
 *
 * \return         RTOS_ERR_NONE on success, error code otherwise.
 */
static inline int THREADING_GiveMutex(mbedtls_threading_mutex_t *mutex)
{
  if (mutex == NULL) {
    return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
  }
  sl_status_t sl_status = sli_psec_osal_give_lock(mutex);
  return (sl_status == SL_STATUS_OK ? 0 : MBEDTLS_ERR_THREADING_MUTEX_ERROR);
}

/**
 * \brief          The thread create function implementation
 *
 * \param thread   Pointer to the thread being created
 * \param thread_func  Pointer to the thread function
 * \param thread_data  Pointer to the thread data
 */
static inline int THREADING_ThreadCreate(mbedtls_test_thread_t *thread,
                                         void (*thread_func)(
                                           void *),
                                         void *thread_data)
{
  if (thread == NULL || thread_func == NULL) {
    return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
  }

  thread->thread_ID = osThreadNew(thread_func, thread_data, &thread->thread_attr);
  if (thread->thread_ID == NULL) {
    return MBEDTLS_ERR_THREADING_THREAD_ERROR;
  }

  return 0;
}

/**
 * \brief          The thread join function implementation
 *
 * \param thread   Pointer to the thread being joined
 */
static inline int THREADING_ThreadJoin(mbedtls_test_thread_t *thread)
{
  if (thread == NULL) {
    return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
  }

  if (osThreadJoin(thread->thread_ID) != 0) {
    return MBEDTLS_ERR_THREADING_THREAD_ERROR;
  }

  return 0;
}

#ifdef __cplusplus
}
#endif

#endif // SL_CATALOG_MICRIUMOS_KERNEL_PRESENT || SL_CATALOG_FREERTOS_KERNEL_PRESENT

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declaration of threading_set_alt */
void mbedtls_threading_set_alt(void (*mutex_init)(mbedtls_threading_mutex_t *),
                               void (*mutex_free)(mbedtls_threading_mutex_t *),
                               int (*mutex_lock)(mbedtls_threading_mutex_t *),
                               int (*mutex_unlock)(mbedtls_threading_mutex_t *) );

/* Forward declaration of test_thread_set_alt */
void mbedtls_test_thread_set_alt(int (*thread_create)(mbedtls_test_thread_t *thread,
                                                      void (*thread_func)(
                                                        void *),
                                                      void *thread_data),
                                 int (*thread_join)(mbedtls_test_thread_t *thread));

/**
 * \brief          Helper function for setting up the mbed TLS threading subsystem
 */
static inline void THREADING_setup(void)
{
  mbedtls_threading_set_alt(&THREADING_InitMutex,
                            &THREADING_FreeMutex,
                            &THREADING_TakeMutexBlocking,
                            &THREADING_GiveMutex);
}

static inline void THREAD_test_setup(void)
{
  mbedtls_test_thread_set_alt(&THREADING_ThreadCreate,
                              &THREADING_ThreadJoin);
}
#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_THREADING_ALT && MBEDTLS_THREADING_C */

/** \} (end addtogroup sl_mbedtls_plugins_threading) */
/** \} (end addtogroup sl_mbedtls_plugins) */
/// @endcond

#endif /* THREADING_ALT_H */
