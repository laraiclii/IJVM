#ifndef _TEXT_H
#define _TEXT_H
#include "ijvm.h"
//Represents Text part of an .ijvm file
typedef struct Text {
	uint32_t isLittleEndian;
	uint32_t size;
	uint32_t origin;
	byte_t* data;
} Text;
#endif