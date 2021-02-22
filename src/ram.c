#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ram[1000]; //ram data structure
int next_available_cell = 0;
int flag = 1; //if the flag is 1, it means we are on the first program.

//                                                  Test functions
/*------------------------------------------------------------------------------------------------------------------------------------*/

void printRAM() {
    for (int j=0; j<6; j++) {
        printf("This is the value inside the %dth cell inside ram : %s\n", j,ram[j]) ;
        //printf("this is the end of ram : %d\n", next_available_cell) ;

    }
} 



//                                                  Methods
/*------------------------------------------------------------------------------------------------------------------------------------*/

void resetNextCell() {
    next_available_cell = 0;
}

void addToRam(FILE *p, int *start, int *end) {

    char buffer[50] ;

    while (fgets(buffer,30,p) != NULL) {

        if (next_available_cell >= 999) {
            printf("ERROR: Not enough RAM to add program") ;
            break ;
        }

        ram[next_available_cell] = strdup(strtok(buffer,"\n")) ;
        //printf("%d %s\n", next_available_cell, ram[next_available_cell]) ;

        next_available_cell = next_available_cell + 1;   
    }
    
   
    if (flag == 0) { //if the flag is zero it indicates that the program being read is NOT the first one.
        *start = *end + 1;
    }

    if (*start == 0) { //the first program is being read.
        *start = 0;
        flag = 0 ;
    }
    *end = next_available_cell-1 ; // end is now pointing to the address of our temporary variable. End is determined when a program is loaded into ram. 

    //printRAM() ; //helper function.
    

    
}
