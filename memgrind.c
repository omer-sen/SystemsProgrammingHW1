#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.c"

int main(){
    
    int* x = malloc(sizeof(int));
    *x =256;
    printf("x=%d \n", *x);
    //int y;
    free(x);
    free(x);
    //free(&y);

    return EXIT_SUCCESS;
}