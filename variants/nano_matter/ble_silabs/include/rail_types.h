/***************************************************************************//**
 * @file
 * @brief This file contains the type definitions for RAIL structures, enums,
 *   and other types.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef __RAIL_TYPES_H__
#define __RAIL_TYPES_H__

// Include standard type headers to help define structures
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/// The RAIL library does not use enumerations because the ARM EABI leaves their
/// size ambiguous, which causes problems if the application is built
/// with different flags than the library. Instead, uint8_t typedefs
/// are used in compiled code for all enumerations. For documentation purposes, this is
/// converted to an actual enumeration since it's much easier to read in Doxygen.
#define RAIL_ENUM(name) enum name
/// This macro is a more generic version of the \ref RAIL_ENUM() macro that
/// allows the size of the type to be overridden instead of forcing the use of
/// a uint8_t. See \ref RAIL_ENUM() for more information.
#define RAIL_ENUM_GENERIC(name, type) enum name
#else//!DOXYGEN_SHOULD_SKIP_THIS
/// Define used for the RAIL library, which sets each enumeration to a uint8_t
/// typedef and creates a named enumeration structure for the enumeration values.
/// For debugging, use the following define to turn this back into a proper enumeration
/// #define RAIL_ENUM(name) typedef enum name##_enum name; enum name##_enum
#define RAIL_ENUM(name) typedef uint8_t name; enum name##_enum
#define RAIL_ENUM_GENERIC(name, type) typedef type name; enum name##_enum
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @addtogroup RAIL_API
 * @{
 */

/******************************************************************************
 * General Structures
 *****************************************************************************/
/**
 * @addtogroup General
 * @{
 */

/**
 * @struct RAIL_Version_t
 * @brief Contains RAIL Library Version Information.
 *   It is filled in by \ref RAIL_GetVersion().
 */
typedef struct RAIL_Version {
  /** Git hash */
  uint32_t hash;
  /** Major number */
  uint8_t major;
  /** Minor number */
  uint8_t minor;
  /** Revision number */
  uint8_t rev;
  /** Build number */
  uint8_t build;
  /** Build flags */
  uint8_t flags;
  /** Boolean to indicate whether this is a multiprotocol library or not. */
  bool multiprotocol;
} RAIL_Version_t;

/**
 * @typedef RAIL_Handle_t
 * @brief A radio-generic handle (e.g., \ref RAIL_EFR32_HANDLE),
 *   or a real RAIL instance handle as returned from \ref RAIL_Init().
 *
 * Generic handles should be used for certain RAIL APIs that are called
 * prior to RAIL initialization. However, once RAIL has been initialized,
 * the real handle returned by \ref RAIL_Init() should be used instead.
 */
typedef void *RAIL_Handle_t;

/**
 * A placeholder for a radio-generic RAIL handle. Using NULL as a RAIL handle
 * is not recommended. As a result, another value that can't be de-referenced
 * is used.
 *
 * This generic handle can and should be used for RAIL APIs that are called
 * prior to RAIL initialization.
 */
#define RAIL_EFR32_HANDLE ((RAIL_Handle_t)0xFFFFFFFFUL)

/**
 * @typedef RAIL_Status_t
 * @brief A status returned by many RAIL API calls indicating their success or
 *   failure. It is a subset of sl_status_t.
 */
typedef sl_status_t RAIL_Status_t;

/** RAIL function reports no error. */
#define RAIL_STATUS_NO_ERROR SL_STATUS_OK // 0x0000

/** Call to RAIL function threw an error because of an invalid parameter. */
#define RAIL_STATUS_INVALID_PARAMETER SL_STATUS_INVALID_PARAMETER // 0x0021

/**
 * Call to RAIL function threw an error because it was called during
 * an invalid radio state.
 */
#define RAIL_STATUS_INVALID_STATE SL_STATUS_INVALID_STATE // 0x0002

/** RAIL function is called in an invalid order. */
#define RAIL_STATUS_INVALID_CALL SL_STATUS_NOT_AVAILABLE // 0x000E

/** RAIL function did not finish in the allotted time. */
#define RAIL_STATUS_SUSPENDED SL_STATUS_IN_PROGRESS // 0x0005

/**
 * RAIL function could not be scheduled by the Radio scheduler.
 * Only issued when using a Multiprotocol application.
 */
#define RAIL_STATUS_SCHED_ERROR SL_STATUS_ABORT // 0x0006

/**
 * A pointer to an initialization complete callback function.
 *
 * @param[in] railHandle The initialized RAIL instance handle.
 */
typedef void (*RAIL_InitCompleteCallbackPtr_t)(RAIL_Handle_t railHandle);

/** A value to signal that RAIL should not use DMA. */
#define RAIL_DMA_INVALID (0xFFU)

/**
 * @struct RAILSched_Config_t
 * @brief Provided for backwards compatibility.
 */
typedef struct RAILSched_Config {
  /** Dummy buffer no longer used. */
  uint8_t buffer[1];
} RAILSched_Config_t;

/**
 * @typedef RAIL_StateBuffer_t
 * @brief Provided for backwards compatibility.
 */
typedef uint8_t RAIL_StateBuffer_t[1];

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * A linked list structure for RAIL state buffers which \ref RAIL_Init()
 * utilizes for managing internal RAIL state.
 */
typedef struct RAIL_StateBufferEntry {
  /** pointer to next buffer in linked list */
  struct RAIL_StateBufferEntry *next;
  /** size of the buffer */
  uint32_t bufferBytes;
  /** pointer to the buffer in RAM */
  uint64_t *buffer;
} RAIL_StateBufferEntry_t;

#endif//DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group General

/******************************************************************************
 * System Timing Structures
 *****************************************************************************/
/**
 * @addtogroup System_Timing
 * @{
 */

/**
 * @typedef RAIL_Time_t
 * @brief Time in microseconds
 */
typedef uint32_t RAIL_Time_t;

/**
 * A pointer to the callback called when the RAIL timer expires.
 *
 * @param[in] cbArg The argument passed to the callback.
 */
typedef void (*RAIL_TimerCallback_t)(RAIL_Handle_t cbArg);

/**
 * @enum RAIL_TimeMode_t
 * @brief Specify a time offset in RAIL APIs.
 *
 * Different APIs use the same constants and may provide more specifics about
 * how they're used but the general use for each is described below.
 */
RAIL_ENUM(RAIL_TimeMode_t) {
  /**
   * The time specified is an exact time in the RAIL timebase. The given
   * event should happen at exactly that time. If this time is already in the
   * past, an error is returned. Because the RAIL timebase wraps at 32
   * bits, there is no real 'past'. Instead, any event greater than
   * 3/4 of the way into the future is considered to be in the past.
   */
  RAIL_TIME_ABSOLUTE = 0,
  /**
   * The time specified is relative to the current time. The event should occur
   * that many ticks in the future. Delays are only guaranteed at least as long
   * as the value specified. An overhead may occur between the time when the
   * API is called and when the delay starts. As a result, using this for
   * operations that must happen at an exact given time is not recommended.
   * For that, you must use \ref RAIL_TIME_ABSOLUTE delays.
   *
   * Note that, if you specify a delay 0, that event is triggered as soon as
   * possible. This is different than specifying an absolute time of now which
   * would return an error unless it was possible.
   */
  RAIL_TIME_DELAY = 1,
  /**
   * The specified time is invalid and should be ignored. For some APIs this
   * can also indicate that any previously stored delay should be invalidated
   * and disabled.
   */
  RAIL_TIME_DISABLED = 2,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_TIME_ABSOLUTE ((RAIL_TimeMode_t) RAIL_TIME_ABSOLUTE)
#define RAIL_TIME_DELAY    ((RAIL_TimeMode_t) RAIL_TIME_DELAY)
#define RAIL_TIME_DISABLED ((RAIL_TimeMode_t) RAIL_TIME_DISABLED)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/// @struct RAIL_MultiTimer
/// Forward structure declaration of \ref RAIL_MultiTimer_t.
struct RAIL_MultiTimer;

/**
 * @typedef RAIL_MultiTimerCallback_t
 * @brief Callback fired when timer expires.
 *
 * @param[in] tmr A pointer to an expired timer.
 * @param[in] expectedTimeOfEvent An absolute time event fired.
 * @param[in] cbArg A user-supplied callback argument.
 */
typedef void (*RAIL_MultiTimerCallback_t)(struct RAIL_MultiTimer *tmr,
                                          RAIL_Time_t expectedTimeOfEvent,
                                          void *cbArg);

/**
 * @struct RAIL_MultiTimer_t
 * @brief RAIL timer state structure.
 *
 * This structure is filled out and maintained internally only.
 * The user/application should not alter any elements of this structure.
 */
typedef struct RAIL_MultiTimer {
  /** Absolute time before the next event. */
  RAIL_Time_t absOffset;
  /** Relative, periodic time between events; 0 = timer is oneshot. */
  RAIL_Time_t relPeriodic;
  /** A user callback. */
  RAIL_MultiTimerCallback_t callback;
  /** A user callback argument. */
  void *cbArg;
  /** A pointer to the next soft timer structure. */
  struct RAIL_MultiTimer *next;
  /** A priority of the callback; 0 = highest priority; 255 = lowest. */
  uint8_t priority;
  /** Indicates the timer is currently running. */
  bool isRunning;
  /** Indicates the callback needs to run. */
  bool doCallback;
} RAIL_MultiTimer_t;

/**
 * @enum RAIL_PacketTimePosition_t
 * @brief The available packet timestamp position choices.
 */
RAIL_ENUM(RAIL_PacketTimePosition_t) {
  /**
   * Indicate that a timestamp is not to be or was not provided.
   * It is useful if the application doesn't care about packet timestamps
   * and doesn't want RAIL to spend time calculating one.
   */
  RAIL_PACKET_TIME_INVALID = 0,
  /**
   * Request the choice most expedient for RAIL to calculate,
   * which may depend on the radio and/or its configuration.
   * The actual choice would always be reflected in the \ref
   * RAIL_PacketTimeStamp_t::timePosition field of the \ref
   * RAIL_RxPacketDetails_t::timeReceived or \ref
   * RAIL_TxPacketDetails_t::timeSent
   * returned and would never be one of the _USED_TOTAL values.
   */
  RAIL_PACKET_TIME_DEFAULT = 1,
  /**
   * Request the timestamp corresponding to the first preamble bit
   * sent or received.
   * Indicate that timestamp did not require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PREAMBLE_START = 2,
  /**
   * Request the timestamp corresponding to the first preamble bit
   * sent or received.
   * Indicate that timestamp did require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PREAMBLE_START_USED_TOTAL = 3,
  /**
   * Request the timestamp corresponding to right after its last
   * SYNC word bit has been sent or received.
   * Indicate that timestamp did not require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_SYNC_END = 4,
  /**
   * Request the timestamp corresponding to right after its last
   * SYNC word bit has been sent or received.
   * Indicate that timestamp did require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_SYNC_END_USED_TOTAL = 5,
  /**
   * Request the timestamp corresponding to right after its last
   * bit has been sent or received.
   * Indicate that timestamp did not require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PACKET_END = 6,
  /**
   * Request the timestamp corresponding to right after its last
   * bit has been sent or received.
   * Indicate that timestamp did require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL = 7,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_PACKET_TIME_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_PACKET_TIME_INVALID                      ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_INVALID)
#define RAIL_PACKET_TIME_DEFAULT                      ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_DEFAULT)
#define RAIL_PACKET_TIME_AT_PREAMBLE_START            ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_AT_PREAMBLE_START)
#define RAIL_PACKET_TIME_AT_PREAMBLE_START_USED_TOTAL ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_AT_PREAMBLE_START_USED_TOTAL)
#define RAIL_PACKET_TIME_AT_SYNC_END                  ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_AT_SYNC_END)
#define RAIL_PACKET_TIME_AT_SYNC_END_USED_TOTAL       ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_AT_SYNC_END_USED_TOTAL)
#define RAIL_PACKET_TIME_AT_PACKET_END                ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_AT_PACKET_END)
#define RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL     ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL)
#define RAIL_PACKET_TIME_COUNT                        ((RAIL_PacketTimePosition_t) RAIL_PACKET_TIME_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_PacketTimeStamp_t
 * @brief Information for calculating and representing a packet timestamp.
 */
typedef struct RAIL_PacketTimeStamp {
  /**
   * Timestamp of the packet in the RAIL timebase.
   */
  RAIL_Time_t packetTime;
  /**
   * A value specifying the total length in bytes of the packet
   * used when calculating the packetTime requested by the timePosition
   * field. This should account for all bytes sent over the air after
   * the Preamble and Sync word(s) including CRC bytes.
   */
  uint16_t totalPacketBytes;
  /**
   * A \ref RAIL_PacketTimePosition_t value specifying the packet position
   * to return in the packetTime field.
   * If this is \ref RAIL_PACKET_TIME_DEFAULT, this field will be
   * updated with the actual position corresponding to the packetTime
   * value filled in by a call using this structure.
   */
  RAIL_PacketTimePosition_t timePosition;
  /**
   * In RX for EFR32xG25 only:
   * A value specifying the on-air duration of the data packet,
   * starting with the first bit of the PHR (i.e., end of sync word);
   * preamble and sync word duration are hence excluded.
   *
   * In Tx for all platforms:
   * A value specifying the on-air duration of the data packet,
   * starting at the preamble (i.e. includes preamble, sync word, PHR,
   * payload and FCS). This value can be used to compute duty cycles.
   * @note This field is currently valid only for normal transmits but
   *   not Auto-Ack transmits which set the field to zero.
   */
  RAIL_Time_t packetDurationUs;
} RAIL_PacketTimeStamp_t;

/** @} */ // end of group System_Timing

/******************************************************************************
 * Sleep Structures
 *****************************************************************************/
/**
 * @addtogroup Sleep
 * @{
 */

/**
 * @enum RAIL_SleepConfig_t
 * @brief The configuration
 */
RAIL_ENUM(RAIL_SleepConfig_t) {
  /** Disable timer sync before and after sleep. */
  RAIL_SLEEP_CONFIG_TIMERSYNC_DISABLED = 0,
  /** Enable timer sync before and after sleep. */
  RAIL_SLEEP_CONFIG_TIMERSYNC_ENABLED = 1,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_SLEEP_CONFIG_TIMERSYNC_DISABLED ((RAIL_SleepConfig_t) RAIL_SLEEP_CONFIG_TIMERSYNC_DISABLED)
#define RAIL_SLEEP_CONFIG_TIMERSYNC_ENABLED  ((RAIL_SleepConfig_t) RAIL_SLEEP_CONFIG_TIMERSYNC_ENABLED)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_TimerSyncConfig_t
 * @brief Channel values used to perform timer sync before and after sleep.
 *
 * The default value of this structure is provided in the
 * \ref RAIL_TIMER_SYNC_DEFAULT macro.
 */
typedef struct RAIL_TimerSyncConfig {
  /**
   * PRS Channel used for timer sync operations.
   * Use \ref RAIL_TIMER_SYNC_PRS_CHANNEL_DEFAULT or another suitable one.
   */
  uint8_t prsChannel;
  /**
   * RTCC Channel used for timer sync operations. Only applies to
   * platforms where the RTCC used for timer sync has multiple channels.
   * Use \ref RAIL_TIMER_SYNC_RTCC_CHANNEL_DEFAULT or another suitable one.
   */
  uint8_t rtccChannel;
  /**
   * Whether to sync the timer before and after sleeping.
   */
  RAIL_SleepConfig_t sleep;
} RAIL_TimerSyncConfig_t;

/// Default timer synchronization configuration.
#define RAIL_TIMER_SYNC_DEFAULT {                        \
    .prsChannel  = RAIL_TIMER_SYNC_PRS_CHANNEL_DEFAULT,  \
    .rtccChannel = RAIL_TIMER_SYNC_RTCC_CHANNEL_DEFAULT, \
    .sleep       = RAIL_SLEEP_CONFIG_TIMERSYNC_ENABLED,  \
}

/** @} */ // end of group Sleep

/******************************************************************************
 * Multiprotocol Structures
 *****************************************************************************/
/**
 * @addtogroup Multiprotocol
 * @{
 */

/**
 * @struct RAIL_SchedulerInfo_t
 * @brief A structure to hold information used by the scheduler.
 *
 * For multiprotocol versions of RAIL, this can be used to control how a receive
 * or transmit operation is run. It's not necessary in single-protocol applications.
 */
typedef struct RAIL_SchedulerInfo {
  /**
   * The scheduler priority to use for this operation. This priority is used to
   * preempt a long running lower-priority task to ensure higher-priority
   * operations complete in time. A lower numerical value represents a higher
   * logical priority meaning 0 is the highest priority and 255 is the lowest.
   */
  uint8_t priority;
  /**
   * The amount of time in microseconds that this operation can slip by into the future
   * and still be run. This time is relative to the start time which may be
   * the current time for relative transmits. If the scheduler can't start the
   * operation by this time, it will be considered a failure.
   */
  RAIL_Time_t slipTime;
  /**
   * The transaction time in microseconds for this operation. Since transaction times may
   * not be known exactly, use a minimum or an expected
   * guess for this time. The scheduler will use the value entered here to look
   * for overlaps between low-priority and high-priority tasks and attempt to
   * find a schedule where all tasks get to run.
   */
  RAIL_Time_t transactionTime;
} RAIL_SchedulerInfo_t;

/** Radio Scheduler Status mask within \ref RAIL_SchedulerStatus_t values. */
#define RAIL_SCHEDULER_STATUS_MASK       0x0FU
/** Radio Scheduler Status shift within \ref RAIL_SchedulerStatus_t values. */
#define RAIL_SCHEDULER_STATUS_SHIFT      0

/** Radio Scheduler Task mask within \ref RAIL_SchedulerStatus_t values. */
#define RAIL_SCHEDULER_TASK_MASK         0xF0U
/** Radio Scheduler Task shift within \ref RAIL_SchedulerStatus_t values. */
#define RAIL_SCHEDULER_TASK_SHIFT        4

/**
 * @enum RAIL_SchedulerStatus_t
 * @brief Multiprotocol scheduler status returned by \ref RAIL_GetSchedulerStatus().
 *
 * \ref Multiprotocol scheduler status is a combination of the upper 4 bits which
 * constitute the type of scheduler task and the lower 4 bits which constitute
 * the type of scheduler error.
 */
RAIL_ENUM(RAIL_SchedulerStatus_t) {
  /** Multiprotocol scheduler reports no error. */
  RAIL_SCHEDULER_STATUS_NO_ERROR = (0U << RAIL_SCHEDULER_STATUS_SHIFT),
  /**
   * The scheduler is disabled or the requested scheduler operation is
   * unsupported.
   */
  RAIL_SCHEDULER_STATUS_UNSUPPORTED = (1U << RAIL_SCHEDULER_STATUS_SHIFT),
  /**
   * The scheduled task was started but was interrupted by a higher-priority
   * event before it could be completed.
   */
  RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED = (2U << RAIL_SCHEDULER_STATUS_SHIFT),
  /**
   * Scheduled task could not be scheduled given its priority and the other
   * tasks running on the system.
   */
  RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL = (3U << RAIL_SCHEDULER_STATUS_SHIFT),
  /**
   * Calling the RAIL API associated with the Radio scheduler task returned
   * an error code. See \ref RAIL_GetSchedulerStatus() or \ref RAIL_GetSchedulerStatusAlt()
   * for more information about \ref RAIL_Status_t status.
   */
  RAIL_SCHEDULER_STATUS_TASK_FAIL = (4U << RAIL_SCHEDULER_STATUS_SHIFT),
  /**
   * An internal error occurred in scheduler data structures, which should
   * not happen and indicates a problem.
   */
  RAIL_SCHEDULER_STATUS_INTERNAL_ERROR = (5U << RAIL_SCHEDULER_STATUS_SHIFT),

  /** Radio scheduler has no task. */
  RAIL_SCHEDULER_TASK_EMPTY = (0U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_ScheduleRx(). */
  RAIL_SCHEDULER_TASK_SCHEDULED_RX = (1U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartScheduledTx(). */
  RAIL_SCHEDULER_TASK_SCHEDULED_TX = (2U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartTx(). */
  RAIL_SCHEDULER_TASK_SINGLE_TX = (3U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartCcaCsmaTx(). */
  RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX = (4U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartCcaLbtTx(). */
  RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX = (5U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartScheduledCcaCsmaTx(). */
  RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX = (6U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartScheduledCcaLbtTx(). */
  RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX = (7U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartTxStream(). */
  RAIL_SCHEDULER_TASK_TX_STREAM = (8U << RAIL_SCHEDULER_TASK_SHIFT),
  /** Radio scheduler calls \ref RAIL_StartAverageRssi(). */
  RAIL_SCHEDULER_TASK_AVERAGE_RSSI = (9U << RAIL_SCHEDULER_TASK_SHIFT),

  /** \ref RAIL_StartScheduledTx() returned error status. */
  RAIL_SCHEDULER_STATUS_SCHEDULED_TX_FAIL = (RAIL_SCHEDULER_TASK_SCHEDULED_TX
                                             | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** \ref RAIL_StartTx() returned error status. */
  RAIL_SCHEDULER_STATUS_SINGLE_TX_FAIL = (RAIL_SCHEDULER_TASK_SINGLE_TX
                                          | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** \ref RAIL_StartCcaCsmaTx() returned error status. */
  RAIL_SCHEDULER_STATUS_CCA_CSMA_TX_FAIL = (RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX
                                            | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** \ref RAIL_StartCcaLbtTx() returned error status. */
  RAIL_SCHEDULER_STATUS_CCA_LBT_TX_FAIL = (RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX
                                           | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** \ref RAIL_ScheduleRx() returned error status. */
  RAIL_SCHEDULER_STATUS_SCHEDULED_RX_FAIL = (RAIL_SCHEDULER_TASK_SCHEDULED_RX
                                             | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** \ref RAIL_StartTxStream() returned error status. */
  RAIL_SCHEDULER_STATUS_TX_STREAM_FAIL = (RAIL_SCHEDULER_TASK_TX_STREAM
                                          | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** \ref RAIL_StartAverageRssi() returned error status. */
  RAIL_SCHEDULER_STATUS_AVERAGE_RSSI_FAIL = (RAIL_SCHEDULER_TASK_AVERAGE_RSSI
                                             | RAIL_SCHEDULER_STATUS_TASK_FAIL),

  /** Multiprotocol scheduled receive function internal error. */
  RAIL_SCHEDULER_SCHEDULED_RX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_RX
                                                | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol scheduled receive scheduling error. */
  RAIL_SCHEDULER_SCHEDULED_RX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_RX
                                                  | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_ScheduleRx() operation interrupted */
  RAIL_SCHEDULER_SCHEDULED_RX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SCHEDULED_RX
                                             | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol scheduled TX internal error. */
  RAIL_SCHEDULER_SCHEDULED_TX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_TX
                                                | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol scheduled TX scheduling error. */
  RAIL_SCHEDULER_SCHEDULED_TX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_TX
                                                  | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartScheduledTx() operation interrupted. */
  RAIL_SCHEDULER_SCHEDULED_TX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SCHEDULED_TX
                                             | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol instantaneous TX internal error. */
  RAIL_SCHEDULER_SINGLE_TX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SINGLE_TX
                                             | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol instantaneous TX scheduling error. */
  RAIL_SCHEDULER_SINGLE_TX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SINGLE_TX
                                               | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartTx() operation interrupted. */
  RAIL_SCHEDULER_SINGLE_TX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SINGLE_TX
                                          | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol single CSMA transmit function internal error. */
  RAIL_SCHEDULER_SINGLE_CCA_CSMA_TX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX
                                                      | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol single CSMA transmit scheduling error. */
  RAIL_SCHEDULER_SINGLE_CCA_CSMA_TX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX
                                                        | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartCcaCsmaTx() operation interrupted. */
  RAIL_SCHEDULER_SINGLE_CCA_CSMA_TX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX
                                                   | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol single LBT transmit function internal error. */
  RAIL_SCHEDULER_SINGLE_CCA_LBT_TX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX
                                                     | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol single LBT transmit scheduling error. */
  RAIL_SCHEDULER_SINGLE_CCA_LBT_TX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX
                                                       | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartCcaLbtTx() operation interrupted. */
  RAIL_SCHEDULER_SINGLE_CCA_LBT_TX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX
                                                  | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol scheduled CSMA transmit function internal error. */
  RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX
                                                         | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** \ref RAIL_StartScheduledCcaCsmaTx() returned error status. */
  RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_FAIL = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX
                                               | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** Multiprotocol scheduled CSMA transmit scheduling error. */
  RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX
                                                           | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartScheduledCcaCsmaTx() operation interrupted. */
  RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX
                                                      | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol scheduled LBT transmit function internal error. */
  RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX
                                                        | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** \ref RAIL_StartScheduledCcaLbtTx() returned error status. */
  RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_FAIL = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX
                                              | RAIL_SCHEDULER_STATUS_TASK_FAIL),
  /** Multiprotocol scheduled LBT transmit scheduling error. */
  RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX
                                                          | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartScheduledCcaLbtTx() operation interrupted. */
  RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_INTERRUPTED = (RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX
                                                     | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol stream transmit function internal error. */
  RAIL_SCHEDULER_TX_STREAM_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_TX_STREAM
                                             | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol stream transmit scheduling error. */
  RAIL_SCHEDULER_TX_STREAM_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_TX_STREAM
                                               | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartTxStream() operation interrupted. */
  RAIL_SCHEDULER_TX_STREAM_INTERRUPTED = (RAIL_SCHEDULER_TASK_TX_STREAM
                                          | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),

  /** Multiprotocol RSSI averaging function internal error. */
  RAIL_SCHEDULER_AVERAGE_RSSI_INTERNAL_ERROR = (RAIL_SCHEDULER_TASK_AVERAGE_RSSI
                                                | RAIL_SCHEDULER_STATUS_INTERNAL_ERROR),
  /** Multiprotocol RSSI average scheduling error. */
  RAIL_SCHEDULER_AVERAGE_RSSI_SCHEDULING_ERROR = (RAIL_SCHEDULER_TASK_AVERAGE_RSSI
                                                  | RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL),
  /** \ref RAIL_StartAverageRssi() operation interrupted. */
  RAIL_SCHEDULER_AVERAGE_RSSI_INTERRUPTED = (RAIL_SCHEDULER_TASK_AVERAGE_RSSI
                                             | RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED),
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_SCHEDULER_STATUS_NO_ERROR                        ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_NO_ERROR)
#define RAIL_SCHEDULER_STATUS_UNSUPPORTED                     ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_UNSUPPORTED)
#define RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED)
#define RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL                   ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL)
#define RAIL_SCHEDULER_STATUS_SCHEDULED_TX_FAIL               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_SCHEDULED_TX_FAIL)
#define RAIL_SCHEDULER_STATUS_SINGLE_TX_FAIL                  ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_SINGLE_TX_FAIL)
#define RAIL_SCHEDULER_STATUS_CCA_CSMA_TX_FAIL                ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_CCA_CSMA_TX_FAIL)
#define RAIL_SCHEDULER_STATUS_CCA_LBT_TX_FAIL                 ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_CCA_LBT_TX_FAIL)
#define RAIL_SCHEDULER_STATUS_SCHEDULED_RX_FAIL               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_SCHEDULED_RX_FAIL)
#define RAIL_SCHEDULER_STATUS_TX_STREAM_FAIL                  ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_TX_STREAM_FAIL)
#define RAIL_SCHEDULER_STATUS_AVERAGE_RSSI_FAIL               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_AVERAGE_RSSI_FAIL)
#define RAIL_SCHEDULER_STATUS_INTERNAL_ERROR                  ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_STATUS_INTERNAL_ERROR)

#define RAIL_SCHEDULER_TASK_EMPTY                             ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_EMPTY)
#define RAIL_SCHEDULER_TASK_SCHEDULED_RX                      ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_SCHEDULED_RX)
#define RAIL_SCHEDULER_TASK_SCHEDULED_TX                      ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_TX)
#define RAIL_SCHEDULER_TASK_SINGLE_TX                         ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_SINGLE_TX)
#define RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX                ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_SINGLE_CCA_CSMA_TX)
#define RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX                 ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_SINGLE_CCA_LBT_TX)
#define RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX             ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_SCHEDULED_CCA_CSMA_TX)
#define RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX              ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_SCHEDULED_CCA_LBT_TX)
#define RAIL_SCHEDULER_TASK_TX_STREAM                         ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_TX_STREAM)
#define RAIL_SCHEDULER_TASK_AVERAGE_RSSI                      ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TASK_AVERAGE_RSSI)

#define RAIL_SCHEDULER_SCHEDULED_RX_INTERNAL_ERROR            ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_RX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_RX_SCHEDULING_ERROR          ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_RX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_RX_INTERRUPTED               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_RX_INTERRUPTED)
#define RAIL_SCHEDULER_SCHEDULED_TX_INTERNAL_ERROR            ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_TX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_TX_SCHEDULING_ERROR          ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_TX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_TX_INTERRUPTED               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_TX_INTERRUPTED)
#define RAIL_SCHEDULER_SINGLE_TX_INTERNAL_ERROR               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SINGLE_TX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_SINGLE_TX_SCHEDULING_ERROR             ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SINGLE_TX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_SINGLE_TX_INTERRUPTED                  ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SINGLE_TX_INTERRUPTED)
#define RAIL_SCHEDULER_CCA_CSMA_TX_INTERNAL_ERROR             ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_CCA_CSMA_TX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_CCA_CSMA_TX_SCHEDULING_ERROR           ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_CCA_CSMA_TX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_CCA_CSMA_TX_INTERRUPTED                ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_CCA_CSMA_TX_INTERRUPTED)
#define RAIL_SCHEDULER_CCA_LBT_TX_INTERNAL_ERROR              ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_CCA_LBT_TX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_CCA_LBT_TX_SCHEDULING_ERROR            ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_CCA_LBT_TX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_CCA_LBT_TX_INTERRUPTED                 ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_CCA_LBT_TX_INTERRUPTED)
#define RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_INTERNAL_ERROR   ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_FAIL             ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_FAIL)
#define RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_SCHEDULING_ERROR ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_INTERRUPTED      ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_CSMA_TX_INTERRUPTED)
#define RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_INTERNAL_ERROR    ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_INTERNAL_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_FAIL              ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_FAIL)
#define RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_SCHEDULING_ERROR  ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_INTERRUPTED       ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_SCHEDULED_CCA_LBT_TX_INTERRUPTED)
#define RAIL_SCHEDULER_TX_STREAM_INTERNAL_ERROR               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TX_STREAM_INTERNAL_ERROR)
#define RAIL_SCHEDULER_TX_STREAM_SCHEDULING_ERROR             ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TX_STREAM_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_TX_STREAM_INTERRUPTED                  ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_TX_STREAM_INTERRUPTED)
#define RAIL_SCHEDULER_AVERAGE_RSSI_INTERNAL_ERROR            ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_AVERAGE_RSSI_INTERNAL_ERROR)
#define RAIL_SCHEDULER_AVERAGE_RSSI_SCHEDULING_ERROR          ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_AVERAGE_RSSI_SCHEDULING_ERROR)
#define RAIL_SCHEDULER_AVERAGE_RSSI_INTERRUPTED               ((RAIL_SchedulerStatus_t) RAIL_SCHEDULER_AVERAGE_RSSI_INTERRUPTED)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @enum RAIL_TaskType_t
 * @brief Multiprotocol radio operation task types, used with
 *   \ref RAIL_SetTaskPriority().
 */
RAIL_ENUM(RAIL_TaskType_t) {
  /** Indicate a task started using \ref RAIL_StartRx(). */
  RAIL_TASK_TYPE_START_RX = 0,
  /** Indicate a task started functions other than \ref RAIL_StartRx(). */
  RAIL_TASK_TYPE_OTHER = 1,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_TASK_TYPE_START_RX ((RAIL_TaskType_t) RAIL_TASK_TYPE_START_RX)
#define RAIL_TASK_TYPE_OTHER    ((RAIL_TaskType_t) RAIL_TASK_TYPE_OTHER)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group Multiprotocol

/******************************************************************************
 * Event Structures
 *****************************************************************************/
/**
 * @addtogroup Events
 * @{
 */

/**
 * @enum RAIL_Events_t
 * @brief RAIL events passed to the event callback. More than one event may be
 *   indicated due to interrupt latency.
 */
RAIL_ENUM_GENERIC(RAIL_Events_t, uint64_t) {
  // RX Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_RSSI_AVERAGE_DONE bit. */
  RAIL_EVENT_RSSI_AVERAGE_DONE_SHIFT = 0,
  /** Shift position of \ref RAIL_EVENT_RX_ACK_TIMEOUT bit. */
  RAIL_EVENT_RX_ACK_TIMEOUT_SHIFT = 1,
  /** Shift position of \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL bit. */
  RAIL_EVENT_RX_FIFO_ALMOST_FULL_SHIFT = 2,
  /** Shift position of \ref RAIL_EVENT_RX_PACKET_RECEIVED bit. */
  RAIL_EVENT_RX_PACKET_RECEIVED_SHIFT = 3,
  /** Shift position of \ref RAIL_EVENT_RX_PREAMBLE_LOST bit. */
  RAIL_EVENT_RX_PREAMBLE_LOST_SHIFT = 4,
  /** Shift position of \ref RAIL_EVENT_RX_PREAMBLE_DETECT bit. */
  RAIL_EVENT_RX_PREAMBLE_DETECT_SHIFT = 5,
  /** Shift position of \ref RAIL_EVENT_RX_SYNC1_DETECT bit. */
  RAIL_EVENT_RX_SYNC1_DETECT_SHIFT = 6,
  /** Shift position of \ref RAIL_EVENT_RX_SYNC2_DETECT bit. */
  RAIL_EVENT_RX_SYNC2_DETECT_SHIFT = 7,
  /** Shift position of \ref RAIL_EVENT_RX_FRAME_ERROR bit. */
  RAIL_EVENT_RX_FRAME_ERROR_SHIFT = 8,
  /** Shift position of \ref RAIL_EVENT_RX_FIFO_FULL bit. */
  RAIL_EVENT_RX_FIFO_FULL_SHIFT = 9,
  /** Shift position of \ref RAIL_EVENT_RX_FIFO_OVERFLOW bit. */
  RAIL_EVENT_RX_FIFO_OVERFLOW_SHIFT = 10,
  /** Shift position of \ref RAIL_EVENT_RX_ADDRESS_FILTERED bit. */
  RAIL_EVENT_RX_ADDRESS_FILTERED_SHIFT = 11,
  /** Shift position of \ref RAIL_EVENT_RX_TIMEOUT bit. */
  RAIL_EVENT_RX_TIMEOUT_SHIFT = 12,
  /** Shift position of \ref RAIL_EVENT_SCHEDULED_RX_STARTED bit. */
  RAIL_EVENT_SCHEDULED_RX_STARTED_SHIFT = 13,
  /** Shift position of \ref RAIL_EVENT_RX_SCHEDULED_RX_END bit. */
  RAIL_EVENT_RX_SCHEDULED_RX_END_SHIFT = 14,
  /** Shift position of \ref RAIL_EVENT_RX_SCHEDULED_RX_MISSED bit. */
  RAIL_EVENT_RX_SCHEDULED_RX_MISSED_SHIFT = 15,
  /** Shift position of \ref RAIL_EVENT_RX_PACKET_ABORTED bit. */
  RAIL_EVENT_RX_PACKET_ABORTED_SHIFT = 16,
  /** Shift position of \ref RAIL_EVENT_RX_FILTER_PASSED bit. */
  RAIL_EVENT_RX_FILTER_PASSED_SHIFT = 17,
  /** Shift position of \ref RAIL_EVENT_RX_TIMING_LOST bit. */
  RAIL_EVENT_RX_TIMING_LOST_SHIFT = 18,
  /** Shift position of \ref RAIL_EVENT_RX_TIMING_DETECT bit. */
  RAIL_EVENT_RX_TIMING_DETECT_SHIFT = 19,
  /** Shift position of \ref RAIL_EVENT_RX_CHANNEL_HOPPING_COMPLETE bit. */
  RAIL_EVENT_RX_CHANNEL_HOPPING_COMPLETE_SHIFT = 20,
  /** Shift position of \ref RAIL_EVENT_RX_DUTY_CYCLE_RX_END bit. */
  RAIL_EVENT_RX_DUTY_CYCLE_RX_END_SHIFT = RAIL_EVENT_RX_CHANNEL_HOPPING_COMPLETE_SHIFT,
  /** Shift position of \ref RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND bit. */
  RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND_SHIFT = 21,
  /** Shift position of \ref RAIL_EVENT_ZWAVE_LR_ACK_REQUEST_COMMAND_SHIFT bit. */
  RAIL_EVENT_ZWAVE_LR_ACK_REQUEST_COMMAND_SHIFT = RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND_SHIFT,
  /** Shift position of \ref RAIL_EVENT_MFM_TX_BUFFER_DONE bit. */
  RAIL_EVENT_MFM_TX_BUFFER_DONE_SHIFT = RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND_SHIFT,

// TX Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_ZWAVE_BEAM bit. */
  RAIL_EVENT_ZWAVE_BEAM_SHIFT = 22,
  /** Shift position of \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY bit. */
  RAIL_EVENT_TX_FIFO_ALMOST_EMPTY_SHIFT = 23,
  /** Shift position of \ref RAIL_EVENT_TX_PACKET_SENT bit. */
  RAIL_EVENT_TX_PACKET_SENT_SHIFT = 24,
  /** Shift position of \ref RAIL_EVENT_TXACK_PACKET_SENT bit. */
  RAIL_EVENT_TXACK_PACKET_SENT_SHIFT = 25,
  /** Shift position of \ref RAIL_EVENT_TX_ABORTED bit. */
  RAIL_EVENT_TX_ABORTED_SHIFT = 26,
  /** Shift position of \ref RAIL_EVENT_TXACK_ABORTED bit. */
  RAIL_EVENT_TXACK_ABORTED_SHIFT = 27,
  /** Shift position of \ref RAIL_EVENT_TX_BLOCKED bit. */
  RAIL_EVENT_TX_BLOCKED_SHIFT = 28,
  /** Shift position of \ref RAIL_EVENT_TXACK_BLOCKED bit. */
  RAIL_EVENT_TXACK_BLOCKED_SHIFT = 29,
  /** Shift position of \ref RAIL_EVENT_TX_UNDERFLOW bit. */
  RAIL_EVENT_TX_UNDERFLOW_SHIFT = 30,
  /** Shift position of \ref RAIL_EVENT_TXACK_UNDERFLOW bit. */
  RAIL_EVENT_TXACK_UNDERFLOW_SHIFT = 31,
  /** Shift position of \ref RAIL_EVENT_TX_CHANNEL_CLEAR bit. */
  RAIL_EVENT_TX_CHANNEL_CLEAR_SHIFT = 32,
  /** Shift position of \ref RAIL_EVENT_TX_CHANNEL_BUSY bit. */
  RAIL_EVENT_TX_CHANNEL_BUSY_SHIFT = 33,
  /** Shift position of \ref RAIL_EVENT_TX_CCA_RETRY bit. */
  RAIL_EVENT_TX_CCA_RETRY_SHIFT = 34,
  /** Shift position of \ref RAIL_EVENT_TX_START_CCA bit. */
  RAIL_EVENT_TX_START_CCA_SHIFT = 35,
  /** Shift position of \ref RAIL_EVENT_TX_STARTED bit. */
  RAIL_EVENT_TX_STARTED_SHIFT = 36,
  /** Shift position of \ref RAIL_EVENT_SCHEDULED_TX_STARTED bit. */
  RAIL_EVENT_SCHEDULED_TX_STARTED_SHIFT = RAIL_EVENT_SCHEDULED_RX_STARTED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_SCHEDULED_TX_MISSED bit. */
  RAIL_EVENT_TX_SCHEDULED_TX_MISSED_SHIFT = 37,

  // Scheduler Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_CONFIG_UNSCHEDULED bit. */
  RAIL_EVENT_CONFIG_UNSCHEDULED_SHIFT = 38,
  /** Shift position of \ref RAIL_EVENT_CONFIG_SCHEDULED bit. */
  RAIL_EVENT_CONFIG_SCHEDULED_SHIFT = 39,
  /** Shift position of \ref RAIL_EVENT_SCHEDULER_STATUS bit. */
  RAIL_EVENT_SCHEDULER_STATUS_SHIFT = 40,

  // Other Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_CAL_NEEDED bit. */
  RAIL_EVENT_CAL_NEEDED_SHIFT = 41,
  /** Shift position of \ref RAIL_EVENT_RF_SENSED bit. */
  RAIL_EVENT_RF_SENSED_SHIFT = 42,
  /** Shift position of \ref RAIL_EVENT_PA_PROTECTION bit. */
  RAIL_EVENT_PA_PROTECTION_SHIFT = 43,
  /** Shift position of \ref RAIL_EVENT_SIGNAL_DETECTED bit. */
  RAIL_EVENT_SIGNAL_DETECTED_SHIFT = 44,
  /** Shift position of \ref RAIL_EVENT_IEEE802154_MODESWITCH_START bit. */
  RAIL_EVENT_IEEE802154_MODESWITCH_START_SHIFT = 45,
  /** Shift position of \ref RAIL_EVENT_IEEE802154_MODESWITCH_END bit. */
  RAIL_EVENT_IEEE802154_MODESWITCH_END_SHIFT = 46,
  /** Shift position of \ref RAIL_EVENT_DETECT_RSSI_THRESHOLD bit. */
  RAIL_EVENT_DETECT_RSSI_THRESHOLD_SHIFT = 47,
  /** Shift position of \ref RAIL_EVENT_THERMISTOR_DONE bit. */
  RAIL_EVENT_THERMISTOR_DONE_SHIFT = 48,
  /** Shift position of \ref RAIL_EVENT_TX_BLOCKED_TOO_HOT bit. */
  RAIL_EVENT_TX_BLOCKED_TOO_HOT_SHIFT = 49,
  /** Shift position of \ref RAIL_EVENT_TEMPERATURE_TOO_HOT bit. */
  RAIL_EVENT_TEMPERATURE_TOO_HOT_SHIFT = 50,
  /** Shift position of \ref RAIL_EVENT_TEMPERATURE_COOL_DOWN bit. */
  RAIL_EVENT_TEMPERATURE_COOL_DOWN_SHIFT = 51,
  /** Shift position of \ref RAIL_EVENT_USER_MBOX bit. */
  RAIL_EVENT_USER_MBOX_SHIFT = 52,
};

// RAIL_Event_t bitmasks

/** A value representing no events. */
#define RAIL_EVENTS_NONE 0ULL

/**
 * Occurs when the hardware-averaged RSSI is done in response to
 * \ref RAIL_StartAverageRssi() to indicate that the hardware has completed
 * averaging.
 *
 * Call \ref RAIL_GetAverageRssi() to get the result.
 */
#define RAIL_EVENT_RSSI_AVERAGE_DONE (1ULL << RAIL_EVENT_RSSI_AVERAGE_DONE_SHIFT)

/**
 * Occurs when the Ack timeout expires while waiting to receive the
 * sync word of an expected Ack. If the timeout occurs within packet
 * reception, this event won't be signaled until after packet
 * completion has determined the packet wasn't the expected Ack.
 * See \ref RAIL_RxPacketDetails_t::isAck for the definition of an
 * expected Ack.
 *
 * This event only occurs after calling \ref RAIL_ConfigAutoAck() and after
 * transmitting a packet with \ref RAIL_TX_OPTION_WAIT_FOR_ACK set.
 */
#define RAIL_EVENT_RX_ACK_TIMEOUT (1ULL << RAIL_EVENT_RX_ACK_TIMEOUT_SHIFT)

/**
 * Keeps occurring as long as the number of bytes in the receive FIFO
 * exceeds the configured threshold value.
 *
 * Call \ref RAIL_GetRxFifoBytesAvailable() to get the number of
 * bytes available. When using this event, the threshold should be set via
 * \ref RAIL_SetRxFifoThreshold().
 *
 * How to avoid sticking in the event handler (even in idle state):
 * 1. Disable the event (via the config events API or the
 *    \ref RAIL_FIFO_THRESHOLD_DISABLED parameter)
 * 2. Increase FIFO threshold
 * 3. Read the FIFO (that's not an option in
 *    \ref RAIL_DataMethod_t::PACKET_MODE) in the event handler
 */
#define RAIL_EVENT_RX_FIFO_ALMOST_FULL (1ULL << RAIL_EVENT_RX_FIFO_ALMOST_FULL_SHIFT)

/**
 * Occurs whenever a packet is received with \ref RAIL_RX_PACKET_READY_SUCCESS
 * or \ref RAIL_RX_PACKET_READY_CRC_ERROR.
 *
 * Call \ref RAIL_GetRxPacketInfo() to get
 * basic information about the packet along with a handle to this packet for
 * subsequent use with \ref RAIL_PeekRxPacket(), \ref RAIL_GetRxPacketDetails(),
 * \ref RAIL_HoldRxPacket(), and \ref RAIL_ReleaseRxPacket() as needed.
 */
#define RAIL_EVENT_RX_PACKET_RECEIVED (1ULL << RAIL_EVENT_RX_PACKET_RECEIVED_SHIFT)

/**
 * Occurs when the radio has lost a preamble.
 *
 * This event can occur multiple
 * times while searching for a packet and is generally used for diagnostic
 * purposes. It can only occur after a
 * \ref RAIL_EVENT_RX_PREAMBLE_DETECT event has already occurred.
 *
 * @note See warning for \ref RAIL_EVENT_RX_PREAMBLE_DETECT.
 */
#define RAIL_EVENT_RX_PREAMBLE_LOST (1ULL << RAIL_EVENT_RX_PREAMBLE_LOST_SHIFT)

/**
 * Occurs when the radio has detected a preamble.
 *
 * This event can occur multiple
 * times while searching for a packet and is generally used for diagnostic
 * purposes. It can only occur after a \ref RAIL_EVENT_RX_TIMING_DETECT
 * event has already occurred.
 *
 * @warning This event, along with \ref RAIL_EVENT_RX_PREAMBLE_LOST,
 *   may not work on some demodulators. Some demodulators usurped the signals
 *   on which these events are based for another purpose. These demodulators
 *   in particular are available on the EFR32xG23, EFR32xG25, and the EFR32xG28
 *   platforms. Enabling these events on these platforms may cause the
 *   events to fire infinitely and possibly freeze the application.
 */
#define RAIL_EVENT_RX_PREAMBLE_DETECT (1ULL << RAIL_EVENT_RX_PREAMBLE_DETECT_SHIFT)

/**
 * Occurs when the first sync word is detected.
 *
 * After this event occurs, one of
 * the events in the \ref RAIL_EVENTS_RX_COMPLETION mask will occur.
 */
#define RAIL_EVENT_RX_SYNC1_DETECT (1ULL << RAIL_EVENT_RX_SYNC1_DETECT_SHIFT)

/**
 * Occurs when the second sync word is detected.
 *
 * After this event occurs, one of
 * the events in the \ref RAIL_EVENTS_RX_COMPLETION mask will occur.
 */
#define RAIL_EVENT_RX_SYNC2_DETECT (1ULL << RAIL_EVENT_RX_SYNC2_DETECT_SHIFT)

/**
 * Occurs when a receive is aborted with \ref RAIL_RX_PACKET_ABORT_CRC_ERROR
 * which only happens after any filtering has passed.
 *
 * For EFR32 parts, this event includes CRC errors, block decoding errors,
 * and illegal frame length -- when detected after filtering. (When such
 * errors are detected during filtering, they're signaled as \ref
 * RAIL_EVENT_RX_PACKET_ABORTED instead.)
 *
 * If \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS is set, this event will not
 * occur for CRC errors, but could still occur for the other errors.
 */
#define RAIL_EVENT_RX_FRAME_ERROR (1ULL << RAIL_EVENT_RX_FRAME_ERROR_SHIFT)

/**
 * When using \ref RAIL_RxDataSource_t::RX_PACKET_DATA this event
 * occurs coincident to a receive packet completion event in which the
 * receive FIFO or any supplemental packet metadata FIFO (see \ref
 * Data_Management) are full and further packet reception is jeopardized.
 *
 * It signals that an overflow is imminent (and may already have occurred)
 * telling the application it should release the oldest packet(s) as soon
 * as possible. This event may may be posted multiple times with subsequent
 * receive completion events if the FIFO(s) remain full, and should also
 * occur coincident with \ref RAIL_EVENT_RX_FIFO_OVERFLOW.
 *
 * When not using \ref RAIL_RxDataSource_t::RX_PACKET_DATA this event
 * is not tied to packet completion and will occur coincident with
 * \ref RAIL_EVENT_RX_FIFO_OVERFLOW when the receive FIFO has filled and
 * overflowed. The application should consume receive FIFO data via
 * \ref RAIL_ReadRxFifo() as soon as possible to minimize lost raw data.
 */
#define RAIL_EVENT_RX_FIFO_FULL (1ULL << RAIL_EVENT_RX_FIFO_FULL_SHIFT)

/**
 * When using \ref RAIL_RxDataSource_t::RX_PACKET_DATA this event
 * occurs when a receive is aborted with \ref RAIL_RX_PACKET_ABORT_OVERFLOW
 * due to overflowing the receive FIFO or any supplemental packet metadata
 * FIFO (see \ref Data_Management).
 *
 * The radio suspends receiving packets until this event is posted and
 * the receive FIFO(s) have been fully processed (drained and released
 * or reset). It is not guaranteed that a \ref RAIL_EVENT_RX_FIFO_FULL
 * will precede this event, but both events should be coincident.
 *
 * When not using \ref RAIL_RxDataSource_t::RX_PACKET_DATA this event
 * is not tied to packet completion and will occur coincident with
 * \ref RAIL_EVENT_RX_FIFO_FULL when the receive FIFO has filled and
 * overflowed. The application should consume receive FIFO data via
 * \ref RAIL_ReadRxFifo() as soon as possible to minimize lost raw data.
 */
#define RAIL_EVENT_RX_FIFO_OVERFLOW (1ULL << RAIL_EVENT_RX_FIFO_OVERFLOW_SHIFT)

/**
 * Occurs when a receive is aborted with \ref RAIL_RX_PACKET_ABORT_FILTERED
 * because its address does not match the filtering settings.
 *
 * This event can only occur after calling \ref RAIL_EnableAddressFilter().
 */
#define RAIL_EVENT_RX_ADDRESS_FILTERED (1ULL << RAIL_EVENT_RX_ADDRESS_FILTERED_SHIFT)

/**
 * Occurs when an RX event times out.
 *
 * This event can only occur if the
 * RAIL_StateTiming_t::rxSearchTimeout passed to \ref RAIL_SetStateTiming() is
 * not zero.
 */
#define RAIL_EVENT_RX_TIMEOUT (1ULL << RAIL_EVENT_RX_TIMEOUT_SHIFT)

/**
 * Occurs when a scheduled RX begins turning on the receiver.
 * This event has the same numerical value as \ref RAIL_EVENT_SCHEDULED_TX_STARTED
 * because one cannot schedule both RX and TX simultaneously.
 */
#define RAIL_EVENT_SCHEDULED_RX_STARTED (1ULL << RAIL_EVENT_SCHEDULED_RX_STARTED_SHIFT)

/**
 * Occurs when the scheduled RX window ends.
 *
 * This event only occurs in response
 * to a scheduled receive timeout after calling \ref RAIL_ScheduleRx(). If
 * RAIL_ScheduleRxConfig_t::rxTransitionEndSchedule was passed as false,
 * this event will occur unless the receive is aborted (due to a call to
 * \ref RAIL_Idle() or a scheduler preemption, for instance). If
 * RAIL_ScheduleRxConfig_t::rxTransitionEndSchedule was passed as true,
 * any of the \ref RAIL_EVENTS_RX_COMPLETION events occurring will also cause
 * this event not to occur, since the scheduled receive will end with the
 * transition at the end of the packet. However, if the application has not
 * enabled the specific \ref RAIL_EVENTS_RX_COMPLETION event which implicitly
 * ended the scheduled receive, this event will be posted instead.
 */
#define RAIL_EVENT_RX_SCHEDULED_RX_END (1ULL << RAIL_EVENT_RX_SCHEDULED_RX_END_SHIFT)

/**
 * Occurs when start of a scheduled receive is missed.
 *
 * This can occur if the radio is put to sleep and not woken up with enough time
 * to configure the scheduled receive event.
 */
#define RAIL_EVENT_RX_SCHEDULED_RX_MISSED (1ULL << RAIL_EVENT_RX_SCHEDULED_RX_MISSED_SHIFT)

/**
 * Occurs when a receive is aborted during filtering with
 * \ref RAIL_RX_PACKET_ABORT_FORMAT or after filtering with
 * \ref RAIL_RX_PACKET_ABORT_ABORTED for reasons other than address
 * filtering mismatch (which triggers \ref RAIL_EVENT_RX_ADDRESS_FILTERED
 * instead).
 *
 * For EFR32 parts, this event includes CRC errors, block decoding errors,
 * illegal frame length, and other RAIL built-in protocol-specific packet
 * content errors -- when detected during filtering. (When such errors
 * are detected after filtering, they're signaled as \ref
 * RAIL_EVENT_RX_FRAME_ERROR instead.) It also includes application or
 * multiprotocol scheduler aborting a receive after filtering has passed.
 */
#define RAIL_EVENT_RX_PACKET_ABORTED (1ULL << RAIL_EVENT_RX_PACKET_ABORTED_SHIFT)

/**
 * Occurs when the packet has passed any configured address and frame
 * filtering options.
 *
 * This event will only occur between the start of the
 * packet, indicated by \ref RAIL_EVENT_RX_SYNC1_DETECT or
 * \ref RAIL_EVENT_RX_SYNC2_DETECT and one of the events in the
 * \ref RAIL_EVENTS_RX_COMPLETION mask. It will always occur before or
 * concurrently with \ref RAIL_EVENT_RX_PACKET_RECEIVED. If IEEE 802.15.4 frame
 * and address filtering are enabled, this event will occur immediately after
 * destination address filtering.
 */
#define RAIL_EVENT_RX_FILTER_PASSED (1ULL << RAIL_EVENT_RX_FILTER_PASSED_SHIFT)

/**
 * Occurs when the modem timing is lost.
 *
 * This event can occur multiple times
 * while searching for a packet and is generally used for diagnostic purposes.
 * It can only occur after a \ref RAIL_EVENT_RX_TIMING_DETECT event has
 * already occurred.
 *
 * @note See warning for \ref RAIL_EVENT_RX_TIMING_DETECT.
 */
#define RAIL_EVENT_RX_TIMING_LOST (1ULL << RAIL_EVENT_RX_TIMING_LOST_SHIFT)

/**
 * Occurs when the modem timing is detected.
 *
 * This event can occur multiple times
 * while searching for a packet and is generally used for diagnostic purposes.
 *
 * @warning This event, along with \ref RAIL_EVENT_RX_TIMING_LOST,
 *   may not work on some demodulators. Some demodulators usurped the signals
 *   on which these events are based for another purpose. These demodulators
 *   in particular are available on the EFR32xG23, EFR32xG25, and the EFR32xG28
 *   platforms. Enabling these events on these platforms may cause the
 *   events to fire infinitely and possibly freeze the application.
 */
#define RAIL_EVENT_RX_TIMING_DETECT (1ULL << RAIL_EVENT_RX_TIMING_DETECT_SHIFT)

/**
 * Occurs when RX Channel Hopping is enabled and channel hopping finishes
 * receiving on the last channel in its sequence.
 *
 * The intent behind this event
 * is to allow the user to keep the radio on for as short a time as possible.
 * That is, once the channel sequence is complete, the application will receive
 * this event and can trigger a sleep/idle until it is necessary to cycle
 * through the channels again. If this event is left on indefinitely and not
 * handled it will likely be a fairly noisy event, as it continues to fire
 * each time the hopping algorithm cycles through the channel sequence.
 */
#define RAIL_EVENT_RX_CHANNEL_HOPPING_COMPLETE (1ULL << RAIL_EVENT_RX_CHANNEL_HOPPING_COMPLETE_SHIFT)

/**
 * Occurs during RX duty cycle mode when the radio finishes its time in
 * receive mode.
 *
 * The application can then trigger a sleep/idle until it
 * needs to listen again.
 */
#define RAIL_EVENT_RX_DUTY_CYCLE_RX_END (1ULL << RAIL_EVENT_RX_DUTY_CYCLE_RX_END_SHIFT)

/**
 * Indicate a Data Request is received when using IEEE 802.15.4
 * functionality.
 *
 * It occurs when the command byte of an incoming Ack-requesting MAC Control
 * frame is for a data request. This callback is called before
 * the packet is fully received to allow the node to have more time to decide
 * whether to indicate a frame is pending in the outgoing Ack. This event only
 * occurs if the RAIL IEEE 802.15.4 functionality is enabled, but will never
 * occur if promiscuous mode is enabled via \ref
 * RAIL_IEEE802154_SetPromiscuousMode().
 *
 * Call \ref RAIL_IEEE802154_GetAddress() to get the source address of the packet.
 */
#define RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND (1ULL << RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND_SHIFT)

/**
 * Indicate a Z-Wave Beam Request relevant to the node was received.
 *
 * This event only occurs if the RAIL Z-Wave functionality is enabled
 * and its \ref RAIL_ZWAVE_OPTION_DETECT_BEAM_FRAMES is enabled.
 * This event is used in lieu of \ref RAIL_EVENT_RX_PACKET_RECEIVED,
 * which is reserved for Z-Wave packets other than Beams.
 *
 * Call \ref RAIL_ZWAVE_GetBeamNodeId() to get the Node Id to which the Beam was
 * targeted, which would be either the broadcast id 0xFF or the node's own
 * single-cast id.
 *
 * @note All Z-Wave Beam requests are generally discarded, triggering
 *   \ref RAIL_EVENT_RX_PACKET_ABORTED.
 */
#define RAIL_EVENT_ZWAVE_BEAM (1ULL << RAIL_EVENT_ZWAVE_BEAM_SHIFT)

/**
 * Indicate a MFM buffer has completely transmitted.
 *
 * This event only occurs if the RAIL MFM functionality is enabled
 * and a MFM buffer has completely transmitted.
 *
 * Following this event, the application can update the MFM buffer
 * that has transmitted to be used for the next transmission.
 */
#define RAIL_EVENT_MFM_TX_BUFFER_DONE (1ULL << RAIL_EVENT_MFM_TX_BUFFER_DONE_SHIFT)

/**
 * Indicate a request for populating Z-Wave LR Ack packet.
 * This event only occurs if the RAIL Z-Wave functionality is enabled.
 *
 * Following this event, the application must call \ref RAIL_ZWAVE_SetLrAckData()
 * to populate noise floor, TX power and receive RSSI fields of the Z-Wave
 * Long Range Ack packet.
 */
#define RAIL_EVENT_ZWAVE_LR_ACK_REQUEST_COMMAND (1ULL << RAIL_EVENT_ZWAVE_LR_ACK_REQUEST_COMMAND_SHIFT)

/**
 * The mask representing all events that determine the end of a received
 * packet.
 *
 * After a \ref RAIL_EVENT_RX_SYNC1_DETECT or a
 * \ref RAIL_EVENT_RX_SYNC2_DETECT,
 * exactly one of the following events will occur. When one of these events
 * occurs, a state transition will take place based on the parameter passed to
 * \ref RAIL_SetRxTransitions(). The \ref RAIL_StateTransitions_t::success transition
 * will be followed only if the \ref RAIL_EVENT_RX_PACKET_RECEIVED event occurs.
 * Any of the other events will trigger the \ref RAIL_StateTransitions_t::error
 * transition.
 */
#define RAIL_EVENTS_RX_COMPLETION (RAIL_EVENT_RX_PACKET_RECEIVED    \
                                   | RAIL_EVENT_RX_PACKET_ABORTED   \
                                   | RAIL_EVENT_RX_FRAME_ERROR      \
                                   | RAIL_EVENT_RX_FIFO_OVERFLOW    \
                                   | RAIL_EVENT_RX_ADDRESS_FILTERED \
                                   | RAIL_EVENT_RX_SCHEDULED_RX_MISSED)

// TX Event Bitmasks

/**
 * Occurs when the number of bytes in the transmit FIFO falls below the
 * configured threshold value.
 *
 * This event does not occur on initialization or after resetting the transmit
 * FIFO with \ref RAIL_ResetFifo().
 *
 * Call \ref RAIL_GetTxFifoSpaceAvailable() to get the
 * number of bytes available in the transmit FIFO at the time of the callback
 * dispatch. When using this event, the threshold should be set via \ref
 * RAIL_SetTxFifoThreshold().
 */
#define RAIL_EVENT_TX_FIFO_ALMOST_EMPTY (1ULL << RAIL_EVENT_TX_FIFO_ALMOST_EMPTY_SHIFT)

/**
 * Occurs after a packet has been transmitted.
 *
 * Call \ref RAIL_GetTxPacketDetails()
 * to get information about the packet that was transmitted.
 *
 * @note \ref RAIL_GetTxPacketDetails() is only valid to call during the time frame
 *   of the \ref RAIL_Config_t::eventsCallback.
 */
#define RAIL_EVENT_TX_PACKET_SENT (1ULL << RAIL_EVENT_TX_PACKET_SENT_SHIFT)

/**
 * Occurs after an Ack packet has been transmitted.
 *
 * Call \ref RAIL_GetTxPacketDetails()
 * to get information about the packet that was transmitted. This event can only occur
 * after calling \ref RAIL_ConfigAutoAck().
 *
 * @note \ref RAIL_GetTxPacketDetails() is only valid to call during the time frame
 *   of the \ref RAIL_Config_t::eventsCallback.
 */
#define RAIL_EVENT_TXACK_PACKET_SENT (1ULL << RAIL_EVENT_TXACK_PACKET_SENT_SHIFT)

/**
 * Occurs when a transmit is aborted by the user.
 *
 * This can happen due to calling \ref RAIL_Idle() or due to a scheduler
 * preemption.
 *
 * @note The Transmit FIFO is left in an indeterminate state and should be
 *   reset prior to reuse for sending a new packet. Contrast this
 *   with \ref RAIL_EVENT_TX_BLOCKED.
 */
#define RAIL_EVENT_TX_ABORTED (1ULL << RAIL_EVENT_TX_ABORTED_SHIFT)

/**
 * Occurs when an Ack transmit is aborted by the user.
 *
 * This event can only
 * occur after calling \ref RAIL_ConfigAutoAck(), which can happen due to calling
 * \ref RAIL_Idle() or due to a scheduler preemption.
 */
#define RAIL_EVENT_TXACK_ABORTED (1ULL << RAIL_EVENT_TXACK_ABORTED_SHIFT)

/**
 * Occurs when a transmit is blocked from occurring because
 * \ref RAIL_EnableTxHoldOff() was called.
 *
 * @note Since the transmit never started, the Transmit FIFO remains intact
 *   after this event -- no packet data was consumed from it. Contrast this
 *   with \ref RAIL_EVENT_TX_ABORTED.
 */
#define RAIL_EVENT_TX_BLOCKED (1ULL << RAIL_EVENT_TX_BLOCKED_SHIFT)

/**
 * Occurs when an Ack transmit is blocked from occurring because
 * \ref RAIL_EnableTxHoldOff() was called.
 *
 * This event can only occur after calling \ref RAIL_ConfigAutoAck().
 */
#define RAIL_EVENT_TXACK_BLOCKED (1ULL << RAIL_EVENT_TXACK_BLOCKED_SHIFT)

/**
 * Occurs when the transmit buffer underflows.
 *
 * This can happen due to the
 * transmitted packet specifying an unintended length based on the current
 * radio configuration or due to \ref RAIL_WriteTxFifo() calls not keeping up with
 * the transmit rate if the entire packet isn't loaded at once.
 *
 * @note The Transmit FIFO is left in an indeterminate state and should be
 *   reset prior to reuse for sending a new packet. Contrast this
 *   with \ref RAIL_EVENT_TX_BLOCKED.
 */
#define RAIL_EVENT_TX_UNDERFLOW (1ULL << RAIL_EVENT_TX_UNDERFLOW_SHIFT)

/**
 * Occurs when the Ack transmit buffer underflows.
 *
 * This can happen due to the
 * transmitted packet specifying an unintended length based on the current
 * radio configuration or due to \ref RAIL_WriteAutoAckFifo() not being called at
 * all before an Ack transmit.
 *
 * This event can only occur after calling \ref RAIL_ConfigAutoAck().
 */
#define RAIL_EVENT_TXACK_UNDERFLOW (1ULL << RAIL_EVENT_TXACK_UNDERFLOW_SHIFT)

/**
 * Occurs when Carrier Sense Multiple Access (CSMA) or Listen Before Talk (LBT)
 * succeeds.
 *
 * This event can only happen after calling \ref RAIL_StartCcaCsmaTx() or
 * \ref RAIL_StartCcaLbtTx() or their scheduled equivalent.
 */
#define RAIL_EVENT_TX_CHANNEL_CLEAR (1ULL << RAIL_EVENT_TX_CHANNEL_CLEAR_SHIFT)

/**
 * Occurs when Carrier Sense Multiple Access (CSMA) or Listen Before Talk (LBT)
 * fails.
 *
 * This event can only happen after calling \ref RAIL_StartCcaCsmaTx() or
 * \ref RAIL_StartCcaLbtTx() or their scheduled equivalent.
 *
 * @note Since the transmit never started, the Transmit FIFO remains intact
 *   after this event -- no packet data was consumed from it.
 */
#define RAIL_EVENT_TX_CHANNEL_BUSY (1ULL << RAIL_EVENT_TX_CHANNEL_BUSY_SHIFT)

/**
 * Occurs during CSMA or LBT when an individual Clear Channel Assessment (CCA)
 * check fails, but there are more tries needed before the overall operation
 * completes.
 *
 * This event can occur multiple times based on the configuration
 * of the ongoing CSMA or LBT transmission. It can only happen after
 * calling \ref RAIL_StartCcaCsmaTx() or \ref RAIL_StartCcaLbtTx()
 * or their scheduled equivalent.
 */
#define RAIL_EVENT_TX_CCA_RETRY (1ULL << RAIL_EVENT_TX_CCA_RETRY_SHIFT)

/**
 * Occurs when the receiver is activated to perform a Clear Channel Assessment
 * (CCA) check.
 *
 * This event generally precedes the actual start of a CCA check by roughly
 * the \ref RAIL_StateTiming_t::idleToRx time (subject to
 * \ref RAIL_MINIMUM_TRANSITION_US). It can
 * occur multiple times based on the configuration of the ongoing CSMA or LBT
 * transmission. It can only happen after calling \ref RAIL_StartCcaCsmaTx()
 * or \ref RAIL_StartCcaLbtTx() or their scheduled equivalent.
 */
#define RAIL_EVENT_TX_START_CCA (1ULL << RAIL_EVENT_TX_START_CCA_SHIFT)

/**
 * Occurs when the radio starts transmitting a normal packet on the air.
 *
 * A start-of-transmit timestamp is captured for this event. It can be
 * retrieved by calling \ref RAIL_GetTxTimePreambleStart() passing \ref
 * RAIL_TX_STARTED_BYTES for its totalPacketBytes parameter.
 *
 * @note This event does not apply to Ack transmits. Currently there
 *   is no equivalent event or timestamp captured for the start of an
 *   Ack transmit.
 */
#define RAIL_EVENT_TX_STARTED (1ULL << RAIL_EVENT_TX_STARTED_SHIFT)

/**
 * A value to pass as \ref RAIL_GetTxTimePreambleStart() totalPacketBytes
 * parameter to retrieve the \ref RAIL_EVENT_TX_STARTED timestamp.
 */
#define RAIL_TX_STARTED_BYTES 0U

/**
 * Occurs when a scheduled TX begins turning on the transmitter.
 * This event has the same numerical value as \ref RAIL_EVENT_SCHEDULED_RX_STARTED
 * because one cannot schedule both RX and TX simultaneously.
 */
#define RAIL_EVENT_SCHEDULED_TX_STARTED (1ULL << RAIL_EVENT_SCHEDULED_TX_STARTED_SHIFT)

/**
 * Occurs when the start of a scheduled transmit is missed
 *
 * This can occur if the radio is put to sleep and not woken up with enough time
 * to configure the scheduled transmit event.
 *
 * @note Since the transmit never started, the Transmit FIFO remains intact
 *   after this event -- no packet data was consumed from it.
 */
#define RAIL_EVENT_TX_SCHEDULED_TX_MISSED (1ULL << RAIL_EVENT_TX_SCHEDULED_TX_MISSED_SHIFT)

/**
 * A mask representing all events that determine the end of a transmitted
 * packet. After a \ref RAIL_STATUS_NO_ERROR return value
 * from one of the transmit functions, exactly one of the following
 * events will occur. When one of these events occurs, a state transition
 * takes place based on the parameter passed to \ref RAIL_SetTxTransitions().
 * The RAIL_StateTransitions_t::success transition will be followed only
 * if the \ref RAIL_EVENT_TX_PACKET_SENT event occurs. Any of the other
 * events will trigger the \ref RAIL_StateTransitions_t::error transition.
 */
#define RAIL_EVENTS_TX_COMPLETION (RAIL_EVENT_TX_PACKET_SENT    \
                                   | RAIL_EVENT_TX_ABORTED      \
                                   | RAIL_EVENT_TX_BLOCKED      \
                                   | RAIL_EVENT_TX_UNDERFLOW    \
                                   | RAIL_EVENT_TX_CHANNEL_BUSY \
                                   | RAIL_EVENT_TX_SCHEDULED_TX_MISSED)

/**
 * A mask representing all events that determine the end of a transmitted
 * Ack packet. After an Ack-requesting receive, exactly one of the
 * following events will occur. When one of these events occurs, a state
 * transition takes place based on the RAIL_AutoAckConfig_t::rxTransitions
 * passed to \ref RAIL_ConfigAutoAck(). The receive transitions are used because the
 * transmitted Ack packet is considered a part of the Ack-requesting received
 * packet. The RAIL_StateTransitions_t::success transition will be followed
 * only if the \ref RAIL_EVENT_TXACK_PACKET_SENT event occurs. Any of the other
 * events will trigger the RAIL_StateTransitions_t::error transition.
 */
#define RAIL_EVENTS_TXACK_COMPLETION (RAIL_EVENT_TXACK_PACKET_SENT \
                                      | RAIL_EVENT_TXACK_ABORTED   \
                                      | RAIL_EVENT_TXACK_BLOCKED   \
                                      | RAIL_EVENT_TXACK_UNDERFLOW)

// Scheduler Event Bitmasks

/**
 * Occurs when the scheduler switches away from this configuration.
 *
 * This event will occur in dynamic multiprotocol scenarios each
 * time a protocol is shutting down. When it does occur, it will be
 * the only event passed to \ref RAIL_Config_t::eventsCallback. Therefore,
 * to optimize protocol switch time, this event should be handled
 * among the first in that callback, and then the application can return
 * immediately.
 *
 * @note: To minimize protocol switch time, Silicon Labs recommends this event
 *   event being turned off unless it is used.
 */
#define RAIL_EVENT_CONFIG_UNSCHEDULED (1ULL << RAIL_EVENT_CONFIG_UNSCHEDULED_SHIFT)

/**
 * Occurs when the scheduler switches to this configuration.
 *
 * This event will occur in dynamic multiprotocol scenarios each time
 * a protocol is starting up. When it does occur, it will
 * be the only event passed to \ref RAIL_Config_t::eventsCallback. Therefore, in
 * order to optimize protocol switch time, this event should be handled among
 * the first in that callback, and then the application can return immediately.
 *
 * @note: To minimize protocol switch time, Silicon Labs recommends this event
 *   event being turned off unless it is used.
 */
#define RAIL_EVENT_CONFIG_SCHEDULED (1ULL << RAIL_EVENT_CONFIG_SCHEDULED_SHIFT)

/**
 * Occurs when the scheduler has a status to report.
 *
 * The exact status can be found with \ref RAIL_GetSchedulerStatus().
 * See \ref RAIL_SchedulerStatus_t for more details. When this event
 * does occur, it will be the only event passed to \ref RAIL_Config_t::eventsCallback.
 * Therefore, to optimize protocol switch time, this event should
 * be handled among the first in that callback, and then the application
 * can return immediately.
 *
 * @note \ref RAIL_GetSchedulerStatus() is only valid to call during the time frame
 *   of the \ref RAIL_Config_t::eventsCallback.
 *
 * @note: To minimize protocol switch time, Silicon Labs recommends this event
 *   event being turned off unless it is used.
 */
#define RAIL_EVENT_SCHEDULER_STATUS (1ULL << RAIL_EVENT_SCHEDULER_STATUS_SHIFT)

// Other Event Bitmasks

/**
 * Occurs when the application needs to run a calibration, as
 * determined by the RAIL library.
 *
 * The application determines the opportune time to call \ref RAIL_Calibrate().
 */
#define RAIL_EVENT_CAL_NEEDED (1ULL << RAIL_EVENT_CAL_NEEDED_SHIFT)

/**
 * Occurs when RF energy is sensed from the radio. This event can be used as
 * an alternative to the callback passed as \ref RAIL_RfSense_CallbackPtr_t
 * or the application polling with \ref RAIL_IsRfSensed().
 *
 * @note This event will not occur when waking up from EM4. Prefer
 *   \ref RAIL_IsRfSensed() when waking from EM4.
 */
#define RAIL_EVENT_RF_SENSED (1ULL << RAIL_EVENT_RF_SENSED_SHIFT)

/**
 * Occurs when PA protection circuit kicks in.
 */
#define RAIL_EVENT_PA_PROTECTION (1ULL << RAIL_EVENT_PA_PROTECTION_SHIFT)

/**
 * Occurs after enabling the signal detection using \ref RAIL_BLE_EnableSignalDetection()
 * or \ref RAIL_IEEE802154_EnableSignalDetection() when a signal is detected.
 * This is only used on platforms that support signal identifier, where
 * \ref RAIL_BLE_SUPPORTS_SIGNAL_IDENTIFIER or
 * \ref RAIL_IEEE802154_SUPPORTS_SIGNAL_IDENTIFIER is true.
 */
#define RAIL_EVENT_SIGNAL_DETECTED (1ULL << RAIL_EVENT_SIGNAL_DETECTED_SHIFT)

/**
 * Occurs when a Wi-SUN mode switch packet has been received, after switching to the new PHY.
 *
 * It doesn't occur when a mode switch packet is transmitted.
 *
 * IEEE 802.15.4 option \ref RAIL_IEEE802154_G_OPTION_WISUN_MODESWITCH must be enabled for this event to occur.
 *
 * Only available on platforms where \ref RAIL_IEEE802154_SUPPORTS_G_MODESWITCH is true.
 */
#define RAIL_EVENT_IEEE802154_MODESWITCH_START (1ULL << RAIL_EVENT_IEEE802154_MODESWITCH_START_SHIFT)

/**
 * Occurs when switching back to the original base PHY in effect prior to the Wi-SUN mode switch reception.
 *
 * This typically occurs if no packet is seen within some timeframe after the mode switch packet was received
 * or if the first packet received in the new PHY is aborted, filtered, or fails CRC.
 *
 * IEEE 802.15.4 option \ref RAIL_IEEE802154_G_OPTION_WISUN_MODESWITCH must be enabled for this event to occur.
 *
 * Only available on platforms where \ref RAIL_IEEE802154_SUPPORTS_G_MODESWITCH is true.
 */
#define RAIL_EVENT_IEEE802154_MODESWITCH_END (1ULL << RAIL_EVENT_IEEE802154_MODESWITCH_END_SHIFT)

/**
 * Occurs when the sampled RSSI is above the threshold set by
 * \ref RAIL_SetRssiDetectThreshold().
 */
#define RAIL_EVENT_DETECT_RSSI_THRESHOLD (1ULL << RAIL_EVENT_DETECT_RSSI_THRESHOLD_SHIFT)

/**
 * Occurs when the thermistor has finished its measurement in response to
 * \ref RAIL_StartThermistorMeasurement().
 */
#define RAIL_EVENT_THERMISTOR_DONE (1ULL << RAIL_EVENT_THERMISTOR_DONE_SHIFT)

/**
 * Occurs when a Tx has been blocked because of temperature exceeding
 * the safety threshold.
 * @deprecated but reserved for possible future use.
 */
#define RAIL_EVENT_TX_BLOCKED_TOO_HOT (1ULL << RAIL_EVENT_TX_BLOCKED_TOO_HOT_SHIFT)

/**
 * Occurs when die internal temperature exceeds the temperature threshold subtracted
 * by the cool down parameter from \ref RAIL_ChipTempConfig_t.
 * Transmits are blocked until temperature has cooled enough, indicated by
 * \ref RAIL_EVENT_TEMPERATURE_COOL_DOWN.
 *
 * Only occurs on platforms where \ref RAIL_SUPPORTS_THERMAL_PROTECTION is true.
 */
#define RAIL_EVENT_TEMPERATURE_TOO_HOT (1ULL << RAIL_EVENT_TEMPERATURE_TOO_HOT_SHIFT)

/**
 * Occurs when die internal temperature falls below the temperature threshold subtracted
 * by the cool down parameter from \ref RAIL_ChipTempConfig_t.
 * Transmits are no longer blocked by temperature limitation, indicated by
 * \ref RAIL_EVENT_TEMPERATURE_TOO_HOT.
 *
 * Only occurs on platforms where \ref RAIL_SUPPORTS_THERMAL_PROTECTION is true.
 */
#define RAIL_EVENT_TEMPERATURE_COOL_DOWN (1ULL << RAIL_EVENT_TEMPERATURE_COOL_DOWN_SHIFT)

/**
 * Occurs when the user received a mailbox message.
 */
#define RAIL_EVENT_USER_MBOX (1ULL << RAIL_EVENT_USER_MBOX_SHIFT)

/** A value representing all possible events */
#define RAIL_EVENTS_ALL 0xFFFFFFFFFFFFFFFFULL

/** @} */ // end of group Events

/******************************************************************************
 * General Structures (part 2)
 *****************************************************************************/
/**
 * @addtogroup General
 * @{
 */

/**
 * @struct RAIL_Config_t
 * @brief RAIL configuration structure.
 */
typedef struct RAIL_Config {
  /**
   * A pointer to a function, which is called whenever a RAIL event occurs.
   *
   * @param[in] railHandle The corresponding real RAIL instance handle.
   * @param[in] events A bit mask of RAIL events.
   *
   * See the \ref RAIL_Events_t documentation for the list of RAIL events.
   */
  void (*eventsCallback)(RAIL_Handle_t railHandle, RAIL_Events_t events);
  /**
   * Provided for backwards compatibility. Ignored.
   */
  void *protocol;
  /**
   * Provided for backwards compatibility. Ignored.
   */
  RAILSched_Config_t *scheduler;
  /**
   * Provided for backwards compatibility. Ignored.
   */
  RAIL_StateBuffer_t buffer;
} RAIL_Config_t;

/** @} */ // end of group General

/******************************************************************************
 * PA Power Amplifier Structures
 *****************************************************************************/
/**
 * @addtogroup PA Power Amplifier (PA)
 * @ingroup Transmit
 * @{
 */

/**
 * The transmit power in deci-dBm units (e.g., 4.5 dBm -> 45 deci-dBm). These
 * values are used by the conversion functions to convert a \ref
 * RAIL_TxPowerLevel_t to deci-dBm for the application consumption. On EFR32,
 * they can range from \ref RAIL_TX_POWER_MIN to \ref RAIL_TX_POWER_MAX.
 */
typedef int16_t RAIL_TxPower_t;

/** The maximum valid value for a \ref RAIL_TxPower_t. */
#define RAIL_TX_POWER_MAX ((RAIL_TxPower_t)0x7FFF)
/** The minimum valid value for a \ref RAIL_TxPower_t. */
#define RAIL_TX_POWER_MIN ((RAIL_TxPower_t)0x8000)

/** The maximum power in deci-dBm the curve supports */
#define RAIL_TX_POWER_CURVE_DEFAULT_MAX ((RAIL_TxPower_t)200)
/** The increment step in deci-dBm for calculating power level*/
#define RAIL_TX_POWER_CURVE_DEFAULT_INCREMENT ((RAIL_TxPower_t)40)

/// mV are used for all TX power voltage values.
/// TX power voltages take and return voltages multiplied by this factor.
#define RAIL_TX_POWER_VOLTAGE_SCALING_FACTOR 1000

/// deci-dBm are used for all TX power dBm values.
/// All dBm inputs to TX power functions take dBm power times this factor.
#define RAIL_TX_POWER_DBM_SCALING_FACTOR 10

/**
 * Raw power levels used directly by \ref RAIL_GetTxPower() and \ref RAIL_SetTxPower() where a higher
 * numerical value corresponds to a higher output power. These are referred to
 * as 'raw (values/units)'. On EFR32, they can range from one of \ref
 * RAIL_TX_POWER_LEVEL_2P4_LP_MIN, \ref RAIL_TX_POWER_LEVEL_2P4_HP_MIN, or
 * \ref RAIL_TX_POWER_LEVEL_SUBGIG_HP_MIN to one of \ref
 * RAIL_TX_POWER_LEVEL_2P4_LP_MAX, \ref RAIL_TX_POWER_LEVEL_2P4_HP_MAX, and \ref
 * RAIL_TX_POWER_LEVEL_SUBGIG_HP_MAX, respectively, depending on the selected \ref
 * RAIL_TxPowerMode_t.
 */
typedef uint8_t RAIL_TxPowerLevel_t;

/**
 * Invalid \ref RAIL_TxPowerLevel_t value returned when an error occurs
 * with \ref RAIL_GetTxPower().
 */
#define RAIL_TX_POWER_LEVEL_INVALID (255U)

/**
 * Sentinel value that can be passed to \ref RAIL_SetTxPower() to set
 * the highest power level available on the current PA, regardless
 * of which one is selected.
 */
#define RAIL_TX_POWER_LEVEL_MAX (254U)

/**
 * PA power setting used directly by \ref RAIL_GetPaPowerSetting() and
 * \ref RAIL_SetPaPowerSetting() which is decoded to the actual
 * hardware register value(s).
 */
typedef uint32_t RAIL_PaPowerSetting_t;

/**
 * Returned by \ref RAIL_GetPaPowerSetting() when the device does
 * not support the dBm to power setting mapping table.
 */
#define RAIL_TX_PA_POWER_SETTING_UNSUPPORTED     (0U)

/**
 * @struct RAIL_TxPowerSettingEntry_t
 *
 * @brief A structure containing power-setting information for a deci-dBm power.
 */
typedef struct RAIL_TxPowerSettingEntry {
  /** PowerSetting information corresponds to currentPaPowerDdbm*/
  RAIL_PaPowerSetting_t paPowerSetting;
  /** Minimum power (in deci-dBm) supported by powersetting table in use */
  RAIL_TxPower_t minPaPowerDdbm;
  /** Maximum power (in deci-dBm) supported by powersetting table in use */
  RAIL_TxPower_t maxPaPowerDdbm;
  /** Current power (in deci-dBm) */
  RAIL_TxPower_t currentPaPowerDdbm;
} RAIL_TxPowerSettingEntry_t;

/**
 * @enum RAIL_TxPowerMode_t
 * @brief An enumeration of the EFR32 power modes.
 *
 * The power modes on the EFR32 correspond to the different on-chip PAs that
 * are available. For more information about the power and performance
 * characteristics of a given amplifier, see the data sheet.
 */
RAIL_ENUM(RAIL_TxPowerMode_t) {
  /**
   *  High-power 2.4 GHz amplifier
   *  EFR32xG21: up to 20 dBm, raw values: 1-180
   *  EFR32xG22: up to  6 dBm, raw values: 1-128
   *  EFR32xG24: up to 20 dBm, raw values: 0-180, or
   *             up to 10 dBm, raw values: 0-90
   *  EFR32xG26: same as EFR32xG24
   *  EFR32xG27: up to  6 dBm, raw values: 1-128
   *  EFR32xG28: up to 10 dBm, raw values: 0-240
   *  Not supported on other platforms.
   */
  RAIL_TX_POWER_MODE_2P4GIG_HP = 0U,
  /** @deprecated Please use \ref RAIL_TX_POWER_MODE_2P4GIG_HP instead. */
  RAIL_TX_POWER_MODE_2P4_HP = RAIL_TX_POWER_MODE_2P4GIG_HP,
  /**
   *  Mid-power 2.4 GHz amplifier
   *  EFR32xG21: up to 10 dBm, raw values: 1-90
   *  Not supported on other platforms.
   */
  RAIL_TX_POWER_MODE_2P4GIG_MP = 1U,
  /** @deprecated Please use \ref RAIL_TX_POWER_MODE_2P4GIG_MP instead. */
  RAIL_TX_POWER_MODE_2P4_MP = RAIL_TX_POWER_MODE_2P4GIG_MP,
  /**
   *  Low-power 2.4 GHz amplifier
   *  EFR32xG21: up to 0 dBm, raw values: 1-64
   *  EFR32xG22: up to 0 dBm, raw values: 1-16
   *  EFR32xG24: up to 0 dBm, raw values: 1-16
   *  EFR32xG26: same as EFR32xG24
   *  EFR32xG27: up to 0 dBm, raw values: 1-16
   *  Not supported on other platforms.
   */
  RAIL_TX_POWER_MODE_2P4GIG_LP = 2U,
  /** @deprecated Please use \ref RAIL_TX_POWER_MODE_2P4GIG_LP instead. */
  RAIL_TX_POWER_MODE_2P4_LP = RAIL_TX_POWER_MODE_2P4GIG_LP,
  /**
   *  Low-Low-power 2.4 GHz amplifier
   *  Not currently supported on any EFR32 platform.
   */
  RAIL_TX_POWER_MODE_2P4GIG_LLP = 3U,
  /**
   *  Select the highest 2.4 GHz power PA available on the current chip.
   */
  RAIL_TX_POWER_MODE_2P4GIG_HIGHEST = 4U,
  /** @deprecated Please use \ref RAIL_TX_POWER_MODE_2P4GIG_HIGHEST instead. */
  RAIL_TX_POWER_MODE_2P4_HIGHEST = RAIL_TX_POWER_MODE_2P4GIG_HIGHEST,
  /**
   *  PA for all Sub-GHz dBm values in range, using \ref
   *  RAIL_PaPowerSetting_t table.
   *  Only supported on platforms with \ref
   *  RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE (e.g., EFR32xG25).
   */
  RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE = 5U,
  /**
   *  High-power Sub-GHz amplifier (Class D mode)
   *  Supported on EFR32xG23 and EFR32xG28.
   *  Not supported other Sub-GHz-incapable platforms or those with \ref
   *  RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE.
   */
  RAIL_TX_POWER_MODE_SUBGIG_HP = 6U,
  /** @deprecated Please use \ref RAIL_TX_POWER_MODE_SUBGIG_HP instead. */
  RAIL_TX_POWER_MODE_SUBGIG = RAIL_TX_POWER_MODE_SUBGIG_HP,
  /**
   *  Mid-power Sub-GHz amplifier
   *  Supported only on EFR32xG23 and EFR32xG28.
   *  Not supported other Sub-GHz-incapable platforms or those with \ref
   *  RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE.
   */
  RAIL_TX_POWER_MODE_SUBGIG_MP = 7U,
  /**
   *  Low-power Sub-GHz amplifier
   *  Supported only on EFR32xG23 and EFR32xG28.
   *  Not supported other Sub-GHz-incapable platforms or those with \ref
   *  RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE.
   */
  RAIL_TX_POWER_MODE_SUBGIG_LP = 8U,
  /**
   *  Low-Low-power Sub-GHz amplifier
   *  Supported only on EFR32xG23 and EFR32xG28.
   *  Not supported other Sub-GHz-incapable platforms or those with \ref
   *  RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE.
   */
  RAIL_TX_POWER_MODE_SUBGIG_LLP = 9U,
  /**
   *  Select the highest Sub-GHz power PA available on the current chip.
   */
  RAIL_TX_POWER_MODE_SUBGIG_HIGHEST = 10U,
  /**
   *  PA for all OFDM Sub-GHz dBm values in range, using \ref
   *  RAIL_PaPowerSetting_t table.
   *  Supported only on platforms with both \ref
   *  RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE and \ref
   *  RAIL_SUPPORTS_OFDM_PA (e.g., EFR32xG25).
   */
  RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE = 11U,
  /** @deprecated Please use \ref RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE instead. */
  RAIL_TX_POWER_MODE_OFDM_PA = RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE,
  /** Invalid amplifier Selection. Must be last. */
  RAIL_TX_POWER_MODE_NONE
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
// Refer to rail_chip_specific.h for per-platform defines of supported ones.
#define RAIL_TX_POWER_MODE_NONE ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_NONE)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_TX_POWER_MODE_NAMES
 * @brief The names of the TX power modes.
 *
 * A list of the names for the TX power modes on EFR32 parts. This
 * macro is useful for test applications and debugging output.
 */
#define RAIL_TX_POWER_MODE_NAMES {                   \
    "RAIL_TX_POWER_MODE_2P4GIG_HP",                  \
    "RAIL_TX_POWER_MODE_2P4GIG_MP",                  \
    "RAIL_TX_POWER_MODE_2P4GIG_LP",                  \
    "RAIL_TX_POWER_MODE_2P4GIG_LLP",                 \
    "RAIL_TX_POWER_MODE_2P4GIG_HIGHEST",             \
    "RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE",  \
    "RAIL_TX_POWER_MODE_SUBGIG_HP",                  \
    "RAIL_TX_POWER_MODE_SUBGIG_MP",                  \
    "RAIL_TX_POWER_MODE_SUBGIG_LP",                  \
    "RAIL_TX_POWER_MODE_SUBGIG_LLP",                 \
    "RAIL_TX_POWER_MODE_SUBGIG_HIGHEST",             \
    "RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE", \
    "RAIL_TX_POWER_MODE_NONE"                        \
}

/**
 * @struct RAIL_TxPowerConfig_t
 *
 * @brief A structure containing values used to initialize the power amplifiers.
 */
typedef struct RAIL_TxPowerConfig {
  /** TX power mode */
  RAIL_TxPowerMode_t mode;
  /**
   * Power amplifier supply voltage in mV, generally:
   * DCDC supply ~ 1800 mV (1.8 V)
   * Battery supply ~ 3300 mV (3.3 V)
   */
  uint16_t voltage;
  /** The amount of time to spend ramping for TX in microseconds. */
  uint16_t rampTime;
} RAIL_TxPowerConfig_t;

/** Convenience macro for any OFDM mapping table mode. */
#define RAIL_POWER_MODE_IS_DBM_POWERSETTING_MAPPING_TABLE_OFDM(x) \
  ((x) == RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE)
/** Convenience macro for any Sub-GHz mapping table mode. */
#define RAIL_POWER_MODE_IS_DBM_POWERSETTING_MAPPING_TABLE_SUBGIG(x) \
  ((x) == RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE)
/** Convenience macro for any OFDM mode. */
#define RAIL_POWER_MODE_IS_ANY_OFDM(x) \
  RAIL_POWER_MODE_IS_DBM_POWERSETTING_MAPPING_TABLE_OFDM(x)

/** @} */ // PA Power Amplifier (PA)

/******************************************************************************
 * Radio Configuration Structures
 *****************************************************************************/
/**
 * @addtogroup Radio_Configuration
 * @{
 */

/**
 * @brief Pointer to a radio configuration array.
 *
 * The radio configuration properly configures the
 * radio for operation on a protocol. These configurations are very
 * chip-specific should not be created or edited by hand.
 */
typedef const uint32_t *RAIL_RadioConfig_t;

/**
 * @struct RAIL_FrameType_t
 * @brief Configures if there is a frame type in your frame and the lengths of
 *   each frame. The number of bits set in the mask determines the number of
 *   elements in frameLen. A maximum of 8 different frame types may be specified.
 */
typedef struct RAIL_FrameType {
  /**
   * A pointer to an array of frame byte lengths for each frame type.
   * The number of elements in this array should be equal to the number of
   * frame types. The memory to which frameLen points should not
   * change location or be modified.
   */
  uint16_t *frameLen;
  /**
   * Zero-indexed byte offset location of the byte containing the frame type field.
   */
  uint8_t offset;
  /**
   * A bitmask of the frame type field, which determines a number of frames expected
   * based on the number of bits set. No more than 3 bits can be set in the mask and
   * they must be contiguous ones. For example, if the highest three bits of the byte
   * specified by offset constitute the frame type, then mask should be 0xE0,
   * which has 3 bits set, indicating 8 possible frame types.
   */
  uint8_t mask;
  /**
   * A bitmask that marks if each frame is valid or should be filtered. Frame type
   * 0 corresponds to the lowest bit in isValid. If the frame is filtered, a
   * \ref RAIL_EVENT_RX_PACKET_ABORTED will be raised.
   */
  uint8_t isValid;
  /**
   * A bitmask that marks if each frame should have the address filter applied.
   * Frame type 0 corresponds to the least significant bit in addressFilter.
   */
  uint8_t addressFilter;
} RAIL_FrameType_t;

/**
 * @def RAIL_SETFIXEDLENGTH_INVALID
 * @brief An invalid return value when calling \ref RAIL_SetFixedLength().
 *
 * An invalid return value when calling \ref RAIL_SetFixedLength() while the radio is
 * not in fixed-length mode.
 */
#define RAIL_SETFIXEDLENGTH_INVALID (0xFFFFU)

/**
 * @struct RAIL_ChannelConfigEntryAttr_t
 * @brief A channel configuration entry attribute structure. Items listed
 *   are designed to be altered and updated during run-time.
 */
typedef struct RAIL_ChannelConfigEntryAttr RAIL_ChannelConfigEntryAttr_t;

/**
 * @enum RAIL_ChannelConfigEntryType_t
 * @brief Define if the channel support using concurrent PHY during channel
 *   hopping. \ref RAIL_RX_CHANNEL_HOPPING_MODE_CONC and \ref RAIL_RX_CHANNEL_HOPPING_MODE_VT
 *   can only be used if the channel supports it.
 */
RAIL_ENUM(RAIL_ChannelConfigEntryType_t) {
  /** Not a concurrent PHY. */
  RAIL_CH_TYPE_NORMAL,
  /** Base concurrent PHY. */
  RAIL_CH_TYPE_CONC_BASE,
  /** Virtual concurrent PHY. */
  RAIL_CH_TYPE_CONC_VIRTUAL,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_CH_TYPE_NORMAL       ((RAIL_ChannelConfigEntryType_t) RAIL_CH_TYPE_NORMAL)
#define RAIL_CH_TYPE_CONC_BASE    ((RAIL_ChannelConfigEntryType_t) RAIL_CH_TYPE_CONC_BASE)
#define RAIL_CH_TYPE_CONC_VIRTUAL ((RAIL_ChannelConfigEntryType_t) RAIL_CH_TYPE_CONC_VIRTUAL)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RADIO_CONFIG_ENABLE_CONC_PHY
 * @brief Indicates this version of RAIL supports concurrent PHY information in
 *   radio configurator output. Needed for backwards compatibility.
 */
#define RADIO_CONFIG_ENABLE_CONC_PHY 1

/**
 * @def RADIO_CONFIG_ENABLE_STACK_INFO
 * @brief Indicates this version of RAIL supports stack info feature in
 *   radio configurator output. Needed for backwards compatibility.
 */
#define RADIO_CONFIG_ENABLE_STACK_INFO

/**
 * @struct RAIL_AlternatePhy_t
 * @brief Alternate PHY configuration entry structure, which gathers some info
 *   on the alternate PHY in the context of concurrent mode.
 */
typedef struct RAIL_AlternatePhy {
  /** A base frequency in Hz of this channel set. */
  uint32_t baseFrequency;
  /** A channel spacing in Hz of this channel set. */
  uint32_t channelSpacing;
  /** The number of channels (and not the channel number !) */
  uint16_t numberOfChannels;
  /** minimum IF for the alternate PHY in kHz. */
  uint16_t minIf_kHz;
  /** minimum IF for the base PHY in kHz. */
  uint16_t minBaseIf_kHz;
  /** Indicates that OFDM modem is used by this alternate PHY. */
  bool isOfdmModem;
  /** Rate info of the alternate PHY. */
  uint32_t rateInfo;
  /** Used to adjust the AGC for CCA between hard and soft modems. */
  uint32_t hwModemAgcCtrl1;
} RAIL_AlternatePhy_t;

/**
 * @struct RAIL_ChannelConfigEntry_t
 * @brief A channel configuration entry structure, which defines a channel range
 *   and parameters across which a corresponding radio configuration is valid.
 *
 * operating frequency hz = baseFrequency
 *   + channelSpacing * (channel - physicalChannelOffset);
 */
typedef struct RAIL_ChannelConfigEntry {
  /**
   * The minimum radio configuration to apply to the base
   * configuration for this channel set.
   */
  RAIL_RadioConfig_t phyConfigDeltaAdd;
  /** A base frequency in Hz of this channel set. */
  uint32_t baseFrequency;
  /** A channel spacing in Hz of this channel set. */
  uint32_t channelSpacing;
  /**
   * The offset to subtract from the logical
   * channel to align them with the zero based physical channels which are
   * relative to baseFrequency. (i.e., By default ch 0 = base freq, but if
   * offset = 11, ch 11 = base freq.)
   */
  uint16_t physicalChannelOffset;
  /** The first valid RAIL channel number for this channel set. */
  uint16_t channelNumberStart;
  /** The last valid RAIL channel number for this channel set. */
  uint16_t channelNumberEnd;
  /** The maximum power allowed in this channel set. */
  RAIL_TxPower_t maxPower;
  /**
   * A pointer to a structure containing attributes specific to this
   * channel set.
   */
  RAIL_ChannelConfigEntryAttr_t *attr;
  /** Indicates channel config type. */
  RAIL_ChannelConfigEntryType_t entryType;
  /** to align to 32-bit boundary. */
  uint8_t reserved[3];
  /**
   * Array containing information according to the \ref RAIL_PtiProtocol_t in
   * the first byte of this array. The first 2 fields are common to all
   * protocols and accessible by RAIL, others are ignored by RAIL
   * and only used by the application. Common fields are listed in
   * \ref RAIL_StackInfoCommon_t.
   */
  const uint8_t *stackInfo;
  /** Pointer to alternate PHY. */
  RAIL_AlternatePhy_t *alternatePhy;
} RAIL_ChannelConfigEntry_t;

/// @struct RAIL_ChannelConfig_t
/// @brief A channel configuration structure, which defines the channel meaning
///   when a channel number is passed into a RAIL function, e.g., \ref RAIL_StartTx()
///   and \ref RAIL_StartRx().
///
/// A \ref RAIL_ChannelConfig_t structure defines the channel scheme that an
/// application uses when registered in \ref RAIL_ConfigChannels().
///
/// These are a few examples of different channel configurations:
/// @code{.c}
/// // 21 channels starting at 2.45 GHz with channel spacing of 1 MHz
/// // ... generated by Simplicity Studio (i.e., rail_config.c) ...
/// const uint32_t generated[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL, // Add this to default configuration for this entry
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr,
///   },
/// };
/// const RAIL_ChannelConfig_t generated_channelConfig = {
///   .phyConfigBase = generated, // Default radio configuration for all entries
///   .phyConfigDeltaSubtract = NULL, // Subtract this to restore the default configuration
///   .configs = generated_channels,
///   .length = 1, // There are this many channel configuration entries
/// };
/// const RAIL_ChannelConfig_t *channelConfigs[] = {
///   &generated_channelConfig,
///   NULL,
/// };
/// // ... in main code ...
/// // Associate a specific channel configuration with a particular RAIL instance.
/// RAIL_ConfigChannels(railHandle, channelConfigs[0]);
///
/// // 4 nonlinear channels
/// // ... in rail_config.c ...
/// const uint32_t generated[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL, // Add this to default configuration for this entry
///     .baseFrequency = 910123456,
///     .channelSpacing = 0,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 0,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr,
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 911654789,
///     .channelSpacing = 0,
///     .physicalChannelOffset = 0, // Since ch spacing = 0, offset can be 0
///     .channelNumberStart = 1,
///     .channelNumberEnd = 1,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr,
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 912321456,
///     .channelSpacing = 100000,
///     .physicalChannelOffset = 2, // Since ch spacing != 0, offset = 2
///     .channelNumberStart = 2, // ch 2 = baseFrequency
///     .channelNumberEnd = 2,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr,
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 913147852,
///     .channelSpacing = 0,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 3,
///     .channelNumberEnd = 3,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr,
///   },
/// };
/// const RAIL_ChannelConfig_t generated_channelConfig = {
///   .phyConfigBase = generated, // Default radio configuration for all entries
///   .phyConfigDeltaSubtract = NULL, // Subtract this to restore the default configuration
///   .configs = generated_channels,
///   .length = 4, // There are this many channel configuration entries
/// };
/// const RAIL_ChannelConfig_t *channelConfigs[] = {
///   &generated_channelConfig,
///   NULL,
/// };
/// // ... in main code ...
/// // Associate a specific channel configuration with a particular RAIL instance.
/// RAIL_ConfigChannels(railHandle, channelConfigs[0]);
///
/// // Multiple radio configurations
/// // ... in rail_config.c ...
/// const uint32_t generated0[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated0_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated0_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL, // Add this to the default configuration for this entry
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated0_entryAttr,
///   },
/// };
/// const RAIL_ChannelConfig_t generated0_channelConfig = {
///   .phyConfigBase = generated0, // Default radio configuration for all entries
///   .phyConfigDeltaSubtract = NULL, // Subtract this to restore default configuration
///   .configs = generated0_channels,
///   .length = 1, // There are this many channel configuration entries
/// };
/// const uint32_t generated1[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated1_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated1_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = -100, // Use this entry when TX power <= -10dBm
///     .attr = &generated1_entryAttr,
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = 15, // Use this entry when TX power > -10dBm
///                     // and TX power <= 1.5dBm
///     .attr = &generated1_entryAttr,
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX, // Use this entry when TX power > 1.5dBm
///     .attr = &generated1_entryAttr,
///   },
/// };
/// const RAIL_ChannelConfig_t generated1_channelConfig = {
///   .phyConfigBase = generated1,
///   .phyConfigDeltaSubtract = NULL,
///   .configs = generated1_channels,
///   .length = 3,
/// };
/// const uint32_t generated2[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated2_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated2_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated2_entryAttr,
///   },
/// };
/// const RAIL_ChannelConfig_t generated2_channelConfig = {
///   .phyConfigBase = generated2,
///   .phyConfigDeltaSubtract = NULL,
///   .configs = generated2_channels,
///   .length = 1,
/// };
/// const RAIL_ChannelConfig_t *channelConfigs[] = {
///   &generated0_channelConfig,
///   &generated1_channelConfig,
///   &generated2_channelConfig,
///   NULL,
/// };
/// // ... in main code ...
/// // Create a unique RAIL handle for each unique channel configuration.
/// railHandle0 = RAIL_Init(&railCfg0, &RAILCb_RfReady0);
/// railHandle1 = RAIL_Init(&railCfg1, &RAILCb_RfReady1);
/// railHandle2 = RAIL_Init(&railCfg2, &RAILCb_RfReady2);
/// // Associate each channel configuration with its corresponding RAIL handle.
/// RAIL_ConfigChannels(railHandle0, channelConfigs[0]);
/// RAIL_ConfigChannels(railHandle1, channelConfigs[1]);
/// RAIL_ConfigChannels(railHandle2, channelConfigs[2]);
/// // Use a RAIL handle and channel to access the desired channel configuration entry.
/// RAIL_SetTxPowerDbm(railHandle1, 100); // set 10.0 dBm TX power
/// RAIL_StartRx(railHandle1, 0, &schedInfo); // RX using generated1_channels[2]
/// RAIL_SetTxPowerDbm(railHandle1, 0); // set 0 dBm TX power
/// RAIL_StartRx(railHandle1, 0, &schedInfo); // RX using generated1_channels[1]
/// RAIL_StartRx(railHandle2, 0, &schedInfo); // RX using generated2_channels[0]
/// @endcode
///
typedef struct RAIL_ChannelConfig {
  /**
   * Base radio configuration for the corresponding
   * channel configuration entries.
   */
  RAIL_RadioConfig_t phyConfigBase;
  /**
   * Minimum radio configuration to restore
   * channel entries back to base configuration.
   */
  RAIL_RadioConfig_t phyConfigDeltaSubtract;
  /** Pointer to an array of \ref RAIL_ChannelConfigEntry_t entries. */
  const RAIL_ChannelConfigEntry_t *configs;
  /** Number of \ref RAIL_ChannelConfigEntry_t entries. */
  uint32_t length;
  /** Signature for this structure. Only used on modules. */
  uint32_t signature;
  /** Crystal Frequency for the channel config. */
  uint32_t xtalFrequencyHz;
} RAIL_ChannelConfig_t;

/**
 * @struct RAIL_ChannelMetadata_t
 * @brief Container for individual channel metadata.
 */
typedef struct RAIL_ChannelMetadata {
  /** Channel number */
  uint16_t channel;
  /** Word alignment */
  uint16_t reserved;
  /** Channel frequency, in Hz */
  uint32_t frequency;
} RAIL_ChannelMetadata_t;

/**
 * @struct RAIL_StackInfoCommon_t
 * @brief Stack info fields common to all protocols.
 */
typedef struct RAIL_StackInfoCommon {
  /** Same as \ref RAIL_PtiProtocol_t. */
  uint8_t protocolId;
  /** PHY Id depending on the protocolId value. */
  uint8_t phyId;
} RAIL_StackInfoCommon_t;

/**
 * @typedef RAIL_RadioConfigChangedCallback_t
 * @brief A pointer to a function called whenever a radio configuration change occurs.
 *
 * @param[in] railHandle The corresponding RAIL instance handle.
 * @param[in] entry A pointer to the radio configuration entry being changed to.
 */
typedef void (*RAIL_RadioConfigChangedCallback_t)(RAIL_Handle_t railHandle,
                                                  const RAIL_ChannelConfigEntry_t *entry);

/** @} */ // end of group Radio_Configuration

/******************************************************************************
 * Packet Trace Interface (PTI) Structures
 *****************************************************************************/
/**
 * @addtogroup PTI PTI Packet Trace
 * @{
 *
 * These enumerations and structures are used with RAIL PTI API. EFR32 supports
 * SPI and UART PTI and is configurable in terms of baud rates and PTI
 * pin locations.
 */

/** A channel type enumeration. */
RAIL_ENUM(RAIL_PtiMode_t) {
  /** Turn PTI off entirely. */
  RAIL_PTI_MODE_DISABLED = 0,
  /** 8-bit SPI mode. */
  RAIL_PTI_MODE_SPI = 1,
  /** 8-bit UART mode. */
  RAIL_PTI_MODE_UART = 2,
  /** 9-bit UART mode. */
  RAIL_PTI_MODE_UART_ONEWIRE = 3,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_PTI_MODE_DISABLED     ((RAIL_PtiMode_t) RAIL_PTI_MODE_DISABLED)
#define RAIL_PTI_MODE_SPI          ((RAIL_PtiMode_t) RAIL_PTI_MODE_SPI)
#define RAIL_PTI_MODE_UART         ((RAIL_PtiMode_t) RAIL_PTI_MODE_UART)
#define RAIL_PTI_MODE_UART_ONEWIRE ((RAIL_PtiMode_t) RAIL_PTI_MODE_UART_ONEWIRE)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_PtiConfig_t
 * @brief A configuration for PTI.
 */
typedef struct RAIL_PtiConfig {
  /** Packet Trace mode (UART or SPI). */
  RAIL_PtiMode_t mode;
  /** Output baudrate for PTI in Hz. */
  uint32_t baud;
  /** @deprecated No longer used (ignored). */
  uint8_t doutLoc;
  /** Data output (DOUT) GPIO port. */
  uint8_t doutPort;
  /** Data output (DOUT) GPIO pin. */
  uint8_t doutPin;
  /** @deprecated No longer used (ignored). */
  uint8_t dclkLoc;
  /** Data clock (DCLK) GPIO port. Only used in SPI mode. */
  uint8_t dclkPort;
  /** Data clock (DCLK) GPIO pin. Only used in SPI mode. */
  uint8_t dclkPin;
  /** @deprecated No longer used (ignored). */
  uint8_t dframeLoc;
  /** Data frame (DFRAME) GPIO port. */
  uint8_t dframePort;
  /** Data frame (DFRAME) GPIO pin. */
  uint8_t dframePin;
} RAIL_PtiConfig_t;

/**
 * @enum RAIL_PtiProtocol_t
 * @brief The protocol that RAIL outputs via the Packet Trace Interface (PTI).
 */
RAIL_ENUM(RAIL_PtiProtocol_t) {
  /** PTI output for a custom protocol. */
  RAIL_PTI_PROTOCOL_CUSTOM = 0,
  /** PTI output for the Thread protocol. */
  RAIL_PTI_PROTOCOL_THREAD = 2,
  /** PTI output for the Bluetooth Smart protocol. */
  RAIL_PTI_PROTOCOL_BLE = 3,
  /** PTI output for the Connect protocol. */
  RAIL_PTI_PROTOCOL_CONNECT = 4,
  /** PTI output for the Zigbee protocol. */
  RAIL_PTI_PROTOCOL_ZIGBEE = 5,
  /** PTI output for the Z-Wave protocol. */
  RAIL_PTI_PROTOCOL_ZWAVE = 6,
  /** PTI output for the Wi-SUN protocol. */
  RAIL_PTI_PROTOCOL_WISUN = 7,
  /** PTI output for a custom protocol using a built-in 802.15.4 radio config. */
  RAIL_PTI_PROTOCOL_802154 = 8,
  /** PTI output for Sidewalk protocol. */
  RAIL_PTI_PROTOCOL_SIDEWALK = 9,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_PTI_PROTOCOL_CUSTOM   ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_CUSTOM)
#define RAIL_PTI_PROTOCOL_THREAD   ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_THREAD)
#define RAIL_PTI_PROTOCOL_BLE      ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_BLE)
#define RAIL_PTI_PROTOCOL_CONNECT  ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_CONNECT)
#define RAIL_PTI_PROTOCOL_ZIGBEE   ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_ZIGBEE)
#define RAIL_PTI_PROTOCOL_ZWAVE    ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_ZWAVE)
#define RAIL_PTI_PROTOCOL_802154   ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_802154)
#define RAIL_PTI_PROTOCOL_SIDEWALK ((RAIL_PtiProtocol_t) RAIL_PTI_PROTOCOL_SIDEWALK)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group PTI

/******************************************************************************
 * Data Management Structures
 *****************************************************************************/
/**
 * @addtogroup Data_Management
 * @{
 */

/// Fixed-width type indicating the needed alignment for RX and TX FIFOs.
#define RAIL_FIFO_ALIGNMENT_TYPE uint32_t

/// Alignment that is needed for the RX and TX FIFOs.
#define RAIL_FIFO_ALIGNMENT (sizeof(RAIL_FIFO_ALIGNMENT_TYPE))

/**
 * @enum RAIL_TxDataSource_t
 * @brief Transmit data sources supported by RAIL.
 */
RAIL_ENUM(RAIL_TxDataSource_t) {
  /** Uses the frame hardware to packetize data. */
  TX_PACKET_DATA = 0,
  /**
   * Uses the multi-level frequency modulation data.
   * @note This is only supported on devices where \ref RAIL_SUPPORTS_MFM
   *   or \ref RAIL_SupportsMfm() are true.
   * @note This feature cannot be used with built-in protocols (802.15.4, BLE,
   *   Z-Wave).
   */
  TX_MFM_DATA = 1,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_TX_DATA_SOURCE_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define TX_PACKET_DATA            ((RAIL_TxDataSource_t) TX_PACKET_DATA)
#define TX_MFM_DATA               ((RAIL_TxDataSource_t) TX_MFM_DATA)
#define RAIL_TX_DATA_SOURCE_COUNT ((RAIL_TxDataSource_t) RAIL_TX_DATA_SOURCE_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @enum RAIL_RxDataSource_t
 * @brief Receive data sources supported by RAIL.
 *
 * @note Data sources other than \ref RX_PACKET_DATA require use of
 *   \ref RAIL_DataMethod_t::FIFO_MODE.
 */
RAIL_ENUM(RAIL_RxDataSource_t) {
  /** Uses the frame hardware to packetize data. */
  RX_PACKET_DATA = 0,
  /** Gets 8-bit data output from the demodulator. */
  RX_DEMOD_DATA = 1,
  /** Gets lower 16 bits of I/Q data provided to the demodulator. */
  RX_IQDATA_FILTLSB = 2,
  /** Gets highest 16 bits of I/Q data provided to the demodulator. */
  RX_IQDATA_FILTMSB = 3,
  /**
   * Gets RX direct mode data output from the demodulator.
   * BCRDEMOD captures bcr_dmod_rawd at the sample rate
   * (faster than the bit rate by the OSR), specifically
   * the demod_samp_rate trigger.
   * Only supported if \ref RAIL_SUPPORTS_RX_DIRECT_MODE_DATA_TO_FIFO is true.
   */
  RX_DIRECT_MODE_DATA = 4,
  /**
   * Gets synchronous RX direct mode data output from the demodulator.
   * BCRDEMOD_SYNCHRONOUS captures bcr_dmod_rxd_ext at the bit
   * rate (bcr_dmod_bitclk_ext trigger).
   * Only supported if \ref RAIL_SUPPORTS_RX_DIRECT_MODE_DATA_TO_FIFO is true.
   * Only efr32xg23, efr32xg25, or efr32xg28 have this mode.
   */
  RX_DIRECT_SYNCHRONOUS_MODE_DATA = 5,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_RX_DATA_SOURCE_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RX_PACKET_DATA            ((RAIL_RxDataSource_t) RX_PACKET_DATA)
#define RX_DEMOD_DATA             ((RAIL_RxDataSource_t) RX_DEMOD_DATA)
#define RX_IQDATA_FILTLSB         ((RAIL_RxDataSource_t) RX_IQDATA_FILTLSB)
#define RX_IQDATA_FILTMSB         ((RAIL_RxDataSource_t) RX_IQDATA_FILTMSB)
#define RX_DIRECT_MODE_DATA       ((RAIL_RxDataSource_t) RX_DIRECT_MODE_DATA)
#define RX_DIRECT_SYNCHRONOUS_MODE_DATA ((RAIL_RxDataSource_t) RX_DIRECT_SYNCHRONOUS_MODE_DATA)
#define RAIL_RX_DATA_SOURCE_COUNT ((RAIL_RxDataSource_t) RAIL_RX_DATA_SOURCE_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @enum RAIL_DataMethod_t
 * @brief Methods for the application to provide and retrieve data from RAIL.
 *
 * For Transmit the distinction between \ref RAIL_DataMethod_t::PACKET_MODE
 * and \ref RAIL_DataMethod_t::FIFO_MODE has become more cosmetic than
 * functional, as the \ref RAIL_WriteTxFifo() and \ref RAIL_SetTxFifoThreshold() APIs
 * and related \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY event can be used in
 * either mode. For Receive the distinction is functionally important because
 * in \ref RAIL_DataMethod_t::PACKET_MODE rollback occurs automatically for
 * unsuccessfully-received packets (\ref RAIL_RxPacketStatus_t ABORT statuses),
 * flushing their data. In \ref RAIL_DataMethod_t::FIFO_MODE rollback is
 * prevented, leaving the data from unsuccessfully-received packets in the
 * receive FIFO for the application to deal with. This allows for packets
 * larger than the receive FIFO size where automatic rollback would corrupt
 * the receive FIFO.
 */
RAIL_ENUM(RAIL_DataMethod_t) {
  /** Packet-based data method. */
  PACKET_MODE = 0,
  /** FIFO-based data method. */
  FIFO_MODE = 1,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_DATA_METHOD_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define PACKET_MODE            ((RAIL_DataMethod_t) PACKET_MODE)
#define FIFO_MODE              ((RAIL_DataMethod_t) FIFO_MODE)
#define RAIL_DATA_METHOD_COUNT ((RAIL_DataMethod_t) RAIL_DATA_METHOD_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_FIFO_THRESHOLD_DISABLED
 * @brief A FIFO threshold value that disables the threshold.
 */
#define RAIL_FIFO_THRESHOLD_DISABLED 0xFFFFU

/**
 * @struct RAIL_DataConfig_t
 * @brief RAIL data configuration structure
 *
 * Select the transmit/receive data sources and the
 * method the application uses to provide/retrieve data from RAIL.
 */
typedef struct {
  /** Source of TX Data. */
  RAIL_TxDataSource_t txSource;
  /** Source of RX Data. */
  RAIL_RxDataSource_t rxSource;
  /** Method of providing transmit data. */
  RAIL_DataMethod_t txMethod;
  /** Method of retrieving receive data. */
  RAIL_DataMethod_t rxMethod;
} RAIL_DataConfig_t;

/** @} */ // end of group Data Management

/******************************************************************************
 * State Transitions
 *****************************************************************************/
/**
 * @addtogroup State_Transitions
 * @{
 */

/**
 * @typedef RAIL_TransitionTime_t
 * @brief Suitable type for the supported transition time range.
 *
 * Refer to platform-specific \ref RAIL_MINIMUM_TRANSITION_US and
 * \ref RAIL_MAXIMUM_TRANSITION_US for the valid range of this type.
 */
typedef uint32_t RAIL_TransitionTime_t;

/**
 * @def RAIL_TRANSITION_TIME_KEEP
 * @brief A value to use in \ref RAIL_StateTiming_t fields when
 *   calling \ref RAIL_SetStateTiming() to keep that timing
 *   parameter at it current setting.
 */
#define RAIL_TRANSITION_TIME_KEEP ((RAIL_TransitionTime_t) -1)

/**
 * @struct RAIL_StateTiming_t
 * @brief A timing configuration structure for the RAIL State Machine.
 *
 * Configure the timings of the radio state transitions for common situations.
 * All of the listed timings are in microseconds. Transitions from an active
 * radio state to idle are not configurable, and will always happen as fast
 * as possible.
 * No timing value can exceed platform-specific \ref RAIL_MAXIMUM_TRANSITION_US.
 * Use \ref RAIL_TRANSITION_TIME_KEEP to keep an existing setting.
 *
 * For idleToRx, idleToTx, rxToTx, txToRx, and txToTx a value of 0 for the
 * transition time means that the specified transition should happen as fast
 * as possible, even if the timing cannot be as consistent. Otherwise, the
 * timing value cannot be below the platform-specific \ref RAIL_MINIMUM_TRANSITION_US.
 *
 * For idleToTx, rxToTx, and txToTx setting a longer \ref
 * RAIL_TxPowerConfig_t::rampTime may result in a larger minimum value.
 *
 * For rxSearchTimeout and txToRxSearchTimeout, there is no minimum value.
 * A value of 0 disables the feature, functioning as an infinite timeout.
 */
typedef struct RAIL_StateTiming {
  /** Transition time from IDLE to RX. */
  RAIL_TransitionTime_t idleToRx;
  /** Transition time from TX to RX. */
  RAIL_TransitionTime_t txToRx;
  /** Transition time from IDLE to TX. */
  RAIL_TransitionTime_t idleToTx;
  /** Transition time from RX packet to TX. */
  RAIL_TransitionTime_t rxToTx;
  /** Length of time the radio will search for a packet when coming from idle or RX. */
  RAIL_TransitionTime_t rxSearchTimeout;
  /** Length of time the radio will search for a packet when coming from TX. */
  RAIL_TransitionTime_t txToRxSearchTimeout;
  /** Transition time from TX packet to TX. */
  RAIL_TransitionTime_t txToTx;
} RAIL_StateTiming_t;

/**
 * @enum RAIL_RadioState_t
 * @brief The state of the radio.
 */
RAIL_ENUM(RAIL_RadioState_t) {
  /** Radio is inactive. */
  RAIL_RF_STATE_INACTIVE = 0u,
  /**
   * Radio is either idle or, in combination with the RX and TX states,
   * receiving or transmitting a frame.
   */
  RAIL_RF_STATE_ACTIVE = (1u << 0),
  /** Radio is in receive. */
  RAIL_RF_STATE_RX = (1u << 1),
  /** Radio is in transmit. */
  RAIL_RF_STATE_TX = (1u << 2),
  /** Radio is idle. */
  RAIL_RF_STATE_IDLE = (RAIL_RF_STATE_ACTIVE),
  /** Radio is actively receiving a frame. */
  RAIL_RF_STATE_RX_ACTIVE = (RAIL_RF_STATE_RX | RAIL_RF_STATE_ACTIVE),
  /** Radio is actively transmitting a frame. */
  RAIL_RF_STATE_TX_ACTIVE = (RAIL_RF_STATE_TX | RAIL_RF_STATE_ACTIVE)
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_RF_STATE_INACTIVE  ((RAIL_RadioState_t) RAIL_RF_STATE_INACTIVE)
#define RAIL_RF_STATE_ACTIVE    ((RAIL_RadioState_t) RAIL_RF_STATE_ACTIVE)
#define RAIL_RF_STATE_RX        ((RAIL_RadioState_t) RAIL_RF_STATE_RX)
#define RAIL_RF_STATE_TX        ((RAIL_RadioState_t) RAIL_RF_STATE_TX)
#define RAIL_RF_STATE_IDLE      ((RAIL_RadioState_t) RAIL_RF_STATE_IDLE)
#define RAIL_RF_STATE_RX_ACTIVE ((RAIL_RadioState_t) RAIL_RF_STATE_RX_ACTIVE)
#define RAIL_RF_STATE_TX_ACTIVE ((RAIL_RadioState_t) RAIL_RF_STATE_TX_ACTIVE)
#endif//DOXYGEN_SHOULD_SKIP_THIS

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @enum RAIL_RadioStateEfr32_t
 * @brief Detailed EFR32 Radio state machine states.
 */
RAIL_ENUM(RAIL_RadioStateEfr32_t) {
  /** Radio is off. */
  RAIL_RAC_STATE_OFF = 0,
  /** Radio is enabling the receiver. */
  RAIL_RAC_STATE_RXWARM = 1,
  /** Radio is listening for incoming frames. */
  RAIL_RAC_STATE_RXSEARCH = 2,
  /** Radio is receiving a frame. */
  RAIL_RAC_STATE_RXFRAME = 3,
  /** Radio is powering down receiver and going to OFF state. */
  RAIL_RAC_STATE_RXPD = 4,
  /** Radio is going back to receive mode after receiving a frame. */
  RAIL_RAC_STATE_RX2RX = 5,
  /** Received data was lost due to full receive buffer. */
  RAIL_RAC_STATE_RXOVERFLOW = 6,
  /** Radio is disabling receiver and enabling transmitter. */
  RAIL_RAC_STATE_RX2TX = 7,
  /** Radio is enabling transmitter. */
  RAIL_RAC_STATE_TXWARM = 8,
  /** Radio is transmitting data. */
  RAIL_RAC_STATE_TX = 9,
  /** Radio is powering down transmitter and going to OFF state. */
  RAIL_RAC_STATE_TXPD = 10,
  /** Radio is disabling transmitter and enabling reception. */
  RAIL_RAC_STATE_TX2RX = 11,
  /** Radio is preparing a transmission after the previous transmission was ended. */
  RAIL_RAC_STATE_TX2TX = 12,
  /** Radio is powering down and going to OFF state. */
  RAIL_RAC_STATE_SHUTDOWN = 13,
  /** Radio power-on-reset state (EFR32xG22 and later). */
  RAIL_RAC_STATE_POR = 14,
  /** Invalid Radio state, must be the last entry. */
  RAIL_RAC_STATE_NONE
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_RAC_STATE_OFF        ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_OFF)
#define RAIL_RAC_STATE_RXWARM     ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RXWARM)
#define RAIL_RAC_STATE_RXSEARCH   ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RXSEARCH)
#define RAIL_RAC_STATE_RXFRAME    ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RXFRAME)
#define RAIL_RAC_STATE_RXPD       ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RXPD)
#define RAIL_RAC_STATE_RX2RX      ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RX2RX)
#define RAIL_RAC_STATE_RXOVERFLOW ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RXOVERFLOW)
#define RAIL_RAC_STATE_RX2TX      ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_RX2TX)
#define RAIL_RAC_STATE_TXWARM     ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_TXWARM)
#define RAIL_RAC_STATE_TX         ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_TX)
#define RAIL_RAC_STATE_TXPD       ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_TXPD)
#define RAIL_RAC_STATE_TX2RX      ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_TX2RX)
#define RAIL_RAC_STATE_TX2TX      ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_TX2TX)
#define RAIL_RAC_STATE_SHUTDOWN   ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_SHUTDOWN)
#define RAIL_RAC_STATE_POR        ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_POR)
#define RAIL_RAC_STATE_NONE       ((RAIL_RadioStateEfr32_t) RAIL_RAC_STATE_NONE)
#endif//DOXYGEN_SHOULD_SKIP_THIS
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_StateTransitions_t
 * @brief Used to specify radio states to transition to on success or failure.
 */
typedef struct RAIL_StateTransitions {
  /**
   * Indicate the state the radio should return to after a successful action.
   */
  RAIL_RadioState_t success;
  /**
   * Indicate the state the radio should return to after an error.
   */
  RAIL_RadioState_t error;
} RAIL_StateTransitions_t;

/**
 * @enum RAIL_RadioStateDetail_t
 * @brief The detailed state of the radio.
 *
 * The three radio state bits \ref RAIL_RF_STATE_DETAIL_IDLE_STATE, \ref
 * RAIL_RF_STATE_DETAIL_RX_STATE, and \ref RAIL_RF_STATE_DETAIL_TX_STATE
 * comprise a set of mutually exclusive core radio states. Only one (or none)
 * of these bits can be set at a time. Otherwise, the value is invalid.
 *
 * The precise meaning of each of these three core bits, when set, depends on
 * the value of the two bits \ref RAIL_RF_STATE_DETAIL_TRANSITION and \ref
 * RAIL_RF_STATE_DETAIL_ACTIVE. When \ref RAIL_RF_STATE_DETAIL_TRANSITION is
 * set, the radio is transitioning into the core radio state corresponding
 * to the set state bit. When it is clear, the radio is already in the core
 * radio state that corresponds to the set state bit. When \ref
 * RAIL_RF_STATE_DETAIL_ACTIVE is set, the radio is actively transmitting or
 * receiving. When it is clear, the radio is not actively transmitting or receiving.
 * This bit will always be clear when \ref RAIL_RF_STATE_DETAIL_IDLE_STATE is
 * set, and will always be set when \ref RAIL_RF_STATE_DETAIL_TX_STATE is set.
 * Otherwise, the value is invalid.
 *
 * The bit \ref RAIL_RF_STATE_DETAIL_NO_FRAMES is set if the radio is currently
 * operating with frame detection disabled, and clear otherwise. The bit \ref
 * RAIL_RF_STATE_DETAIL_LBT_SHIFT is set if an LBT/CSMA operation
 * (e.g., performing CCA) is currently ongoing, and clear otherwise.
 */
RAIL_ENUM(RAIL_RadioStateDetail_t) {
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_IDLE_STATE bit. */
  RAIL_RF_STATE_DETAIL_IDLE_STATE_SHIFT = 0,
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_RX_STATE bit. */
  RAIL_RF_STATE_DETAIL_RX_STATE_SHIFT = 1,
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_TX_STATE bit. */
  RAIL_RF_STATE_DETAIL_TX_STATE_SHIFT = 2,
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_TRANSITION bit. */
  RAIL_RF_STATE_DETAIL_TRANSITION_SHIFT = 3,
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_ACTIVE bit. */
  RAIL_RF_STATE_DETAIL_ACTIVE_SHIFT = 4,
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_NO_FRAMES bit. */
  RAIL_RF_STATE_DETAIL_NO_FRAMES_SHIFT = 5,
  /** Shift position of \ref RAIL_RF_STATE_DETAIL_LBT bit. */
  RAIL_RF_STATE_DETAIL_LBT_SHIFT = 6,
};

/** Radio is inactive. */
#define RAIL_RF_STATE_DETAIL_INACTIVE (0U)
/** Radio is in or headed to the idle state. */
#define RAIL_RF_STATE_DETAIL_IDLE_STATE (1U << RAIL_RF_STATE_DETAIL_IDLE_STATE_SHIFT)
/** Radio is in or headed to the receive state. */
#define RAIL_RF_STATE_DETAIL_RX_STATE (1U << RAIL_RF_STATE_DETAIL_RX_STATE_SHIFT)
/** Radio is in or headed to the transmit state. */
#define RAIL_RF_STATE_DETAIL_TX_STATE (1U << RAIL_RF_STATE_DETAIL_TX_STATE_SHIFT)
/** Radio is headed to the idle, receive, or transmit state. */
#define RAIL_RF_STATE_DETAIL_TRANSITION (1U << RAIL_RF_STATE_DETAIL_TRANSITION_SHIFT)
/** Radio is actively transmitting or receiving. */
#define RAIL_RF_STATE_DETAIL_ACTIVE (1U << RAIL_RF_STATE_DETAIL_ACTIVE_SHIFT)
/** Radio has frame detect disabled. */
#define RAIL_RF_STATE_DETAIL_NO_FRAMES (1U << RAIL_RF_STATE_DETAIL_NO_FRAMES_SHIFT)
/** LBT/CSMA operation is currently ongoing. */
#define RAIL_RF_STATE_DETAIL_LBT (1U << RAIL_RF_STATE_DETAIL_LBT_SHIFT)
/** Mask for core radio state bits. */
#define RAIL_RF_STATE_DETAIL_CORE_STATE_MASK (RAIL_RF_STATE_DETAIL_IDLE_STATE \
                                              | RAIL_RF_STATE_DETAIL_RX_STATE \
                                              | RAIL_RF_STATE_DETAIL_TX_STATE)

/**
 * @enum RAIL_IdleMode_t
 * @brief An enumeration for the different types of supported idle modes. These
 *   vary how quickly and destructively they put the radio into idle.
 */
RAIL_ENUM(RAIL_IdleMode_t) {
  /**
   * Idle the radio by turning off receive and canceling any future scheduled
   * receive or transmit operations. It does not abort a receive or
   * transmit in progress.
   */
  RAIL_IDLE = 0u,
  /**
   * Idle the radio by turning off receive and any scheduled events. It
   * also aborts any receive, transmit, or scheduled events in progress.
   */
  RAIL_IDLE_ABORT = 1u,
  /**
   * Force the radio into a shutdown mode by stopping whatever state is in
   * progress. This is a more destructive shutdown than \ref RAIL_IDLE or
   * \ref RAIL_IDLE_ABORT and can be useful in certain situations when directed
   * by the support team or for debugging. Note that this method may corrupt
   * receive and transmit buffers so it requires a more thorough cleanup
   * and any held packets will be lost.
   */
  RAIL_IDLE_FORCE_SHUTDOWN = 2u,
  /**
   * Similar to the \ref RAIL_IDLE_FORCE_SHUTDOWN command, however, it will also
   * clear any pending RAIL events related to receive and transmit.
   */
  RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS = 3u,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_IDLE                            ((RAIL_IdleMode_t) RAIL_IDLE)
#define RAIL_IDLE_ABORT                      ((RAIL_IdleMode_t) RAIL_IDLE_ABORT)
#define RAIL_IDLE_FORCE_SHUTDOWN             ((RAIL_IdleMode_t) RAIL_IDLE_FORCE_SHUTDOWN)
#define RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS ((RAIL_IdleMode_t) RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group State_Transitions

/******************************************************************************
 * TX Channel Hopping
 *****************************************************************************/
/**
 * @addtogroup Tx_Channel_Hopping TX Channel Hopping
 * @{
 */

/**
 * @struct RAIL_TxChannelHoppingConfigEntry_t
 * @brief Structure that represents one of the channels that is part of a
 *   \ref RAIL_TxChannelHoppingConfig_t sequence of channels used in
 *   channel hopping.
 */
typedef struct RAIL_TxChannelHoppingConfigEntry {
  /**
   * The channel number to be used for this entry in the channel hopping
   * sequence. If this is an invalid channel for the current PHY, the
   * call to \ref RAIL_SetNextTxRepeat() will fail.
   */
  uint16_t channel;
  /**
   * Pad bytes reserved for future use and currently ignored.
   */
  uint8_t reserved[2];
  /**
   * Idle time in microseconds to wait before transmitting on the channel
   * indicated by this entry.
   */
  uint32_t delay;
} RAIL_TxChannelHoppingConfigEntry_t;

/**
 * @struct RAIL_TxChannelHoppingConfig_t
 * @brief Wrapper struct that will contain the sequence of
 *   \ref RAIL_TxChannelHoppingConfigEntry_t that represents the channel
 *   sequence to use during TX Channel Hopping.
 */
typedef struct RAIL_TxChannelHoppingConfig {
  /**
   * Pointer to contiguous global read-write memory that will be used
   * by RAIL to store channel hopping information throughout its operation.
   * It need not be initialized and applications should never write
   * data anywhere in this buffer.
   *
   * @note The size of this buffer must be at least as large as
   *   3 + \ref RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL * numberOfChannels,
   *   plus the sum of the sizes of the
   *   radioConfigDeltaAdd's of the required channels, plus the size of the
   *   radioConfigDeltaSubtract. In the case that one channel
   *   appears two or more times in your channel sequence
   *   (e.g., 1, 2, 3, 2), you must account for the radio configuration
   *   size that number of times (i.e., need to count channel 2's
   *   radio configuration size twice for the given example). The buffer is
   *   for internal use to the library.
   */
  uint32_t *buffer;
  /**
   * This parameter must be set to the length of the buffer array, in 32 bit
   * words. This way, during configuration, the software can confirm it's
   * writing within the bounds of the buffer. The configuration API will return
   * an error or trigger \ref RAIL_ASSERT_CHANNEL_HOPPING_BUFFER_TOO_SHORT if
   * bufferLength is insufficient.
   */
  uint16_t bufferLength;
  /**
   * The number of channels in the channel hopping sequence, which is the
   * number of elements in the array that entries points to.
   */
  uint8_t numberOfChannels;
  /**
   * Pad byte reserved for future use and currently ignored.
   */
  uint8_t reserved;
  /**
   * A pointer to the first element of an array of \ref
   * RAIL_TxChannelHoppingConfigEntry_t that represents the channels
   * used during channel hopping. The length of this array must be
   * numberOfChannels.
   */
  RAIL_TxChannelHoppingConfigEntry_t *entries;
} RAIL_TxChannelHoppingConfig_t;

/// The worst-case platform-agnostic static amount of memory needed per
/// channel for channel hopping, measured in 32 bit words, regardless of
/// the size of radio configuration structures.
#define RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL_WORST_CASE (65U)

/** @} */ // end of group Tx_Channel_Hopping

/******************************************************************************
 * TX/RX Configuration Structures
 *****************************************************************************/
/**
 * @addtogroup Transmit
 * @{
 */

/**
 * @enum RAIL_StopMode_t
 * @brief Stop radio operation options bit mask
 */
RAIL_ENUM(RAIL_StopMode_t) {
  /** Shift position of \ref RAIL_STOP_MODE_ACTIVE bit. */
  RAIL_STOP_MODE_ACTIVE_SHIFT = 0,
  /** Shift position of \ref RAIL_STOP_MODE_PENDING bit. */
  RAIL_STOP_MODE_PENDING_SHIFT = 1,
};

/** Do not stop any radio operations */
#define RAIL_STOP_MODES_NONE   (0U)
/** Stop active radio operations only */
#define RAIL_STOP_MODE_ACTIVE (1U << RAIL_STOP_MODE_ACTIVE_SHIFT)
/** Stop pending radio operations */
#define RAIL_STOP_MODE_PENDING (1U << RAIL_STOP_MODE_PENDING_SHIFT)
/** Stop all radio operations */
#define RAIL_STOP_MODES_ALL (0xFFU)

/**
 * @enum RAIL_TxOptions_t
 * @brief Transmit options, in reality a bitmask.
 */
RAIL_ENUM_GENERIC(RAIL_TxOptions_t, uint32_t) {
  /** Shift position of \ref RAIL_TX_OPTION_WAIT_FOR_ACK bit. */
  RAIL_TX_OPTION_WAIT_FOR_ACK_SHIFT = 0,
  /** Shift position of \ref RAIL_TX_OPTION_REMOVE_CRC bit. */
  RAIL_TX_OPTION_REMOVE_CRC_SHIFT = 1,
  /** Shift position of \ref RAIL_TX_OPTION_SYNC_WORD_ID bit. */
  RAIL_TX_OPTION_SYNC_WORD_ID_SHIFT = 2,
  /** Shift position of \ref RAIL_TX_OPTION_ANTENNA0 bit. */
  RAIL_TX_OPTION_ANTENNA0_SHIFT = 3,
  /** Shift position of \ref RAIL_TX_OPTION_ANTENNA1 bit. */
  RAIL_TX_OPTION_ANTENNA1_SHIFT = 4,
  /** Shift position of \ref RAIL_TX_OPTION_ALT_PREAMBLE_LEN bit. */
  RAIL_TX_OPTION_ALT_PREAMBLE_LEN_SHIFT = 5,
  /** Shift position of \ref RAIL_TX_OPTION_CCA_PEAK_RSSI bit. */
  RAIL_TX_OPTION_CCA_PEAK_RSSI_SHIFT = 6,
  /** Shift position of \ref RAIL_TX_OPTION_CCA_ONLY bit. */
  RAIL_TX_OPTION_CCA_ONLY_SHIFT = 7,
  /** Shift position of \ref RAIL_TX_OPTION_RESEND bit. */
  RAIL_TX_OPTION_RESEND_SHIFT = 8,
  /** Shift position of \ref RAIL_TX_OPTION_CONCURRENT_PHY_ID bit. */
  RAIL_TX_OPTION_CONCURRENT_PHY_ID_SHIFT = 9,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_TX_OPTIONS_COUNT
};

/** A value representing no options enabled. */
#define RAIL_TX_OPTIONS_NONE 0UL

/** All options disabled by default. This is the fastest TX option to apply. */
#define RAIL_TX_OPTIONS_DEFAULT RAIL_TX_OPTIONS_NONE

/**
 * An option when Auto-Ack has been configured, enabled, and not TX paused, to
 * configure whether or not the transmitting node will listen for an Ack
 * response.
 * If this is false, the \ref RAIL_RxPacketDetails_t::isAck flag of a received
 * packet will always be false.
 * If Auto-Ack is enabled, for instance using \ref RAIL_ConfigAutoAck() or
 * \ref RAIL_IEEE802154_Init(), and if this option is false, the radio
 * transitions to \ref RAIL_AutoAckConfig_t::txTransitions's
 * \ref RAIL_StateTransitions_t::success state directly after transmitting a
 * packet and does not wait for an Ack.
 */
#define RAIL_TX_OPTION_WAIT_FOR_ACK (1UL << RAIL_TX_OPTION_WAIT_FOR_ACK_SHIFT)

/**
 * An option to remove CRC bytes from TX packets. To receive packets when the
 * sender has this option set true, set \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS
 * on the receive side.
 */
#define RAIL_TX_OPTION_REMOVE_CRC (1UL << RAIL_TX_OPTION_REMOVE_CRC_SHIFT)

/**
 * An option to select which sync word is used during the transmission.
 * When two sync words are configured by the PHY or \ref RAIL_ConfigSyncWords()
 * enabling this option selects SYNC2 rather than SYNC1 for the upcoming transmit.
 *
 * This option should not be used when only one sync word has been configured.
 *
 * @note There are a few special radio configurations (e.g., BLE Viterbi) that do
 *   not support transmitting different sync words.
 */
#define RAIL_TX_OPTION_SYNC_WORD_ID (1UL << RAIL_TX_OPTION_SYNC_WORD_ID_SHIFT)

/**
 * An option to select antenna 0 for transmission. If the antenna selection
 * option is not set or if both antenna options are set, then the transmit
 * will occur on either antenna depending on the last receive or transmit
 * selection. This option is only valid on platforms that support
 * \ref Antenna_Control and have been configured via \ref RAIL_ConfigAntenna().
 *
 * @note These TX antenna options do not control the antenna used for
 *   \ref Auto_Ack transmissions, which always occur on the same antenna
 *   used to receive the packet being acknowledged.
 */
#define RAIL_TX_OPTION_ANTENNA0 (1UL << RAIL_TX_OPTION_ANTENNA0_SHIFT)

/**
 * An option to select antenna 1 for transmission. If the antenna selection
 * option is not set or if both antenna options are set, then the transmit
 * will occur on either antenna depending on the last receive or transmit
 * selection. This option is only valid on platforms that support
 * \ref Antenna_Control and have been configured via \ref RAIL_ConfigAntenna().
 *
 * @note These TX antenna options do not control the antenna used for
 *   \ref Auto_Ack transmissions, which always occur on the same antenna
 *   used to receive the packet being acknowledged.
 */
#define RAIL_TX_OPTION_ANTENNA1 (1UL << RAIL_TX_OPTION_ANTENNA1_SHIFT)

/**
 * An option to use the alternate preamble length established
 * by \ref RAIL_SetTxAltPreambleLength() for the transmission.
 * When not set, the PHY configuration's preamble length is used.
 */
#define RAIL_TX_OPTION_ALT_PREAMBLE_LEN (1UL << RAIL_TX_OPTION_ALT_PREAMBLE_LEN_SHIFT)

/**
 * An option to use peak rather than average RSSI energy detected during
 * CSMA's \ref RAIL_CsmaConfig_t::ccaDuration or LBT's \ref
 * RAIL_LbtConfig_t::lbtDuration to determine whether the channel is clear
 * or busy. This option is only valid when calling one of the CCA transmit
 * routines: \ref RAIL_StartCcaCsmaTx(), \ref RAIL_StartCcaLbtTx(), \ref
 * RAIL_StartScheduledCcaCsmaTx(), or \ref RAIL_StartScheduledCcaLbtTx().
 */
#define RAIL_TX_OPTION_CCA_PEAK_RSSI (1UL << RAIL_TX_OPTION_CCA_PEAK_RSSI_SHIFT)

/**
 * An option to only perform the CCA (CSMA/LBT) operation but *not*
 * automatically transmit if the channel is clear. This option is only valid
 * when calling one of the CCA transmit routines: \ref RAIL_StartCcaCsmaTx(),
 * \ref RAIL_StartCcaLbtTx(), \ref RAIL_StartScheduledCcaCsmaTx(), or \ref
 * RAIL_StartScheduledCcaLbtTx().
 *
 * Application can then use the \ref RAIL_EVENT_TX_CHANNEL_CLEAR to
 * initiate transmit manually, e.g., giving it the opportunity to adjust
 * outgoing packet data before the packet goes out.
 *
 * @note Configured state transitions to Rx or Idle are suspended during
 *   this CSMA/LBT operation. If packet reception occurs, the radio will
 *   return to the state it was in just prior to the CSMA/LBT operation
 *   when that reception (including any Auto-Ack response) is complete.
 */
#define RAIL_TX_OPTION_CCA_ONLY (1UL << RAIL_TX_OPTION_CCA_ONLY_SHIFT)

/**
 * An option to resend packet at the beginning of the Transmit FIFO.
 *
 * The packet to be resent must have been previously provided by
 * \ref RAIL_SetTxFifo() or \ref RAIL_WriteTxFifo() passing true for
 * the latter's reset parameter. It works by setting the
 * transmit FIFO's read offset to the beginning of the FIFO while
 * leaving its write offset intact. For this to work,
 * \ref RAIL_DataConfig_t::txMethod must be RAIL_DataMethod_t::PACKET_MODE
 * (i.e., the packet can't exceed the Transmit FIFO's size), otherwise
 * undefined behavior will result.
 *
 * This option can also be used with \ref RAIL_SetNextTxRepeat() to cause
 * the repeated packet(s) to all be the same as the first.
 */
#define RAIL_TX_OPTION_RESEND (1UL << RAIL_TX_OPTION_RESEND_SHIFT)

/**
 * An option to specify which PHY is used to transmit in the case of concurrent mode.
 * Concurrent mode is only allowed on EFR32xG25 for some predefined combinations of Wi-SUN PHYs.
 * When set/unset, the alternate/base PHY is used to transmit.
 */
#define RAIL_TX_OPTION_CONCURRENT_PHY_ID (1UL << RAIL_TX_OPTION_CONCURRENT_PHY_ID_SHIFT)

/** A value representing all possible options. */
#define RAIL_TX_OPTIONS_ALL 0xFFFFFFFFUL

/**
 * @struct RAIL_TxPacketDetails_t
 * @brief Detailed information requested about the packet that was just,
 *   or is currently being, transmitted.
 */
typedef struct RAIL_TxPacketDetails {
  /**
   * The timestamp of the transmitted packet in the RAIL timebase,
   * filled in by \ref RAIL_GetTxPacketDetails().
   */
  RAIL_PacketTimeStamp_t timeSent;
  /**
   * Indicate whether the transmitted packet was an automatic Ack. In a generic
   * sense, an automatic Ack is defined as a packet sent in response to a
   * received Ack-requesting frame when Auto-Ack is enabled. In a protocol
   * specific sense this definition may be more or less restrictive to match the
   * specification and you should refer to that protocol's documentation.
   */
  bool isAck;
} RAIL_TxPacketDetails_t;

/**
 * @enum RAIL_ScheduledTxDuringRx_t
 * @brief Enumerates the possible outcomes of what will occur if a
 *   scheduled TX ends up firing during RX. Because RX and TX can't
 *   happen at the same time, it is up to the user how the TX should be
 *   handled. This enumeration is passed into \ref RAIL_StartScheduledTx()
 *   as part of \ref RAIL_ScheduleTxConfig_t.
 */
RAIL_ENUM(RAIL_ScheduledTxDuringRx_t) {
  /**
   * The scheduled TX will be postponed until RX completes and then sent.
   */
  RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX = 0,
  /**
   * The scheduled TX will be aborted and a
   * \ref RAIL_EVENT_TX_BLOCKED event will fire.
   */
  RAIL_SCHEDULED_TX_DURING_RX_ABORT_TX = 1,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX ((RAIL_ScheduledTxDuringRx_t) RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX)
#define RAIL_SCHEDULED_TX_DURING_RX_ABORT_TX    ((RAIL_ScheduledTxDuringRx_t) RAIL_SCHEDULED_TX_DURING_RX_ABORT_TX)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_ScheduleTxConfig_t
 * @brief A configuration structure for a scheduled transmit.
 */
typedef struct RAIL_ScheduleTxConfig {
  /**
   * The time when to transmit this packet. The exact interpretation of
   * this value depends on the mode specified below.
   */
  RAIL_Time_t when;
  /**
   * The type of delay. See the \ref RAIL_TimeMode_t documentation for
   * more information. Be sure to use \ref RAIL_TIME_ABSOLUTE delays for
   * time-critical protocols.
   */
  RAIL_TimeMode_t mode;
  /**
   * Indicate which action to take with a scheduled TX if it occurs during RX.
   * See \ref RAIL_ScheduledTxDuringRx_t structure for more information on
   * potential options.
   */
  RAIL_ScheduledTxDuringRx_t txDuringRx;
} RAIL_ScheduleTxConfig_t;

/**
 * @def RAIL_MAX_LBT_TRIES
 * @brief The maximum number of LBT/CSMA retries supported.
 */
#define RAIL_MAX_LBT_TRIES      (15U)

/**
 * @def RAIL_MAX_CSMA_EXPONENT
 * @brief The maximum power-of-2 exponent for CSMA backoffs.
 */
#define RAIL_MAX_CSMA_EXPONENT  (8U)

///
/// @struct RAIL_CsmaConfig_t
/// @brief A configuration structure for the CSMA transmit algorithm.
///
/// One of RAIL's schemes for polite spectrum access is an implementation of
/// a Carrier-Sense Multiple Access (CSMA) algorithm based on IEEE 802.15.4
/// (unslotted).
/// \n In pseudo-code it works like this, showing relevant event notifications:
/// @code{.c}
/// // Return true to transmit packet, false to not transmit packet.
/// bool performCsma(const RAIL_CsmaConfig_t *csmaConfig)
/// {
///   bool isFixedBackoff = ((csmaConfig->csmaMinBoExp == 0)
///                          && (csmaConfig->csmaMaxBoExp == 0));
///   int backoffExp = csmaConfig->csmaMinBoExp; // Initial backoff exponent
///   int backoffMultiplier = 1; // Assume fixed backoff
///   int try;
///
///   // Special-case tries == 0 to transmit immediately without backoff+CCA
///   if (csmaConfig->csmaTries == 0) {
///     return true;
///   }
///
///   // Start overall timeout if specified:
///   if (csmaConfig->csmaTimeout > 0) {
///     StartAbortTimer(csmaConfig->csmaTimeout, RAIL_EVENT_TX_CHANNEL_BUSY);
///     // If timeout occurs, abort and signal the indicated event.
///   }
///
///   for (try = 0; try < csmaConfig->csmaTries; try++) {
///     if (try > 0) {
///       signalEvent(RAIL_EVENT_TX_CCA_RETRY);
///     }
///     // Determine the backoff multipler for this try:
///     if (isFixedBackoff) {
///       // backoffMultiplier already set to 1 for fixed backoff
///     } else {
///       // Start with the backoff exponent for this try:
///       if (try > 0) {
///         backoffExp++;
///         if (backoffExp > csmaConfig->csmaMaxBoExp) {
///           backoffExp = csmaConfig->csmaMaxBoExp;
///         }
///       }
///       // Pick random multiplier between 0 and 2^backoffExp - 1 inclusive:
///       backoffMultiplier = pickRandomInteger(0, (1 << backoffExp) - 1);
///     }
///     // Perform the backoff:
///     delayMicroseconds(backoffMultiplier * csmaConfig->ccaBackoff);
///     // Perform the Clear-Channel Assessment (CCA):
///     // Channel is considered busy if radio is actively receiving or
///     // transmitting, or the average energy detected across duration
///     // is above the threshold.
///     signalEvent(RAIL_EVENT_TX_START_CCA);
///     if (performCca(csmaConfig->ccaDuration, csmaConfig->ccaThreshold)) {
///       // CCA (and CSMA) success: Transmit after RX-to-TX turnaround
///       StopAbortTimer();
///       signalEvent(RAIL_EVENT_TX_CHANNEL_CLEAR);
///       return true;
///     } else {
///       // CCA failed: loop to try again, or exit if out of tries
///     }
///   }
///   // Overall CSMA failure: Don't transmit
///   StopAbortTimer();
///   signalEvent(RAIL_EVENT_TX_CHANNEL_BUSY);
///   return false;
/// }
/// @endcode
///
typedef struct RAIL_CsmaConfig {
  /**
   * The minimum (starting) exponent for CSMA random backoff (2^exp - 1).
   * It can range from 0 to \ref RAIL_MAX_CSMA_EXPONENT.
   *
   * @warning On EFR32, due to a hardware limitation, this can only be 0
   *   if \ref csmaMaxBoExp is also 0 specifying a non-random fixed backoff.
   *   \ref RAIL_STATUS_INVALID_PARAMETER will result otherwise.
   *   If you really want CSMA's first iteration to have no backoff prior to
   *   CCA, with subsequent iterations having random backoff as the exponent
   *   is increased, you must do a fixed backoff of 0 operation first
   *   (\ref csmaMinBoExp = 0, \ref csmaMaxBoExp = 0, \ref ccaBackoff = 0,
   *   \ref csmaTries = 1), and if that fails (\ref RAIL_EVENT_TX_CHANNEL_BUSY),
   *   follow up with a random backoff operation starting at \ref csmaMinBoExp
   *   = 1 for the remaining iterations.
   */
  uint8_t csmaMinBoExp;
  /**
   * The maximum exponent for CSMA random backoff (2^exp - 1).
   * It can range from 0 to \ref RAIL_MAX_CSMA_EXPONENT and must be greater
   * than or equal to \ref csmaMinBoExp.
   * \n If both exponents are 0, a non-random fixed backoff of \ref ccaBackoff
   * duration results.
   */
  uint8_t csmaMaxBoExp;
  /**
   * The number of backoff-then-CCA iterations that can fail before reporting
   * \ref RAIL_EVENT_TX_CHANNEL_BUSY. Typically ranges from 1 to \ref
   * RAIL_MAX_LBT_TRIES; higher values are disallowed. A value 0 always
   * transmits immediately without performing CSMA, similar to calling
   * \ref RAIL_StartTx().
   */
  uint8_t csmaTries;
  /**
   * The CCA RSSI threshold, in dBm, above which the channel is
   * considered 'busy'.
   */
  int8_t ccaThreshold;
  /**
   * The backoff unit period in RAIL's microsecond time base. It is
   * multiplied by the random backoff exponential controlled by \ref
   * csmaMinBoExp and \ref csmaMaxBoExp to determine the overall backoff
   * period. For random backoffs, any value above 32768 microseconds for
   * the 'EFR Series 2' and 8192 microseconds for the 'Series 3' will be truncated
   * for a single backoff period. Up to 255 backoff periods are supported.
   * For fixed backoffs it can go up to 65535 microseconds.
   */
  uint16_t ccaBackoff;
  /**
   * The minimum desired CCA check duration in microseconds. The RSSI is
   * averaged over this duration by default but can be set to use the peak RSSI,
   * on supported platforms, using the \ref RAIL_TX_OPTION_CCA_PEAK_RSSI option.
   *
   * @note Depending on the radio configuration, due to hardware constraints,
   *   the actual duration may be longer. Also, if the requested duration
   *   is too large for the radio to accommodate, \ref RAIL_StartCcaCsmaTx()
   *   will fail returning \ref RAIL_STATUS_INVALID_PARAMETER.
   */
  uint16_t ccaDuration;
  /**
   * An overall timeout, in RAIL's microsecond time base, for the operation.
   * If the transmission doesn't start before this timeout expires, the
   * transmission will fail with \ref RAIL_EVENT_TX_CHANNEL_BUSY.
   * A value 0 means no timeout is imposed.
   */
  RAIL_Time_t csmaTimeout;
} RAIL_CsmaConfig_t;

/**
 * @def RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA
 * @brief \ref RAIL_CsmaConfig_t initializer configuring CSMA per IEEE 802.15.4-2003
 *   on 2.4 GHz OSPSK, commonly used by Zigbee.
 */
#define RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA {                \
    /* CSMA per 802.15.4-2003 on 2.4 GHz OSPSK, commonly used by Zigbee */ \
    .csmaMinBoExp      = 3,   /* 2^3-1 for 0..7 backoffs on 1st try     */ \
    .csmaMaxBoExp      = 5,   /* 2^5-1 for 0..31 backoffs on 3rd+ tries */ \
    .csmaTries         = 5,   /* 5 tries overall (4 re-tries)           */ \
    .ccaThreshold      = -75, /* 10 dB above sensitivity                */ \
    .ccaBackoff        = 320, /* 20 symbols at 16 us/symbol             */ \
    .ccaDuration       = 128, /* 8 symbols at 16 us/symbol              */ \
    .csmaTimeout       = 0,   /* No timeout                             */ \
}

/**
 * @def RAIL_CSMA_CONFIG_SINGLE_CCA
 * @brief \ref RAIL_CsmaConfig_t initializer configuring a single CCA prior to TX.
 *   It can be used to as a basis for implementing other channel access schemes
 *   with custom backoff delays. Users can override ccaBackoff with a fixed
 *   delay on each use.
 */
#define RAIL_CSMA_CONFIG_SINGLE_CCA {                                      \
    /* Perform a single CCA after 'fixed' delay                         */ \
    .csmaMinBoExp      = 0,   /* Used for fixed backoff                 */ \
    .csmaMaxBoExp      = 0,   /* Used for fixed backoff                 */ \
    .csmaTries         = 1,   /* Single try                             */ \
    .ccaThreshold      = -75, /* Override if not desired choice         */ \
    .ccaBackoff        = 0,   /* No backoff (override with fixed value) */ \
    .ccaDuration       = 128, /* Override if not desired length         */ \
    .csmaTimeout       = 0,   /* no timeout                             */ \
}

///
/// @struct RAIL_LbtConfig_t
/// @brief A configuration structure for the LBT transmit algorithm.
///
/// One of RAIL's schemes for polite spectrum access is an implementation of
/// a Listen-Before-Talk (LBT) algorithm, loosely based on ETSI 300 220-1.
/// \n Currently, however, it is constrained by the EFR32's CSMA-oriented hardware
/// so is turned into an equivalent \ref RAIL_CsmaConfig_t configuration and
/// passed to the CSMA engine:
/// @code{.c}
/// if (lbtMaxBoRand == lbtMinBoRand) {
///   // Fixed backoff
///   csmaMinBoExp = csmaMaxBoExp = 0;
///   if (lbtMinBoRand == 0) {
///     ccaBackoff = lbtBackoff;
///   } else {
///     ccaBackoff = lbtMinBoRand * lbtBackoff;
///   }
///   ccaDuration = lbtDuration;
/// } else {
///   // Random backoff: map to random range 0 .. (lbtMaxBoRand - lbtMinBoRand)
///   csmaMinBoExp = csmaMaxBoExp = ceiling(log2(lbtMaxBoRand - lbtMinBoRand));
///   ccaBackoff = round((lbtBackoff * (lbtMaxBoRand - lbtMinBoRand))
///                      / (1 << csmaMinBoExp));
///   ccaDuration = lbtDuration + (lbtMinBoRand * lbtBackoff);
/// }
/// csmaTries    = lbtTries;
/// ccaThreshold = lbtThreshold;
/// csmaTimeout  = lbtTimeout;
/// @endcode
///
typedef struct RAIL_LbtConfig {
  /**
   * The minimum backoff random multiplier.
   */
  uint8_t lbtMinBoRand;
  /**
   * The maximum backoff random multiplier.
   * It must be greater than or equal to \ref lbtMinBoRand.
   * \n If both backoff multipliers are identical, a non-random fixed backoff
   * of \ref lbtBackoff times the multiplier (minimum 1) duration results.
   */
  uint8_t lbtMaxBoRand;
  /**
   * The number of LBT iterations that can fail before reporting
   * \ref RAIL_EVENT_TX_CHANNEL_BUSY. Typically ranges from 1 to \ref
   * RAIL_MAX_LBT_TRIES; higher values are disallowed. A value 0 always
   * transmits immediately without performing LBT, similar to calling
   * \ref RAIL_StartTx().
   */
  uint8_t lbtTries;
  /**
   * The LBT RSSI threshold, in dBm, above which the channel is
   * considered 'busy'.
   */
  int8_t lbtThreshold;
  /**
   * The backoff unit period, in RAIL's microsecond time base. It is
   * multiplied by the random backoff multiplier controlled by \ref
   * lbtMinBoRand and \ref lbtMaxBoRand to determine the overall backoff
   * period. For random backoffs, any value above 32768 microseconds for
   * the 'EFR Series 2' and 8192 microseconds for the 'Series 3' will be truncated
   * for a single backoff period. Up to 255 backoff periods are supported.
   * For fixed backoffs, it can go up to 65535 microseconds.
   */
  uint16_t lbtBackoff;
  /**
   * The minimum desired LBT check duration in microseconds.
   *
   * @note Depending on the radio configuration, due to hardware constraints,
   *   the actual duration may be longer. Also, if the requested duration
   *   is too large for the radio to accommodate, \ref RAIL_StartCcaLbtTx()
   *   will fail returning \ref RAIL_STATUS_INVALID_PARAMETER.
   */
  uint16_t lbtDuration;
  /**
   * An overall timeout, in RAIL's microsecond time base, for the operation.
   * If the transmission doesn't start before this timeout expires, the
   * transmission will fail with \ref RAIL_EVENT_TX_CHANNEL_BUSY.
   * This is important for limiting LBT due to LBT's unbounded requirement
   * that if the channel is busy, the next try must wait for the channel to
   * clear. A value 0 means no timeout is imposed.
   */
  RAIL_Time_t lbtTimeout;
} RAIL_LbtConfig_t;

/**
 * @def RAIL_LBT_CONFIG_ETSI_EN_300_220_1_V2_4_1
 * @brief \ref RAIL_LbtConfig_t initializer configuring LBT per ETSI 300 220-1
 *   V2.4.1 for a typical Sub-GHz band. To be practical, users should override
 *   lbtTries and/or lbtTimeout so channel access failure will be reported in a
 *   reasonable time frame rather than the unbounded time frame ETSI defined.
 */
#define RAIL_LBT_CONFIG_ETSI_EN_300_220_1_V2_4_1 {                                \
    /* LBT per ETSI 300 220-1 V2.4.1                                           */ \
    /* LBT time = random backoff of 0-5 ms in .5 ms increments plus 5 ms fixed */ \
    .lbtMinBoRand      = 0,    /*                                              */ \
    .lbtMaxBoRand      = 10,   /*                                              */ \
    .lbtTries          = RAIL_MAX_LBT_TRIES, /* the maximum supported          */ \
    .lbtThreshold      = -87,  /*                                              */ \
    .lbtBackoff        = 500,  /* 0.5 ms                                       */ \
    .lbtDuration       = 5000, /* 5 ms                                         */ \
    .lbtTimeout        = 0,    /* No timeout (recommend user override)         */ \
}

/**
 * @def RAIL_LBT_CONFIG_ETSI_EN_300_220_1_V3_1_0
 * @brief \ref RAIL_LbtConfig_t initializer configuring LBT per ETSI 300 220-1
 *   V3.1.0 for a typical Sub-GHz band. To be practical, users should override
 *   lbtTries and/or lbtTimeout so channel access failure will be reported in a
 *   reasonable time frame rather than the unbounded time frame ETSI defined.
 */
#define RAIL_LBT_CONFIG_ETSI_EN_300_220_1_V3_1_0  {                              \
    /* LBT per ETSI 300 220-1 V3.1.0                                          */ \
    /* LBT time = random backoff of 160-4960 us in 160 us increments          */ \
    .lbtMinBoRand      = 1,    /*                                             */ \
    .lbtMaxBoRand      = 31,   /* app-chosen; 31*lbtBackoff = 4960 us         */ \
    .lbtTries          = RAIL_MAX_LBT_TRIES, /* the maximum supported         */ \
    .lbtThreshold      = -85,  /* 15 dB above Rx sensitivity per Table 45     */ \
    .lbtDuragion       = 160,  /* 160 us per Table 48 Minimum CCA interval    */ \
    .lbtTimeout        = 0,    /* No timeout (recommend user override)        */ \
}

/**
 * @struct RAIL_SyncWordConfig_t
 * @brief RAIL sync words and length configuration.
 *
 */
typedef struct RAIL_SyncWordConfig {
  /** Sync word length in bits, between 2 and 32, inclusive.*/
  uint8_t syncWordBits;
  /**
   * Sync Word1
   * @note Only the least-significant \ref syncWordBits bits are used,
   *   which are sent or received on air least-significant-bit first.
   */
  uint32_t syncWord1;
  /**
   * Sync Word2
   * @note Only the least-significant \ref syncWordBits bits are used,
   *   which are sent or received on air least-significant-bit first.
   */
  uint32_t syncWord2;
} RAIL_SyncWordConfig_t;

/**
 * @enum RAIL_TxRepeatOptions_t
 * @brief Transmit repeat options, in reality a bitmask.
 */
RAIL_ENUM_GENERIC(RAIL_TxRepeatOptions_t, uint16_t) {
  /** Shift position of \ref RAIL_TX_REPEAT_OPTION_HOP bit. */
  RAIL_TX_REPEAT_OPTION_HOP_SHIFT = 0,
  /** Shift position of the \ref RAIL_TX_REPEAT_OPTION_START_TO_START bit. */
  RAIL_TX_REPEAT_OPTION_START_TO_START_SHIFT = 1,
};

/** A value representing no repeat options enabled. */
#define RAIL_TX_REPEAT_OPTIONS_NONE 0U
/** All repeat options disabled by default. */
#define RAIL_TX_REPEAT_OPTIONS_DEFAULT RAIL_TX_REPEAT_OPTIONS_NONE
/**
 * An option to configure whether or not to channel-hop before each
 * repeated transmit.
 */
#define RAIL_TX_REPEAT_OPTION_HOP (1U << RAIL_TX_REPEAT_OPTION_HOP_SHIFT)

/**
 * An option to configure the delay between transmissions to be from start to start
 * instead of end to start. Delay must be long enough to cover the prior transmit's time.
 */
#define RAIL_TX_REPEAT_OPTION_START_TO_START (1 << RAIL_TX_REPEAT_OPTION_START_TO_START_SHIFT)

/// @struct RAIL_TxRepeatConfig_t
/// @brief A configuration structure for repeated transmits
///
/// @note The PA will always be ramped down and up in between transmits so
///   there will always be some minimum delay between transmits depending on the
///   ramp time configuration.
typedef struct RAIL_TxRepeatConfig {
  /**
   * The number of repeated transmits to run. A total of (iterations + 1)
   * transmits will go on-air in the absence of errors.
   */
  uint16_t iterations;
  /**
   * Repeat option(s) to apply.
   */
  RAIL_TxRepeatOptions_t repeatOptions;
  /**
   * Per-repeat delay or hopping configuration, depending on repeatOptions.
   */
  union {
    /**
     * When \ref RAIL_TX_REPEAT_OPTION_HOP is not set, specifies
     * the delay time between each repeated transmit. Specify \ref
     * RAIL_TRANSITION_TIME_KEEP to use the current \ref
     * RAIL_StateTiming_t::txToTx transition time setting.
     * When using \ref RAIL_TX_REPEAT_OPTION_START_TO_START the delay
     * must be long enough to cover the prior transmit's time.
     */
    RAIL_TransitionTime_t delay;
    /**
     * When \ref RAIL_TX_REPEAT_OPTION_HOP is set, this specifies
     * the channel hopping configuration to use when hopping between
     * repeated transmits. Per-hop delays are configured within each
     * \ref RAIL_TxChannelHoppingConfigEntry_t::delay rather than
     * this union's delay field.
     * When using \ref RAIL_TX_REPEAT_OPTION_START_TO_START the hop delay
     * must be long enough to cover the prior transmit's time.
     */
    RAIL_TxChannelHoppingConfig_t channelHopping;
  } delayOrHop;
} RAIL_TxRepeatConfig_t;

/// \ref RAIL_TxRepeatConfig_t::iterations initializer configuring infinite
/// repeated transmissions.
#define RAIL_TX_REPEAT_INFINITE_ITERATIONS (0xFFFFU)

/** @} */ // end of group Transmit

/******************************************************************************
 * Receive Structures
 *****************************************************************************/
/**
 * @addtogroup Receive
 * @{
 */

/**
 * @addtogroup Address_Filtering
 * @{
 */

/// A default address filtering match table for configurations that use only one
/// address field. The truth table for address matching is shown below.
///
/// |                | No Match | Address 0 | Address 1 | Address 2 | Address 3 |
/// |----------------|----------|-----------|-----------|-----------|-----------|
/// | __No Match__   |    0     |     1     |     1     |     1     |     1     |
/// | __Address 0__  |    1     |     1     |     1     |     1     |     1     |
/// | __Address 1__  |    1     |     1     |     1     |     1     |     1     |
/// | __Address 2__  |    1     |     1     |     1     |     1     |     1     |
/// | __Address 3__  |    1     |     1     |     1     |     1     |     1     |
///
#define ADDRCONFIG_MATCH_TABLE_SINGLE_FIELD (0x1FFFFFE)
/// A default address filtering match table for configurations that use two
/// address fields and want to match the same index in each. The truth
/// table for address matching is shown below.
///
/// |                | No Match | Address 0 | Address 1 | Address 2 | Address 3 |
/// |----------------|----------|-----------|-----------|-----------|-----------|
/// | __No Match__   |    0     |    0      |    0      |    0      |    0      |
/// | __Address 0__  |    0     |    1      |    0      |    0      |    0      |
/// | __Address 1__  |    0     |    0      |    1      |    0      |    0      |
/// | __Address 2__  |    0     |    0      |    0      |    1      |    0      |
/// | __Address 3__  |    0     |    0      |    0      |    0      |    1      |
#define ADDRCONFIG_MATCH_TABLE_DOUBLE_FIELD (0x1041040)

/// The maximum number of address fields that can be used by the address
/// filtering logic.
#define ADDRCONFIG_MAX_ADDRESS_FIELDS (2)

/**
 * @struct RAIL_AddrConfig_t
 * @brief A structure to configure the address filtering functionality in RAIL.
 */
typedef struct RAIL_AddrConfig {
  /**
   * A list of the start offsets for each field.
   *
   * These offsets are specified relative to the previous field's end.
   * For the first field, it is relative to either the beginning of the packet
   * or the end of the frame type byte if frame type decoding is enabled. If a
   * field is unused, it's offset should be set to 0.
   */
  uint8_t offsets[ADDRCONFIG_MAX_ADDRESS_FIELDS];

  /**
   * A list of the address field sizes.
   *
   * These sizes are specified in bytes from 0 to 8. If you choose a
   * size of 0, this field is effectively disabled.
   */
  uint8_t sizes[ADDRCONFIG_MAX_ADDRESS_FIELDS];

  /**
   * The truth table to determine how the two fields combine to create a match.
   *
   * For detailed information about how this truth table is formed, see the
   * detailed description of \ref Address_Filtering.
   *
   * For simple predefined configurations use the following defines.
   *  - ADDRCONFIG_MATCH_TABLE_SINGLE_FIELD
   *    - For filtering that only uses a single address field.
   *  - ADDRCONFIG_MATCH_TABLE_DOUBLE_FIELD for two field filtering where you
   *    - For filtering that uses two address fields in a configurations where
   *      you want the following logic `((Field_0, Index_0) && (Field_1, Index_0))
   *      || ((Field_0, Index_1) && (Field_1, Index_1)) || ...`
   */
  uint32_t matchTable;
} RAIL_AddrConfig_t;

/**
 * @brief A bitmask representation of which 4 filters passed for each
 *   \ref ADDRCONFIG_MAX_ADDRESS_FIELDS when filtering has completed
 *   successfully.
 *
 * It's layout is:
 * |  Bit 7 |  Bit 6 |  Bit 5 |  Bit 4 |  Bit 3 |  Bit 2 |  Bit 1 |  Bit 0 |
 * |--------+--------+--------+--------+--------+--------+--------+--------|
 * |   Second Address Field Nibble     |     First Address Field Nibble    |
 * | Addr 3 | Addr 2 | Addr 1 | Addr 0 | Addr 3 | Addr 2 | Addr 1 | Addr 0 |
 * |  match |  match |  match |  match |  match |  match |  match |  match |
 * |--------+--------+--------+--------+--------+--------+--------+--------|
 *
 * @note This information is valid in \ref RAIL_IEEE802154_Address_t on all
 *   platforms, but is only valid in \ref RAIL_RxPacketInfo_t on platforms
 *   where \ref RAIL_SUPPORTS_ADDR_FILTER_MASK is true.
 */
typedef uint8_t RAIL_AddrFilterMask_t;

/** @} */ // end of group Address_Filtering

/**
 * @enum RAIL_RxOptions_t
 * @brief Receive options, in reality a bitmask.
 */
RAIL_ENUM_GENERIC(RAIL_RxOptions_t, uint32_t) {
  /** Shift position of \ref RAIL_RX_OPTION_STORE_CRC bit. */
  RAIL_RX_OPTION_STORE_CRC_SHIFT = 0,
  /** Shift position of \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS bit. */
  RAIL_RX_OPTION_IGNORE_CRC_ERRORS_SHIFT = 1,
  /** Shift position of \ref RAIL_RX_OPTION_ENABLE_DUALSYNC bit. */
  RAIL_RX_OPTION_ENABLE_DUALSYNC_SHIFT = 2,
  /** Shift position of \ref RAIL_RX_OPTION_TRACK_ABORTED_FRAMES bit. */
  RAIL_RX_OPTION_TRACK_ABORTED_FRAMES_SHIFT = 3,
  /** Shift position of \ref RAIL_RX_OPTION_REMOVE_APPENDED_INFO bit. */
  RAIL_RX_OPTION_REMOVE_APPENDED_INFO_SHIFT = 4,
  /** Shift position of \ref RAIL_RX_OPTION_ANTENNA0 bit. */
  RAIL_RX_OPTION_ANTENNA0_SHIFT = 5,
  /** Shift position of \ref RAIL_RX_OPTION_ANTENNA1 bit. */
  RAIL_RX_OPTION_ANTENNA1_SHIFT = 6,
  /** Shift position of \ref RAIL_RX_OPTION_DISABLE_FRAME_DETECTION bit. */
  RAIL_RX_OPTION_DISABLE_FRAME_DETECTION_SHIFT = 7,
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /** Shift position of \ref RAIL_RX_OPTION_SKIP_DC_CAL bit. */
  RAIL_RX_OPTION_SKIP_DC_CAL_SHIFT = 8,
  /** Shift position of \ref RAIL_RX_OPTION_SKIP_SYNTH_CAL bit. */
  RAIL_RX_OPTION_SKIP_SYNTH_CAL_SHIFT = 9,
#endif //DOXYGEN_SHOULD_SKIP_THIS
  /** Shift position of \ref RAIL_RX_OPTION_CHANNEL_SWITCHING bit. */
  RAIL_RX_OPTION_CHANNEL_SWITCHING_SHIFT = 10,
  /** Shift position of \ref RAIL_RX_OPTION_FAST_RX2RX bit. */
  RAIL_RX_OPTION_FAST_RX2RX_SHIFT = 11,
  /** Shift position of \ref RAIL_RX_OPTION_ENABLE_COLLISION_DETECTION bit. */
  RAIL_RX_OPTION_ENABLE_COLLISION_DETECTION_SHIFT = 12,
};

/** A value representing no options enabled. */
#define RAIL_RX_OPTIONS_NONE 0
/** All options are disabled by default. */
#define RAIL_RX_OPTIONS_DEFAULT RAIL_RX_OPTIONS_NONE

/**
 * An option to configure whether the CRC portion of the packet is included in
 * the packet payload exposed to the app on packet reception.
 * Defaults to false.
 */
#define RAIL_RX_OPTION_STORE_CRC (1UL << RAIL_RX_OPTION_STORE_CRC_SHIFT)

/**
 * An option to configure whether CRC errors will be ignored.
 * If this is set, RX will still be successful, even if
 * the CRC does not pass the check. Defaults to false.
 *
 * @note An expected Ack that fails CRC with this option set
 *   will still be considered the expected Ack, terminating
 *   the \ref RAIL_AutoAckConfig_t::ackTimeout period.
 */
#define RAIL_RX_OPTION_IGNORE_CRC_ERRORS (1UL << RAIL_RX_OPTION_IGNORE_CRC_ERRORS_SHIFT)

/**
 * An option to control which sync words will be accepted. Setting it to
 * 0 (default) will cause the receiver to listen for SYNC1 only. Setting it to
 * 1 causes the receiver to listen for either SYNC1 or SYNC2. RX appended info
 * will contain which sync word was detected. Note, this only affects which
 * sync word(s) are received, but not what each of the sync words actually are.
 * This feature may not be available on some combinations of chips, PHYs, and
 * protocols. Use the compile time symbol \ref RAIL_SUPPORTS_DUAL_SYNC_WORDS or
 * the runtime call \ref RAIL_SupportsDualSyncWords() to check whether the
 * platform supports this feature. Also, DUALSYNC may be incompatible
 * with certain radio configurations. In these cases, setting this bit will
 * be ignored. See the data sheet or support team for more details.
 */
#define RAIL_RX_OPTION_ENABLE_DUALSYNC (1UL << RAIL_RX_OPTION_ENABLE_DUALSYNC_SHIFT)

/**
 * An option to configure whether frames which are aborted during reception
 * should continue to be tracked. Setting this option allows viewing Packet
 * Trace information for frames which get discarded. Defaults to false.
 *
 * This option is ignored when doing a \ref RAIL_IDLE_FORCE_SHUTDOWN or
 * \ref RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS.
 *
 * @note This option should not be used with coded PHYs since packet data
 *   received after the abort will not be decoded properly.
 */
#define RAIL_RX_OPTION_TRACK_ABORTED_FRAMES (1UL << RAIL_RX_OPTION_TRACK_ABORTED_FRAMES_SHIFT)

/**
 * An option to suppress capturing the appended information after
 * received frames. Defaults to false. When suppressed, certain
 * \ref RAIL_RxPacketDetails_t details will not be available for received
 * packets whose \ref RAIL_RxPacketStatus_t is among the RAIL_RX_PACKET_READY_
 * set.
 *
 * @warning This option should be changed only when the radio is idle
 *   and the receive FIFO is empty or has been reset,
 *   otherwise \ref RAIL_GetRxPacketInfo() and \ref RAIL_GetRxPacketDetails()
 *   may think appended info is packet data or vice-versa.
 */
#define RAIL_RX_OPTION_REMOVE_APPENDED_INFO (1UL << RAIL_RX_OPTION_REMOVE_APPENDED_INFO_SHIFT)

/**
 * An option to select the use of antenna 0 during receive (including
 * \ref Auto_Ack receive). If no antenna option is selected, the packet
 * will be received on the last antenna used for receive or transmit.
 * Defaults to false. This option is only valid on platforms that support
 * \ref Antenna_Control and have been configured via \ref RAIL_ConfigAntenna().
 */
#define RAIL_RX_OPTION_ANTENNA0 (1UL << RAIL_RX_OPTION_ANTENNA0_SHIFT)

/**
 * An option to select the use of antenna 1 during receive (including
 * \ref Auto_Ack receive). If no antenna option is selected, the packet
 * will be received on the last antenna used for receive or transmit.
 * Defaults to false. This option is only valid on platforms that support
 * \ref Antenna_Control and have been configured via \ref RAIL_ConfigAntenna().
 */
#define RAIL_RX_OPTION_ANTENNA1 (1UL << RAIL_RX_OPTION_ANTENNA1_SHIFT)

/**
 * An option combination to automatically choose an antenna during receive
 * (including \ref Auto_Ack receive). If both antenna 0 and antenna 1
 * options are set, the radio will dynamically switch between antennas
 * during packet detection and choose the best one for completing the
 * reception. This option is only valid when the antenna diversity
 * field is properly configured via Simplicity Studio.
 * This option is only valid on platforms that support
 * \ref Antenna_Control and have been configured via \ref RAIL_ConfigAntenna().
 */
#define RAIL_RX_OPTION_ANTENNA_AUTO (RAIL_RX_OPTION_ANTENNA0 | RAIL_RX_OPTION_ANTENNA1)

/**
 * An option to disable frame detection. This can be useful for doing energy
 * detection without risking packet reception. Enabling this will abort any
 * frame currently being received in addition to preventing further frames
 * from being received. Defaults to false.
 */
#define RAIL_RX_OPTION_DISABLE_FRAME_DETECTION (1UL << RAIL_RX_OPTION_DISABLE_FRAME_DETECTION_SHIFT)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
 * An option to skip DC calibration when transitioning from RX to RX. This can be
 * useful for reducing the state transition time, but risks impacting
 * receive capability. Enabling this bypasses DC calibration (like
 * \ref RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_DC_CAL)
 * Defaults to false.
 */
#define RAIL_RX_OPTION_SKIP_DC_CAL (1UL << RAIL_RX_OPTION_SKIP_DC_CAL_SHIFT)

/**
 * An option to skip synth calibration when transitioning from RX to RX. This can
 * be useful for reducing the state transition time, but risks impacting receive
 * capability. Enabling this bypasses synth calibration (like
 * \ref RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_SYNTH_CAL)
 * Defaults to false.
 */
#define RAIL_RX_OPTION_SKIP_SYNTH_CAL (1U << RAIL_RX_OPTION_SKIP_SYNTH_CAL_SHIFT)
#endif //DOXYGEN_SHOULD_SKIP_THIS

/**
 * An option to enable IEEE 802.15.4 RX channel switching.
 * See \ref RAIL_IEEE802154_ConfigRxChannelSwitching() for more information.
 * Defaults to false.
 *
 * @note This option is only supported on specific chips where
 *   \ref RAIL_IEEE802154_SUPPORTS_RX_CHANNEL_SWITCHING is true.
 *
 * @note This option overrides \ref RAIL_RX_OPTION_ANTENNA0,
 *   \ref RAIL_RX_OPTION_ANTENNA1 and \ref RAIL_RX_OPTION_ANTENNA_AUTO antenna
 *   selection options.
 */
#define RAIL_RX_OPTION_CHANNEL_SWITCHING (1U << RAIL_RX_OPTION_CHANNEL_SWITCHING_SHIFT)

/**
 * An option to enable fast RX2RX state transition.
 *
 * Once enabled, the sequencer will send the radio to RXSEARCH and get ready to
 * receive the next packet while still processing the previous one. This will
 * reduce RX to RX state transition time but risks impacting receive capability.
 *
 * @note This option is only supported on specific chips where
 *   \ref RAIL_SUPPORTS_FAST_RX2RX is true.
 */
#define RAIL_RX_OPTION_FAST_RX2RX (1U << RAIL_RX_OPTION_FAST_RX2RX_SHIFT)

/**
 * An option to enable collision detection.
 *
 * Once enabled, when a collision with a strong enough packet is detected, the demod
 * will stop the current packet decoding and try to detect the preamble of the incoming
 * packet.
 *
 * @note This option is only supported on specific chips where
 *   \ref RAIL_SUPPORTS_COLLISION_DETECTION is true.
 */
#define RAIL_RX_OPTION_ENABLE_COLLISION_DETECTION (1U << RAIL_RX_OPTION_ENABLE_COLLISION_DETECTION_SHIFT)

/** A value representing all possible options. */
#define RAIL_RX_OPTIONS_ALL 0xFFFFFFFFUL

/** The value returned by RAIL for an invalid RSSI, in dBm. */
#define RAIL_RSSI_INVALID_DBM     (-128)
/** The value returned by RAIL for an invalid RSSI: in quarter dBm. */
#define RAIL_RSSI_INVALID         ((int16_t)(RAIL_RSSI_INVALID_DBM * 4))
/** The lowest RSSI value returned by RAIL: in quarter dBm. */
#define RAIL_RSSI_LOWEST          ((int16_t)(RAIL_RSSI_INVALID + 1))

/** Maximum absolute value for RSSI offset */
#define RAIL_RSSI_OFFSET_MAX      35

/** A sentinel value to indicate waiting for a valid RSSI without a timeout. */
#define RAIL_GET_RSSI_WAIT_WITHOUT_TIMEOUT ((RAIL_Time_t)0xFFFFFFFFU)
/** A sentinel value to indicate no waiting for a valid RSSI. */
#define RAIL_GET_RSSI_NO_WAIT ((RAIL_Time_t)0U)

/**
 * @struct RAIL_ScheduleRxConfig_t
 * @brief Configures the scheduled RX algorithm.
 *
 * Defines the start and end times of the receive window created
 * for a scheduled receive. If either start or end times are disabled, they
 * will be ignored.
 */
typedef struct RAIL_ScheduleRxConfig {
  /**
   * The time to start receive. See startMode for more information about the
   * types of start times that you can specify.
   */
  RAIL_Time_t start;
  /**
   * How to interpret the time value specified in the start parameter. See the
   * \ref RAIL_TimeMode_t documentation for more information. Use
   * \ref RAIL_TIME_ABSOLUTE for absolute times, \ref RAIL_TIME_DELAY for times
   * relative to the current time and \ref RAIL_TIME_DISABLED to ignore the
   * start time.
   */
  RAIL_TimeMode_t startMode;
  /**
   * The time to end receive. See endMode for more information about the types
   * of end times you can specify.
   */
  RAIL_Time_t end;
  /**
   * How to interpret the time value specified in the end parameter. See the
   * \ref RAIL_TimeMode_t documentation for more information. Note that, in
   * this API, if you specify a \ref RAIL_TIME_DELAY, it is relative to the
   * start time if given and relative to now if none is specified. Also, using
   * \ref RAIL_TIME_DISABLED means that this window will not end unless you
   * explicitly call \ref RAIL_Idle() or add an end event through a future
   * update to this configuration.
   */
  RAIL_TimeMode_t endMode;
  /**
   * While in scheduled RX, you can still control the radio state via
   * state transitions. This option configures whether a transition
   * to RX goes back to scheduled RX or to the normal RX state. Once in the
   * normal RX state, you will effectively end the scheduled RX window and can
   * continue to receive indefinitely depending on the state transitions. Set
   * to 1 to transition to normal RX and 0 to stay in the scheduled RX.
   *
   * This setting also influences the posting of
   * \ref RAIL_EVENT_RX_SCHEDULED_RX_END when the scheduled Rx window is
   * implicitly ended by a packet receive (any of the
   * \ref RAIL_EVENTS_RX_COMPLETION events). See that event for details.
   *
   * @note An Rx transition to Idle state will always terminate the
   *   scheduled Rx window, regardless of this setting. This can be used
   *   to ensure Scheduled RX terminates on the first packet received
   *   (or first successful packet if the RX error transition is to Rx
   *   while the Rx success transition is to Idle).
   */
  uint8_t rxTransitionEndSchedule;
  /**
   * This setting tells RAIL what to do with a packet being received
   * when the window end event occurs. If set to 0, such a packet
   * will be allowed to complete. Any other setting will cause that
   * packet to be aborted. In either situation, any posting of
   * \ref RAIL_EVENT_RX_SCHEDULED_RX_END is deferred briefly to when
   * the packet's corresponding \ref RAIL_EVENTS_RX_COMPLETION occurs.
   */
  uint8_t hardWindowEnd;
} RAIL_ScheduleRxConfig_t;

/**
 * @enum RAIL_RxPacketStatus_t
 * @brief The packet status code associated with a packet received or
 *   currently being received.
 *
 * @note RECEIVING implies some packet data may be available, but
 *   is untrustworthy (not CRC-verified) and might disappear if the packet
 *   is rolled back on error. No packet details are yet available.
 * @note In RX \ref RAIL_DataMethod_t::FIFO_MODE, ABORT statuses imply some
 *   packet data may be available, but it's incomplete and not trustworthy.
 */
RAIL_ENUM(RAIL_RxPacketStatus_t) {
  /**
   * The radio is idle or searching for a packet.
   */
  RAIL_RX_PACKET_NONE = 0,
  /**
   * The packet was aborted during filtering because of illegal frame length,
   * CRC or block decoding errors, other RAIL built-in protocol-specific
   * packet content errors, or by the application or multiprotocol scheduler
   * idling the radio with \ref RAIL_IDLE_ABORT or higher.
   *
   * Corresponding \ref RAIL_EVENT_RX_PACKET_ABORTED is triggered.
   */
  RAIL_RX_PACKET_ABORT_FORMAT = 1,
  /**
   * The packet failed address filtering.
   *
   * Corresponding \ref RAIL_EVENT_RX_ADDRESS_FILTERED is triggered.
   */
  RAIL_RX_PACKET_ABORT_FILTERED = 2,
  /**
   * The packet passed any filtering but was aborted by the application
   * or multiprotocol scheduler idling the radio with \ref RAIL_IDLE_ABORT
   * or higher.
   *
   * Corresponding \ref RAIL_EVENT_RX_PACKET_ABORTED is triggered.
   */
  RAIL_RX_PACKET_ABORT_ABORTED = 3,
  /**
   * The packet overflowed the receive buffer.
   *
   * Corresponding \ref RAIL_EVENT_RX_FIFO_OVERFLOW is triggered.
   */
  RAIL_RX_PACKET_ABORT_OVERFLOW = 4,
  /**
   * The packet passed any filtering but subsequently failed CRC check(s)
   * block decoding, or illegal frame length, and was aborted.
   *
   * Corresponding \ref RAIL_EVENT_RX_FRAME_ERROR is triggered.
   */
  RAIL_RX_PACKET_ABORT_CRC_ERROR = 5,
  /**
   * The packet passed any filtering but subsequently failed CRC check(s)
   * with \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS in effect. Can also occur
   * when the packet prematurely ended successfully during filtering,
   * and either the \ref RAIL_EVENT_RX_PACKET_ABORTED or
   * \ref RAIL_EVENT_RX_ADDRESS_FILTERED events had been enabled
   * requesting notification of such packets.
   *
   * Corresponding \ref RAIL_EVENT_RX_PACKET_RECEIVED is triggered.
   */
  RAIL_RX_PACKET_READY_CRC_ERROR = 6,
  /**
   * The packet was successfully received, passing CRC check(s).
   *
   * Corresponding \ref RAIL_EVENT_RX_PACKET_RECEIVED is triggered.
   */
  RAIL_RX_PACKET_READY_SUCCESS = 7,
  /**
   * A packet is being received and is not yet complete.
   */
  RAIL_RX_PACKET_RECEIVING = 8,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_RX_PACKET_NONE            ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_NONE)
#define RAIL_RX_PACKET_ABORT_FORMAT    ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_ABORT_FORMAT)
#define RAIL_RX_PACKET_ABORT_FILTERED  ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_ABORT_FILTERED)
#define RAIL_RX_PACKET_ABORT_ABORTED   ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_ABORT_ABORTED)
#define RAIL_RX_PACKET_ABORT_OVERFLOW  ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_ABORT_OVERFLOW)
#define RAIL_RX_PACKET_ABORT_CRC_ERROR ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_ABORT_CRC_ERROR)
#define RAIL_RX_PACKET_READY_CRC_ERROR ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_READY_CRC_ERROR)
#define RAIL_RX_PACKET_READY_SUCCESS   ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_READY_SUCCESS)
#define RAIL_RX_PACKET_RECEIVING       ((RAIL_RxPacketStatus_t) RAIL_RX_PACKET_RECEIVING)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @typedef RAIL_RxPacketHandle_t
 * @brief A handle used to reference a packet during reception processing.
 *   There are several sentinel handle values that pertain to certain
 *   circumstances: \ref RAIL_RX_PACKET_HANDLE_INVALID, \ref
 *   RAIL_RX_PACKET_HANDLE_OLDEST, \ref RAIL_RX_PACKET_HANDLE_OLDEST_COMPLETE
 *   and \ref RAIL_RX_PACKET_HANDLE_NEWEST.
 */
typedef const void *RAIL_RxPacketHandle_t;

/** An invalid RX packet handle value. */
#define RAIL_RX_PACKET_HANDLE_INVALID  (NULL)

/** A special RX packet handle to refer to the oldest unreleased packet.
 * This includes the newest unread packet which is possibly incomplete or not
 * yet started.
 * This handle is used implicitly by \ref RAIL_ReadRxFifo().
 */
#define RAIL_RX_PACKET_HANDLE_OLDEST   ((RAIL_RxPacketHandle_t) 1)

/** A special RX packet handle to refer to the oldest unreleased
 *  complete packet. This never includes incomplete or unstarted packets.
 *  (Using \ref RAIL_RX_PACKET_HANDLE_OLDEST is inappropriate for this
 *  purpose because it can refer to an unstarted, incomplete, or
 *  unheld packet which are inappropriate to be consumed by the application.)
 */
#define RAIL_RX_PACKET_HANDLE_OLDEST_COMPLETE   ((RAIL_RxPacketHandle_t) 2)

/** A special RX packet handle to refer to the newest unreleased packet
 *  when in callback context. For a callback involving a completed
 *  receive event, this refers to the packet just completed. For
 *  other callback events, this refers to the next packet to be
 *  completed, which might be in-progress or might not have even
 *  started yet.
 */
#define RAIL_RX_PACKET_HANDLE_NEWEST   ((RAIL_RxPacketHandle_t) 3)

/**
 * @struct RAIL_RxPacketInfo_t
 * @brief Basic information about a packet being received or already
 *   completed and awaiting processing, including memory pointers to
 *   its data in the circular receive FIFO buffer. This packet information
 *   refers to remaining packet data that has not already been consumed
 *   by \ref RAIL_ReadRxFifo().
 *
 * @note Because the receive FIFO buffer is circular, a packet might start
 *   near the end of the buffer and wrap around to the beginning of
 *   the buffer to finish, hence the distinction between the first
 *   and last portions. Packets that fit without wrapping only have
 *   a first portion (firstPortionBytes == packetBytes and lastPortionData
 *   will be NULL).
 */
typedef struct RAIL_RxPacketInfo {
  /** The packet status of this packet. */
  RAIL_RxPacketStatus_t packetStatus;
  /** The number of packet data bytes available to read in this packet. */
  uint16_t packetBytes;
  /** The number of bytes in the first portion. */
  uint16_t firstPortionBytes;
  /**
   * The pointer to the first portion of packet data containing
   * firstPortionBytes number of bytes.
   */
  uint8_t *firstPortionData;
  /**
   * The pointer to the last portion of a packet, if any; NULL otherwise.
   * The number of bytes in this portion is
   * packetBytes - firstPortionBytes.
   */
  uint8_t *lastPortionData;
  /**
   * A bitmask representing which address filter(s) this packet has passed.
   * Will be 0 when not filtering or if packet info is retrieved before
   * filtering has completed. It's undefined on platforms lacking \ref
   * RAIL_SUPPORTS_ADDR_FILTER_MASK.
   */
  RAIL_AddrFilterMask_t filterMask;
} RAIL_RxPacketInfo_t;

/**
 * @struct RAIL_RxPacketDetails_t
 * @brief Received packet details obtained via \ref RAIL_GetRxPacketDetails()
 *   or RAIL_GetRxPacketDetailsAlt().
 *
 * @note Certain details are always available, while others are only available
 *   if the \ref RAIL_RxOptions_t \ref RAIL_RX_OPTION_REMOVE_APPENDED_INFO
 *   option is not in effect and the received packet's
 *   \ref RAIL_RxPacketStatus_t is among the RAIL_RX_PACKET_READY_ set.
 *   Each detail's description indicates its availability.
 *
 */
typedef struct RAIL_RxPacketDetails {
  /**
   * The timestamp of the received packet in the RAIL timebase.
   *
   * When not available it will be \ref RAIL_PACKET_TIME_INVALID.
   */
  RAIL_PacketTimeStamp_t timeReceived;
  /**
   * Indicates whether the CRC passed or failed for the received packet.
   * It is true for \ref RAIL_RX_PACKET_READY_SUCCESS packets and false
   * for all others.
   *
   * It is always available.
   */
  bool crcPassed;
  /**
   * Indicate whether the received packet was the expected Ack.
   * It is true for the expected Ack and false otherwise.
   *
   * It is always available.
   *
   * An expected Ack is defined as a protocol-correct Ack packet
   * successfully-received (\ref RAIL_RX_PACKET_READY_SUCCESS or
   * \ref RAIL_RX_PACKET_READY_CRC_ERROR) and whose sync word was
   * detected within the
   * RAIL_AutoAckConfig_t::ackTimeout period following a transmit
   * which specified \ref RAIL_TX_OPTION_WAIT_FOR_ACK, requested
   * an Ack, and Auto-Ack is enabled. When true, the ackTimeout
   * period was terminated so no \ref RAIL_EVENT_RX_ACK_TIMEOUT
   * will be subsequently posted for the transmit.
   *
   * A "protocol-correct Ack" applies to the 802.15.4 or Z-Wave
   * protocols for which RAIL can discern the frame type and match
   * the Ack's sequence number with that of the transmitted frame.
   * For other protocols, the first packet successfully-received
   * whose sync word was detected within the \ref
   * RAIL_AutoAckConfig_t::ackTimeout period is
   * considered the expected Ack; upper layers are responsible for
   * confirming this.
   */
  bool isAck;
  /**
   * RSSI of the received packet in integer dBm. This RSSI measurement is
   * started as soon as the sync word is detected. The duration of the
   * measurement is PHY-specific.
   *
   * When not available it will be \ref RAIL_RSSI_INVALID_DBM.
   */
  int8_t rssi;
  /**
   * The link quality indicator of the received packet. A zero would
   * indicate a very low quality packet while a 255 would indicate a very
   * high quality packet.
   *
   * When not available it will be 0.
   */
  uint8_t lqi;
  /**
   * For radios and PHY configurations that support multiple sync words, this
   * number is the ID of the sync word that was used for this packet.
   *
   * It is always available.
   */
  uint8_t syncWordId;
  /**
   * In configurations where the radio has the option of receiving a given
   * packet in multiple ways, indicates which of the sub-PHY options
   * was used to receive the packet. Most radio configurations do not have
   * this ability and the subPhyId is set to 0.
   *
   * Currently, this field is used by the BLE Coded PHY, the BLE Simulscan PHY
   * and the SUN OFDM PHYs.
   * In BLE cases, a value of 0 marks a 500 kbps packet, a value of 1 marks a 125
   * kbps packet, and a value of 2 marks a 1 Mbps packet.
   * Also, see \ref RAIL_BLE_ConfigPhyCoded() and \ref RAIL_BLE_ConfigPhySimulscan().
   *
   * In SUN OFDM cases, the value corresponds to the numerical value of the
   * Modulation and Coding Scheme (MCS) level of the last received packet.
   * The packet bitrate depends on the MCS value, as well as the OFDM option.
   * Packets bitrates for SUN OFDM PHYs can be found in 802.15.4-2020 specification,
   * chapter 20.3, table 20-10.
   * Ex: Packet bitrate for OFDM option 1 MCS0 is 100kb/s and 2400kb/s for MCS6.
   *
   * In WMBUS cases, when using PHY_wMbus_ModeTC_M2O_100k_frameA with simultaneous
   * RX of T and C modes enabled (\ref RAIL_WMBUS_Config()), the value corresponds
   * to \ref RAIL_WMBUS_Phy_t.
   *
   * It is always available.
   */
  uint8_t subPhyId;
  /**
   * For \ref Antenna_Control configurations where the device has multiple
   * antennas, this indicates which antenna received the packet. When there
   * is only one antenna, this will be set to the default of 0.
   *
   * It is always available.
   */
  uint8_t antennaId;
  /**
   * When channel hopping is enabled, this field will contain the index
   * of the channel in \ref RAIL_RxChannelHoppingConfig_t::entries on which
   * this packet was received, or a sentinel value.
   *
   * It is always available.
   */
  uint8_t channelHoppingChannelIndex;
  /**
   * The channel on which the packet was received.
   *
   * It is always available.
   *
   * @note It is best to fully process (empty or clear) the receive FIFO
   *   before changing channel configurations (\ref RAIL_ConfigChannels()
   *   or a built-in configuration) as unprocessed packets' channel
   *   could reflect the wrong configuration.
   */
  uint16_t channel;
} RAIL_RxPacketDetails_t;

/**
 * @typedef RAIL_ConvertLqiCallback_t
 * @brief A pointer to a function called before LQI is copied into the
 *   \ref RAIL_RxPacketDetails_t structure.
 *
 * @param[in] lqi The LQI value obtained by hardware and being readied for
 *   application consumption. This LQI value is in integral units ranging from
 *   0 to 255.
 * @param[in] rssi The RSSI value corresponding to the packet from which the
 *   hardware LQI value was obtained. This RSSI value is in integral dBm units.
 * @return uint8_t The converted LQI value that will be loaded into the
 *   \ref RAIL_RxPacketDetails_t structure in preparation for application
 *   consumption. This value should likewise be in integral units ranging from
 *   0 to 255.
 */
typedef uint8_t (*RAIL_ConvertLqiCallback_t)(uint8_t lqi,
                                             int8_t rssi);

/**
 * @struct RAIL_PrsLnaBypassConfig_t
 * @brief Configures the automatic PRS LNA bypass.
 */
typedef struct RAIL_PrsLnaBypassConfig {
  /**
   * Maximum time in microseconds to wait for frame detection after the LNA has
   * been bypassed. It must be greater than 0 to enable automatic PRS LNA
   * bypass with \ref RAIL_EnablePrsLnaBypass().
   */
  uint32_t timeoutUs;
  /**
   * Threshold (without unit) from which LNA bypass is turned on.
   * The table below shows EFR32XG25 thresholds corresponding to received power
   * level without the LNA gain.
   *
   * |  Level dB  | FSK_1a | FSK_1b | FSK_2a | FSK_2b | FSK_3 | FSK_4a | FSK_4b | FSK_5 | OFDM1 | OFDM2 | OFDM3 | OFDM4 |
   * |------------|--------|--------|--------|--------|-------|--------|--------|-------|-------|-------|-------|-------|
   * |   __-25__  |        |        |        |        |       |        |        |       |   9   |   9   |   9   |   10  |
   * |   __-20__  |        |    7   |    7   |    7   |   8   |    8   |    7   |   8   |   11  |   12  |   12  |   12  |
   * |   __-15__  |    7   |    10  |    10  |    10  |   9   |    9   |    10  |   10  |   14  |   14  |   14  |   15  |
   * |   __-10__  |    9   |    12  |    12  |    12  |   12  |    12  |    12  |   12  |   16  |   16  |   16  |   16  |
   * |   __-5__   |    11  |    14  |    14  |    14  |   16  |    16  |    14  |   16  |       |       |       |       |
   * |    __0__   |    14  |    17  |    18  |    17  |   17  |    18  |    18  |   18  |       |       |       |       |
   *
   * For example, with OFDM1 PHY, setting the threshold to 11 will turn on the
   * bypass when the power level at EFR32XG25 input is greater than -20 dB.
   */
  uint8_t threshold;
  /**
   * Compensation in dBm applied by RAIL to RSSI during LNA bypass. The RSSI
   * offset set using \ref RAIL_SetRssiOffset() must corespond to the case
   * with FEM LNA not bypassed. deltaRssiDbm is typically the FEM LNA gain
   * value.
   */
  uint8_t deltaRssiDbm;
  /**
   * PRS Channel used for the bypass.
   * PRS_GetFreeChannel() can be use to find a free channel. Then the signal
   * can be routed to GPIO pin and port using PRS_PinOutput(). This allows
   * logical operations with other PRS channels and so to adapt to the FEM
   * control logic table. Any call to PRS_Combine() with
   * \ref RAIL_PrsLnaBypassConfig_t::prsChannel as chA must be done after
   * the \ref RAIL_EnablePrsLnaBypass() call.
   */
  uint8_t prsChannel;
  /**
   * PRS signal polarity for bypass.
   *
   * With a polarity of 1, PRS signal is set to 1 for bypass and 0 for un-bypass.
   * with a polarity of 0, PRS signal is set to 0 for bypass and 1 for un-bypass.
   */
  bool polarity;
} RAIL_PrsLnaBypassConfig_t;

/** @} */ // end of group Receive

/******************************************************************************
 * Auto-Ack Structures
 *****************************************************************************/
/**
 * @addtogroup Auto_Ack
 * @{
 */
/**
 * @struct RAIL_AutoAckConfig_t
 * @brief Enable/disable the Auto-Ack algorithm, based on "enable".
 *
 * The structure provides a default state (the "success" of TX/RX transitions
 * when Acking is enabled) for the radio to return to after an Ack
 * operation occurs (transmitting or attempting to receive an Ack), or normal
 * state transitions to return to in the case Acking is
 * disabled. Regardless whether the Ack operation was successful, the
 * radio returns to the specified success state.
 *
 * ackTimeout specifies how long to stay in receive and wait for an Ack
 * to start (sync detected) before issuing a \ref RAIL_EVENT_RX_ACK_TIMEOUT
 * event and return to the default state.
 */
typedef struct RAIL_AutoAckConfig {
  /**
   * Indicate whether Auto-Acking should be enabled or disabled.
   */
  bool enable;
  // Unnamed 'uint8_t reserved1[1]' pad byte field here.
  /**
   * Define the RX Ack timeout duration in microseconds up to 65535
   * microseconds maximum. Only applied when Auto-Acking is enabled.
   * The Ack timeout timer starts at the completion of a \ref
   * RAIL_TX_OPTION_WAIT_FOR_ACK transmit and expires only while waiting
   * for a packet (prior to SYNC detect), triggering \ref
   * RAIL_EVENT_RX_ACK_TIMEOUT. During packet reception that event is
   * held off until packet completion and suppressed entirely if the
   * received packet is the expected Ack.
   */
  uint16_t ackTimeout;
  /**
   * State transitions to do after receiving a packet. When Auto-Acking is
   * enabled, the "error" transition is always ignored and the radio will
   * return to the "success" state after any Acking sequence
   * (\ref RAIL_RF_STATE_RX or \ref RAIL_RF_STATE_IDLE).
   * See \ref RAIL_ConfigAutoAck() for more details on this.
   */
  RAIL_StateTransitions_t rxTransitions;
  /**
   * State transitions to do after transmitting a packet. When Auto-Acking is
   * enabled, the "error" transition is always ignored and the radio will
   * return to the "success" state after any Acking sequence
   * (\ref RAIL_RF_STATE_RX or \ref RAIL_RF_STATE_IDLE).
   * See \ref RAIL_ConfigAutoAck() for more details on this.
   */
  RAIL_StateTransitions_t txTransitions;
} RAIL_AutoAckConfig_t;

/**
 * @def RAIL_AUTOACK_MAX_LENGTH
 * @brief Acknowledgment packets cannot be longer than 64 bytes.
 */
#define RAIL_AUTOACK_MAX_LENGTH (64U)

/** @} */ // end of group Auto_Ack

/******************************************************************************
 * Antenna Control
 *****************************************************************************/
/**
 * @addtogroup Antenna_Control
 * @{
 *
 * These enumerations and structures are used with RAIL Antenna Control API.
 * EFR32 supports up to two antennas with configurable pin locations.
 */

/** Antenna path Selection enumeration. */
RAIL_ENUM(RAIL_AntennaSel_t) {
  /** Enum for antenna path 0. */
  RAIL_ANTENNA_0 = 0,
  /** Enum for antenna path 1. */
  RAIL_ANTENNA_1 = 1,
  /** Enum for antenna path auto. */
  RAIL_ANTENNA_AUTO = 255,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_ANTENNA_0    ((RAIL_AntennaSel_t) RAIL_ANTENNA_0)
#define RAIL_ANTENNA_1    ((RAIL_AntennaSel_t) RAIL_ANTENNA_1)
#define RAIL_ANTENNA_AUTO ((RAIL_AntennaSel_t) RAIL_ANTENNA_AUTO)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_AntennaConfig_t
 * @brief A configuration for antenna selection.
 */
typedef struct RAIL_AntennaConfig {
  /** Antenna 0 Pin Enable */
  bool ant0PinEn;
  /** Antenna 1 Pin Enable */
  bool ant1PinEn;
  /**
   * A \ref RAIL_AntennaSel_t
   * value specifying the internal default RF path. It is ignored
   * on EFR32 parts that have only one RF path bonded
   * out and on EFR32xG28 dual-band OPNs where the appropriate
   * RF path is automatically set by RAIL to 0 for 2.4 GHz band
   * and 1 for Sub-GHz band PHYs. On EFR32xG23 and EFR32xG28
   * single-band OPNs where both RF paths are bonded out this can
   * be set to \ref RAIL_ANTENNA_AUTO to effect internal RF path
   * diversity on PHYs supporting diversity. This avoids the need
   * for an external RF switch and the associated GPIO(s) needed
   * to control its antenna selection.
   */
  uint8_t defaultPath;
  /** Antenna 0 output GPIO port */
  uint8_t ant0Port;
  /** Antenna 0 output GPIO pin */
  uint8_t ant0Pin;
  /** @deprecated No longer used (ignored). */
  uint8_t ant1Loc;
  /** Antenna 1 output GPIO port */
  uint8_t ant1Port;
  /** Antenna 1 output GPIO pin */
  uint8_t ant1Pin;
} RAIL_AntennaConfig_t;

/**
 * @deprecated Backwards compatible name for
 *   \ref RAIL_AntennaConfig_t::defaultPath field.
 */
#define ant0Loc defaultPath

/** @} */ // end of group Antenna_Control

/******************************************************************************
 * External_Thermistor Structures
 *****************************************************************************/
/**
 * @addtogroup External_Thermistor
 * @{
 */

/// A sentinel value to indicate an invalid thermistor measurement value.
#define RAIL_INVALID_THERMISTOR_VALUE (0xFFFFFFFFU)
/// A sentinel value to indicate an invalid PPM calculation value.
#define RAIL_INVALID_PPM_VALUE   (-128)

/**
 * @struct RAIL_HFXOThermistorConfig_t
 * @brief Configure the port and pin of the thermistor.
 *
 * @note This configuration is chip OPN dependent.
 */
typedef struct RAIL_HFXOThermistorConfig {
  /**
   * The GPIO port to access the thermistor.
   */
  uint8_t port;
  /**
   * The GPIO pin to set the thermistor.
   */
  uint8_t pin;
} RAIL_HFXOThermistorConfig_t;

/**
 * @struct RAIL_HFXOCompensationConfig_t
 * @brief Set compensation specific parameters
 */
typedef struct RAIL_HFXOCompensationConfig {
  /**
   * Indicates whether the HFXO compensation in temperature is activated.
   */
  bool enableCompensation;
  /**
   * The temperature reference delimiting the nominal zone from the critical one.
   * This field is relevant if enableCompensation is set to true.
   */
  int8_t zoneTemperatureC;
  /**
   * The temperature shift used to start a new compensation, in the nominal zone.
   * This field is relevant if enableCompensation is set to true.
   */
  uint8_t deltaNominal;
  /**
   * The temperature shift used to start a new compensation, in the critical zone.
   * This field is relevant if enableCompensation is set to true.
   */
  uint8_t deltaCritical;
} RAIL_HFXOCompensationConfig_t;

/** @} */ // end of group External_Thermistor

/******************************************************************************
 * Calibration Structures
 *****************************************************************************/
/**
 * @addtogroup Calibration
 * @{
 *
 * The EFR32 supports the Image Rejection (IR)
 * calibration and a temperature-dependent calibration. The IR calibration
 * can be computed once and stored off or computed each time at
 * startup. Because it is PHY-specific and provides sensitivity improvements,
 * it is highly recommended. The IR calibration should only be run when the
 * radio is IDLE.
 *
 * The temperature-dependent calibrations are used to recalibrate the synth if
 * the temperature crosses 0C or the temperature delta since the last
 * calibration exceeds 70C while in receive. RAIL will run the VCO calibration
 * automatically upon entering receive or transmit states, so the application
 * can omit this calibration if the stack re-enters receive or transmit with
 * enough frequency to avoid reaching the temperature delta. If the application
 * does not calibrate for temperature, it's possible to miss receive packets due
 * to a drift in the carrier frequency.
 */

/**
 * @typedef RAIL_CalMask_t
 * @brief A calibration mask type
 *
 * This type is a bitmask of different RAIL calibration values. The exact
 * meaning of these bits depends on what a particular chip supports.
 */
typedef uint32_t RAIL_CalMask_t;

/** EFR32-specific temperature calibration bit. */
#define RAIL_CAL_TEMP_VCO         (0x00000001U)
/** EFR32-specific HFXO temperature check bit.
 *  (Ignored if platform lacks \ref RAIL_SUPPORTS_HFXO_COMPENSATION.) */
#define RAIL_CAL_TEMP_HFXO        (0x00000002U)
/** EFR32-specific HFXO compensation bit.
 *  (Ignored if platform lacks \ref RAIL_SUPPORTS_HFXO_COMPENSATION.) */
#define RAIL_CAL_COMPENSATE_HFXO  (0x00000004U)
/** EFR32-specific IR calibration bit. */
#define RAIL_CAL_RX_IRCAL         (0x00010000U)
/** EFR32-specific Tx IR calibration bit.
 *  (Ignored if platform lacks \ref RAIL_SUPPORTS_OFDM_PA.) */
#define RAIL_CAL_OFDM_TX_IRCAL    (0x00100000U)

/** A mask to run EFR32-specific IR calibrations. */
#define RAIL_CAL_ONETIME_IRCAL    (RAIL_CAL_RX_IRCAL | RAIL_CAL_OFDM_TX_IRCAL)
/** A mask to run temperature-dependent calibrations. */
#define RAIL_CAL_TEMP             (RAIL_CAL_TEMP_VCO | RAIL_CAL_TEMP_HFXO | RAIL_CAL_COMPENSATE_HFXO)
/** A mask to run one-time calibrations. */
#define RAIL_CAL_ONETIME          (RAIL_CAL_ONETIME_IRCAL)
/** A mask to run optional performance calibrations. */
#define RAIL_CAL_PERF             (0)
/** A mask for calibrations that require the radio to be off. */
#define RAIL_CAL_OFFLINE          (RAIL_CAL_ONETIME_IRCAL)
/** A mask to run all possible calibrations for this chip. */
#define RAIL_CAL_ALL              (RAIL_CAL_TEMP | RAIL_CAL_ONETIME)
/** A mask to run all pending calibrations. */
#define RAIL_CAL_ALL_PENDING      (0x00000000U)
/** An invalid calibration value. */
#define RAIL_CAL_INVALID_VALUE    (0xFFFFFFFFU)

/**
 * @def RAIL_MAX_RF_PATHS
 * @brief Indicates the maximum number of RF Paths supported across all
 *   platforms.
 */
#define RAIL_MAX_RF_PATHS 2

/**
 * RAIL_RxIrCalValues_t
 * @brief RX IR calibration values.
 *
 * Platforms with fewer \ref RAIL_RF_PATHS than \ref RAIL_MAX_RF_PATHS
 * will only respect and update \ref RAIL_RF_PATHS indices and ignore
 * the rest.
 */
typedef uint32_t RAIL_RxIrCalValues_t[RAIL_MAX_RF_PATHS];

/**
 * A define to set all \ref RAIL_RxIrCalValues_t values to uninitialized.
 *
 * This define can be used when you have no data to pass to the calibration
 * routines but wish to compute and save all possible calibrations.
 */
#define RAIL_IRCALVALUES_RX_UNINIT {                        \
    [0 ... RAIL_MAX_RF_PATHS - 1] = RAIL_CAL_INVALID_VALUE, \
}

/**
 * @struct RAIL_TxIrCalValues_t
 * @brief A Tx IR calibration value structure.
 *
 * This definition contains the set of persistent calibration values for
 * OFDM on EFR32. You can set these beforehand and apply them at startup
 * to save the time required to compute them. Any of these values may be
 * set to \ref RAIL_CAL_INVALID_VALUE to force the code to compute that
 * calibration value.
 *
 * Only supported on platforms with \ref RAIL_SUPPORTS_OFDM_PA enabled.
 */
typedef struct RAIL_TxIrCalValues {
  /** TXIRCAL result */
  uint32_t dcOffsetIQ;
  /** TXIRCAL result */
  uint32_t phiEpsilon;
} RAIL_TxIrCalValues_t;

/**
 * A define to set all \ref RAIL_TxIrCalValues_t values to uninitialized.
 *
 * This define can be used when you have no data to pass to the calibration
 * routines but wish to compute and save all possible calibrations.
 */
#define RAIL_IRCALVALUES_TX_UNINIT  {     \
    .dcOffsetIQ = RAIL_CAL_INVALID_VALUE, \
    .phiEpsilon = RAIL_CAL_INVALID_VALUE, \
}

/**
 * @struct RAIL_IrCalValues_t
 * @brief An IR calibration value structure.
 *
 * This definition contains the set of persistent calibration values for
 * EFR32. You can set these beforehand and apply them at startup to save the
 * time required to compute them. Any of these values may be set to
 * \ref RAIL_CAL_INVALID_VALUE to force the code to compute that calibration value.
 */
typedef struct RAIL_IrCalValues {
  /** RX Image Rejection (IR) calibration value(s) */
  RAIL_RxIrCalValues_t rxIrCalValues;
  /** TX Image Rejection (IR) calibration value(s) for OFDM */
  RAIL_TxIrCalValues_t txIrCalValues;
} RAIL_IrCalValues_t;

/**
 * A define to set all \ref RAIL_IrCalValues_t values to uninitialized.
 *
 * This define can be used when you have no data to pass to the calibration
 * routines but wish to compute and save all possible calibrations.
 */
#define RAIL_IRCALVALUES_UNINIT {                \
    .rxIrCalValues = RAIL_IRCALVALUES_RX_UNINIT, \
    .txIrCalValues = RAIL_IRCALVALUES_TX_UNINIT, \
}

/**
 * A define allowing Rx calibration value access compatibility
 * between non-OFDM and OFDM platforms.
 */
#define RAIL_IRCALVAL(irCalStruct, rfPath) \
  (((RAIL_IrCalValues_t *)(&(irCalStruct)))->rxIrCalValues[(rfPath)])

/**
 * @typedef RAIL_CalValues_t
 * @brief A calibration value structure.
 *
 * This structure contains the set of persistent calibration values for
 * EFR32. You can set these beforehand and apply them at startup to save the
 * time required to compute them. Any of these values may be set to \ref
 * RAIL_CAL_INVALID_VALUE to force the code to compute that calibration value.
 */
typedef RAIL_IrCalValues_t RAIL_CalValues_t;

/**
 * A define to set all \ref RAIL_CalValues_t values to uninitialized.
 *
 * This define can be used when you have no data to pass to the calibration
 * routines but wish to compute and save all possible calibrations.
 */
#define RAIL_CALVALUES_UNINIT RAIL_IRCALVALUES_UNINIT

/// Use this value with either TX or RX values in \ref RAIL_SetPaCTune()
/// to use whatever value is already set and do no update. This
/// value is provided to provide consistency across EFR32 chips,
/// but technically speaking, all PA capacitance tuning values are
/// invalid on EFR32xG21 parts, as \ref RAIL_SetPaCTune() is not supported
/// on those parts.
#define RAIL_PACTUNE_IGNORE (255U)

/** @} */ // end of group Calibration

/******************************************************************************
 * RF Sense Structures
 *****************************************************************************/
/**
 * @addtogroup Rf_Sense
 * @{
 */

/**
 * A pointer to an RF Sense callback function.
 *
 * Consider using the event \ref RAIL_EVENT_RF_SENSED as an alternative.
 */
typedef void (*RAIL_RfSense_CallbackPtr_t)(void);

/**
 * RF Sense low sensitivity offset.
 */
#define RAIL_RFSENSE_LOW_SENSITIVITY_OFFSET   (0x20U)

/**
 * @enum RAIL_RfSenseBand_t
 * @brief An enumeration for specifying the RF Sense frequency band.
 */
RAIL_ENUM(RAIL_RfSenseBand_t) {
  /** RF Sense is disabled. */
  RAIL_RFSENSE_OFF,
  /** RF Sense is in 2.4 GHz band. */
  RAIL_RFSENSE_2_4GHZ,
  /** RF Sense is in Sub-GHz band. */
  RAIL_RFSENSE_SUBGHZ,
  /** RF Sense is in both bands. */
  RAIL_RFSENSE_ANY,
  /**
   * A count of the basic choices in this enumeration.
   * Must be last before sensitivity options.
   */
  RAIL_RFSENSE_MAX,

  /** RF Sense is in low sensitivity 2.4 GHz band */
  RAIL_RFSENSE_2_4GHZ_LOW_SENSITIVITY = RAIL_RFSENSE_LOW_SENSITIVITY_OFFSET + RAIL_RFSENSE_2_4GHZ,
  /** RF Sense is in low sensitivity Sub-GHz band */
  RAIL_RFSENSE_SUBGHZ_LOW_SENSITIVITY = RAIL_RFSENSE_LOW_SENSITIVITY_OFFSET + RAIL_RFSENSE_SUBGHZ,
  /** RF Sense is in low sensitivity for both bands. */
  RAIL_RFENSE_ANY_LOW_SENSITIVITY = RAIL_RFSENSE_LOW_SENSITIVITY_OFFSET + RAIL_RFSENSE_ANY,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_RFSENSE_OFF                    ((RAIL_RfSenseBand_t) RAIL_RFSENSE_OFF)
#define RAIL_RFSENSE_2_4GHZ                 ((RAIL_RfSenseBand_t) RAIL_RFSENSE_2_4GHZ)
#define RAIL_RFSENSE_SUBGHZ                 ((RAIL_RfSenseBand_t) RAIL_RFSENSE_SUBGHZ)
#define RAIL_RFSENSE_ANY                    ((RAIL_RfSenseBand_t) RAIL_RFSENSE_ANY)
#define RAIL_RFSENSE_MAX                    ((RAIL_RfSenseBand_t) RAIL_RFSENSE_MAX)
#define RAIL_RFSENSE_2_4GHZ_LOW_SENSITIVITY ((RAIL_RfSenseBand_t) RAIL_RFSENSE_2_4GHZ_LOW_SENSITIVITY)
#define RAIL_RFSENSE_SUBGHZ_LOW_SENSITIVITY ((RAIL_RfSenseBand_t) RAIL_RFSENSE_SUBGHZ_LOW_SENSITIVITY)
#define RAIL_RFENSE_ANY_LOW_SENSITIVITY     ((RAIL_RfSenseBand_t) RAIL_RFENSE_ANY_LOW_SENSITIVITY)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * Use the MODEM default sync word.
 */
#define RAIL_RFSENSE_USE_HW_SYNCWORD    (0U)

/**
 * @struct RAIL_RfSenseSelectiveOokConfig_t
 * @brief Structure to configure RFSENSE Selective(OOK) mode.
 */
typedef struct RAIL_RfSenseSelectiveOokConfig {
  /**
   * The frequency band(s) on which to sense the
   * RF energy. To stop RF Sense, specify \ref RAIL_RFSENSE_OFF.
   */
  RAIL_RfSenseBand_t band;
  /**
   * Syncword Length in bytes, 1-4 bytes.
   * @note When \ref syncWord is set to use \ref RAIL_RFSENSE_USE_HW_SYNCWORD,
   *   the \ref syncWordNumBytes value will be ignored since we rely on the
   *   HW default settings for sync word.
   */
  uint8_t syncWordNumBytes;
  /**
   * Sync Word Value.
   * To use HW default sync word, set to \ref RAIL_RFSENSE_USE_HW_SYNCWORD.
   */
  uint32_t syncWord;
  /**
   * The callback function, called when RF is sensed.
   *
   * @note Set to NULL and instead use \ref RAIL_EVENT_RF_SENSED or poll
   *   via \ref RAIL_IsRfSensed().
   */
  RAIL_RfSense_CallbackPtr_t cb;
} RAIL_RfSenseSelectiveOokConfig_t;

/** @} */ // end of group Rf_Sense

/******************************************************************************
 * RX Channel Hopping
 *****************************************************************************/
/**
 * @addtogroup Rx_Channel_Hopping RX Channel Hopping
 * @{
 */

/**
 * @enum RAIL_RxChannelHoppingMode_t
 * @brief Modes by which RAIL can determine when to proceed to the next
 * channel during channel hopping
 */
RAIL_ENUM(RAIL_RxChannelHoppingMode_t) {
  /**
   * Switch to the next channel each time the radio re-enters RX after
   * packet reception or a transmit based on the corresponding \ref
   * State_Transitions. A hop can also be manually triggered by calling
   * \ref RAIL_TriggerRxChannelHop() while the radio is listening.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL = 0,
  /**
   * Switch to the next channel after a certain amount of time passes.
   * The time should be specified in microseconds in \ref
   * RAIL_RxChannelHoppingConfigEntry_t::parameter, and must be less
   * than \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT = 1,
  /**
   * Listen in receive RX for at least a specified timeout. If,
   * by the end of the timeout, the radio has packet timing,
   * remain in the current channel until the radio loses it. The
   * timeout should be specified in microseconds in \ref
   * RAIL_RxChannelHoppingConfigEntry_t::parameter, and must be less
   * than \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE = 2,
  /**
   * Listen in receive RX for at least a specified timeout. If,
   * by the end of the timeout, the radio has a packet preamble,
   * remain in the current channel until the radio loses it. The
   * timeout should be specified in microseconds in \ref
   * RAIL_RxChannelHoppingConfigEntry_t::parameter, and must be less
   * than \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE = 3,
  /**
   * Placeholder for a reserved hopping mode that is not supported.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1 = 4,
  /**
   * A mode that combines modes TIMING_SENSE, PREAMBLE_SENSE, and
   * TIMEOUT (sync detect) all running in parallel. Refer to \ref
   * RAIL_RxChannelHoppingConfigMultiMode_t for further details.
   * A pointer to that structure, allocated in global read-write
   * memory and initialized to the desired configuration values, is
   * specified as the \ref RAIL_RxChannelHoppingConfigEntry_t::parameter
   * or \ref RAIL_RxDutyCycleConfig_t::parameter, cast appropriately:
   * @code{.c}
   *   .parameter = (uint32_t)(void *)&hoppingConfigMultiMode,
   * @endcode
   *
   * @note RAIL will overwrite the contents of the \ref
   *   RAIL_RxChannelHoppingConfigMultiMode_t during operation so it
   *   must be reinitialized with the desired configuration prior to
   *   every call to \ref RAIL_ConfigRxChannelHopping() or
   *   \ref RAIL_ConfigRxDutyCycle().
   *
   * @note Multiple \ref RAIL_RxChannelHoppingConfigEntry_t entries may
   *   share the same \ref RAIL_RxChannelHoppingConfigMultiMode_t if their
   *   settings are identical, otherwise a separate \ref
   *   RAIL_RxChannelHoppingConfigMultiMode_t is needed for each
   *   \ref RAIL_RxChannelHoppingConfigEntry_t that uses this mode.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE = 5,
  /**
   * Switch to the next channel based on the demodulation settings in the PHY
   * config. This mode is PHY and chip dependent. The
   * \ref RAIL_RxChannelHoppingConfigEntry_t::parameter is ignored, and should
   * be set to 0 for future compatibility.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_SQ = 6,
  /**
   * Marks that the channel is concurrent with another channel, and otherwise
   * behaves identically to \ref RAIL_RX_CHANNEL_HOPPING_MODE_SQ.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_CONC = 7,
  /**
   * Indicates that this channel is a virtual channel that is concurrently
   * detected with the channel indicated by the
   * \ref RAIL_RxChannelHoppingConfigEntry_t::parameter. Otherwise behaves
   * identically to \ref RAIL_RX_CHANNEL_HOPPING_MODE_SQ.
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_VT = 8,
  /**
   * This is the transmit channel used for Auto-Ack if the regular channel,
   * specified in RAIL_RxChannelHoppingConfigEntry::parameter, is
   * optimized for RX which may degrade some TX performance
   */
  RAIL_RX_CHANNEL_HOPPING_MODE_TX = 9,
  /**
   * A count of the basic choices in this enumeration.
   * Must be last before _WITH_OPTIONS twins.
   */
  RAIL_RX_CHANNEL_HOPPING_MODES_COUNT,

  /**
   * The start of equivalent modes requiring non-default \ref
   * RAIL_RxDutyCycleConfig_t::options, needed for backwards-compatibility
   * with earlier \ref RAIL_RxDutyCycleConfig_t format. Non-default options
   * are supported with \ref RAIL_RxChannelHoppingConfigEntry_t in all modes.
   */
  RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE = 0x80,
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1 with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_SQ with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_SQ_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_SQ),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_CONC with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_CONC_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_CONC),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_VT with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_VT_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_VT),
  /** Variant of \ref RAIL_RX_CHANNEL_HOPPING_MODE_TX with options. */
  RAIL_RX_CHANNEL_HOPPING_MODE_TX_WITH_OPTIONS
    = (RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE
       + RAIL_RX_CHANNEL_HOPPING_MODE_TX),
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL                      ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL)
#define RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT                     ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT)
#define RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE                ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE)
#define RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE              ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE)
#define RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1                   ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1)
#define RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE                 ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE)
#define RAIL_RX_CHANNEL_HOPPING_MODE_SQ                          ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_SQ)
#define RAIL_RX_CHANNEL_HOPPING_MODE_CONC                        ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_CONC)
#define RAIL_RX_CHANNEL_HOPPING_MODE_VT                          ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_VT)
#define RAIL_RX_CHANNEL_HOPPING_MODE_TX                          ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_TX)
#define RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE          ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODES_WITH_OPTIONS_BASE)
#define RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL_WITH_OPTIONS         ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT_WITH_OPTIONS        ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_TIMEOUT_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE_WITH_OPTIONS   ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_TIMING_SENSE_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE_WITH_OPTIONS ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1_WITH_OPTIONS      ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_RESERVED1_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE_WITH_OPTIONS    ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_SQ_WITH_OPTIONS             ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_SQ_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_CONC_WITH_OPTIONS           ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_CONC_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_VT_WITH_OPTIONS             ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_VT_WITH_OPTIONS)
#define RAIL_RX_CHANNEL_HOPPING_MODE_TX_WITH_OPTIONS             ((RAIL_RxChannelHoppingMode_t) RAIL_RX_CHANNEL_HOPPING_MODE_TX_WITH_OPTIONS)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * The maximum sense time supported for those \ref RAIL_RxChannelHoppingMode_t
 * modes whose parameter(s) specify a sensing time.
 */
#define RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US 0x08000000UL

/**
 * @enum RAIL_RxChannelHoppingDelayMode_t
 * @deprecated Set only to RAIL_RX_CHANNEL_DELAY_MODE_STATIC.
 */
RAIL_ENUM(RAIL_RxChannelHoppingDelayMode_t) {
  /**
   * Always delay for exactly the amount of time specified
   * in the delay parameter, regardless of how other channel
   * hopping channels were extended via preamble sense or other means.
   */
  RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC = 0,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC ((RAIL_RxChannelHoppingDelayMode_t) RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @typedef RAIL_RxChannelHoppingParameter_t
 * @brief Rx channel hopping on-channel time
 */
typedef uint32_t RAIL_RxChannelHoppingParameter_t;

/**
 * @enum RAIL_RxChannelHoppingOptions_t
 * @brief Options that can customize channel hopping behavior
 *   on a per-hop basis.
 */
RAIL_ENUM(RAIL_RxChannelHoppingOptions_t) {
  /** Shift position of \ref RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_SYNTH_CAL bit. */
  RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_SYNTH_CAL_SHIFT = 0,
  /** Shift position of \ref RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_DC_CAL bit. */
  RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_DC_CAL_SHIFT = 1,
  /** Shift position of \ref RAIL_RX_CHANNEL_HOPPING_OPTION_RSSI_THRESHOLD bit. */
  RAIL_RX_CHANNEL_HOPPING_OPTION_RSSI_THRESHOLD_SHIFT = 2,
  /** Shift position of \ref RAIL_RX_CHANNEL_HOPPING_OPTION_STOP bit. */
  RAIL_RX_CHANNEL_HOPPING_OPTION_STOP_SHIFT = 3,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_RX_CHANNEL_HOPPING_OPTIONS_COUNT
};

/** A value representing no options enabled. */
#define RAIL_RX_CHANNEL_HOPPING_OPTIONS_NONE 0U
/**
 * All options disabled by default.
 * Channel hopping will behave as described by other
 * parameters as it did in RAIL 2.7 and earlier.
 */
#define RAIL_RX_CHANNEL_HOPPING_OPTIONS_DEFAULT RAIL_RX_CHANNEL_HOPPING_OPTIONS_NONE
/**
 * @deprecated Please use \ref RAIL_RX_CHANNEL_HOPPING_OPTIONS_DEFAULT instead.
 */
#define RAIL_RX_CHANNEL_HOPPING_OPTION_DEFAULT RAIL_RX_CHANNEL_HOPPING_OPTIONS_DEFAULT
/**
 * An option to skip synth calibration while *hopping into* the channel
 * specified in the current entry.
 */
#define RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_SYNTH_CAL (1U << RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_SYNTH_CAL_SHIFT)
/**
 * An option to skip DC calibration while *hopping into* the channel
 * specified in the current entry.
 */
#define RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_DC_CAL (1U << RAIL_RX_CHANNEL_HOPPING_OPTION_SKIP_DC_CAL_SHIFT)
/**
 * An option to check RSSI after *hopping into* the channel specified
 * in the current entry and hop if that RSSI is below the threshold
 * specified in \ref RAIL_RxChannelHoppingConfigEntry_t::rssiThresholdDbm.
 * This check runs in parallel with the \ref RAIL_RxChannelHoppingMode_t
 * specified and may cause a hop sooner than that mode otherwise would.
 */
#define RAIL_RX_CHANNEL_HOPPING_OPTION_RSSI_THRESHOLD (1U << RAIL_RX_CHANNEL_HOPPING_OPTION_RSSI_THRESHOLD_SHIFT)
/**
 * An option to stop the hopping sequence at this entry in the hop
 * table, which also idles the radio. Intended only for testing
 * purposes and not supported on EFR32xG21.
 */
#define RAIL_RX_CHANNEL_HOPPING_OPTION_STOP (1U << RAIL_RX_CHANNEL_HOPPING_OPTION_STOP_SHIFT)

/// @struct RAIL_RxChannelHoppingConfigMultiMode_t
/// @brief Structure that parameterizes \ref
///   RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE.
///
/// Every \ref RAIL_RxChannelHoppingConfigEntry_t or
/// \ref RAIL_RxDutyCycleConfig_t that uses \ref
/// RAIL_RX_CHANNEL_HOPPING_MODE_MULTI_SENSE must allocate one of these
/// structures in global read-write memory to provide the settings
/// for this mode and for RAIL to use during hopping or duty cycling.
/// A pointer to this structure, cast appropriately, is what is passed
/// in the corresponding \ref RAIL_RxChannelHoppingConfigEntry_t::parameter
/// or \ref RAIL_RxDutyCycleConfig_t::parameter.
///
/// The contents of this structure must be initialized prior to each
/// \ref RAIL_ConfigRxChannelHopping() or \ref RAIL_ConfigRxDutyCycle()
/// call and must not be touched thereafter until the next such call.
/// RAIL may change these contents during configuration or operation.
///
/// This mode of operation functions algorithmically like this pseudocode:
/// @code{.c}
/// extern bool channelHopping; // true if channel hopping, false if duty cycling
/// extern RAIL_RxChannelHoppingConfigEntry_t *hopConfigEntry; // current channel
///
/// static RAIL_RxChannelHoppingConfigMultiMode_t *multiParams;
/// static RAIL_Time_t rxStartTime;
/// static bool preambleSensed;
///
/// static void hopOrSuspendRx(uint32_t delay)
/// {
///   disableDemodEvents();
///   disableTimerEvents();
///   stopTimer();
///   if (channelHopping) {
///     hopToNextChannel(delay, &hopConfigEntry); // updates hopConfigEntry
///   } else {
///     suspendRx(delay);
///   }
///   onStartRx(); // resume receive after delay (on new channel if hopping)
/// }
///
/// void onStartRx(void) // called upon entry to receive
/// {
///   rxStartTime = RAIL_GetTime();
///   multiParams = (RAIL_RxChannelHoppingConfigMultiMode_t *)
///                 (void *)hopConfigEntry->parameter;
///   startTimer(rxStartTime + multiParams->timingSense);
///   preambleSensed = false;
///   enableTimerEvents(); // timer will trigger onTimerEvent() handler
///   enableDemodEvents(); // demod will trigger onDemodEvent() handler
/// }
///
/// void onTimerEvent(void) // called when timer expires
/// {
///   hopOrSuspendRx(hopConfigEntry->delay);
/// }
///
/// void onDemodEvent(void) // called when demodulator state changes
/// {
///   if (DEMOD_TIMING_SENSED) {
///     stopTimer();
///     startTimer(rxStartTime + multiParams->syncDetect);
///   }
///   if (DEMOD_TIMING_LOST) {
///     stopTimer();
///     uint32_t newTimeout = RAIL_GetTime() + multiParams->timingReSense;
///     uint32_t limitTimeout;
///     if (preambleSensed) {
///       limitTimeout = rxStartTime + multiParams->syncDetect;
///     } else {
///       limitTimeout = rxStartTime + multiParams->preambleSense;
///     }
///     if (newTimeout > limitTimeout) {
///       newTimeout = limitTimeout;
///     }
///     if (newTimeout > RAIL_GetTime()) {
///       startTimer(newTimeout);
///     } else {
///       hopOrSuspendRx(hopConfigEntry->delay);
///     }
///   }
///   if (DEMOD_PREAMBLE_SENSED) {
///     preambleSensed = true;
///   }
///   if (DEMOD_PREAMBLE_LOST) {
///     preambleSensed = false;
///   }
///   if (DEMOD_SYNC_DETECTED) {
///     disableDemodEvents();
///     disableTimerEvents();
///     stopTimer();
///     receivePacket(); // stay on channel to receive frame
///     hopOrSuspendRx(0); // continue RX per state transitions with no delay
///   }
/// }
/// @endcode

typedef struct RAIL_RxChannelHoppingConfigMultiMode {
  /**
   * Switch to the next channel if sync is not detected before
   * this time, in microseconds, measured from entry to Rx.
   * This must be greater than preambleSense and less than
   * \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  uint32_t syncDetect;
  /**
   * Switch to the next channel if timing was sensed but then
   * lost after this time, in microseconds, measured from entry
   * to Rx -- unless preamble had been sensed in which case any
   * switching is deferred to timingReSense and, if timing is
   * regained, to syncDetect. This must be greater than timingSense
   * and less than \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  uint32_t preambleSense;
  /**
   * Switch to the next channel if timing is not sensed before
   * this time, in microseconds, measured from entry to Rx. This
   * must be greater than 2 and less than
   * \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  uint32_t timingSense;
  /**
   * Switch to the next channel if timing was sensed but then
   * lost and not regained before this time, in microseconds,
   * measured from when timing was lost. This must be less than
   * \ref RAIL_RX_CHANNEL_HOPPING_MAX_SENSE_TIME_US.
   */
  uint32_t timingReSense;
  /**
   * Set this to 0. This field, along with the others, may be
   * used internally by RAIL during configuration or operation.
   */
  uint32_t status;
} RAIL_RxChannelHoppingConfigMultiMode_t;

/**
 * @struct RAIL_RxChannelHoppingConfigEntry_t
 * @brief Structure that represents one of the channels that is part of a
 *   \ref RAIL_RxChannelHoppingConfig_t sequence of channels used in
 *   channel hopping.
 */
typedef struct RAIL_RxChannelHoppingConfigEntry {
  /**
   * The channel number to be used for this entry in the channel hopping
   * sequence. If this is an invalid channel for the current PHY, the
   * call to \ref RAIL_ConfigRxChannelHopping() will fail.
   */
  uint16_t channel;
  /** The mode by which RAIL determines when to hop to the next channel. */
  RAIL_RxChannelHoppingMode_t mode;
  // Unnamed 'uint8_t reserved1[1]' pad byte field here.
  /**
   * Depending on the 'mode' parameter that was specified, this member
   * is used to parameterize that mode. See the comments on each value of
   * \ref RAIL_RxChannelHoppingMode_t to learn what to specify here.
   */
  RAIL_RxChannelHoppingParameter_t parameter;
  /**
   * Idle time in microseconds to wait before hopping into the
   * channel indicated by this entry.
   */
  uint32_t delay;
  /** @deprecated Set delayMode to \ref RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC. */
  RAIL_RxChannelHoppingDelayMode_t delayMode;
  /**
   * Bitmask of various options that can be applied to the current
   * channel hop.
   */
  RAIL_RxChannelHoppingOptions_t options;
  /**
   * The RSSI threshold (in dBm) below which a hop will occur in
   * any mode when \ref RAIL_RX_CHANNEL_HOPPING_OPTION_RSSI_THRESHOLD is
   * specified.
   */
  int8_t rssiThresholdDbm;
  /**
   * Pad bytes reserved for future use and currently ignored.
   */
  uint8_t reserved2[1];
} RAIL_RxChannelHoppingConfigEntry_t;

/**
 * @struct RAIL_RxChannelHoppingConfig_t
 * @brief Wrapper struct that will contain the sequence of
 *   \ref RAIL_RxChannelHoppingConfigEntry_t that represents the channel
 *   sequence to use during RX Channel Hopping.
 */
typedef struct RAIL_RxChannelHoppingConfig {
  /**
   * Pointer to contiguous global read-write memory that will be used
   * by RAIL to store channel hopping information throughout its operation.
   * It need not be initialized and applications should never write
   * data anywhere in this buffer.
   *
   * @note The size of this buffer must be at least as large as
   *   3 + \ref RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL * numberOfChannels,
   *   plus the sum of the sizes of the
   *   radioConfigDeltaAdd's of the required channels, plus the size of the
   *   radioConfigDeltaSubtract. In the case that one channel
   *   appears two or more times in your channel sequence
   *   (e.g., 1, 2, 3, 2), you must account for the radio configuration
   *   size that number of times (i.e., need to count channel 2's
   *   radio configuration size twice for the given example). The buffer is
   *   for internal use to the library.
   */
  uint32_t *buffer;
  /**
   * This parameter must be set to the length of the buffer array, in 32 bit
   * words. This way, during configuration, the software can confirm it's
   * writing within the bounds of the buffer. The configuration API will return
   * an error or trigger \ref RAIL_ASSERT_CHANNEL_HOPPING_BUFFER_TOO_SHORT if
   * bufferLength is insufficient.
   */
  uint16_t bufferLength;
  /**
   * The number of channels in the channel hopping sequence, which is the
   * number of elements in the array that entries points to.
   */
  uint8_t numberOfChannels;
  /**
   * A pointer to the first element of an array of \ref
   * RAIL_RxChannelHoppingConfigEntry_t that represents the channels
   * used during channel hopping. This array must have numberOfChannels
   * entries.
   */
  RAIL_RxChannelHoppingConfigEntry_t *entries;
} RAIL_RxChannelHoppingConfig_t;

/**
 * @struct RAIL_RxDutyCycleConfig_t
 * @brief Structure to configure duty cycled receive mode.
 */
typedef struct RAIL_RxDutyCycleConfig {
  /** The mode by which RAIL determines when to exit RX. */
  RAIL_RxChannelHoppingMode_t mode;
  // Unnamed 'uint8_t reserved[3]' pad byte field here.
  /**
   * Depending on the 'mode' parameter that was specified, this member
   * is used to parameterize that mode. See the comments on each value of
   * \ref RAIL_RxChannelHoppingMode_t to learn what to specify here.
   */
  RAIL_RxChannelHoppingParameter_t parameter;
  /**
   * Idle time in microseconds to wait before re-entering RX.
   */
  uint32_t delay;
  /** @deprecated Set delayMode to \ref RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC. */
  RAIL_RxChannelHoppingDelayMode_t delayMode;
  /**
   * Bitmask of various options that can be applied to the current
   * duty cycle operation when the mode is >= \ref
   * RAIL_RX_CHANNEL_HOPPING_MODE_MANUAL_WITH_OPTIONS (ignored otherwise).
   */
  RAIL_RxChannelHoppingOptions_t options;
  /**
   * The RSSI threshold (in dBm) below which Rx will end in
   * any mode when \ref RAIL_RX_CHANNEL_HOPPING_OPTION_RSSI_THRESHOLD
   * is specified.
   */
  int8_t rssiThresholdDbm;
  /**
   * Pad bytes reserved for future use and currently ignored.
   */
  uint8_t reserved2[1];
} RAIL_RxDutyCycleConfig_t;

/// A sentinel value to flag an invalid channel hopping index.
#define RAIL_CHANNEL_HOPPING_INVALID_INDEX (0xFEU)

/** @} */ // end of group Rx_Channel_Hopping

/******************************************************************************
 * Diagnostic Structures
 *****************************************************************************/
/**
 * @addtogroup Diagnostic
 * @{
 */

/**
 * @typedef RAIL_FrequencyOffset_t
 * @brief Type that represents the number of Frequency Offset
 *   units. It is used with \ref RAIL_GetRxFreqOffset() and
 *   \ref RAIL_SetFreqOffset().
 *
 * The units are chip-specific. For EFR32 they are radio synthesizer
 * resolution steps (synth ticks) and is limited to 15 bits.
 * A value of \ref RAIL_FREQUENCY_OFFSET_INVALID
 * means that this value is invalid.
 */
typedef int16_t RAIL_FrequencyOffset_t;

/**
 * The maximum frequency offset value supported.
 */
#define RAIL_FREQUENCY_OFFSET_MAX ((RAIL_FrequencyOffset_t) 0x3FFF)

/**
 * The minimum frequency offset value supported.
 */
#define RAIL_FREQUENCY_OFFSET_MIN ((RAIL_FrequencyOffset_t) -RAIL_FREQUENCY_OFFSET_MAX)

/**
 * Specify an invalid frequency offset value. This will be returned if you
 * call \ref RAIL_GetRxFreqOffset() at an invalid time.
 */
#define RAIL_FREQUENCY_OFFSET_INVALID ((RAIL_FrequencyOffset_t) 0x8000)

/**
 * @struct RAIL_DirectModeConfig_t
 * @brief Allows the user to specify direct mode
 *   parameters using \ref RAIL_ConfigDirectMode().
 */
typedef struct RAIL_DirectModeConfig {
  /** Enable synchronous RX DOUT using DCLK vs. asynchronous RX DOUT. */
  bool syncRx;
  /** Enable synchronous TX DIN using DCLK vs. asynchronous TX DIN. */
  bool syncTx;

  /** RX Data output (DOUT) GPIO port. */
  uint8_t doutPort;
  /** RX Data output (DOUT) GPIO pin. */
  uint8_t doutPin;

  /** Data clock (DCLK) GPIO port. Only used in synchronous mode. */
  uint8_t dclkPort;
  /** Data clock (DCLK) GPIO pin. Only used in synchronous mode. */
  uint8_t dclkPin;

  /** TX Data input (DIN) GPIO port. */
  uint8_t dinPort;
  /** TX Data input (DIN) GPIO pin. */
  uint8_t dinPin;

  /** @deprecated No longer used (ignored). */
  uint8_t doutLoc;
  /** @deprecated No longer used (ignored). */
  uint8_t dclkLoc;
  /** @deprecated No longer used (ignored). */
  uint8_t dinLoc;
} RAIL_DirectModeConfig_t;

/**
 * @enum RAIL_StreamMode_t
 * @brief Possible stream output modes.
 */
RAIL_ENUM(RAIL_StreamMode_t) {
  /** An unmodulated carrier wave. */
  RAIL_STREAM_CARRIER_WAVE = 0,
  /** PN9 byte sequence. */
  RAIL_STREAM_PN9_STREAM = 1,
  /** 101010 sequence. */
  RAIL_STREAM_10_STREAM = 2,
  /** An unmodulated carrier wave with no change to PLL BW. Same as \ref RAIL_STREAM_CARRIER_WAVE. */
  RAIL_STREAM_CARRIER_WAVE_PHASENOISE = 3,
  /** ramp sequence starting at a different offset for consecutive packets. Only available for some modulations. Fall back to \ref RAIL_STREAM_PN9_STREAM if not available. */
  RAIL_STREAM_RAMP_STREAM = 4,
  /** An unmodulated carrier wave not centered on DC but shifted roughly by channel_bandwidth/6 allowing an easy check of the residual DC. Only available for OFDM PA. Fall back to \ref RAIL_STREAM_CARRIER_WAVE_PHASENOISE if not available. */
  RAIL_STREAM_CARRIER_WAVE_SHIFTED = 5,
  /** 10001000 sequence. */
  RAIL_STREAM_1000_STREAM = 6,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_STREAM_MODES_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_STREAM_CARRIER_WAVE ((RAIL_StreamMode_t) RAIL_STREAM_CARRIER_WAVE)
#define RAIL_STREAM_PN9_STREAM   ((RAIL_StreamMode_t) RAIL_STREAM_PN9_STREAM)
#define RAIL_STREAM_10_STREAM    ((RAIL_StreamMode_t) RAIL_STREAM_10_STREAM)
#define RAIL_STREAM_CARRIER_WAVE_PHASENOISE ((RAIL_StreamMode_t) RAIL_STREAM_CARRIER_WAVE_PHASENOISE)
#define RAIL_STREAM_RAMP_STREAM  ((RAIL_StreamMode_t) RAIL_STREAM_RAMP_STREAM)
#define RAIL_STREAM_CARRIER_WAVE_SHIFTED ((RAIL_StreamMode_t) RAIL_STREAM_CARRIER_WAVE_SHIFTED)
#define RAIL_STREAM_1000_STREAM ((RAIL_StreamMode_t) RAIL_STREAM_1000_STREAM)
#define RAIL_STREAM_MODES_COUNT  ((RAIL_StreamMode_t) RAIL_STREAM_MODES_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_VERIFY_DURATION_MAX
 * @brief This radio state verification duration indicates to RAIL that
 *   all memory contents should be verified by RAIL before returning to the
 *   application.
 */
#define RAIL_VERIFY_DURATION_MAX 0xFFFFFFFFUL

/**
 * A pointer to a verification callback function. This will be called by the
 * radio state verification feature built into RAIL when a verified memory
 * value is different from its reference value.
 *
 * @param[in] address The address of the data in question.
 * @param[in] expectedValue The expected value of the data in question.
 * @param[in] actualValue The actual value of the data in question.
 * @return true indicates a data value difference is acceptable. false
 *   indicates a data value difference in unacceptable.
 *
 * @note This callback will be issued when an address' value is different from
 *   its reference value and either of the following conditions are met:
 *   1. The default radio configuration provided by the radio configurator is used
 *      for verification purposes (i.e., a custom radio configuration is not supplied
 *      as an input to \ref RAIL_ConfigVerification()), and the radio
 *      configurator has flagged the address under question as being verifiable.
 *   2. A custom radio configuration is provided to the verification API (i.e.,
 *      a custom radio configuration is supplied as an input to \ref
 *      RAIL_ConfigVerification()). When providing a custom radio configuration for
 *      verification purposes, all addresses in that configuration will be verified,
 *      regardless of whether or not the addresses are flagged as verifiable.
 */
typedef bool (*RAIL_VerifyCallbackPtr_t)(uint32_t address,
                                         uint32_t expectedValue,
                                         uint32_t actualValue);

/**
 * @struct RAIL_VerifyConfig_t
 * @brief The configuration array provided to RAIL for use by the radio state
 *   verification feature. This structure will be populated with appropriate
 *   values by calling \ref RAIL_ConfigVerification(). The application should
 *   not set or alter any of these structure elements.
 */
typedef struct RAIL_VerifyConfig {
  /** Internal verification tracking information. */
  RAIL_Handle_t correspondingHandle;
  /** Internal verification tracking information. */
  uint32_t nextIndexToVerify;
  /** Internal verification tracking information. */
  RAIL_RadioConfig_t override;
  /** Internal verification tracking information. */
  RAIL_VerifyCallbackPtr_t cb;
} RAIL_VerifyConfig_t;

/** @} */ // end of group Diagnostic

/******************************************************************************
 * Front End Module Voltage Detection (VDET)
 *****************************************************************************/
/**
 * @addtogroup VDET
 * @{
 */

/**
 * @enum RAIL_Vdet_Mode_t
 * @brief VDET Modes.
 *
 * The VDET Mode is passed to \ref RAIL_ConfigVdet() via \ref RAIL_VdetConfig_t.
 * The \ref rail_util_vdet component allows customers to measure their Front End Module performance
 * at specified points in the Transmit packet.
 */
RAIL_ENUM(RAIL_Vdet_Mode_t) {
  /** VDET is completely disabled. */
  RAIL_VDET_MODE_DISABLED = 0u,
  /** AUTOMATIC causes VDET measurements to be taken every Tx packet at the specified time. */
  RAIL_VDET_MODE_AUTOMATIC = 1u,
  /** IMMEDIATE causes an immediate VDET measurement. VDET must not be in \ref RAIL_VDET_MODE_AUTOMATIC. */
  RAIL_VDET_MODE_IMMEDIATE = 2u,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_VDET_MODE_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_VDET_MODE_DISABLED  ((RAIL_Vdet_Mode_t) RAIL_VDET_MODE_DISABLED)
#define RAIL_VDET_MODE_AUTOMATIC ((RAIL_Vdet_Mode_t) RAIL_VDET_MODE_AUTOMATIC)
#define RAIL_VDET_MODE_IMMEDIATE ((RAIL_Vdet_Mode_t) RAIL_VDET_MODE_IMMEDIATE)
#define RAIL_VDET_MODE_COUNT     ((RAIL_Vdet_Mode_t) RAIL_VDET_MODE_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_VDET_MODE_ENUM_NAMES
 * @brief A macro that is string versions of the calibration enums.
 */
#define RAIL_VDET_MODE_ENUM_NAMES { \
    "RAIL_VDET_MODE_DISABLED",      \
    "RAIL_VDET_MODE_AUTOMATIC",     \
    "RAIL_VDET_MODE_IMMEDIATE",     \
}

/**
 * @enum RAIL_Vdet_Resolution_t
 * @brief VDET Resolution for the Aux ADC.
 *
 * The VDET Resolution is passed to \ref RAIL_ConfigVdet() via \ref RAIL_VdetConfig_t.
 * Shows available resolution options.
 */
RAIL_ENUM(RAIL_Vdet_Resolution_t) {
  /** ~10 bit resolution. */
  RAIL_VDET_RESOLUTION_10_BIT = 0u,
  /** ~11 bit resolution. */
  RAIL_VDET_RESOLUTION_11_BIT = 1u,
  /** ~12 bit resolution. */
  RAIL_VDET_RESOLUTION_12_BIT = 2u,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_VDET_RESOLUTION_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_VDET_RESOLUTION_10_BIT ((RAIL_Vdet_Resolution_t) RAIL_VDET_RESOLUTION_10_BIT)
#define RAIL_VDET_RESOLUTION_11_BIT ((RAIL_Vdet_Resolution_t) RAIL_VDET_RESOLUTION_11_BIT)
#define RAIL_VDET_RESOLUTION_12_BIT ((RAIL_Vdet_Resolution_t) RAIL_VDET_RESOLUTION_12_BIT)
#define RAIL_VDET_RESOLUTION_COUNT  ((RAIL_Vdet_Resolution_t) RAIL_VDET_RESOLUTION_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_VDET_RESOLUTION_ENUM_NAMES
 * @brief A macro that is string versions of the resolution enums.
 */
#define RAIL_VDET_RESOLUTION_ENUM_NAMES { \
    "RAIL_VDET_RESOLUTION_10_BIT",        \
    "RAIL_VDET_RESOLUTION_11_BIT",        \
    "RAIL_VDET_RESOLUTION_12_BIT",        \
}

/**
 * @enum RAIL_Vdet_Status_t
 * @brief VDET Status.
 *
 * The VDET Status for internal debug.
 * Shows states.
 */
// MUST BE KEPT IN ALIGNMENT WITH #DEFINES FOR VDET_STATUS IN RFLDMA YAML FILE!
RAIL_ENUM(RAIL_Vdet_Status_t) {
  /** IDLE - Waiting for next command/measurement */
  RAIL_VDET_STATUS_IDLE = 0u,
  /** START of the VDET measurement activity. */
  RAIL_VDET_STATUS_START = 1u,
  /** Completion of a 10 bit measurement. */
  RAIL_VDET_STATUS_10_BIT_DONE = 2u,
  /** Completion of a 11 bit measurement. */
  RAIL_VDET_STATUS_11_BIT_DONE = 3u,
  /** Completion of a 12 bit measurement. */
  RAIL_VDET_STATUS_12_BIT_DONE = 4u,
  /** Conflict with another AuxADC user */
  RAIL_VDET_STATUS_BLOCKED = 5u,
  /** An error has occurred. */
  RAIL_VDET_STATUS_ERROR = 6u,
  /** A count of the choices in this enumeration. Must be last. */
  RAIL_VDET_STATUS_COUNT
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_VDET_STATUS_IDLE        ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_IDLE)
#define RAIL_VDET_STATUS_START       ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_START)
#define RAIL_VDET_STATUS_10_BIT_DONE ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_10_BIT_DONE)
#define RAIL_VDET_STATUS_11_BIT_DONE ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_11_BIT_DONE)
#define RAIL_VDET_STATUS_12_BIT_DONE ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_12_BIT_DONE)
#define RAIL_VDET_STATUS_BLOCKED     ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_BLOCKED)
#define RAIL_VDET_STATUS_ERROR       ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_ERROR)
#define RAIL_VDET_STATUS_COUNT       ((RAIL_Vdet_Status_t) RAIL_VDET_STATUS_COUNT)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_VDET_STATUS_ENUM_NAMES
 * @brief A macro that is string versions of the status enums.
 */
#define RAIL_VDET_STATUS_ENUM_NAMES { \
    "RAIL_VDET_STATUS_IDLE",          \
    "RAIL_VDET_STATUS_START",         \
    "RAIL_VDET_STATUS_10_BIT_DONE",   \
    "RAIL_VDET_STATUS_11_BIT_DONE",   \
    "RAIL_VDET_STATUS_12_BIT_DONE",   \
    "RAIL_VDET_STATUS_BLOCKED",       \
    "RAIL_VDET_STATUS_ERROR",         \
}

/**
 * @struct RAIL_VdetConfig_t
 * @brief Configuration information for FEM Voltage Detection plugin.
 *
 * A structure of type \ref RAIL_VdetConfig_t is passed to \ref RAIL_ConfigVdet().
 */
typedef struct RAIL_VdetConfig {
  /** Mode for the VDET. */
  RAIL_Vdet_Mode_t mode;
  /** Resolution to use for the capture. */
  RAIL_Vdet_Resolution_t resolution;
  /** Delay in microseconds for the capture from Tx Start in \ref RAIL_VDET_MODE_AUTOMATIC. Minimum 5 us, maximum 100000 us. */
  uint32_t delayUs;
} RAIL_VdetConfig_t;

/** @} */ // end of group VDET

/******************************************************************************
 * Thermal Protection
 *****************************************************************************/
/**
 * @addtogroup Thermal_Protection Thermal Protection
 * @{
 */

/** Maximum junction temperature in Kelvin. A margin is subtracted before using it when
 * \ref RAIL_SUPPORTS_THERMAL_PROTECTION is enabled.
 */
#define RAIL_CHIP_TEMP_THRESHOLD_MAX      (398U)

/**
 * Default number of Kelvin degrees below threshold needed to allow transmissions.
 */
#define RAIL_CHIP_TEMP_COOLDOWN_DEFAULT   (7U)

/**
 * @struct RAIL_ChipTempConfig_t
 * @brief Configuration parameters for thermal protection.
 */
typedef struct RAIL_ChipTempConfig {
  /** Indicates whether the protection is enabled */
  bool enable;
  /** Mandatory temperature cool down when the threshold is exceeded, in degrees Kelvin */
  uint8_t coolDownK;
  /** Temperature above which transmit is blocked, in degrees Kelvin */
  uint16_t thresholdK;
} RAIL_ChipTempConfig_t;

/** Number of temperature values provided for the chip thermal protection */
#define RAIL_CHIP_TEMP_MEASURE_COUNT      (3U)

/**
 * @struct RAIL_ChipTempMetrics_t
 * @brief Data used for thermal protection.
 */
typedef struct RAIL_ChipTempMetrics {
  /** Store chip temperature for metrics */
  uint16_t tempK;
  /** Minimum temperature recorded */
  uint16_t minTempK;
  /** Maximum temperature recorded */
  uint16_t maxTempK;
  /** Indicates if data should be reset */
  bool resetPending;
  /** Reserved for future use */
  uint8_t reservedChipTemp;
} RAIL_ChipTempMetrics_t;

/** @} */ // end of group Thermal_Protection

/******************************************************************************
 * Retiming Structures
 *****************************************************************************/
/**
 * @addtogroup Retiming
 * @{
 */

/**
 * @enum RAIL_RetimeOptions_t
 * @brief Retiming options bit shifts.
 */
RAIL_ENUM(RAIL_RetimeOptions_t) {
  /** Shift position of \ref RAIL_RETIME_OPTION_HFXO bit. */
  RAIL_RETIME_OPTION_HFXO_SHIFT = 0,
  /** Shift position of \ref RAIL_RETIME_OPTION_HFRCO bit. */
  RAIL_RETIME_OPTION_HFRCO_SHIFT = 1,
  /** Shift position of \ref RAIL_RETIME_OPTION_DCDC bit. */
  RAIL_RETIME_OPTION_DCDC_SHIFT = 2,
  /** Shift position of \ref RAIL_RETIME_OPTION_LCD bit. */
  RAIL_RETIME_OPTION_LCD_SHIFT = 3,
};

/**
 * An option to configure HFXO retiming.
 */
#define RAIL_RETIME_OPTION_HFXO \
  (1U << RAIL_RETIME_OPTION_HFXO_SHIFT)

/**
 * An option to configure HFRCO retiming.
 */
#define RAIL_RETIME_OPTION_HFRCO \
  (1U << RAIL_RETIME_OPTION_HFRCO_SHIFT)

/**
 * An option to configure DCDC retiming.
 * Ignored on platforms that lack DCDC.
 */
#define RAIL_RETIME_OPTION_DCDC \
  (1U << RAIL_RETIME_OPTION_DCDC_SHIFT)

/**
 * An option to configure LCD retiming.
 * Ignored on platforms that lack LCD.
 */
#define RAIL_RETIME_OPTION_LCD \
  (1U << RAIL_RETIME_OPTION_LCD_SHIFT)

/** A value representing no retiming options. */
#define RAIL_RETIME_OPTIONS_NONE 0x0U

/** A value representing all retiming options. */
#define RAIL_RETIME_OPTIONS_ALL 0xFFU

/** @} */ // end of group Retiming

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************
 * Debug Structures
 *****************************************************************************/
/**
 * @addtogroup Debug
 * @{
 */

/**
 * @def RAIL_DEBUG_MODE_FREQ_OVERRIDE
 * @brief A bitmask to enable the frequency override debug mode to
 *   manually tune to a specified frequency. Note that this should only be used
 *   for testing and is not as tuned as frequencies from the calculator.
 */
#define RAIL_DEBUG_MODE_FREQ_OVERRIDE  0x00000001UL

/**
 * @def RAIL_DEBUG_MODE_VALID_MASK
 * @brief Any debug mode bits outside of this mask are invalid and ignored.
 */
#define RAIL_DEBUG_MODE_VALID_MASK     (~(RAIL_DEBUG_MODE_FREQ_OVERRIDE))

/** @} */ // end of group Debug

/******************************************************************************
 * Detailed Timing Structures
 *****************************************************************************/
/**
 * @addtogroup Detailed_Timing Detailed Timing
 * @{
 */

/**
 * @enum RAIL_TimerTickType_t
 * @brief Enumerate the timer tick channel.
 */
RAIL_ENUM(RAIL_TimerTickType_t) {
  /**
   * RAIL default timer tick channel.
   * This is used to query the current RAIL timer tick value.
   */
  RAIL_TIMER_TICK_DEFAULT = 0,
  /**
   * Radio state timer tick channel.
   * This is used to query the timer tick corresponding to the latest radio
   * state.
   */
  RAIL_TIMER_TICK_RADIO_STATE = 1,
  /**
   * RX timestamp timer tick channel.
   * This is used to query the timer tick at the time of latest RX frame
   * detection.
   */
  RAIL_TIMER_TICK_RXSTAMP = 2,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_TIMER_TICK_DEFAULT     ((RAIL_TimerTickType_t) RAIL_TIMER_TICK_DEFAULT)
#define RAIL_TIMER_TICK_RADIO_STATE ((RAIL_TimerTickType_t) RAIL_TIMER_TICK_RADIO_STATE)
#define RAIL_TIMER_TICK_RXSTAMP     ((RAIL_TimerTickType_t) RAIL_TIMER_TICK_RXSTAMP)
#endif //DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group Detailed Timing

#endif //DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************
 * TrustZone
 *****************************************************************************/
/**
 * @addtogroup TrustZone
 * @{
 */

/**
 * @typedef RAIL_TZ_ChangedDcdcCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 *   \ref RAIL_ChangedDcdc().
 *
 * @return Status code indicating success of the function call.
 */
typedef RAIL_Status_t (*RAIL_TZ_ChangedDcdcCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_ConfigAntennaGpioCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_ConfigAntennaGpio().
 *
 * @param[in] config A pointer to a configuration structure applied to the relevant Antenna
 *   Configuration registers. A NULL configuration will produce undefined behavior.
 * @return Status code indicating success of the function call.
 *
 */
typedef RAIL_Status_t (*RAIL_TZ_ConfigAntennaGpioCallbackPtr_t)(const RAIL_AntennaConfig_t *config);

/**
 * @typedef RAIL_TZ_RadioClockEnableCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_RadioClockEnable().
 *
 */
typedef void (*RAIL_TZ_RadioClockEnableCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_GetRadioClockFreqHzCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_GetRadioClockFreqHz().
 *
 * @return Radio subsystem clock frequency in Hz.
 *
 */
typedef uint32_t (*RAIL_TZ_GetRadioClockFreqHzCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_RfecaClockEnableCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_RfecaClockEnable().
 *
 */
typedef void (*RAIL_TZ_RfecaClockEnableCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_RfecaIsClockEnabledCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_RfecaIsClockEnabled().
 *
 * @return true if RFECA clocks are enabled; false otherwise
 *
 */
typedef bool (*RAIL_TZ_RfecaIsClockEnabledCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_ReadInternalTemperatureCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_ReadInternalTemperature().
 *
 * @param[out] internalTemperatureKelvin A pointer to the internal temperature
 *   in Kelvin.
 * @param[in] enableTemperatureInterrupts Indicate whether temperature
 *   interrupts are enabled.
 * @return Status code indicating success of the function call.
 *
 */
typedef RAIL_Status_t (*RAIL_TZ_ReadInternalTemperatureCallbackPtr_t)(uint16_t *internalTemperatureKelvin,
                                                                      bool enableTemperatureInterrupts);

/**
 * @typedef RAIL_TZ_EnableSecureRadioIrqsCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_EnableSecureRadioIrqs().
 *
 */
typedef void (*RAIL_TZ_EnableSecureRadioIrqsCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_DisableSecureRadioIrqsCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_DisableSecureRadioIrqs().
 *
 */
typedef void (*RAIL_TZ_DisableSecureRadioIrqsCallbackPtr_t)(void);

/**
 * @typedef RAIL_TZ_RadioPerformM2mLdmaCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_RadioPerformM2mLdma().
 *
 * @param[in] pDest A pointer to the destination data.
 * @param[in] pSrc A pointer to the source data.
 * @param[in] numWords Number of words to transfer.
 * @return Status code indicating success of the function call.
 *
 */
typedef RAIL_Status_t (*RAIL_TZ_RadioPerformM2mLdmaCallbackPtr_t)(uint32_t *pDest,
                                                                  const uint32_t *pSrc,
                                                                  uint32_t numWords);

/**
 * @typedef RAIL_TZ_ConfigureHfxoCallbackPtr_t
 * @brief A pointer to the callback used to switch to secure world and run
 * \ref RAIL_TZ_ConfigureHfxo().
 *
 */
typedef RAIL_Status_t (*RAIL_TZ_ConfigureHfxoCallbackPtr_t)(void);

/**
 * @struct RAIL_TZ_Config_t
 * @brief Gather RAIL TrustZone callbacks pointers and booleans indicating
 *   peripheral secure configuration.
 */
typedef struct RAIL_TZ_Config {
  /**
   * See \ref RAIL_TZ_ChangedDcdcCallbackPtr_t.
   * In non-secure world, it must be NULL if CMU is a non-secure peripheral.
   */
  RAIL_TZ_ChangedDcdcCallbackPtr_t changedDcdcCallback;
  /**
   * See \ref RAIL_TZ_ConfigAntennaGpioCallbackPtr_t.
   * In non-secure world, it must be NULL if CMU and GPIO are non-secure
   * peripherals.
   */
  RAIL_TZ_ConfigAntennaGpioCallbackPtr_t configAntennaGpioCallback;
  /**
   * See \ref RAIL_TZ_RadioClockEnableCallbackPtr_t.
   * In non-secure world, it must be NULL if CMU is a non-secure peripheral.
   */
  RAIL_TZ_RadioClockEnableCallbackPtr_t radioClockEnableCallback;
  /**
   * See \ref RAIL_TZ_GetRadioClockFreqHzCallbackPtr_t.
   * In non-secure world, it must be NULL if CMU is a non-secure peripheral.
   */
  RAIL_TZ_GetRadioClockFreqHzCallbackPtr_t getRadioClockFreqHzCallback;
  /**
   * See \ref RAIL_TZ_RfecaClockEnableCallbackPtr_t.
   * In non-secure world, it must be NULL if CMU is a non-secure peripheral.
   */
  RAIL_TZ_RfecaClockEnableCallbackPtr_t rfecaClockEnableCallback;
  /**
   * See \ref RAIL_TZ_RfecaIsClockEnabledCallbackPtr_t.
   * In non-secure world, it must be NULL if CMU is a non-secure peripheral.
   */
  RAIL_TZ_RfecaIsClockEnabledCallbackPtr_t rfecaIsClockEnabledCallback;
  /**
   * See \ref RAIL_TZ_ReadInternalTemperatureCallbackPtr_t.
   * In non-secure world, it must be NULL if EMU is a non-secure peripheral.
   */
  RAIL_TZ_ReadInternalTemperatureCallbackPtr_t readInternalTemperatureCallback;
  /**
   * See \ref RAIL_TZ_EnableSecureRadioIrqsCallbackPtr_t.
   * In non-secure world, it must be NULL if EMU is a non-secure peripheral.
   */
  RAIL_TZ_EnableSecureRadioIrqsCallbackPtr_t enableSecureRadioIrqsCallback;
  /**
   * See \ref RAIL_TZ_DisableSecureRadioIrqsCallbackPtr_t.
   * In non-secure world, it must be NULL if EMU is a non-secure peripheral.
   */
  RAIL_TZ_DisableSecureRadioIrqsCallbackPtr_t disableSecureRadioIrqsCallback;
  /**
   * See \ref RAIL_TZ_RadioPerformM2mLdmaCallbackPtr_t.
   * In non-secure world, it must be NULL if LDMA is a non-secure peripheral or
   * if RAIL must not use LDMA.
   */
  RAIL_TZ_RadioPerformM2mLdmaCallbackPtr_t radioPerformM2mLdmaCallback;
  /**
   * See \ref RAIL_TZ_ConfigureHfxoCallbackPtr_t.
   * In non-secure world, it must be NULL if HFXO is a non-secure peripheral.
   */
  RAIL_TZ_ConfigureHfxoCallbackPtr_t configureHfxoCallback;
  /**
   * Indicate whether CMU is configured as secure peripheral.
   */
  bool isCmuSecure;
  /**
   * Indicate whether EMU is configured as secure peripheral.
   */
  bool isEmuSecure;
  /**
   * Indicate whether GPIO is configured as secure peripheral.
   */
  bool isGpioSecure;
  /**
   * Indicate whether LDMA is configured as secure peripheral.
   */
  bool isLdmaSecure;
  /**
   * Indicate whether HFXO is configured as secure peripheral.
   */
  bool isHfxoSecure;
  /**
   * Indicate whether PRS is configured as secure peripheral.
   */
  bool isPrsSecure;
  /**
   * Indicate whether SYSRTC is configured as secure peripheral.
   */
  bool isSysrtcSecure;
} RAIL_TZ_Config_t;

/** @} */ // end of group TrustZone

#ifdef  SLI_LIBRARY_BUILD
#ifndef DOXYGEN_SHOULD_SKIP_THIS
// SLI_LIBRARY_BUILD is defined for library builds that do not include
// chip-dependent defines. This may limit functionality but allows building
// generic libraries that are not tied to any given chip.

// Platform-agnostic worst-case settings and types

#define RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL \
  RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL_WORST_CASE

struct RAIL_ChannelConfigEntryAttr {
  RAIL_RxIrCalValues_t calValues;
  RAIL_TxIrCalValues_t txCalValues; // placeholder for size
};

#endif//DOXYGEN_SHOULD_SKIP_THIS
#endif//SLI_LIBRARY_BUILD

/** @} */ // end of RAIL_API

#ifdef __cplusplus
}
#endif

#ifndef  SLI_LIBRARY_BUILD
// Include appropriate chip-specific types and APIs *after* common types, and
// *before* types that depend on chip-specific types.
#include "rail_chip_specific.h"

// (Currently no types depend on chip-specific types.)

#endif //!SLI_LIBRARY_BUILD

#endif  // __RAIL_TYPES_H__
