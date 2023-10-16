#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 512
#define FREE 0
#define ALOC 1
#define ROUNDUP8(x) (((x) + 7) & (-8))

static double memory[MEMLENGTH];
char *heapstart = (char *) memory;//heapstart will refer to the first byte of memory,

    // b/c it is memory casted as a char array.

                // typedef struct Metadata{ //16 bytes
                //     int size; //size of FULL chunk  ( meta+payload)
                //     int state;
                //     struct Metadata *next; //8 bytes
                //     //struct Metadata *prev
                // }meta;

                // int metasize = sizeof(meta);
                // static meta* header  = NULL;
//int firstmalloc = 1; //need to initialize the first header on first malloc

//metadata and payload must both be multiples of 8
    //metadata should always be 8 bytes
int getSize(char* ptr){
    int* p = (int*) ptr;
    return *p;

}
bool isFree(char*ptr){
    int* p = (int*) (ptr+8);
    return (*p==0);
}
bool setSize(char* ptr, int size){
    int* p2 = (int*) ptr;
    *p2 = size;
    return  (*p2 = size);
}
bool setState(char* ptr, int state){
    int* p2 = (int*) (ptr+8);
    return (*p2 = state);
}
int getSizeNext(char* ptr){
    ptr = getNext(ptr);
    int *p = (int*) ptr;
    return *p;
    
}

bool isFreeNext(char* ptr){
    ptr = getNext(ptr);
    int* p = (int*) (ptr+8);
    return (*p==0);
}

bool setSizeNext(char* ptr, int size){
    ptr = getNext(ptr);
    int* p = (int*) ptr;
    return (*p=size);
}

bool setStateNext(char* ptr, int state){
    ptr = getNext(ptr);
    int* p = (int*) (ptr+8);
    return (*p=state);
}

char* getNext(char* ptr){
    int* p = (int*) ptr;
    int size1 = *p;
    return ptr + size1;
}
void* mymalloc(size_t size, char* file, int line){
    //if firstmalloc, place first meta block
    if(size==0){
        printf("Cannot allocate 0 bytes");
        return NULL;
    }

    size= ROUNDUP8(size);
                // if(!header){
                //     //inititialize first header on first malloc
                //     header = (meta*) heapstart;
                //     header->next = NULL;
                //     header->size = MEMLENGTH;
                //     header->state = FREE;

                //     //firstmalloc = 0;

                // }
    char* ptr = heapstart;
    //now look for chunk big enough to hold memory
    /* Conditions:
        if chunk is not big enough OR is not free go to next chunk
    */
    while(ptr<heapstart +MEMLENGTH){
        bool IsFree = isFree(ptr);
        int chunkSize = getSize(ptr);

        if(chunkSize==0 || (IsFree && chunkSize >= size+8) ){
                                                //size for next metadata too
            setSize(ptr, size);
            setState(ptr, 1);
            //set up next metadata
            if(chunkSize==0)
                setSizeNext(ptr, MEMLENGTH - (size + 8));
            else 
                setSizeNext(ptr, chunkSize - (size+8));
            
            setStateNext(ptr, 1);
            //*firstmalloc = 0;
            return ptr+8;

                // if(ptr->size < (size + metasize) || ptr->state != FREE){
                    
                //     ptr = ptr->next;
                // }else{
                    
                //     //set up next metadata
                //     ptr->next = (char*)ptr + size + metasize; //cast to char* so it's refering to 1 byte
                //     ptr->next->state = FREE;        //^ size of ptr
                //     ptr->next->size = ptr->size - size - metasize;

                //     ptr->state = ALOC;
                //     ptr->size = metasize + size;
                //     return (char*)ptr + metasize;
                // }
        }else{
            ptr= getNext(ptr);
        }

    }

    //not enough space 
    printf("Not enough space for %zu BYTES, requested in FILE %s at LINE %d", size, file, line);
    return NULL;

}

void* myfree(void* ptr,char* file, int line){
    //probably really handy to have a prev pointer for this
    //need to coalesce too
    
    printf("Invalid pointer, requested in FILE %s at LINE %d", file, line);
    return NULL;
}