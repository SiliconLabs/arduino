/***************************************************************************//**
 * @file
 * @brief Device Manager GPIO.
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

#ifndef SL_DEVICE_GPIO_H
#define SL_DEVICE_GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_enum.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup device_gpio Device Manager GPIO
 * @details
 * ## Overview
 *
 * The Device Manager GPIO component defines the macros,
 * structures, and enums that are used common across GPIO driver and
 * peripheral.
 *
 * @{
 ******************************************************************************/

// ----------------------------------------------------------------------------
// ENUMS

/// GPIO ports IDs.
SL_ENUM(sl_gpio_port_t) {
  SL_GPIO_PORT_A = 0,
  SL_GPIO_PORT_B = 1,
  SL_GPIO_PORT_C = 2,
  SL_GPIO_PORT_D = 3,
  SL_GPIO_PORT_E = 4,
  SL_GPIO_PORT_F = 5,
  SL_GPIO_PORT_G = 6,
  SL_GPIO_PORT_H = 7,
  SL_GPIO_PORT_I = 8,
  SL_GPIO_PORT_J = 9,
  SL_GPIO_PORT_K = 10,
};

/// GPIO Pin Modes.
SL_ENUM(sl_gpio_mode_t) {
  /// Input disabled. Pull-up if DOUT is set.
  SL_GPIO_MODE_DISABLED,

  /// Input enabled. Filter if DOUT is set.
  SL_GPIO_MODE_INPUT,

  /// Input enabled. DOUT determines pull direction.
  SL_GPIO_MODE_INPUT_PULL,

  /// Input enabled with filter. DOUT determines pull direction.
  SL_GPIO_MODE_INPUT_PULL_FILTER,

  /// Push-pull output.
  SL_GPIO_MODE_PUSH_PULL,

  /// Push-pull using alternate control.
  SL_GPIO_MODE_PUSH_PULL_ALTERNATE,

  /// Wired-or output.
  SL_GPIO_MODE_WIRED_OR,

  /// Wired-or output with pull-down.
  SL_GPIO_MODE_WIRED_OR_PULL_DOWN,

  /// Open-drain output.
  SL_GPIO_MODE_WIRED_AND,

  /// Open-drain output with filter.
  SL_GPIO_MODE_WIRED_AND_FILTER,

  /// Open-drain output with pull-up.
  SL_GPIO_MODE_WIRED_AND_PULLUP,

  /// Open-drain output with filter and pull-up.
  SL_GPIO_MODE_WIRED_AND_PULLUP_FILTER,

  /// Open-drain output using alternate control.
  SL_GPIO_MODE_WIRED_AND_ALTERNATE,

  /// Open-drain output using alternate control with filter.
  SL_GPIO_MODE_WIRED_AND_ALTERNATE_FILTER,

  /// Open-drain output using alternate control with pull-up.
  SL_GPIO_MODE_WIRED_AND_ALTERNATE_PULLUP,

  /// Open-drain output using alternate control with filter and pull-up.
  SL_GPIO_MODE_WIRED_AND_ALTERNATE_PULLUP_FILTER,
};

/// GPIO Interrupt Configuration flags.
SL_ENUM(sl_gpio_interrupt_flag_t) {
  /// No edge configured.
  SL_GPIO_INTERRUPT_NO_EDGE = 0,

  /// Interrupt on rising edge.
  SL_GPIO_INTERRUPT_RISING_EDGE  = (1 << 0),

  /// Interrupt on falling edge.
  SL_GPIO_INTERRUPT_FALLING_EDGE = (1 << 1),

  /// Interrupt on both rising and falling edge.
  SL_GPIO_INTERRUPT_RISING_FALLING_EDGE = (1 << 2)
};

/*******************************************************************************
 ********************************   DEFINES   **********************************
 ******************************************************************************/

#define SL_GPIO_INTERRUPT_UNAVAILABLE (-1)

/// Validation of flag.
#define SL_GPIO_FLAG_IS_VALID(flag)  ((flag == SL_GPIO_INTERRUPT_NO_EDGE) || (flag == SL_GPIO_INTERRUPT_RISING_EDGE) || (flag == SL_GPIO_INTERRUPT_FALLING_EDGE) || (flag == SL_GPIO_INTERRUPT_RISING_FALLING_EDGE))

/// Define for available ports and pins
#define PA0 (&pa0)
#define PA1 (&pa1)
#define PA2 (&pa2)
#define PA3 (&pa3)
#define PA4 (&pa4)
#define PA5 (&pa5)
#define PA6 (&pa6)
#define PA7 (&pa7)
#define PA8 (&pa8)
#define PA9 (&pa9)
#define PA10 (&pa10)
#define PA11 (&pa11)
#define PA12 (&pa12)
#define PA13 (&pa13)
#define PA14 (&pa14)
#define PA15 (&pa15)
#define PA16 (&pa16)
#define PA17 (&pa17)
#define PA18 (&pa18)
#define PA19 (&pa19)
#define PA20 (&pa20)
#define PA21 (&pa21)
#define PA22 (&pa22)
#define PA23 (&pa23)
#define PA24 (&pa24)
#define PA25 (&pa25)
#define PA26 (&pa26)
#define PA27 (&pa27)
#define PA28 (&pa28)
#define PA29 (&pa29)
#define PA30 (&pa30)
#define PA31 (&pa31)

#define PB0 (&pb0)
#define PB1 (&pb1)
#define PB2 (&pb2)
#define PB3 (&pb3)
#define PB4 (&pb4)
#define PB5 (&pb5)
#define PB6 (&pb6)
#define PB7 (&pb7)
#define PB8 (&pb8)
#define PB9 (&pb9)
#define PB10 (&pb10)
#define PB11 (&pb11)
#define PB12 (&pb12)
#define PB13 (&pb13)
#define PB14 (&pb14)
#define PB15 (&pb15)
#define PB16 (&pb16)
#define PB17 (&pb17)
#define PB18 (&pb18)
#define PB19 (&pb19)
#define PB20 (&pb20)
#define PB21 (&pb21)
#define PB22 (&pb22)
#define PB23 (&pb23)
#define PB24 (&pb24)
#define PB25 (&pb25)
#define PB26 (&pb26)
#define PB27 (&pb27)
#define PB28 (&pb28)
#define PB29 (&pb29)
#define PB30 (&pb30)
#define PB31 (&pb31)

#define PC0 (&pc0)
#define PC1 (&pc1)
#define PC2 (&pc2)
#define PC3 (&pc3)
#define PC4 (&pc4)
#define PC5 (&pc5)
#define PC6 (&pc6)
#define PC7 (&pc7)
#define PC8 (&pc8)
#define PC9 (&pc9)
#define PC10 (&pc10)
#define PC11 (&pc11)
#define PC12 (&pc12)
#define PC13 (&pc13)
#define PC14 (&pc14)
#define PC15 (&pc15)
#define PC16 (&pc16)
#define PC17 (&pc17)
#define PC18 (&pc18)
#define PC19 (&pc19)
#define PC20 (&pc20)
#define PC21 (&pc21)
#define PC22 (&pc22)
#define PC23 (&pc23)
#define PC24 (&pc24)
#define PC25 (&pc25)
#define PC26 (&pc26)
#define PC27 (&pc27)
#define PC28 (&pc28)
#define PC29 (&pc29)
#define PC30 (&pc30)
#define PC31 (&pc31)

#define PD0 (&pd0)
#define PD1 (&pd1)
#define PD2 (&pd2)
#define PD3 (&pd3)
#define PD4 (&pd4)
#define PD5 (&pd5)
#define PD6 (&pd6)
#define PD7 (&pd7)
#define PD8 (&pd8)
#define PD9 (&pd9)
#define PD10 (&pd10)
#define PD11 (&pd11)
#define PD12 (&pd12)
#define PD13 (&pd13)
#define PD14 (&pd14)
#define PD15 (&pd15)
#define PD16 (&pd16)
#define PD17 (&pd17)
#define PD18 (&pd18)
#define PD19 (&pd19)
#define PD20 (&pd20)
#define PD21 (&pd21)
#define PD22 (&pd22)
#define PD23 (&pd23)
#define PD24 (&pd24)
#define PD25 (&pd25)
#define PD26 (&pd26)
#define PD27 (&pd27)
#define PD28 (&pd28)
#define PD29 (&pd29)
#define PD30 (&pd30)
#define PD31 (&pd31)

#define PE0 (&pe0)
#define PE1 (&pe1)
#define PE2 (&pe2)
#define PE3 (&pe3)
#define PE4 (&pe4)
#define PE5 (&pe5)
#define PE6 (&pe6)
#define PE7 (&pe7)
#define PE8 (&pe8)
#define PE9 (&pe9)
#define PE10 (&pe10)
#define PE11 (&pe11)
#define PE12 (&pe12)
#define PE13 (&pe13)
#define PE14 (&pe14)
#define PE15 (&pe15)
#define PE16 (&pe16)
#define PE17 (&pe17)
#define PE18 (&pe18)
#define PE19 (&pe19)
#define PE20 (&pe20)
#define PE21 (&pe21)
#define PE22 (&pe22)
#define PE23 (&pe23)
#define PE24 (&pe24)
#define PE25 (&pe25)
#define PE26 (&pe26)
#define PE27 (&pe27)
#define PE28 (&pe28)
#define PE29 (&pe29)
#define PE30 (&pe30)
#define PE31 (&pe31)

#define PF0 (&pf0)
#define PF1 (&pf1)
#define PF2 (&pf2)
#define PF3 (&pf3)
#define PF4 (&pf4)
#define PF5 (&pf5)
#define PF6 (&pf6)
#define PF7 (&pf7)
#define PF8 (&pf8)
#define PF9 (&pf9)
#define PF10 (&pf10)
#define PF11 (&pf11)
#define PF12 (&pf12)
#define PF13 (&pf13)
#define PF14 (&pf14)
#define PF15 (&pf15)
#define PF16 (&pf16)
#define PF17 (&pf17)
#define PF18 (&pf18)
#define PF19 (&pf19)
#define PF20 (&pf20)
#define PF21 (&pf21)
#define PF22 (&pf22)
#define PF23 (&pf23)
#define PF24 (&pf24)
#define PF25 (&pf25)
#define PF26 (&pf26)
#define PF27 (&pf27)
#define PF28 (&pf28)
#define PF29 (&pf29)
#define PF30 (&pf30)
#define PF31 (&pf31)

#define PG0 (&pg0)
#define PG1 (&pg1)
#define PG2 (&pg2)
#define PG3 (&pg3)
#define PG4 (&pg4)
#define PG5 (&pg5)
#define PG6 (&pg6)
#define PG7 (&pg7)
#define PG8 (&pg8)
#define PG9 (&pg9)
#define PG10 (&pg10)
#define PG11 (&pg11)
#define PG12 (&pg12)
#define PG13 (&pg13)
#define PG14 (&pg14)
#define PG15 (&pg15)
#define PG16 (&pg16)
#define PG17 (&pg17)
#define PG18 (&pg18)
#define PG19 (&pg19)
#define PG20 (&pg20)
#define PG21 (&pg21)
#define PG22 (&pg22)
#define PG23 (&pg23)
#define PG24 (&pg24)
#define PG25 (&pg25)
#define PG26 (&pg26)
#define PG27 (&pg27)
#define PG28 (&pg28)
#define PG29 (&pg29)
#define PG30 (&pg30)
#define PG31 (&pg31)

#define PH0 (&ph0)
#define PH1 (&ph1)
#define PH2 (&ph2)
#define PH3 (&ph3)
#define PH4 (&ph4)
#define PH5 (&ph5)
#define PH6 (&ph6)
#define PH7 (&ph7)
#define PH8 (&ph8)
#define PH9 (&ph9)
#define PH10 (&ph10)
#define PH11 (&ph11)
#define PH12 (&ph12)
#define PH13 (&ph13)
#define PH14 (&ph14)
#define PH15 (&ph15)
#define PH16 (&ph16)
#define PH17 (&ph17)
#define PH18 (&ph18)
#define PH19 (&ph19)
#define PH20 (&ph20)
#define PH21 (&ph21)
#define PH22 (&ph22)
#define PH23 (&ph23)
#define PH24 (&ph24)
#define PH25 (&ph25)
#define PH26 (&ph26)
#define PH27 (&ph27)
#define PH28 (&ph28)
#define PH29 (&ph29)
#define PH30 (&ph30)
#define PH31 (&ph31)

#define PI0 (&pi0)
#define PI1 (&pi1)
#define PI2 (&pi2)
#define PI3 (&pi3)
#define PI4 (&pi4)
#define PI5 (&pi5)
#define PI6 (&pi6)
#define PI7 (&pi7)
#define PI8 (&pi8)
#define PI9 (&pi9)
#define PI10 (&pi10)
#define PI11 (&pi11)
#define PI12 (&pi12)
#define PI13 (&pi13)
#define PI14 (&pi14)
#define PI15 (&pi15)
#define PI16 (&pi16)
#define PI17 (&pi17)
#define PI18 (&pi18)
#define PI19 (&pi19)
#define PI20 (&pi20)
#define PI21 (&pi21)
#define PI22 (&pi22)
#define PI23 (&pi23)
#define PI24 (&pi24)
#define PI25 (&pi25)
#define PI26 (&pi26)
#define PI27 (&pi27)
#define PI28 (&pi28)
#define PI29 (&pi29)
#define PI30 (&pi30)
#define PI31 (&pi31)

#define PJ0 (&pj0)
#define PJ1 (&pj1)
#define PJ2 (&pj2)
#define PJ3 (&pj3)
#define PJ4 (&pj4)
#define PJ5 (&pj5)
#define PJ6 (&pj6)
#define PJ7 (&pj7)
#define PJ8 (&pj8)
#define PJ9 (&pj9)
#define PJ10 (&pj10)
#define PJ11 (&pj11)
#define PJ12 (&pj12)
#define PJ13 (&pj13)
#define PJ14 (&pj14)
#define PJ15 (&pj15)
#define PJ16 (&pj16)
#define PJ17 (&pj17)
#define PJ18 (&pj18)
#define PJ19 (&pj19)
#define PJ20 (&pj20)
#define PJ21 (&pj21)
#define PJ22 (&pj22)
#define PJ23 (&pj23)
#define PJ24 (&pj24)
#define PJ25 (&pj25)
#define PJ26 (&pj26)
#define PJ27 (&pj27)
#define PJ28 (&pj28)
#define PJ29 (&pj29)
#define PJ30 (&pj30)
#define PJ31 (&pj31)

#define PK0 (&pk0)
#define PK1 (&pk1)
#define PK2 (&pk2)
#define PK3 (&pk3)
#define PK4 (&pk4)
#define PK5 (&pk5)
#define PK6 (&pk6)
#define PK7 (&pk7)
#define PK8 (&pk8)
#define PK9 (&pk9)
#define PK10 (&pk10)
#define PK11 (&pk11)
#define PK12 (&pk12)
#define PK13 (&pk13)
#define PK14 (&pk14)
#define PK15 (&pk15)
#define PK16 (&pk16)
#define PK17 (&pk17)
#define PK18 (&pk18)
#define PK19 (&pk19)
#define PK20 (&pk20)
#define PK21 (&pk21)
#define PK22 (&pk22)
#define PK23 (&pk23)
#define PK24 (&pk24)
#define PK25 (&pk25)
#define PK26 (&pk26)
#define PK27 (&pk27)
#define PK28 (&pk28)
#define PK29 (&pk29)
#define PK30 (&pk30)
#define PK31 (&pk31)

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/// Structure for GPIO Port and Pin.
typedef struct {
  uint8_t port;
  uint8_t pin;
} sl_gpio_t;

// ----------------------------------------------------------------------------
// EXTERNS

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

extern const sl_gpio_t pa0;
extern const sl_gpio_t pa1;
extern const sl_gpio_t pa2;
extern const sl_gpio_t pa3;
extern const sl_gpio_t pa4;
extern const sl_gpio_t pa5;
extern const sl_gpio_t pa6;
extern const sl_gpio_t pa7;
extern const sl_gpio_t pa8;
extern const sl_gpio_t pa9;
extern const sl_gpio_t pa10;
extern const sl_gpio_t pa11;
extern const sl_gpio_t pa12;
extern const sl_gpio_t pa13;
extern const sl_gpio_t pa14;
extern const sl_gpio_t pa15;
extern const sl_gpio_t pa16;
extern const sl_gpio_t pa17;
extern const sl_gpio_t pa18;
extern const sl_gpio_t pa19;
extern const sl_gpio_t pa20;
extern const sl_gpio_t pa21;
extern const sl_gpio_t pa22;
extern const sl_gpio_t pa23;
extern const sl_gpio_t pa24;
extern const sl_gpio_t pa25;
extern const sl_gpio_t pa26;
extern const sl_gpio_t pa27;
extern const sl_gpio_t pa28;
extern const sl_gpio_t pa29;
extern const sl_gpio_t pa30;
extern const sl_gpio_t pa31;
extern const sl_gpio_t pa32;

extern const sl_gpio_t pb0;
extern const sl_gpio_t pb1;
extern const sl_gpio_t pb2;
extern const sl_gpio_t pb3;
extern const sl_gpio_t pb4;
extern const sl_gpio_t pb5;
extern const sl_gpio_t pb6;
extern const sl_gpio_t pb7;
extern const sl_gpio_t pb8;
extern const sl_gpio_t pb9;
extern const sl_gpio_t pb10;
extern const sl_gpio_t pb11;
extern const sl_gpio_t pb12;
extern const sl_gpio_t pb13;
extern const sl_gpio_t pb14;
extern const sl_gpio_t pb15;
extern const sl_gpio_t pb16;
extern const sl_gpio_t pb17;
extern const sl_gpio_t pb18;
extern const sl_gpio_t pb19;
extern const sl_gpio_t pb20;
extern const sl_gpio_t pb21;
extern const sl_gpio_t pb22;
extern const sl_gpio_t pb23;
extern const sl_gpio_t pb24;
extern const sl_gpio_t pb25;
extern const sl_gpio_t pb26;
extern const sl_gpio_t pb27;
extern const sl_gpio_t pb28;
extern const sl_gpio_t pb29;
extern const sl_gpio_t pb30;
extern const sl_gpio_t pb31;
extern const sl_gpio_t pb32;

extern const sl_gpio_t pc0;
extern const sl_gpio_t pc1;
extern const sl_gpio_t pc2;
extern const sl_gpio_t pc3;
extern const sl_gpio_t pc4;
extern const sl_gpio_t pc5;
extern const sl_gpio_t pc6;
extern const sl_gpio_t pc7;
extern const sl_gpio_t pc8;
extern const sl_gpio_t pc9;
extern const sl_gpio_t pc10;
extern const sl_gpio_t pc11;
extern const sl_gpio_t pc12;
extern const sl_gpio_t pc13;
extern const sl_gpio_t pc14;
extern const sl_gpio_t pc15;
extern const sl_gpio_t pc16;
extern const sl_gpio_t pc17;
extern const sl_gpio_t pc18;
extern const sl_gpio_t pc19;
extern const sl_gpio_t pc20;
extern const sl_gpio_t pc21;
extern const sl_gpio_t pc22;
extern const sl_gpio_t pc23;
extern const sl_gpio_t pc24;
extern const sl_gpio_t pc25;
extern const sl_gpio_t pc26;
extern const sl_gpio_t pc27;
extern const sl_gpio_t pc28;
extern const sl_gpio_t pc29;
extern const sl_gpio_t pc30;
extern const sl_gpio_t pc31;
extern const sl_gpio_t pc32;

extern const sl_gpio_t pd0;
extern const sl_gpio_t pd1;
extern const sl_gpio_t pd2;
extern const sl_gpio_t pd3;
extern const sl_gpio_t pd4;
extern const sl_gpio_t pd5;
extern const sl_gpio_t pd6;
extern const sl_gpio_t pd7;
extern const sl_gpio_t pd8;
extern const sl_gpio_t pd9;
extern const sl_gpio_t pd10;
extern const sl_gpio_t pd11;
extern const sl_gpio_t pd12;
extern const sl_gpio_t pd13;
extern const sl_gpio_t pd14;
extern const sl_gpio_t pd15;
extern const sl_gpio_t pd16;
extern const sl_gpio_t pd17;
extern const sl_gpio_t pd18;
extern const sl_gpio_t pd19;
extern const sl_gpio_t pd20;
extern const sl_gpio_t pd21;
extern const sl_gpio_t pd22;
extern const sl_gpio_t pd23;
extern const sl_gpio_t pd24;
extern const sl_gpio_t pd25;
extern const sl_gpio_t pd26;
extern const sl_gpio_t pd27;
extern const sl_gpio_t pd28;
extern const sl_gpio_t pd29;
extern const sl_gpio_t pd30;
extern const sl_gpio_t pd31;
extern const sl_gpio_t pd32;

/// @endcond

/** @} (end addtogroup device_gpio) */

#ifdef __cplusplus
}
#endif

#endif // SL_DEVICE_GPIO_H
