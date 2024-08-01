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

#ifndef SL_DEVICE_INIT_DCDC_CONFIG_H
#define SL_DEVICE_INIT_DCDC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <q SL_DEVICE_INIT_DCDC_ENABLE> Enable DC/DC Converter
// <i>
// <i> Default: 1
#define SL_DEVICE_INIT_DCDC_ENABLE         1

// <q SL_DEVICE_INIT_DCDC_BYPASS> Set DC/DC Converter in Bypass Mode
// <i>
// <i> Default: 0
#define SL_DEVICE_INIT_DCDC_BYPASS         0

// <q SL_DEVICE_INIT_DCDC_PFMX_IPKVAL_OVERRIDE> Override for DCDC PFMX Mode Peak Current Setting
// <i>
// <i> Default: 0
#define SL_DEVICE_INIT_DCDC_PFMX_IPKVAL_OVERRIDE  1

// <o SL_DEVICE_INIT_DCDC_PFMX_IPKVAL> DCDC PFMX Mode Peak Current Setting <0-15>
// <i>
// <i> Default: DCDC_PFMXCTRL_IPKVAL_DEFAULT
#define SL_DEVICE_INIT_DCDC_PFMX_IPKVAL    3

// <<< end of configuration section >>>

#endif // SL_DEVICE_INIT_DCDC_CONFIG_H
