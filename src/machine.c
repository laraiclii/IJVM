#include "../include/ijvm.h"
//#include "ijvm.h"
#include <stdio.h> 
#include <stdlib.h>
#include "../include/stack.h"
#include "../include/opcodes.h"
// #include "opcodes.c"
// #include "stack.c"
// #include "framepointerlist.c"
#include "../include/constant.h"
#include "../include/text.h"
#include "../include/framepointerlist.h"

#define STACK_SIZE 128

Stack* stack;

int isInitialized = 0;
Constant constantData;
Text textData;
int PC;
FrameList fp;
int wide;


static uint32_t swap_uint32(uint32_t num)
{
	return((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000);
}
//Turn 4 individual bytes into 4 byte integer
int  byteToInt32(byte_t bytes[4]){
	int result = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
	return result;
}

uint32_t read32Bit(FILE* file, int* buffer, int count, int isLittleEndian){
	int i;
	for(i = 0; i < count; i++){
		uint32_t val;
		fread(&val, sizeof(uint32_t), 1, file);
		if(isLittleEndian) val = swap_uint32(val);
		buffer[i] = val;
	}
}
void readConstant(FILE* in_file, Constant* constantData, int isLittleEndian){
	read32Bit(in_file, &constantData->origin, 1, isLittleEndian);
	read32Bit(in_file, &constantData->size, 1, isLittleEndian);

	constantData->poolData = (uint32_t*)malloc(sizeof(char) * constantData->size);
	read32Bit(in_file, constantData->poolData, constantData->size/4, isLittleEndian);

}
void readText(FILE* in_file, Text* textData, int isLittleEndian){
	read32Bit(in_file, &textData->origin, 1, isLittleEndian);
	read32Bit(in_file, &textData->size, 1, isLittleEndian);

	textData->data = (byte_t*)malloc(sizeof(char) * textData->size + textData->origin);
	fread(textData->data + textData->origin,sizeof(byte_t), textData->size, in_file);
}
void setPC(int newIndex){
	PC = newIndex;
}

int get_program_counter(){
	return PC;
}
int init_ijvm(char* binary_file)
{
	if(!isInitialized){
		isInitialized = 1;
	}
	else{
		return -1;
	}
	FILE* in_file;
	in_file = fopen(binary_file, "rb"); 
	if (!in_file) { 
		return -1;
	}

	uint32_t magicNumber;
	fread(&magicNumber, sizeof(uint32_t), 1, in_file);

	int isLittleEndian = (magicNumber ^ MAGIC_NUMBER);

	readConstant(in_file, &constantData, isLittleEndian);
	readText(in_file, &textData, isLittleEndian);
	fclose(in_file);

	stack = init_Stack(STACK_SIZE);
	init_fpList(&fp);
	Frame* mainFrame = createFrame(0);
	addFrame(&fp, mainFrame);
	setPC(textData.origin);
	set_input(stdin);
	set_output(stdout);
	wide = 0;
	return 0;
}


// int main(){
// 	init_ijvm("files/task4/LoadTest1.ijvm");
// }


int text_size(void)
{
	return textData.size;
}

byte_t* get_text(void)
{
	return textData.data;
}


bool step(void)
{
	byte_t instruction = 0x0;
	instruction = get_instruction();
	if(instruction == OP_WIDE)
	{
		wide = 1;
		PC++;
		instruction = get_instruction();
	}
	PC++;
	switch (instruction)
	{
	case OP_BIPUSH:{
		byte_t elementByte = textData.data[PC++];
		BIPUSH(stack, elementByte);
		break;
	}
	case OP_DUP:
	 	DUP(stack);
		break;
	case OP_ERR:
		ERR();
	case OP_HALT:
		return false;
		break;
	case OP_GOTO:{
		GOTO(&textData, &PC);
		break;
	}
	case OP_IADD:
		IADD(stack);
		break;
	case OP_IAND:
		IAND(stack);
		break;
	case OP_ICMPEQ:
		IF_ICMPEQ(stack, &textData, &PC);
		break;
	case OP_IFEQ:
		IFEQ(stack, &textData, &PC);
		break;
	case OP_IFLT:
		IFLT(stack, &textData, &PC);
		break;
	case OP_IINC:
		IINC(stack, &textData, &fp, &constantData, &PC, &wide);
		break;
	case OP_ILOAD:
		ILOAD(stack, &textData, &fp, &PC, &wide);
		break;
	case OP_IN:
		IN(stack);
		break;
	case OP_INVOKEVIRTUAL:
		INVOKEVIRTUAL(stack, &textData, &fp, &constantData, &PC);
		break;
	case OP_IOR:
		IOR(stack);
		break;
	case OP_IRETURN:
		IRETURN(stack, &textData, &fp, &constantData, &PC);
		break;
	case OP_ISTORE:
		ISTORE(stack, &textData, &fp, &PC, &wide);
		break;
	case OP_ISUB:
		ISUB(stack);
		break;
	case OP_LDC_W:
		LDC_W(stack, &textData, &constantData, &PC);
		break;
	case OP_NOP:
		break;
	case OP_OUT:
		OUT(stack);
		break;
	case OP_POP:
		POP(stack);
		break;
	case OP_SWAP:
		SWAP(stack);
		break;
	default:
		break;
	}
	return true;
}



/*
if (in_file) {
  while (fscanf(in_file, "%s",str)!=EOF)
  printf("%s",str);
  fclose(in_file);
}
return 0;
*/

void destroy_ijvm()
{
	if(!isInitialized) return;

	isInitialized = 0;
	free(constantData.poolData);
	free(textData.data);
	free(stack->Data);
	free(stack);
	while(fp.head != NULL)
		removeFP(&fp);
	
}
bool isFinished = 0;
void run()
{
	bool result = 0;
	do {
		result = step();
	}
	while(result);
	isFinished = 1;
		// Step while you can
}


byte_t get_instruction(void){
	return textData.data[PC];
}


void printHex(byte_t* arr, int size){
    int index;
    for(index = 0; index < size; index++){
        fprintf(output, "%x ", arr[index]);
		if((index+1)%8==0){
			fprintf(output, "\n");
		}
    }
}

word_t* get_stack(){
    return stack->Data;
}
word_t tos(){
	return TOP(stack);
}

word_t get_local_variable(int i){
	return getCurrentFrame(&fp)->locals[i];
}

word_t get_constant(int i){
	word_t constant = (word_t)constantData.poolData[i];
	return constant;
}

bool finished(void){
	isFinished = 1;
}

int stack_size(void)
{
	return stack->top - fp.tail->stackPointer + 2;
}