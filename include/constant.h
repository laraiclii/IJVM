#ifndef _CONSTANT_H
#define _CONSTANT_H
#include "ijvm.h"

//Represents constant pool
typedef struct Constant {
	uint32_t isLittleEndian;
	uint32_t origin;
	uint32_t size;
	int* poolData;
} Constant;
#endif