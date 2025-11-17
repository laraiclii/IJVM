#include "../include/framepointerlist.h"


void init_fpList(FrameList* list){
    list->head = NULL;
    list->tail = NULL;
}


Frame* createFrame(int previousPC){
    //Allocate memory for the frame and initialize its values
    Frame* newNode = (Frame*) malloc(sizeof(Frame));
    newNode->locals = NULL;
    newNode->localVarCount = 0;
    newNode->previousPC = previousPC;
    newNode->next = NULL;
    newNode->previous = NULL;
}
void addFrame(FrameList* list, Frame* newNode){
    
    //If this is the first frame 
    //set both head and tail to new node
    if(list->head == NULL){
        list->head = newNode;
        list->tail = newNode;
    }
    //Else add the frame to end of the list
    else{
        list->tail->next = newNode;
        newNode->previous = list->tail;
        list->tail = newNode;
    }
}
void storeLocalVar(FrameList* list, word_t var, int index){
    Frame* localVarList = getCurrentFrame(list);
    //If no memory allocated for local variables, allocate memory for one word
     if(localVarList->locals == NULL){
        localVarList->locals = (word_t*) malloc(sizeof(word_t));
        localVarList->locals[0] = var;
        localVarList->localVarCount = 1;
        return;
    }
    //If given index is in the range of local variables, store the value
    if(index < localVarList->localVarCount){
        localVarList->locals[index] = var;
        return;
    }
    //If given index is out of bounds, increase the allocated memory and then store the value
    else {
        word_t* newLocals = (word_t*) realloc(localVarList->locals, sizeof(word_t)*(index+1));
        localVarList->locals = newLocals;
        newLocals[index] = var;
        localVarList->localVarCount = index+1;
        return;
    }
    
}
void removeFP(FrameList* list){
    //If there is only one frame left, free the allocated 
    //memory and set head and tail to NULL
    if(list->head->next == NULL){
        free(list->tail->locals);
        free(list->tail);
        list->tail = NULL;
        list->head = NULL;
        return;
    }
    //Free the allocated memory for the tail and 
    //put tail to tail's previous frame
    list->tail = list->tail->previous;
    free(list->tail->next->locals);
    free(list->tail->next);
    list->tail->next = NULL;
}
Frame* getCurrentFrame(FrameList* list){
    return list->tail;
}
