/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "wiring.h"
#include "gpiointerrupt.h"
#include <list>

typedef struct {
  uint8_t pin;
  void (*callback)(void);
  uint32_t interrupt_num;
} gpio_interrupt_handler_t;

static std::list<gpio_interrupt_handler_t> gpio_interrupt_handlers;

GPIO_Port_TypeDef getSilabsPortFromArduinoPin(uint8_t pin)
{
  if (pin >= PD0) {
    return gpioPortD;
  } else if (pin >= PC0) {
    return gpioPortC;
  } else if (pin >= PB0) {
    return gpioPortB;
  }
  return gpioPortA;
}

uint32_t getSilabsPinFromArduinoPin(uint8_t pin)
{
  return pin % 16;
}

void pinMode(uint8_t pin, uint8_t mode)
{
  GPIO_Port_TypeDef hw_port = getSilabsPortFromArduinoPin(pin);
  uint32_t hw_pin = getSilabsPinFromArduinoPin(pin);
  switch (mode) {
    case pinmode::OUTPUT:
      GPIO_PinModeSet(hw_port, hw_pin, gpioModePushPull, 0);
      break;

    case pinmode::INPUT:
      GPIO_PinModeSet(hw_port, hw_pin, gpioModeInput, 0);
      break;

    case pinmode::INPUT_PULLUP:
      GPIO_PinModeSet(hw_port, hw_pin, gpioModeInputPull, 1);
      break;

    default:
      break;
  }
}

int digitalRead(uint8_t pin)
{
  GPIO_Port_TypeDef hw_port = getSilabsPortFromArduinoPin(pin);
  uint32_t hw_pin = getSilabsPinFromArduinoPin(pin);
  unsigned int pin_value = GPIO_PinInGet(hw_port, hw_pin);
  return pin_value ? digital_pin_state::HIGH : digital_pin_state::LOW;
}

void digitalWrite(uint8_t pin, uint8_t state)
{
  GPIO_Port_TypeDef out_port = getSilabsPortFromArduinoPin(pin);
  uint32_t out_pin = getSilabsPinFromArduinoPin(pin);
  if (state == digital_pin_state::LOW) {
    GPIO_PinOutClear(out_port, out_pin);
  } else {
    GPIO_PinOutSet(out_port, out_pin);
  }
}

uint8_t digitalPinToInterrupt(uint8_t pin)
{
  // We don't need to do any mapping, it's done in attachInterrupt
  // This function is only here for compatibility
  return pin;
}

static void gpio_irq_handler(uint8_t interrupt_num, void *ctx)
{
  (void)ctx;
  // Go through all the interrupt handler entries
  for (auto& entry : gpio_interrupt_handlers) {
    // If the entry matches the triggered interrupt number - call the registered callback
    if (entry.interrupt_num == interrupt_num && entry.callback) {
      entry.callback();
    }
  }
}

void attachInterrupt(uint8_t pin, void (*callback)(void), int mode)
{
  if (pin >= PIN_MAX || callback == nullptr || mode < 0 || mode > RISING) {
    return;
  }

  GPIO_Port_TypeDef sl_port = getSilabsPortFromArduinoPin(pin);
  uint32_t sl_pin = getSilabsPinFromArduinoPin(pin);

  bool rising_edge = false;
  bool falling_edge = false;
  switch (mode) {
    case CHANGE:
      rising_edge = true;
      falling_edge = true;
      break;

    case LOW:
    case FALLING:
      rising_edge = false;
      falling_edge = true;
      break;

    case RISING:
      rising_edge = true;
      falling_edge = false;
      break;
  }

  // Allocate an interrupt number for the pin
  uint32_t interrupt_num = GPIOINT_CallbackRegisterExt(sl_pin, &gpio_irq_handler, nullptr);
  if (interrupt_num == INTERRUPT_UNAVAILABLE) {
    return;
  }

  // Configure the external interrupt for the pin
  GPIO_ExtIntConfig(sl_port, sl_pin, interrupt_num, rising_edge, falling_edge, true);

  // Register the GPIO interrupt handler
  gpio_interrupt_handler_t inthandler_entry;
  inthandler_entry.pin = pin;
  inthandler_entry.callback = callback;
  inthandler_entry.interrupt_num = interrupt_num;
  gpio_interrupt_handlers.push_back(inthandler_entry);
}

void detachInterrupt(uint8_t pin)
{
  bool entry_found = false;
  uint8_t interrupt_num;
  // Find the handler entry for the requested pin
  for (auto it = gpio_interrupt_handlers.begin(); it != gpio_interrupt_handlers.end(); it++) {
    // If found - remove it
    if (it->pin == pin) {
      interrupt_num = it->interrupt_num;
      GPIOINT_CallbackUnRegister(interrupt_num);
      entry_found = true;
      gpio_interrupt_handlers.erase(it);
      break;
    }
  }

  // Return if the entry for the pin was not found
  if (!entry_found) {
    return;
  }

  // Deregister the external interrupt as well
  GPIO_Port_TypeDef sl_port = getSilabsPortFromArduinoPin(pin);
  uint32_t sl_pin = getSilabsPinFromArduinoPin(pin);
  GPIO_ExtIntConfig(sl_port, sl_pin, interrupt_num, false, false, false);
}
