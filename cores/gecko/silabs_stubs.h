/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2026 Silicon Laboratories Inc. www.silabs.com
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

#ifndef SILABS_STUBS_H
#define SILABS_STUBS_H

#ifdef ARDUINO_MATTER
#include <cstddef>
#include <cstdint>

static constexpr size_t kArduinoMatterDeviceNameCapacity = 33;
static constexpr size_t kArduinoMatterHardwareVersionStringCapacity = 33;
static constexpr size_t kArduinoMatterSoftwareVersionStringCapacity = 64;

extern "C" {
extern char arduino_matter_vendor_name[kArduinoMatterDeviceNameCapacity];
extern char arduino_matter_product_name[kArduinoMatterDeviceNameCapacity];
extern char arduino_matter_hardware_version_string[kArduinoMatterHardwareVersionStringCapacity];
extern uint16_t arduino_matter_hardware_version;
extern char arduino_matter_software_version_string[kArduinoMatterSoftwareVersionStringCapacity];
extern uint32_t arduino_matter_software_version;
}
#endif // ARDUINO_MATTER

#endif // SILABS_STUBS_H
