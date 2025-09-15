/***************************************************************************//**
 * @file
 * @brief Silabs Compiler definitions.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_COMPILER_H
#define SL_COMPILER_H

/***************************************************************************//**
 * @addtogroup compiler Compiler definitions
 * @brief Compiler definitions
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__GNUC__)

// Fallback for __has_builtin.
  #ifndef __has_builtin
    #define __has_builtin(x) (0)
  #endif

// Compiler specific defines.
  #ifndef   __ASM
    #define __ASM                  __asm
  #endif
  #ifndef   __INLINE
    #define __INLINE               inline
  #endif
  #ifndef   __STATIC_INLINE
    #define __STATIC_INLINE        static inline
  #endif
  #ifndef   __STATIC_FORCEINLINE
    #define __STATIC_FORCEINLINE   __attribute__((always_inline)) static inline
  #endif
  #ifndef   __NO_RETURN
    #define __NO_RETURN            __attribute__((__noreturn__))
  #endif
  #ifndef   __USED
    #define __USED                 __attribute__((used))
  #endif
  #ifndef   __WEAK
    #define __WEAK                 __attribute__((weak))
  #endif
  #ifndef   __PACKED
    #define __PACKED               __attribute__((packed, aligned(1)))
  #endif
  #ifndef   __PACKED_STRUCT
    #define __PACKED_STRUCT        struct __attribute__((packed, aligned(1)))
  #endif
  #ifndef   __PACKED_UNION
    #define __PACKED_UNION         union __attribute__((packed, aligned(1)))
  #endif
  #ifndef   __ALIGNED
    #define __ALIGNED(x)           __attribute__((aligned(x)))
  #endif
  #ifndef   __RESTRICT
    #define __RESTRICT             __restrict
  #endif

#elif defined(__IAR_SYSTEMS_ICC__)

  #pragma system_include

  #if (__VER__ >= 8000000)
    #define __ICCARM_V8 1
  #else
    #define __ICCARM_V8 0
  #endif

  #ifndef __ALIGNED
    #if __ICCARM_V8
      #define __ALIGNED(x) __attribute__((aligned(x)))
    #elif (__VER__ >= 7080000)
/* Needs IAR language extensions */
      #define __ALIGNED(x) __attribute__((aligned(x)))
    #else
      #warning No compiler specific solution for __ALIGNED.__ALIGNED is ignored.
      #define __ALIGNED(x)
    #endif
  #endif

  #ifndef __ASM
    #define __ASM __asm
  #endif

  #ifndef __INLINE
    #define __INLINE inline
  #endif

  #ifndef   __NO_RETURN
    #if __ICCARM_V8
      #define __NO_RETURN __attribute__((__noreturn__))
    #else
      #define __NO_RETURN _Pragma("object_attribute=__noreturn")
    #endif
  #endif

  #ifndef   __PACKED
    #if __ICCARM_V8
      #define __PACKED __attribute__((packed, aligned(1)))
    #else
/* Needs IAR language extensions */
      #define __PACKED __packed
    #endif
  #endif

  #ifndef   __PACKED_STRUCT
    #if __ICCARM_V8
      #define __PACKED_STRUCT struct __attribute__((packed, aligned(1)))
    #else
/* Needs IAR language extensions */
      #define __PACKED_STRUCT __packed struct
    #endif
  #endif

  #ifndef   __PACKED_UNION
    #if __ICCARM_V8
      #define __PACKED_UNION union __attribute__((packed, aligned(1)))
    #else
/* Needs IAR language extensions */
      #define __PACKED_UNION __packed union
    #endif
  #endif

  #ifndef   __RESTRICT
    #define __RESTRICT            restrict
  #endif

  #ifndef   __STATIC_INLINE
    #define __STATIC_INLINE       static inline
  #endif

  #ifndef   __FORCEINLINE
    #define __FORCEINLINE         _Pragma("inline=forced")
  #endif

  #ifndef   __STATIC_FORCEINLINE
    #define __STATIC_FORCEINLINE  __FORCEINLINE __STATIC_INLINE
  #endif

  #ifndef   __USED
    #if __ICCARM_V8
      #define __USED __attribute__((used))
    #else
      #define __USED _Pragma("__root")
    #endif
  #endif

  #ifndef   __WEAK
    #if __ICCARM_V8
      #define __WEAK __attribute__((weak))
    #else
      #define __WEAK _Pragma("__weak")
    #endif
  #endif

#else
  #error "Unknown compiler."
#endif

// IO definitions (access restrictions to peripheral registers).
#ifdef __cplusplus
  #define   __I     volatile        ///< Defines 'read only' permissions
#else
  #define   __I     volatile const  ///< Defines 'read only' permissions
#endif
#define     __O     volatile        ///< Defines 'write only' permissions
#define     __IO    volatile        ///< Defines 'read / write' permissions

// The following defines should be used for structure members.
#define     __IM    volatile const  ///< Defines 'read only' structure member permissions
#define     __OM    volatile        ///< Defines 'write only' structure member permissions
#define     __IOM   volatile        ///< Defines 'read / write' structure member permissions

#ifdef __cplusplus
}
#endif

/** @} (end group compiler) */

#endif // SL_COMPILER_H
