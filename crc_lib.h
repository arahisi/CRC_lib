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

#ifndef _CRC_LIB_H_
#define _CRC_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define CRC_LIB_DEFINE_SET(CRCTYPE)\
typedef struct {\
	CRCTYPE Table[256];\
	CRCTYPE Initial;\
} CRCTYPE##_SET;

#if _CRC_LIB_
#define CRC_LIB_DEFINE_FUNCTION(CRCTYPE)\
static void CRCTYPE##_GenerateTable(CRCTYPE table[256], CRCTYPE ipolynominal) {\
	int dividend;\
	CRCTYPE remainder;\
	uint8_t bit;\
	for (dividend = 0; dividend < 256; dividend++) {\
		remainder = dividend;\
		for (bit = 8; bit > 0; bit--) {\
			if (remainder & 1) {\
				remainder = (remainder >> 1) ^ ipolynominal;\
			} else {\
				remainder = (remainder >> 1);\
			}\
		}\
		table[dividend] = remainder;\
	}\
}\
void CRCTYPE##_InitializeSET(CRCTYPE##_SET* set, CRCTYPE ipolynominal, CRCTYPE initial) {\
	CRCTYPE##_GenerateTable(set->Table, ipolynominal);\
	set->Initial = initial;\
}\
CRCTYPE CRCTYPE##_CRC(const CRCTYPE##_SET* set, const void* data, CRCTYPE crc) {\
	const uint8_t *p = (const uint8_t*) data;\
	return crc = set->Table[crc ^ *p];\
}\
CRCTYPE CRCTYPE##_ContinueCRC(const CRCTYPE##_SET* set, const void* data, size_t size, CRCTYPE crc) {\
	const uint8_t *p = (const uint8_t*) data;\
	while (size) {\
		crc = set->Table[crc ^ *p];\
		p++;\
		size--;\
	}\
	return crc;\
}\
CRCTYPE CRCTYPE##_GetCRC(const CRCTYPE##_SET* set, const void* data, size_t size) {\
	return CRCTYPE##_ContinueCRC(set, data, size, set->Initial);\
}
#else
#define CRC_LIB_DEFINE_FUNCTION(CRCTYPE)\
extern void CRCTYPE##_InitializeSET(CRCTYPE##_SET* set, CRCTYPE ipolynominal, CRCTYPE initial);\
extern CRCTYPE CRCTYPE##_CRC(const CRCTYPE##_SET* set, const void* data, CRCTYPE crc);\
extern CRCTYPE CRCTYPE##_ContinueCRC(const CRCTYPE##_SET* set, const void* data, size_t size, CRCTYPE crc);\
extern CRCTYPE CRCTYPE##_GetCRC(const CRCTYPE##_SET* set, const void* data, size_t size);
#endif

CRC_LIB_DEFINE_SET(uint8_t)
CRC_LIB_DEFINE_SET(uint16_t)
CRC_LIB_DEFINE_SET(uint32_t)

CRC_LIB_DEFINE_FUNCTION(uint8_t)
CRC_LIB_DEFINE_FUNCTION(uint16_t)
CRC_LIB_DEFINE_FUNCTION(uint32_t)

#ifdef __cplusplus
}
#endif

#endif /* _CRC_LIB_H_ */
