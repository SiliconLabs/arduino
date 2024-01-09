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

#include "sl_iostream.h"
#include "sl_iostream_handles.h"
#include "string.h"

const sl_iostream_instance_info_t *sl_iostream_instances_info[] = {

    &sl_iostream_instance_vcom_info,
  
    &sl_iostream_instance_xg27devkit1_info,
  
};

const uint32_t sl_iostream_instances_count = sizeof(sl_iostream_instances_info) / sizeof(sl_iostream_instances_info[0]);

/***************************************************************************//**
 * Get iostream instance handle for a given name
 *
 * @return  Instance handle if it exist, NULL otherwise.
 ******************************************************************************/
sl_iostream_t *sl_iostream_get_handle(char *name)
{
  for (uint32_t i = 0; i < sl_iostream_instances_count; i++) {
    if (strcmp(sl_iostream_instances_info[i]->name, name) == 0) {
      return sl_iostream_instances_info[i]->handle;
    }
  }

  return NULL;
}