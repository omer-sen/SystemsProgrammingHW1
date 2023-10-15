#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.c"

int main(){
    
    int* x = malloc(sizeof(int));
    *x =256;
    printf("x=%d", *x);

    return EXIT_SUCCESS;
}