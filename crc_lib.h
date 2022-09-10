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

// 20220810A

#define CRC_LIB_DEFINE_TABLESET(CRCTYPE)\
typedef struct {\
	CRCTYPE Table[256];\
	CRCTYPE Initial;\
} CRC_LIB_##CRCTYPE##_TABLESET;

#if _CRC_LIB_
#define CRC_LIB_DEFINE_FUNCTION(CRCTYPE)\
static void CRC_LIB_##CRCTYPE##_GenerateTable(CRCTYPE table[256], CRCTYPE polynominal) {\
	int dividend;\
	for (dividend = 0; dividend <= 0xFF; dividend++) {\
    	int bit;\
		CRCTYPE remainder = (CRCTYPE)dividend;\
		remainder <<= ((8 * sizeof(CRCTYPE)) - 8);\
		for (bit = 0; bit < 8; bit++) {\
			const CRCTYPE top_bit  = (CRCTYPE)((CRCTYPE)1 << ((8 * sizeof(CRCTYPE)) - 1));\
			remainder <<= 1;\
			if (remainder & top_bit) {\
				remainder ^= (CRCTYPE)polynominal;\
			}\
		}\
		table[dividend] = remainder;\
	}\
}\
void CRC_LIB_##CRCTYPE##_Initialize(CRC_LIB_##CRCTYPE##_TABLESET* set, CRCTYPE ipolynominal, CRCTYPE initial) {\
	CRC_LIB_##CRCTYPE##_GenerateTable(set->Table, ipolynominal);\
	set->Initial = initial;\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_CRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, uint8_t value, CRCTYPE crc) {\
	crc = set->Table[(value ^ (crc >> ((8 * sizeof(CRCTYPE)) - 8))) & 0xFF] ^ (CRCTYPE)(crc << 8);\
	return crc;\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_ContinueCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size, CRCTYPE crc) {\
	const uint8_t *p = (const uint8_t*) data;\
	while (size) {\
		crc = CRC_LIB_##CRCTYPE##_CRC(set, *p, crc);\
		p++;\
		size--;\
	}\
	return crc;\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_GetCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size) {\
	return CRC_LIB_##CRCTYPE##_ContinueCRC(set, data, size, set->Initial);\
}
#else
#define CRC_LIB_DEFINE_FUNCTION(CRCTYPE)\
	extern void CRC_LIB_##CRCTYPE##_Initialize(CRC_LIB_##CRCTYPE##_TABLESET* set, CRCTYPE polynominal, CRCTYPE initial);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_CRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, uint8_t value, CRCTYPE crc);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_ContinueCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size, CRCTYPE crc);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_GetCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size);
#endif

CRC_LIB_DEFINE_TABLESET(uint8_t)
CRC_LIB_DEFINE_TABLESET(uint16_t)
CRC_LIB_DEFINE_TABLESET(uint32_t)

CRC_LIB_DEFINE_FUNCTION(uint8_t)
CRC_LIB_DEFINE_FUNCTION(uint16_t)
CRC_LIB_DEFINE_FUNCTION(uint32_t)

#define CRC_LIB_TABLESET(CRCTYPE) CRC_LIB_##CRCTYPE##_TABLESET
#define CRC_LIB_Initialize(CRCTYPE, set, polynominal, initial) CRC_LIB_##CRCTYPE##_Initialize(set, polynominal, initial)
#define CRC_LIB_CRC(CRCTYPE, set, value, crc) CRC_LIB_##CRCTYPE##_CRC(set, value, crc)
#define CRC_LIB_ContinueCRC(CRCTYPE, set, data, size, crc) CRC_LIB_##CRCTYPE##_ContinueCRC(set, data, size, crc)
#define CRC_LIB_GetCRC(CRCTYPE, set, data, size) CRC_LIB_##CRCTYPE##_GetCRC(set, data, size)

#ifdef __cplusplus
}
#endif

#endif /* _CRC_LIB_H_ */
