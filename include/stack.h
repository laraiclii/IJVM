#ifndef STACK_H
#define STACK_H
#include "ijvm.h"


//Represents stack for all frames
typedef struct Stack{
    word_t* Data;
    int top;
    int size;
} Stack;


//Initializes stack
Stack* init_Stack(int size);

//Returns whether stack is full
bool isFull(Stack* stack);

//Returns whether stack is empty
bool isEmpty(Stack* stack);

//Pushes element to given stack
void PUSH(Stack* stack, word_t element);

//Returns top element of given stack
word_t TOP(Stack* stack);

//Pops and returns the top element of given stack
word_t POP(Stack* stack);

//Returns the size of the stack(Not number of elements)
int SIZE(Stack* stack);
#endif