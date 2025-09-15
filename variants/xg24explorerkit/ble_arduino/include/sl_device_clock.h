/***************************************************************************//**
 * @file
 * @brief Device Manager Clocks.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories, Inc. www.silabs.com</b>
 ******************************************************************************
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
 *****************************************************************************/

#ifndef SL_DEVICE_CLOCK_H
#define SL_DEVICE_CLOCK_H

#include "sl_enum.h"
#include <stdint.h>

#if defined(DEVICE_CLOCK_INTERNAL_PRESENT)
#include "sli_device_clock_internal.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup device_clock Device Manager Clock
 * @details
 * ## Overview
 *
 * The Device Manager Clock module defines the different oscillators,
 * clock branches and bus clock values that exist across all Silicon Labs
 * devices.
 *
 * @{
 ******************************************************************************/

// ----------------------------------------------------------------------------
// ENUMS

/// Oscillators
SL_ENUM(sl_oscillator_t) {
  SL_OSCILLATOR_FSRCO,      ///< FSRCO Oscillator
  SL_OSCILLATOR_HFRCODPLL,  ///< HFRCODPLL Oscillator
  SL_OSCILLATOR_HFXO,       ///< HFXO Oscillator
  SL_OSCILLATOR_HFRCOEM23,  ///< HFRCOEM23 Oscillator
  SL_OSCILLATOR_RFFPLL,     ///< RFFPLL Oscillator
  SL_OSCILLATOR_USBPLL,     ///< USBPLL Oscillator
  SL_OSCILLATOR_SOCPLL,     ///< SOCPLL Oscillator
  SL_OSCILLATOR_LFXO,       ///< LFXO Oscillator
  SL_OSCILLATOR_LFRCO,      ///< LFRCO Oscillator
  SL_OSCILLATOR_ULFRCO,     ///< ULFRCO Oscillator
  SL_OSCILLATOR_CLKIN0,     ///< CLKIN0 Oscillator
  SL_OSCILLATOR_FLPLL       ///< FLPLL Oscillator
};

/// Clock Branches
SL_ENUM(sl_clock_branch_t) {
  SL_CLOCK_BRANCH_SYSCLK,        ///< SYSCLK Clock Branch
  SL_CLOCK_BRANCH_HCLK,          ///< HCLK Clock Branch
  SL_CLOCK_BRANCH_HCLKRADIO,     ///< HCLK Radio Clock Branch
  SL_CLOCK_BRANCH_PCLK,          ///< PCLK Clock Branch
  SL_CLOCK_BRANCH_LSPCLK,        ///< LSPCLK Clock Branch
  SL_CLOCK_BRANCH_TRACECLK,      ///< TRACECLK Clock Branch
  SL_CLOCK_BRANCH_ADCCLK,        ///< ADCCLK Clock Branch
  SL_CLOCK_BRANCH_EXPORTCLK,     ///< EXPORTCLK Clock Branch
  SL_CLOCK_BRANCH_EM01GRPACLK,   ///< EM01GRPACLK Clock Branch
  SL_CLOCK_BRANCH_EM01GRPBCLK,   ///< EM01GRPBCLK Clock Branch
  SL_CLOCK_BRANCH_EM01GRPCCLK,   ///< EM01GRPCCLK Clock Branch
  SL_CLOCK_BRANCH_EM01GRPDCLK,   ///< EM01GRPDCLK Clock Branch
  SL_CLOCK_BRANCH_EM23GRPACLK,   ///< EM23GRPACLK Clock Branch
  SL_CLOCK_BRANCH_EM4GRPACLK,    ///< EM4GRPACLK Clock Branch
  SL_CLOCK_BRANCH_QSPISYSCLK,    ///< QSPISYSCLK Clock Branch
  SL_CLOCK_BRANCH_IADCCLK,       ///< IADCCLK Clock Branch
  SL_CLOCK_BRANCH_WDOG0CLK,      ///< WDOG0CLK Clock Branch
  SL_CLOCK_BRANCH_WDOG1CLK,      ///< WDOG1CLK Clock Branch
  SL_CLOCK_BRANCH_RTCCCLK,       ///< RTCCCLK Clock Branch
  SL_CLOCK_BRANCH_SYSRTCCLK,     ///< SYSRTCCLK Clock Branch
  SL_CLOCK_BRANCH_EUART0CLK,     ///< EUART0CLK Clock Branch
  SL_CLOCK_BRANCH_EUSART0CLK,    ///< EUSART0CLK Clock Branch
  SL_CLOCK_BRANCH_DPLLREFCLK,    ///< DPLLREFCLK Clock Branch
  SL_CLOCK_BRANCH_I2C0CLK,       ///< I2C0CLK Clock Branch
  SL_CLOCK_BRANCH_LCDCLK,        ///< LCDCLK Clock Branch
  SL_CLOCK_BRANCH_PIXELRZCLK,    ///< PIXELRZCLK Clock Branch
  SL_CLOCK_BRANCH_PCNT0CLK,      ///< PCNT0CLK Clock Branch
  SL_CLOCK_BRANCH_PRORTCCLK,     ///< PCNT0CLK Clock Branch
  SL_CLOCK_BRANCH_SYSTICKCLK,    ///< SYSTICKCLK Clock Branch
  SL_CLOCK_BRANCH_LESENSEHFCLK,  ///< LESENSEHFCLK Clock Branch
  SL_CLOCK_BRANCH_VDAC0CLK,      ///< VDAC0CLK Clock Branch
  SL_CLOCK_BRANCH_VDAC1CLK,      ///< VDAC1CLK Clock Branch
  SL_CLOCK_BRANCH_USB0CLK,       ///< USB0CLK Clock Branch
  SL_CLOCK_BRANCH_FLPLLREFCLK,   ///< FLPLLREFCLK Clock Branch
  SL_CLOCK_BRANCH_INVALID        ///< INVALID Clock Branch
};

// ----------------------------------------------------------------------------
// DEFINES

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

// CLKEN bitfield shift in bus clock value.
#define _BUS_CLOCK_CLKEN_BIT_SHIFT 0

// CLKEN bitfield mask in bus clock value.
#define _BUS_CLOCK_CLKEN_BIT_MASK  0x3FUL

// CLKENx register number shift in bus clock value.
#define _BUS_CLOCK_CLKENX_SHIFT  6

// CLKENx register number mask in bus clock value.
#define _BUS_CLOCK_CLKENX_MASK   0x1C0UL

// CLKEN0 value in bus clock.
#define BUS_CLOCK_CLKEN0   0x0UL

// CLKEN1 value in bus clock.
#define BUS_CLOCK_CLKEN1   0x1UL

// CLKEN2 value in bus clock.
#define BUS_CLOCK_CLKEN2   0x2UL

// CLKENHV value in bus clock.
#define BUS_CLOCK_CLKENHV  0x3UL

/// @endcond

/***************************************************************************//**
 * @name Bus Clock Defines
 * Those defines can be used as constant of type sl_bus_clock_t and thus can
 * be used as argument for function sl_clock_manager_enable_bus_clock() and
 * sl_clock_manager_disable_bus_clock() in @ref clock_manager.
 * The values of those defines are device specific.
 * @{
 ******************************************************************************/

/// Define for INVALID peripheral bus clock pointer.
#define SL_BUS_CLOCK_INVALID (0)

/// Define for ACMP0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_ACMP0 (&SL_BUS_CLOCK_ACMP0_VALUE)

/// Define for ACMP1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_ACMP1 (&SL_BUS_CLOCK_ACMP1_VALUE)

/// Define for ADC0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_ADC0 (&SL_BUS_CLOCK_ADC0_VALUE)

/// Define for AGC peripheral bus clock pointer.
#define SL_BUS_CLOCK_AGC (&SL_BUS_CLOCK_AGC_VALUE)

/// Define for AMUXCP0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_AMUXCP0 (&SL_BUS_CLOCK_AMUXCP0_VALUE)

/// Define for BUFC peripheral bus clock pointer.
#define SL_BUS_CLOCK_BUFC (&SL_BUS_CLOCK_BUFC_VALUE)

/// Define for BURAM peripheral bus clock pointer.
#define SL_BUS_CLOCK_BURAM (&SL_BUS_CLOCK_BURAM_VALUE)

/// Define for BURTC peripheral bus clock pointer.
#define SL_BUS_CLOCK_BURTC (&SL_BUS_CLOCK_BURTC_VALUE)

/// Define for CRYPTOACC peripheral bus clock pointer.
#define SL_BUS_CLOCK_CRYPTOACC (&SL_BUS_CLOCK_CRYPTOACC_VALUE)

/// Define for DCDC peripheral bus clock pointer.
#define SL_BUS_CLOCK_DCDC (&SL_BUS_CLOCK_DCDC_VALUE)

/// Define for DEVINFO peripheral bus clock pointer.
#define SL_BUS_CLOCK_DEVINFO (&SL_BUS_CLOCK_DEVINFO_VALUE)

/// Define for DMEM peripheral bus clock pointer.
#define SL_BUS_CLOCK_DMEM (&SL_BUS_CLOCK_DMEM_VALUE)

/// Define for DPLL0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_DPLL0 (&SL_BUS_CLOCK_DPLL0_VALUE)

/// Define for ECAIFADC peripheral bus clock pointer.
#define SL_BUS_CLOCK_ECAIFADC (&SL_BUS_CLOCK_ECAIFADC_VALUE)

/// Define for ETAMPDET peripheral bus clock pointer.
#define SL_BUS_CLOCK_ETAMPDET (&SL_BUS_CLOCK_ETAMPDET_VALUE)

/// Define for EUART0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_EUART0 (&SL_BUS_CLOCK_EUART0_VALUE)

/// Define for EUSART0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_EUSART0 (&SL_BUS_CLOCK_EUSART0_VALUE)

/// Define for EUSART1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_EUSART1 (&SL_BUS_CLOCK_EUSART1_VALUE)

/// Define for EUSART2 peripheral bus clock pointer.
#define SL_BUS_CLOCK_EUSART2 (&SL_BUS_CLOCK_EUSART2_VALUE)

/// Define for EUSART3 peripheral bus clock pointer.
#define SL_BUS_CLOCK_EUSART3 (&SL_BUS_CLOCK_EUSART3_VALUE)

/// Define for EUSART4 peripheral bus clock pointer.
#define SL_BUS_CLOCK_EUSART4 (&SL_BUS_CLOCK_EUSART4_VALUE)

/// Define for FRC peripheral bus clock pointer.
#define SL_BUS_CLOCK_FRC (&SL_BUS_CLOCK_FRC_VALUE)

/// Define for FSRCO peripheral bus clock pointer.
#define SL_BUS_CLOCK_FSRCO (&SL_BUS_CLOCK_FSRCO_VALUE)

/// Define for GPCRC0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_GPCRC0 (&SL_BUS_CLOCK_GPCRC0_VALUE)

/// Define for GPIO peripheral bus clock pointer.
#define SL_BUS_CLOCK_GPIO (&SL_BUS_CLOCK_GPIO_VALUE)

/// Define for HFRCO0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_HFRCO0 (&SL_BUS_CLOCK_HFRCO0_VALUE)

/// Define for HFRCOEM23 peripheral bus clock pointer.
#define SL_BUS_CLOCK_HFRCOEM23 (&SL_BUS_CLOCK_HFRCOEM23_VALUE)

/// Define for HFXO0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_HFXO0 (&SL_BUS_CLOCK_HFXO0_VALUE)

/// Define for HOSTMAILBOX peripheral bus clock pointer.
#define SL_BUS_CLOCK_HOSTMAILBOX (&SL_BUS_CLOCK_HOSTMAILBOX_VALUE)

/// Define for HOSTPORTAL peripheral bus clock pointer.
#define SL_BUS_CLOCK_HOSTPORTAL (&SL_BUS_CLOCK_HOSTPORTAL_VALUE)

/// Define for I2C0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_I2C0 (&SL_BUS_CLOCK_I2C0_VALUE)

/// Define for I2C1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_I2C1 (&SL_BUS_CLOCK_I2C1_VALUE)

/// Define for I2C2 peripheral bus clock pointer.
#define SL_BUS_CLOCK_I2C2 (&SL_BUS_CLOCK_I2C2_VALUE)

/// Define for I2C3 peripheral bus clock pointer.
#define SL_BUS_CLOCK_I2C3 (&SL_BUS_CLOCK_I2C3_VALUE)

/// Define for IADC0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_IADC0 (&SL_BUS_CLOCK_IADC0_VALUE)

/// Define for ICACHE0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_ICACHE0 (&SL_BUS_CLOCK_ICACHE0_VALUE)

/// Define for IFADCDEBUG peripheral bus clock pointer.
#define SL_BUS_CLOCK_IFADCDEBUG (&SL_BUS_CLOCK_IFADCDEBUG_VALUE)

/// Define for KEYSCAN peripheral bus clock pointer.
#define SL_BUS_CLOCK_KEYSCAN (&SL_BUS_CLOCK_KEYSCAN_VALUE)

/// Define for KSU peripheral bus clock pointer.
#define SL_BUS_CLOCK_KSU (&SL_BUS_CLOCK_KSU_VALUE)

/// Define for L2ICACHE0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_L2ICACHE0 (&SL_BUS_CLOCK_L2ICACHE0_VALUE)

/// Define for LCD peripheral bus clock pointer.
#define SL_BUS_CLOCK_LCD (&SL_BUS_CLOCK_LCD_VALUE)

/// Define for LDMA0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_LDMA0 (&SL_BUS_CLOCK_LDMA0_VALUE)

/// Define for LDMAXBAR0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_LDMAXBAR0 (&SL_BUS_CLOCK_LDMAXBAR0_VALUE)

/// Define for LEDDRV0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_LEDDRV0 (&SL_BUS_CLOCK_LEDDRV0_VALUE)

/// Define for LESENSE peripheral bus clock pointer.
#define SL_BUS_CLOCK_LESENSE (&SL_BUS_CLOCK_LESENSE_VALUE)

/// Define for LETIMER0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_LETIMER0 (&SL_BUS_CLOCK_LETIMER0_VALUE)

/// Define for LFRCO peripheral bus clock pointer.
#define SL_BUS_CLOCK_LFRCO (&SL_BUS_CLOCK_LFRCO_VALUE)

/// Define for LFXO peripheral bus clock pointer.
#define SL_BUS_CLOCK_LFXO (&SL_BUS_CLOCK_LFXO_VALUE)

/// Define for LPWAES peripheral bus clock pointer.
#define SL_BUS_CLOCK_LPWAES (&SL_BUS_CLOCK_LPWAES_VALUE)

/// Define for LPW0PORTAL peripheral bus clock pointer.
#define SL_BUS_CLOCK_LPW0PORTAL (&SL_BUS_CLOCK_LPW0PORTAL_VALUE)

/// Define for MODEM peripheral bus clock pointer.
#define SL_BUS_CLOCK_MODEM (&SL_BUS_CLOCK_MODEM_VALUE)

/// Define for MSC peripheral bus clock pointer.
#define SL_BUS_CLOCK_MSC (&SL_BUS_CLOCK_MSC_VALUE)

/// Define for MVP peripheral bus clock pointer.
#define SL_BUS_CLOCK_MVP (&SL_BUS_CLOCK_MVP_VALUE)

/// Define for PCNT0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_PCNT0 (&SL_BUS_CLOCK_PCNT0_VALUE)

/// Define for PDM peripheral bus clock pointer.
#define SL_BUS_CLOCK_PDM (&SL_BUS_CLOCK_PDM_VALUE)

/// Define for PIXELRZ0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_PIXELRZ0 (&SL_BUS_CLOCK_PIXELRZ0_VALUE)

/// Define for PIXELRZ1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_PIXELRZ1 (&SL_BUS_CLOCK_PIXELRZ1_VALUE)

/// Define for PRORTC peripheral bus clock pointer.
#define SL_BUS_CLOCK_PRORTC (&SL_BUS_CLOCK_PRORTC_VALUE)

/// Define for PROTIMER peripheral bus clock pointer.
#define SL_BUS_CLOCK_PROTIMER (&SL_BUS_CLOCK_PROTIMER_VALUE)

/// Define for PRS peripheral bus clock pointer.
#define SL_BUS_CLOCK_PRS (&SL_BUS_CLOCK_PRS_VALUE)

/// Define for RAC peripheral bus clock pointer.
#define SL_BUS_CLOCK_RAC (&SL_BUS_CLOCK_RAC_VALUE)

/// Define for RADIOAES peripheral bus clock pointer.
#define SL_BUS_CLOCK_RADIOAES (&SL_BUS_CLOCK_RADIOAES_VALUE)

/// Define for RDMAILBOX0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_RDMAILBOX0 (&SL_BUS_CLOCK_RDMAILBOX0_VALUE)

/// Define for RDMAILBOX1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_RDMAILBOX1 (&SL_BUS_CLOCK_RDMAILBOX1_VALUE)

/// Define for RDSCRATCHPAD peripheral bus clock pointer.
#define SL_BUS_CLOCK_RDSCRATCHPAD (&SL_BUS_CLOCK_RDSCRATCHPAD_VALUE)

/// Define for RFCRC peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFCRC (&SL_BUS_CLOCK_RFCRC_VALUE)

/// Define for RFECA0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFECA0 (&SL_BUS_CLOCK_RFECA0_VALUE)

/// Define for RFECA1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFECA1 (&SL_BUS_CLOCK_RFECA1_VALUE)

/// Define for RFFPLL0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFFPLL0 (&SL_BUS_CLOCK_RFFPLL0_VALUE)

/// Define for RFMAILBOX peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFMAILBOX (&SL_BUS_CLOCK_RFMAILBOX_VALUE)

/// Define for RFSCRATCHPAD peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFSCRATCHPAD (&SL_BUS_CLOCK_RFSCRATCHPAD_VALUE)

/// Define for RFSENSE peripheral bus clock pointer.
#define SL_BUS_CLOCK_RFSENSE (&SL_BUS_CLOCK_RFSENSE_VALUE)

/// Define for RPA peripheral bus clock pointer.
#define SL_BUS_CLOCK_RPA (&SL_BUS_CLOCK_RPA_VALUE)

/// Define for RTCC peripheral bus clock pointer.
#define SL_BUS_CLOCK_RTCC (&SL_BUS_CLOCK_RTCC_VALUE)

/// Define for SCRATCHPAD peripheral bus clock pointer.
#define SL_BUS_CLOCK_SCRATCHPAD (&SL_BUS_CLOCK_SCRATCHPAD_VALUE)

/// Define for SEMAILBOX peripheral bus clock pointer.
#define SL_BUS_CLOCK_SEMAILBOX (&SL_BUS_CLOCK_SEMAILBOX_VALUE)

/// Define for SEMAPHORE0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_SEMAPHORE0 (&SL_BUS_CLOCK_SEMAPHORE0_VALUE)

/// Define for SEMAPHORE1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_SEMAPHORE1 (&SL_BUS_CLOCK_SEMAPHORE1_VALUE)

/// Define for SEPORTAL peripheral bus clock pointer.
#define SL_BUS_CLOCK_SEPORTAL (&SL_BUS_CLOCK_SEPORTAL_VALUE)

/// Define for SMU peripheral bus clock pointer.
#define SL_BUS_CLOCK_SMU (&SL_BUS_CLOCK_SMU_VALUE)

/// Define for SOCPLL0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_SOCPLL0 (&SL_BUS_CLOCK_SOCPLL0_VALUE)

/// Define for SYMCRYPTO peripheral bus clock pointer.
#define SL_BUS_CLOCK_SYMCRYPTO (&SL_BUS_CLOCK_SYMCRYPTO_VALUE)

/// Define for SYNTH peripheral bus clock pointer.
#define SL_BUS_CLOCK_SYNTH (&SL_BUS_CLOCK_SYNTH_VALUE)

/// Define for SYSCFG peripheral bus clock pointer.
#define SL_BUS_CLOCK_SYSCFG (&SL_BUS_CLOCK_SYSCFG_VALUE)

/// Define for SYSRTC0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_SYSRTC0 (&SL_BUS_CLOCK_SYSRTC0_VALUE)

/// Define for TIMER0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER0 (&SL_BUS_CLOCK_TIMER0_VALUE)

/// Define for TIMER1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER1 (&SL_BUS_CLOCK_TIMER1_VALUE)

/// Define for TIMER2 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER2 (&SL_BUS_CLOCK_TIMER2_VALUE)

/// Define for TIMER3 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER3 (&SL_BUS_CLOCK_TIMER3_VALUE)

/// Define for TIMER4 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER4 (&SL_BUS_CLOCK_TIMER4_VALUE)

/// Define for TIMER5 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER5 (&SL_BUS_CLOCK_TIMER5_VALUE)

/// Define for TIMER6 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER6 (&SL_BUS_CLOCK_TIMER6_VALUE)

/// Define for TIMER7 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER7 (&SL_BUS_CLOCK_TIMER7_VALUE)

/// Define for TIMER8 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER8 (&SL_BUS_CLOCK_TIMER8_VALUE)

/// Define for TIMER9 peripheral bus clock pointer.
#define SL_BUS_CLOCK_TIMER9 (&SL_BUS_CLOCK_TIMER9_VALUE)

/// Define for ULFRCO peripheral bus clock pointer.
#define SL_BUS_CLOCK_ULFRCO (&SL_BUS_CLOCK_ULFRCO_VALUE)

/// Define for USART0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_USART0 (&SL_BUS_CLOCK_USART0_VALUE)

/// Define for USART1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_USART1 (&SL_BUS_CLOCK_USART1_VALUE)

/// Define for USART2 peripheral bus clock pointer.
#define SL_BUS_CLOCK_USART2 (&SL_BUS_CLOCK_USART2_VALUE)

/// Define for USB peripheral bus clock pointer.
#define SL_BUS_CLOCK_USB (&SL_BUS_CLOCK_USB_VALUE)

/// Define for VDAC0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_VDAC0 (&SL_BUS_CLOCK_VDAC0_VALUE)

/// Define for VDAC1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_VDAC1 (&SL_BUS_CLOCK_VDAC1_VALUE)

/// Define for WDOG0 peripheral bus clock pointer.
#define SL_BUS_CLOCK_WDOG0 (&SL_BUS_CLOCK_WDOG0_VALUE)

/// Define for WDOG1 peripheral bus clock pointer.
#define SL_BUS_CLOCK_WDOG1 (&SL_BUS_CLOCK_WDOG1_VALUE)

/// @} (end bus_clock_defines)

// ----------------------------------------------------------------------------
// TYPEDEFS

/// The bus clock typedef.
typedef const uint32_t* sl_bus_clock_t;

// ----------------------------------------------------------------------------
// EXTERNS

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

// External declaration for invalid peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_INVALID_VALUE;

// External declaration for ACMP0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ACMP0_VALUE;

// External declaration for ACMP1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ACMP1_VALUE;

// External declaration for ADC0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ADC0_VALUE;

// External declaration for AGC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_AGC_VALUE;

// External declaration for AMUXCP0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_AMUXCP0_VALUE;

// External declaration for BUFC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_BUFC_VALUE;

// External declaration for BURAM peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_BURAM_VALUE;

// External declaration for BURTC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_BURTC_VALUE;

// External declaration for CRYPTOACC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_CRYPTOACC_VALUE;

// External declaration for DCDC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_DCDC_VALUE;

// External declaration for DEVINFO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_DEVINFO_VALUE;

// External declaration for DMEM peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_DMEM_VALUE;

// External declaration for DPLL0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_DPLL0_VALUE;

// External declaration for ECAIFADC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ECAIFADC_VALUE;

// External declaration for ETAMPDET peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ETAMPDET_VALUE;

// External declaration for EUART0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_EUART0_VALUE;

// External declaration for EUSART0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_EUSART0_VALUE;

// External declaration for EUSART1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_EUSART1_VALUE;

// External declaration for EUSART2 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_EUSART2_VALUE;

// External declaration for EUSART3 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_EUSART3_VALUE;

// External declaration for EUSART4 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_EUSART4_VALUE;

// External declaration for FRC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_FRC_VALUE;

// External declaration for FSRCO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_FSRCO_VALUE;

// External declaration for GPCRC0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_GPCRC0_VALUE;

// External declaration for GPIO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_GPIO_VALUE;

// External declaration for HFRCO0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_HFRCO0_VALUE;

// External declaration for HFRCOEM23 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_HFRCOEM23_VALUE;

// External declaration for HFXO0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_HFXO0_VALUE;

// External declaration for HOSTMAILBOX peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_HOSTMAILBOX_VALUE;

// External declaration for HOSTPORTAL peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_HOSTPORTAL_VALUE;

// External declaration for I2C0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_I2C0_VALUE;

// External declaration for I2C1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_I2C1_VALUE;

// External declaration for I2C2 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_I2C2_VALUE;

// External declaration for I2C3 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_I2C3_VALUE;

// External declaration for IADC0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_IADC0_VALUE;

// External declaration for ICACHE0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ICACHE0_VALUE;

// External declaration for IFADCDEBUG peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_IFADCDEBUG_VALUE;

// External declaration for KEYSCAN peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_KEYSCAN_VALUE;

// External declaration for KSU peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_KSU_VALUE;

// External declaration for L2ICACHE0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_L2ICACHE0_VALUE;

// External declaration for LCD peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LCD_VALUE;

// External declaration for LDMA0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LDMA0_VALUE;

// External declaration for LDMAXBAR0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LDMAXBAR0_VALUE;

// External declaration for LEDDRV0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LEDDRV0_VALUE;

// External declaration for LESENSE peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LESENSE_VALUE;

// External declaration for LETIMER0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LETIMER0_VALUE;

// External declaration for LFRCO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LFRCO_VALUE;

// External declaration for LFXO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LFXO_VALUE;

// External declaration for LPWAES peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LPWAES_VALUE;

// External declaration for LPW0PORTAL peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_LPW0PORTAL_VALUE;

// External declaration for MODEM peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_MODEM_VALUE;

// External declaration for MSC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_MSC_VALUE;

// External declaration for MVP peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_MVP_VALUE;

// External declaration for PCNT0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PCNT0_VALUE;

// External declaration for PDM peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PDM_VALUE;

// External declaration for PIXELRZ0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PIXELRZ0_VALUE;

// External declaration for PIXELRZ1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PIXELRZ1_VALUE;

// External declaration for PRORTC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PRORTC_VALUE;

// External declaration for PROTIMER peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PROTIMER_VALUE;

// External declaration for PRS peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_PRS_VALUE;

// External declaration for RAC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RAC_VALUE;

// External declaration for RADIOAES peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RADIOAES_VALUE;

// External declaration for RDMAILBOX0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RDMAILBOX0_VALUE;

// External declaration for RDMAILBOX1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RDMAILBOX1_VALUE;

// External declaration for RDSCRATCHPAD peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RDSCRATCHPAD_VALUE;

// External declaration for RFCRC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFCRC_VALUE;

// External declaration for RFECA0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFECA0_VALUE;

// External declaration for RFECA1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFECA1_VALUE;

// External declaration for RFFPLL0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFFPLL0_VALUE;

// External declaration for RFMAILBOX peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFMAILBOX_VALUE;

// External declaration for RFSCRATCHPAD peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFSCRATCHPAD_VALUE;

// External declaration for RFSENSE peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RFSENSE_VALUE;

// External declaration for RPA peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RPA_VALUE;

// External declaration for RTCC peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_RTCC_VALUE;

// External declaration for SCRATCHPAD peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SCRATCHPAD_VALUE;

// External declaration for SEMAILBOX peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SEMAILBOX_VALUE;

// External declaration for SEMAPHORE0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SEMAPHORE0_VALUE;

// External declaration for SEMAPHORE1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SEMAPHORE1_VALUE;

// External declaration for SEPORTAL peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SEPORTAL_VALUE;

// External declaration for SMU peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SMU_VALUE;

// External declaration for SOCPLL0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SOCPLL0_VALUE;

// External declaration for SYMCRYPTO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SYMCRYPTO_VALUE;

// External declaration for SYNTH peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SYNTH_VALUE;

// External declaration for SYSCFG peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SYSCFG_VALUE;

// External declaration for SYSRTC0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_SYSRTC0_VALUE;

// External declaration for TIMER0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER0_VALUE;

// External declaration for TIMER1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER1_VALUE;

// External declaration for TIMER2 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER2_VALUE;

// External declaration for TIMER3 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER3_VALUE;

// External declaration for TIMER4 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER4_VALUE;

// External declaration for TIMER5 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER5_VALUE;

// External declaration for TIMER6 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER6_VALUE;

// External declaration for TIMER7 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER7_VALUE;

// External declaration for TIMER8 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER8_VALUE;

// External declaration for TIMER9 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_TIMER9_VALUE;

// External declaration for ULFRCO peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_ULFRCO_VALUE;

// External declaration for USART0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_USART0_VALUE;

// External declaration for USART1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_USART1_VALUE;

// External declaration for USART2 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_USART2_VALUE;

// External declaration for USB peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_USB_VALUE;

// External declaration for VDAC0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_VDAC0_VALUE;

// External declaration for VDAC1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_VDAC1_VALUE;

// External declaration for WDOG0 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_WDOG0_VALUE;

// External declaration for WDOG1 peripheral bus clock value.
extern const uint32_t SL_BUS_CLOCK_WDOG1_VALUE;

/// @endcond

/** @} (end addtogroup device_clock) */

#ifdef __cplusplus
}
#endif

#endif // SL_DEVICE_CLOCK_H
