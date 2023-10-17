#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.c"
#include <time.h>

int memCleared(){
    int chunkSize = getSize(heapstart);
    int IsFree = isFree(heapstart);

    if((chunkSize==0 && IsFree ==1 ) || (chunkSize==MEMLENGTH && IsFree==1)){
        return 1;
    }return 0;
}

bool test1(){
    for(int i = 0; i < 120; i++) {
        char *ptr = malloc(1);  // Allocate 1 byte of memory
        free(ptr);  // Release the memory
    }
    return memCleared() ==1;
}
int main(){


    double timeSum =0;
    
    //test1
    for(int i = 0; i < 50; i++) {
        clock_t start = clock();
        test1();
        clock_t end = clock();
        double time = (double) (end-start) / CLOCKS_PER_SEC;
        timeSum+=time;
    }
    double avgTime =timeSum/50;
    printf("Average time for test1: %f\n", avgTime);

    return EXIT_SUCCESS;
}