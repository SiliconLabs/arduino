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

#ifndef SL_MATTER_PROVISION_H
#define SL_MATTER_PROVISION_H

// <<< Use Configuration Wizard in Context Menu >>>

// <q ENABLE_EXAMPLE_CREDENTIALS> With this flag enabled if the attestation certificates are not present in non-volatile memory, the device will use the hardcoded test certificates. The flag should be disabled in production builds. 
// <i> Default: 1
#define ENABLE_EXAMPLE_CREDENTIALS 1

// <<< end of configuration section >>> 

#if ENABLE_EXAMPLE_CREDENTIALS
    #define CHIP_DEVICE_CONFIG_ENABLE_EXAMPLE_CREDENTIALS 1
#endif

#endif // SL_MATTER_PROVISION_H
