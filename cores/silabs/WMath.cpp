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

#include "WMath.h"

uint32_t random(uint32_t max)
{
  uint32_t rand = 0;
  (void)psa_generate_random((uint8_t*)&rand, sizeof(rand));
  return rand % max;
}

uint32_t random(uint32_t min, uint32_t max)
{
  uint32_t rand = 0;
  (void)psa_generate_random((uint8_t*)&rand, sizeof(rand));
  return (rand % (max - min)) + min;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  const long dividend = out_max - out_min;
  const long divisor = in_max - in_min;
  const long delta = x - in_min;

  return (delta * dividend + (divisor / 2)) / divisor + out_min;
}
