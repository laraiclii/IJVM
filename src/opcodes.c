
#include "../include/opcodes.h"
#include <stdlib.h>

short byteToShort(byte_t* bytes){
	short result = (bytes[0] << 8) + bytes[1];
	return result;
}


void set_input(FILE* fp)
{
	input = fp;
}

void set_output(FILE* fp)
{
	output = fp;
}

void  IADD(Stack* stack){
    word_t element2 = POP(stack);
    word_t element1 = POP(stack);
    PUSH(stack, element1 + element2);
}

void  IAND(Stack* stack){
    word_t element2 = POP(stack);
    word_t element1 = POP(stack);
    PUSH(stack, element1 & element2);
}

void  ISUB(Stack* stack){
    word_t element2 = POP(stack);
    word_t element1 = POP(stack);
    PUSH(stack, element1 - element2);
}

void  IOR(Stack* stack){
    word_t element2 = POP(stack);
    word_t element1 = POP(stack);
    PUSH(stack, element1 | element2);
}
void BIPUSH(Stack* stack, byte_t element){
    word_t newElement;
    if(0x00000080 & element)
        newElement = 0xFFFFFF00 + element;
    else
        newElement = element;
    PUSH(stack, newElement);
}
void  SWAP(Stack* stack){
    word_t element2 = POP(stack);
    word_t element1 = POP(stack);
    PUSH(stack, element2);
    PUSH(stack, element1);
}

void OUT(Stack* stack){
    word_t element = POP(stack);
    fprintf(output,"%c", element);
}

void GOTO(Text* textData, int* PC){
    byte_t offsetArr[2];
    offsetArr[0] = textData->data[*PC];
    offsetArr[1] = textData->data[*PC+1];
    short offset = byteToShort(offsetArr);
    *PC += offset - 1;
}
void IFEQ(Stack* stack, Text* textData, int* PC){
    word_t element1 = POP(stack);
    if(element1 == 0){
        GOTO(textData, PC);
    }
    else{
        (*PC) += 2;
    }
}
void IFLT(Stack* stack, Text* textData, int* PC){
    word_t element1 = POP(stack);
    if(element1 < 0){
        GOTO(textData, PC);
    }
    else{
        (*PC) += 2;
    }
}
void IF_ICMPEQ(Stack* stack, Text* textData, int* PC){
    word_t element2 = POP(stack);
    word_t element1 = POP(stack);
    
    if(element1 == element2){
        GOTO(textData, PC);
    }
    else{
        (*PC) += 2;
    }
}
void DUP(Stack* stack){
    word_t element = TOP(stack);
    PUSH(stack, element);
}
void ISTORE(Stack* stack,Text* textData, FrameList* list, int* PC, int* wide){
    word_t element = POP(stack);
    unsigned short index;
     
    if(*wide){
        byte_t indices[2];
        indices[0] = textData->data[(*PC)++];
        indices[1] = textData->data[(*PC)++];
        index = byteToShort(indices);
    }
    else{
        index = textData->data[(*PC)++];
    }
    storeLocalVar(list, element, index);
    
    *wide = 0;
}

void ILOAD(Stack* stack,Text* textData, FrameList* list, int* PC, int* wide){
    unsigned short index;
     
    if(*wide){
        byte_t indices[2];
        indices[0] = textData->data[(*PC)++];
        indices[1] = textData->data[(*PC)++];
        index = byteToShort(indices);
    }
    else{
        index = textData->data[(*PC)++];
    }
    word_t* fp = getCurrentFrame(list)->locals;
    word_t element = fp[index];
    PUSH(stack,element);
    *wide = 0;
}
void LDC_W(Stack* stack,Text* textData, Constant* constantPool, int* PC){
    byte_t twobytes[2];
    twobytes[0] = textData->data[*PC];
    twobytes[1] = textData->data[(*PC)+1];
    unsigned short index = byteToShort(twobytes);
    int element = constantPool->poolData[index];
    PUSH(stack,(word_t)element);
    (*PC)+=2;
}
void IINC(Stack* stack,Text* textData, FrameList* list, Constant* constantPool, int* PC, int* wide){
    unsigned short index;
     
    if(*wide){
        byte_t indices[2];
        indices[0] = textData->data[(*PC)++];
        indices[1] = textData->data[(*PC)++];
        index = byteToShort(indices);
    }
    else{
        index = textData->data[(*PC)++];
    }
    int8_t constant = (int)textData->data[(*PC)++];
    word_t localVar = get_local_variable(index);
    storeLocalVar(list, localVar + constant, index);
    *wide = 0;
}

void INVOKEVIRTUAL(Stack* stack,Text* textData, FrameList* list, Constant* constantPool, int* PC){
    byte_t index_arr[2];
    index_arr[0] = textData->data[(*PC)++];
    index_arr[1] = textData->data[(*PC)++];
    short index = byteToShort(index_arr);
    word_t offset = constantPool->poolData[index];
    Frame* newFrame = createFrame((*PC));
    addFrame(list, newFrame);
    *PC = offset;

    byte_t argc_arr[2];
    argc_arr[0] = textData->data[(*PC)++];
    argc_arr[1] = textData->data[(*PC)++];
    unsigned short argc = byteToShort(argc_arr);

    byte_t localVarSize_arr[2];
    localVarSize_arr[0] = textData->data[(*PC)++];
    localVarSize_arr[1] = textData->data[(*PC)++];
    unsigned short localVarSize = byteToShort(localVarSize_arr);
   
   
    list->tail->locals = (word_t*)malloc(sizeof(word_t)* localVarSize);
    list->tail->localVarCount = localVarSize;

    int i;
    storeLocalVar(list, list->tail->previousPC, 0);
    for(i = 1; i < argc; i++){
        storeLocalVar(list, POP(stack), argc-i);
    }

    POP(stack);
    list->tail->stackPointer = stack->top;
}

void IRETURN(Stack* stack,Text* textData, FrameList* list, Constant* constantPool, int* PC){
    word_t returnElement = POP(stack);
    while(stack->top != list->tail->stackPointer){
        POP(stack);
    }
    PUSH(stack, returnElement);
    *PC = list->tail->previousPC;
    removeFP(list);
}


void ERR(){
    fprintf(stderr, "An error occured!\n");
}

void IN(Stack* stack){
    char c = NULL;
    fread(&c,sizeof(char), 1, input);
    PUSH(stack, (word_t)c);
}