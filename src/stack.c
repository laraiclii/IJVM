#include "../include/stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* init_Stack(int size){
    //Check if given size is valid
    if (size <= 0){
        return NULL;
    }
    //Allocate memory for stack
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->size = size;
    stack->top = 0;
    stack->Data = (word_t*)malloc(sizeof(word_t) * size);
    return stack;
}

bool isFull(Stack* stack){
    return stack->size == stack->top;
}

bool isEmpty(Stack* stack){
    return stack->top == 0;
}

void PUSH(Stack* stack, word_t element){
    //Allocate more memory if stack is full
    if(isFull(stack)){
        word_t* newAddress = realloc(stack->Data, sizeof(word_t)*stack->size+1);
        if(newAddress == NULL){
            fprintf(stderr, "STACK FULL\n");
        }
        else{
            stack->Data = newAddress;
            stack->size += 1; 
        }
    }
    stack->Data[stack->top++] = element;
}

word_t TOP(Stack* stack){
    return stack->Data[stack->top-1];
}
word_t POP(Stack* stack){
    return stack->Data[--stack->top];
}
int SIZE(Stack* stack){
    return stack->size;
}
