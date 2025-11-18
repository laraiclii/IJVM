#ifndef _FRAMEPOINTERLIST_H
#define _FRAMEPOINTERLIST_H
#include "ijvm.h"

//Represents Frame of current function
typedef struct Frame{
    word_t* locals;
    unsigned int localVarCount;

    unsigned int previousPC;
    int stackPointer;
    struct Frame* next;
    struct Frame* previous;
} Frame;

//List of alive frames
typedef struct FrameList{
    Frame* head;
    Frame* tail;
} FrameList;

//Initializes a new frame
Frame* createFrame(int previousPC);

//Initialize given frame list
void init_fpList(FrameList* list);

//Add a frame to given frame list
void addFrame(FrameList* list, Frame* newNode);

//Put a value to local varible at given index of the current frame
void storeLocalVar(FrameList* list, word_t var, int index);

//Returns current frame
Frame* getCurrentFrame(FrameList* list);

//Delete current frame
void removeFP(FrameList* list);
#endif