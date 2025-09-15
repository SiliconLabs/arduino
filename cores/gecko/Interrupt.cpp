/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2025 Silicon Laboratories Inc. www.silabs.com
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

#include "Arduino.h"
#include "pinDefinitions.h"

#include <list>
#include "gpiointerrupt.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct {
  PinName pin_name;
  void (*callback)(void);
  uint32_t interrupt_num;
} gpio_interrupt_handler_t;

static std::list<gpio_interrupt_handler_t> gpio_interrupt_handlers;

SemaphoreHandle_t gpio_isr_mutex;
StaticSemaphore_t gpio_isr_mutex_buf;

static void gpio_irq_handler(uint8_t interrupt_num, void *ctx)
{
  (void)ctx;
  UBaseType_t uxSavedInterruptStatus;
  uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

  // Go through all the interrupt handler entries
  for (auto& entry : gpio_interrupt_handlers) {
    // If the entry matches the triggered interrupt number - call the registered callback
    if (entry.interrupt_num == interrupt_num && entry.callback) {
      entry.callback();
    }
  }

  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

void gpio_interrupt_handler_init()
{
  gpio_isr_mutex = xSemaphoreCreateMutexStatic(&gpio_isr_mutex_buf);
  configASSERT(gpio_isr_mutex);
}

void detachInterrupt(PinName interruptNumber)
{
  xSemaphoreTake(gpio_isr_mutex, portMAX_DELAY);
  bool entry_found = false;
  uint8_t interrupt_num;
  // Find the handler entry for the requested pin
  for (auto it = gpio_interrupt_handlers.begin(); it != gpio_interrupt_handlers.end(); it++) {
    // If found - remove it
    if (it->pin_name == interruptNumber) {
      interrupt_num = it->interrupt_num;
      GPIOINT_CallbackUnRegister(interrupt_num);
      entry_found = true;
      gpio_interrupt_handlers.erase(it);
      break;
    }
  }

  // Return if the entry for the pin was not found
  if (!entry_found) {
    xSemaphoreGive(gpio_isr_mutex);
    return;
  }

  // Deregister the external interrupt as well
  GPIO_Port_TypeDef sl_port = getSilabsPortFromArduinoPin(interruptNumber);
  uint32_t sl_pin = getSilabsPinFromArduinoPin(interruptNumber);
  GPIO_ExtIntConfig(sl_port, sl_pin, interrupt_num, false, false, false);
  xSemaphoreGive(gpio_isr_mutex);
}

void detachInterrupt(pin_size_t interruptNumber)
{
  PinName actual_pin = pinToPinName(interruptNumber);
  if (actual_pin == PIN_NAME_NC) {
    return;
  }
  detachInterrupt(actual_pin);
}

void attachInterruptParam(PinName interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param)
{
  (void)interruptNumber;
  (void)callback;
  (void)mode;
  (void)param;
}

void attachInterrupt(PinName interruptNumber, voidFuncPtr callback, PinStatus mode)
{
  if (interruptNumber >= PIN_NAME_MAX || callback == nullptr || mode < LOW || mode > RISING || !get_system_init_finished()) {
    return;
  }

  xSemaphoreTake(gpio_isr_mutex, portMAX_DELAY);
  GPIO_Port_TypeDef sl_port = getSilabsPortFromArduinoPin(interruptNumber);
  uint32_t sl_pin = getSilabsPinFromArduinoPin(interruptNumber);

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

    case HIGH:
    case RISING:
      rising_edge = true;
      falling_edge = false;
      break;
  }

  // Allocate an interrupt number for the pin
  uint32_t interrupt_num = GPIOINT_CallbackRegisterExt(sl_pin, &gpio_irq_handler, nullptr);
  if (interrupt_num == INTERRUPT_UNAVAILABLE) {
    xSemaphoreGive(gpio_isr_mutex);
    return;
  }

  // Configure the external interrupt for the pin
  GPIO_ExtIntConfig(sl_port, sl_pin, interrupt_num, rising_edge, falling_edge, true);

  // Register the GPIO interrupt handler
  gpio_interrupt_handler_t inthandler_entry;
  inthandler_entry.pin_name = interruptNumber;
  inthandler_entry.callback = callback;
  inthandler_entry.interrupt_num = interrupt_num;
  gpio_interrupt_handlers.push_back(inthandler_entry);
  xSemaphoreGive(gpio_isr_mutex);
}

void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param)
{
  (void)interruptNumber;
  (void)callback;
  (void)mode;
  (void)param;
}

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode)
{
  PinName pin_name = pinToPinName(interruptNumber);
  if (pin_name == PIN_NAME_NC) {
    return;
  }
  attachInterrupt(pin_name, callback, mode);
}
