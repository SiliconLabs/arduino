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

#ifndef SL_DEVICE_INIT_LFXO_CONFIG_H
#define SL_DEVICE_INIT_LFXO_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <o SL_DEVICE_INIT_LFXO_MODE> Mode
// <i>
// <cmuLfxoOscMode_Crystal=> Crystal oscillator
// <cmuLfxoOscMode_AcCoupledSine=> AC-coupled buffer
// <cmuLfxoOscMode_External=> External digital clock
// <i> Default: cmuLfxoOscMode_Crystal
#define SL_DEVICE_INIT_LFXO_MODE           cmuLfxoOscMode_Crystal

// <o SL_DEVICE_INIT_LFXO_CTUNE> CTUNE <0-127>
// <i> Default: 63
#define SL_DEVICE_INIT_LFXO_CTUNE          25

// <o SL_DEVICE_INIT_LFXO_PRECISION> LFXO precision in PPM <0-65535>
// <i> Default: 500
#define SL_DEVICE_INIT_LFXO_PRECISION      100

// <o SL_DEVICE_INIT_LFXO_TIMEOUT> Startup Timeout Delay
// <i>
// <cmuLfxoStartupDelay_2Cycles=> 2 cycles
// <cmuLfxoStartupDelay_256Cycles=> 256 cycles
// <cmuLfxoStartupDelay_1KCycles=> 1K cycles
// <cmuLfxoStartupDelay_2KCycles=> 2K cycles
// <cmuLfxoStartupDelay_4KCycles=> 4K cycles
// <cmuLfxoStartupDelay_8KCycles=> 8K cycles
// <cmuLfxoStartupDelay_16KCycles=> 16K cycles
// <cmuLfxoStartupDelay_32KCycles=> 32K cycles
// <i> Default: cmuLfxoStartupDelay_4KCycles
#define SL_DEVICE_INIT_LFXO_TIMEOUT           cmuLfxoStartupDelay_4KCycles
// <<< end of configuration section >>>

#endif // SL_DEVICE_INIT_LFXO_CONFIG_H
