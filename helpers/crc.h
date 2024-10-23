#pragma once

#define LittleLong( val )     ( val )
#define CRC32_XOR_VALUE  0xFFFFFFFFUL
#define CRC32_INIT_VALUE 0xFFFFFFFFUL

typedef unsigned long CRC32_t;

#define NUM_BYTES 256
extern CRC32_t pulCRCTable[NUM_BYTES];

void		NEWCRC32_Init(CRC32_t* pulCRC);
void		NEWCRC32_Final(CRC32_t* pulCRC);
void		NEWCRC32_ProcessBuffer(CRC32_t* pulCRC, const void* pBuffer, int nBuffer);
void		NEWCRC32_Final(CRC32_t* pulCRC);
CRC32_t		NEWCRC32_GetTableEntry(unsigned int slot);
CRC32_t		NEWCRC32_ProcessSingleBuffer(const void* p, int len);