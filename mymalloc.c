#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];

int main(){
#define FREE 0
#define ALOC 1


static double memory[MEMLENGTH];
char *heapstart = (char *) memory;//heapstart will refer to the first byte of memory,
    // b/c it is memory casted as a char array.

typedef struct Metadata{ //16 bytes
    int size; //size of FULL chunk  ( meta+payload)
    int state;
    struct Metadata *next; //8 bytes
    //struct Metadata *prev
}meta;

int metasize = sizeof(meta);
static meta* header  =NULL;
int firstmalloc = 1; //need to initialize the first header on first malloc

//metadata and payload must both be multiples of 8
    //metadata should always be 8 bytes

}
void* mymalloc(size_t size, char* file, int line){
    //if firstmalloc, place first meta block
    if(firstmalloc){
        //inititialize first header on first malloc
        header = (meta*) heapstart;
        header->next = NULL;
        header->size = MEMLENGTH;
        header->state = FREE;

        firstmalloc = 0;

    }
    meta* ptr = header;
    //now look for chunk big enough to hold memory
    /* Conditions:
        if chunk is not big enough OR is not free go to next chunk
    */
    while(ptr!=NULL){
        if(ptr->size < (size + metasize) || ptr->state != FREE){
            
            ptr = ptr->next;
        }else{
            
            //set up next metadata
            ptr->next = (char*)ptr + size + metasize; //cast to char* so it's refering to 1 byte
            ptr->next->state = FREE;        //^ size of ptr
            ptr->next->size = ptr->size - size - metasize;

            ptr->state = ALOC;
            ptr->size = metasize + size;
            return (char*)ptr + metasize;
        }
    }

    //not enough space 
    printf("Not enough space for %d bytes, requested in file %s at line %d", size, file, line);
    return NULL;

}

void* myfree(void* ptr,char* file, int line){
    
}