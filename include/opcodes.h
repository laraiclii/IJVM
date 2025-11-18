#ifndef _OPCODES_H
#define _OPCODES_H
#include "stack.h"
#include "../include/ijvm.h"
#include "../include/text.h"
#include "../include/framepointerlist.h"
#include "../include/constant.h"


FILE* output;
FILE* input;

void IAND(Stack* stack);
void IADD(Stack* stack);
void ISUB(Stack* stack);
void IOR(Stack* stack);
void SWAP(Stack* stack);
void OUT(Stack* stack);
void BIPUSH(Stack* stack, byte_t element);
void SWAP(Stack* stack);
void OUT(Stack* stack);
void GOTO(Text* textData, int* PC);
void IFEQ(Stack* stack, Text* textData, int* PC);
void IFLT(Stack* stack, Text* textData, int* PC);
void IF_ICMPEQ(Stack* stack, Text* textData, int* PC);
void DUP(Stack* stack);
void ISTORE(Stack* stack,Text* textData, FrameList* list, int* PC, int* wide);
void ILOAD(Stack* stack,Text* textData, FrameList* list, int* PC, int* wide);
void LDC_W(Stack* stack,Text* textData, Constant* constantPool, int* PC);
void IINC(Stack* stack,Text* textData, FrameList* list, Constant* constantPool, int* PC, int* wide);
void INVOKEVIRTUAL(Stack* stack,Text* textData, FrameList* list, Constant* constantPool, int* PC);
void IRETURN(Stack* stack,Text* textData, FrameList* list, Constant* constantPool, int* PC);
void ERR();
void IN(Stack* stack);

#endif