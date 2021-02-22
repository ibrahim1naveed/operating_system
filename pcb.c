#include <stdio.h>
#include <stdlib.h>
struct PCB {
    int PC;
    int start;
    int end;

};

struct PCB* makePCB(int start, int end) {

    struct PCB *retVal = malloc (sizeof (struct PCB));
    if (retVal == NULL) {
        printf("Space not allocated") ;
        return NULL;
    }
    else {
        retVal->start = start;
        retVal ->end = end;
        retVal->PC = start;


        //printf("this is the start : %d\n", retVal->start) ;
        //printf("this is the end : %d\n", retVal->end) ;
        //printf("this is the pc : %d\n", retVal->PC) ;
    }
    return retVal;

}