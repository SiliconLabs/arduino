/*
   Copyright (c) 2014 Arduino.  All right reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _RING_BUFFER_
#define _RING_BUFFER_

#include <stdint.h>

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.

template <int N>
class RingBuffer
{
private:
  uint8_t _aucBuffer[N];
  int _iHead;
  int _iTail;

public:
  RingBuffer(void)
  {
    memset(_aucBuffer, 0, N);
    clear();
  }

  void store_char(uint8_t c)
  {
    int i = nextIndex(_iHead);

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if ( i != _iTail ) {
      _aucBuffer[_iHead] = c;
      _iHead = i;
    }
  }

  void clear()
  {
    _iHead = 0;
    _iTail = 0;
  }

  int read_char()
  {
    if (_iTail == _iHead) {
      return -1;
    }

    uint8_t value = _aucBuffer[_iTail];
    _iTail = nextIndex(_iTail);

    return value;
  }

  int available()
  {
    int delta = _iHead - _iTail;

    if (delta < 0) {
      return N + delta;
    } else {
      return delta;
    }
  }

  int peek()
  {
    if (_iTail == _iHead) {
      return -1;
    }

    return _aucBuffer[_iTail];
  }

  bool isFull()
  {
    return (nextIndex(_iHead) == _iTail);
  }

private:
  int nextIndex(int index)
  {
    return (uint32_t)(index + 1) % N;
  }
};

#endif /* _RING_BUFFER_ */
