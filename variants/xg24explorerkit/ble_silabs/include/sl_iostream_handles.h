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

#ifndef SL_IOSTREAM_HANDLES_H
#define SL_IOSTREAM_HANDLES_H
#include "sl_iostream.h"
#include "sl_iostream_init_eusart_instances.h"


#ifdef __cplusplus
extern "C" {
#endif

extern const sl_iostream_instance_info_t *sl_iostream_instances_info[];
extern const uint32_t sl_iostream_instances_count;

extern sl_iostream_t *sl_iostream_recommended_console_stream;

sl_iostream_t *sl_iostream_get_handle(char *name);

void sl_iostream_set_console_instance(void);

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_HANDLES_H
