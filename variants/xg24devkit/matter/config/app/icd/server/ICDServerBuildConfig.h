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


#ifndef SL_MATTER_ICD_SERVER_BUILDCONFIG_H
#define SL_MATTER_ICD_SERVER_BUILDCONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <q ICD_REPORT_ON_ENTER_ACTIVE_MODE> Matter Report on Entering Active
// <i> Default: 0
#define ICD_REPORT_ON_ENTER_ACTIVE_MODE 0

// <q ICD_ENFORCE_SIT_SLOW_POLL_LIMIT> Matter ICD Slow Polling Limit Enforcement
// <i> Default: 0
#define ICD_ENFORCE_SIT_SLOW_POLL_LIMIT 0

// <q ICD_MAX_NOTIFICATION_SUBSCRIBERS> Matter Max Notification Subscribers <1-5>
// <i> Default: 1
#define ICD_MAX_NOTIFICATION_SUBSCRIBERS 1

// <q CHIP_CONFIG_ENABLE_ICD_LIT> Matter LIT Configurations
// <i> Default: 0
#define CHIP_CONFIG_ENABLE_ICD_LIT 0

// <q CHIP_CONFIG_ENABLE_ICD_CIP> Matter Check in Sender
// <i> Default: 0
#define CHIP_CONFIG_ENABLE_ICD_CIP 0

// <q CHIP_CONFIG_ENABLE_ICD_UAT> Matter UAT
// <i> Default: 0
#define CHIP_CONFIG_ENABLE_ICD_UAT 0

// <<< end of configuration section >>>

#endif // SL_MATTER_ICD_SERVER_BUILDCONFIG_H
