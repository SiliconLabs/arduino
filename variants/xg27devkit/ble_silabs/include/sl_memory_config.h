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

#ifndef SL_MEMORY_CONFIG_H
#define SL_MEMORY_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> Memory configuration

// <o SL_STACK_SIZE> Stack size for the application.
// <i> Default: 4096
// <i> The stack size configured here will be used by the stack that the
// <i> application uses when coming out of a reset.
#ifndef SL_STACK_SIZE
  #define SL_STACK_SIZE  2752
#endif

// <o SL_HEAP_SIZE> Minimum heap size for the application.
// <i> Default: 2048
// <i> Note that this value will configure the c heap which is normally used by
// <i> malloc() and free() from the c library. The value defines a minimum heap
// <i> size that is guaranteed to be available. The available heap may be larger
// <i> to make use of any memory that would otherwise remain unused.
#ifndef SL_HEAP_SIZE
  #define SL_HEAP_SIZE   9200
#endif

// </h>
// <<< end of configuration section >>>

#endif
