/***************************************************************************//**
 * @file
 * @brief NVM3 utility functions
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef NVM3_UTILS_H
#define NVM3_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

/***************************************************************************//**
 * @brief
 *  This function calculates the Berger Code of the supplied input variable.
 *  The Berger Code is calculated by looking at the binary value of the variable
 *  and counting the number of binary zeros.
 *
 * @param[in, out] pResult
 *   A pointer to a variable that will accumulate the berger code for
 *   the input variable specified in the next two variables.
 *
 * @param[in] pInput
 *   A pointer to the variable that contains data that is used.
 *
 * @param[in] numberOfBits
 *   The number of bits in the input variable used in the calculation.
 *   The calculation is starting from the least significant bit in the input
 *   variable.
 ******************************************************************************/
void nvm3_utilsComputeBergerCode(uint8_t *pResult, void *pInput, uint8_t numberOfBits);

/// @endcond

#ifdef __cplusplus
}
#endif

#endif /* NVM3_UTILS_H */
