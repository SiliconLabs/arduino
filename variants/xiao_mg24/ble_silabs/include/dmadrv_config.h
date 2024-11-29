/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
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

#ifndef DMADRV_CONFIG_H
#define DMADRV_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <o EMDRV_DMADRV_DMA_IRQ_PRIORITY> DMA interrupt priority <0-15>
// <i> Priority of the DMA interrupt. Smaller number equals higher priority.
// <i> Default: 8
#define EMDRV_DMADRV_DMA_IRQ_PRIORITY 8

// <o EMDRV_DMADRV_DMA_CH_COUNT> Number of available channels <1-8>
// <i> Number of DMA channels supported by the driver. A lower channel count
// <i> will reduce RAM memory footprint. The default is to support all channels
// <i> on the device.
// <i> Default: 8
#define EMDRV_DMADRV_DMA_CH_COUNT 8

// <o EMDRV_DMADRV_DMA_CH_PRIORITY> Number of fixed priority channels
// <i> This will configure channels [0, CH_PRIORITY - 1] as fixed priority,
// <i> and channels [CH_PRIORITY, CH_COUNT] as round-robin.
// <i> Default: 0
#define EMDRV_DMADRV_DMA_CH_PRIORITY 0

// <<< end of configuration section >>>

#endif // DMADRV_CONFIG_H
