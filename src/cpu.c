#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ram.h"
#include "interpreter.h"

struct CPU {
    bool in_use;
    int IP; //points to the next instruction to execute from ram.
    char IR[1000]; //the currently executing instruction.
    int quanta; // the number of lines the cpu can execute at once.
    }cpu;


int run(int quanta) {  
    int i = 0;
    while (i<quanta) { 
        strcpy(cpu.IR, ram[cpu.IP]) ;
        //printf("%s\n", cpu.IR) ;
        interpret(cpu.IR) ;

        i++;
        cpu.IP++ ;

    }
    

    return 0;
}
  

