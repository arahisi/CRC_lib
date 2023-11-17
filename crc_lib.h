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

#ifndef CRC_LIB_H_
#define CRC_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

// 20220810B

#define CRC_LIB_DEFINE_TABLESET(CRCTYPE)\
typedef struct {\
	CRCTYPE Table[256];\
	CRCTYPE Initial;\
} CRC_LIB_##CRCTYPE##_TABLESET;

#if _CRC_LIB_
#define CRC_LIB_DEFINE_FUNCTION(CRCTYPE)\
static void CRC_LIB_##CRCTYPE##_GenerateTable(CRCTYPE table[256], CRCTYPE polynomial) {\
	int dividend;\
	for (dividend = 0; dividend <= 0xFF; dividend++) {\
    	int bit;\
		CRCTYPE remainder = (CRCTYPE)dividend;\
		remainder <<= ((8 * sizeof(CRCTYPE)) - 8);\
		for (bit = 0; bit < 8; bit++) {\
			const CRCTYPE top_bit  = (CRCTYPE)((CRCTYPE)1 << ((8 * sizeof(CRCTYPE)) - 1));\
			if (remainder & top_bit) {\
				remainder <<= 1;\
				remainder ^= (CRCTYPE)polynomial;\
			} else {\
				remainder <<= 1;\
			}\
		}\
		table[dividend] = remainder;\
	}\
}\
static void CRC_LIB_##CRCTYPE##_RefGenerateTable(CRCTYPE table[256], CRCTYPE ipolynomial) {\
	int dividend;\
	for (dividend = 0; dividend <= 0xFF; dividend++) {\
    	int bit;\
		CRCTYPE remainder = (CRCTYPE)dividend;\
		for (bit = 0; bit < 8; bit++) {\
			if (remainder & 1) {\
				remainder >>= 1;\
				remainder ^= (CRCTYPE)ipolynomial;\
			} else {\
				remainder >>= 1;\
			}\
		}\
		table[dividend] = remainder;\
	}\
}\
void CRC_LIB_##CRCTYPE##_Initialize(CRC_LIB_##CRCTYPE##_TABLESET* set, CRCTYPE polynomial, CRCTYPE initial) {\
	CRC_LIB_##CRCTYPE##_GenerateTable(set->Table, polynomial);\
	set->Initial = initial;\
}\
void CRC_LIB_##CRCTYPE##_RefInitialize(CRC_LIB_##CRCTYPE##_TABLESET* set, CRCTYPE ipolynomial, CRCTYPE initial) {\
	CRC_LIB_##CRCTYPE##_RefGenerateTable(set->Table, ipolynomial);\
	set->Initial = initial;\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_CRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, uint8_t value, CRCTYPE crc) {\
	crc = set->Table[(value ^ (crc >> ((8 * sizeof(CRCTYPE)) - 8))) & 0xFF] ^ (CRCTYPE)(crc << 8);\
	return crc;\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_RefCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, uint8_t value, CRCTYPE crc) {\
	crc = set->Table[(value ^ crc) & 0xFF] ^ (CRCTYPE)(crc >> 8);\
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
CRCTYPE CRC_LIB_##CRCTYPE##_RefContinueCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size, CRCTYPE crc) {\
	const uint8_t *p = (const uint8_t*) data;\
	while (size) {\
		crc = CRC_LIB_##CRCTYPE##_RefCRC(set, *p, crc);\
		p++;\
		size--;\
	}\
	return crc;\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_GetCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size) {\
	return CRC_LIB_##CRCTYPE##_ContinueCRC(set, data, size, set->Initial);\
}\
CRCTYPE CRC_LIB_##CRCTYPE##_RefGetCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size) {\
	return CRC_LIB_##CRCTYPE##_RefContinueCRC(set, data, size, set->Initial);\
}
#else
#define CRC_LIB_DEFINE_FUNCTION(CRCTYPE)\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_Reflect(CRCTYPE value);\
	extern void CRC_LIB_##CRCTYPE##_Initialize(CRC_LIB_##CRCTYPE##_TABLESET* set, CRCTYPE polynomial, CRCTYPE initial);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_CRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, uint8_t value, CRCTYPE crc);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_ContinueCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size, CRCTYPE crc);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_GetCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size);\
	extern void CRC_LIB_##CRCTYPE##_RefInitialize(CRC_LIB_##CRCTYPE##_TABLESET* set, CRCTYPE ipolynomial, CRCTYPE initial);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_RefCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, uint8_t value, CRCTYPE crc);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_RefContinueCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size, CRCTYPE crc);\
	extern CRCTYPE CRC_LIB_##CRCTYPE##_RefGetCRC(const CRC_LIB_##CRCTYPE##_TABLESET* set, const void* data, size_t size);
#endif

CRC_LIB_DEFINE_TABLESET(uint8_t)
CRC_LIB_DEFINE_TABLESET(uint16_t)
CRC_LIB_DEFINE_TABLESET(uint32_t)

CRC_LIB_DEFINE_FUNCTION(uint8_t)
CRC_LIB_DEFINE_FUNCTION(uint16_t)
CRC_LIB_DEFINE_FUNCTION(uint32_t)

#define CRC_LIB_TABLESET(CRCTYPE) CRC_LIB_##CRCTYPE##_TABLESET
#define CRC_LIB_Reflect(CRCTYPE, value) CRC_LIB_##CRCTYPE##_Reflect(value)
#define CRC_LIB_Initialize(CRCTYPE, set, polynomial, initial) CRC_LIB_##CRCTYPE##_Initialize(set, polynomial, initial)
#define CRC_LIB_CRC(CRCTYPE, set, value, crc) CRC_LIB_##CRCTYPE##_CRC(set, value, crc)
#define CRC_LIB_ContinueCRC(CRCTYPE, set, data, size, crc) CRC_LIB_##CRCTYPE##_ContinueCRC(set, data, size, crc)
#define CRC_LIB_GetCRC(CRCTYPE, set, data, size) CRC_LIB_##CRCTYPE##_GetCRC(set, data, size)
#define CRC_LIB_RefInitialize(CRCTYPE, set, ipolynomial, initial) CRC_LIB_##CRCTYPE##_RefInitialize(set, ipolynomial, initial)
#define CRC_LIB_RefCRC(CRCTYPE, set, value, crc) CRC_LIB_##CRCTYPE##_RefCRC(set, value, crc)
#define CRC_LIB_RefContinueCRC(CRCTYPE, set, data, size, crc) CRC_LIB_##CRCTYPE##_RefContinueCRC(set, data, size, crc)
#define CRC_LIB_RefGetCRC(CRCTYPE, set, data, size) CRC_LIB_##CRCTYPE##_RefGetCRC(set, data, size)

#ifdef __cplusplus
}
#endif

#endif /* CRC_LIB_H_ */
