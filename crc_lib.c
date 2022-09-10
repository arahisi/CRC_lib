/*
Copyright 2021, Shoji Yamamoto

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
*/

#define _CRC_LIB_ 1
#include "crc_lib.h"
#undef _CRC_LIB_
#define _CRC_LIB_ 0

uint8_t CRC_LIB_uint8_t_Reflect(uint8_t x) {
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	x = (x << 4) | (x >> 4);
	return x;
}

uint16_t CRC_LIB_uint16_t_Reflect(uint16_t x) {
	x = ((x & 0x5555) << 1) | ((x & 0xAAAA) >> 1);
	x = ((x & 0x3333) << 2) | ((x & 0xCCCC) >> 2);
	x = ((x & 0x0F0F) << 4) | ((x & 0xF0F0) >> 4);
	x = (x << 8) | (x >> 8);
	return x;
}

uint32_t CRC_LIB_uint32_t_Reflect(uint32_t x) {
	x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
	x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
	x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
	x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
	x = (x << 16) | (x >> 16);
	return x;
}