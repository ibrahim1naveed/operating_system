#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "shell.h"
#include "ram.h"
#include "pcb.h"


//                                              GlOBAL DECLARATIONS
/*------------------------------------------------------------------------------------------------------------------------------------*/

struct ReadyQueue {
    struct cell* head;
    struct cell* tail;
    int size;
};

struct cell {
    struct PCB* pcb;
    struct cell* next;
};

struct ReadyQueue *rq;    

int start = 0; // keeping track of each program's start point inside ram.
int end = 0; // keeping track of each program's end point inside ram.





//                                                  Test functions
/*------------------------------------------------------------------------------------------------------------------------------------*/
void printPCB(struct PCB *p)
{
    printf("PC: %d, Start: %d, End: %d\n", p->PC,p->start, p->end);
}
void printcell(struct cell *ptr) 
{
    printPCB(ptr->pcb);
}
void printRQ() {
    int nodeNumber = 1;
    struct cell *temp;
    temp = rq->head;
    while(temp!=NULL) 
    {
        printf("Node #: %d - ",nodeNumber);
        printcell(temp);
        temp = temp->next;
        nodeNumber++;
    }
}

//                                                  Methods
/*------------------------------------------------------------------------------------------------------------------------------------*/
void addToReady(struct PCB *pcb) {
    // 1 if the pcb being added is the first one.
    
    if (rq == NULL) {
       
       //printf("please show up\n") ;
       rq = malloc (sizeof (struct ReadyQueue));

        struct cell *c = malloc(sizeof(struct cell));
        c->pcb = pcb;
        c->next = NULL;
        rq->size++;
       


        
        if (rq == NULL) {
        printf("Space not allocated") ;
        }
       else {
        rq->head = c; 
        rq->tail = c;
        rq->size++;

        //printRQ() ;
        }


    }

    //2 if the ready queue has been initialized
    else { 
        //printf("hello boys\n") ;
        //struct cell* temp = malloc(sizeof(struct cell)) ;
        struct cell* c = malloc(sizeof(struct cell)) ;
        c->pcb = pcb; //assign the cell the value we got as input
        c->next = NULL; //c is the last in the queue so the next is a null.

        rq->tail->next = c; //the previous tails next is the new cell c
        rq->tail = c; // the new tail is c.


        

    }


}

void resetVars(int* start, int* end, int* flag) {
    *start = 0;
    *end = 0;
    *flag = 1;
}

void free_ram(int start, int end) {
    for (int i=start; i<end+1; i++) {
        ram[i] = NULL;
    }
}

 struct PCB* getHeadPCB(struct ReadyQueue* rq) {
    return rq->head->pcb;
}

int scheduler() {
    cpu.quanta = 20; //declaring the cpu quanta to be 2 by default. 
    //printf("Came inside scheduler\n") ;

    // 1) Checks if the CPU is available.
    
        if (cpu.in_use) {
            printf("The CPU is being used right now!") ;
            return -1;
        }
        else {
                while (1) { //while the rq is not empty.
                    //printf("Came inside while\n") ;
                    //printf("this is the heads start of the rq : %d\n", rq->head->pcb->start) ;
                    

                    //2) Copy the PC from the PCB into the IP of the cpu.
                    struct PCB* current_head_pcb = getHeadPCB(rq) ;

                    //printf("AFTER PCB STATEMENT") ;

                    cpu.IP = current_head_pcb->PC ;



                    //printf("This is the IP of the CPU and the current heads PC number : %d\n", cpu.IP) ;

                    // how to figure out what the quanta passed should be. 
                    // if the program is shorter than the quanta then it needs to run only till that point
                    // else the program should run up untill the cpus quanta.
                

                    int num_of_lines_left_for_program = (rq->head->pcb->end - current_head_pcb->PC) + 1;

                    //printf("The quanta is the number of lines the program has left : %d\n", num_of_lines_left_for_program) ;
                    int final = 0;

                    if (num_of_lines_left_for_program > cpu.quanta) { //if the program runs till its quanta and now has to do round robin.
                        
                        final = run(cpu.quanta) ;

                        current_head_pcb->PC = current_head_pcb->PC + cpu.quanta ; //increased the pc of the pcb to showcase the change in instruction.

                        

                        //current_head_pcb->start = current_head_pcb->start + 2; // the start of the program is updated.

                        if (rq->head->next != NULL) {
                            rq->head = rq->head->next; //the new head is the next PCB in line.
                            addToReady(current_head_pcb) ; // the pcb is added back to the readyqueue. 
                        }

                        

                        
                        

                    }
                    else { // the program executes one whole pcb and now we need to remove it from the readyqueue. 
                        rq->size = rq->size - 1;
                        final = run(num_of_lines_left_for_program) ;

                        free_ram(current_head_pcb->start, current_head_pcb->end) ;

                        if (rq->head->next == NULL) {
                            rq->head = NULL;
                            free(current_head_pcb) ;
                            rq = NULL;
                            resetNextCell();
                            resetVars(&start, &end, &flag) ;
                            
                            break;
                        }
                        rq->head = rq->head->next ;
                        
                        free(current_head_pcb) ;


                  //      printf("Staying inisde the loop with size of the queue being : %d", rq->size) ;

                    }
                }
                //printf("out of the loop\n") ;
               // printRAM();
                

           
            
        }
        return 0;

}


    
    // 3) Call int run(int quanta) in cpu.c to run the program, by copying quanta lines of code from ram[] using IP into the IR, which then calls the interpreter. This executes quanta instructions from the program or untill the script file is at the end.  If the program is not at the end, the PCB PC pointer is updated with the IP value and the PCB is placed at the tail of the ready queue. If the program is at the end, then it terminates.


int myinit(char *filename) {
    //1 load data into the RAM by calling the addtoram function. 
    FILE *p = fopen(filename,"r") ;
    addToRam(p, &start, &end) ;
    //2 make pcb out of the program
    struct PCB* temp = malloc(sizeof(struct PCB)) ;
    //printf("This is start : %d, this is end : %d", start, end) ;
    temp = makePCB(start,end) ;

    //printPCB(temp) ;
// GOOD UP TILL HERE !!!!!



    //3 add this PCB to the ready queue!
    addToReady(temp) ;

    //printRQ();
    return 0; // means that the function was successful 
}



//                                                          MAIN METHOD
/*------------------------------------------------------------------------------------------------------------------------------------*/




int main(int argc, char* argv[]) {




    

   printf("Kernel 1.0 loaded!\n") ;

   int result = shellUI() ;

    


    
    return 0;

}