#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 512
#define FREE 1
#define ALOC 0
#define ROUNDUP8(x) (((x) + 7) & (-8))

static double memory[MEMLENGTH];
char *heapstart = (char *) memory;//heapstart will refer to the first byte of memory,
 
bool validPtr(char* ptr){
    return (ptr<heapstart+MEMLENGTH) && (ptr>=heapstart);
}
int getSize(char* ptr){
    int* p = (int*) ptr;
    return *p;

}
bool isFree(char*ptr){
    int* p = (int*) (ptr+8);
    return (*p==1);
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
    return (*p==1);
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
    if(size==0){
        printf("Cannot allocate 0 bytes");
        return NULL;
    }
    size= ROUNDUP8(size); //metadata should always be a multiple of 8 bytes

    char* ptr = heapstart;
    while(ptr<heapstart +MEMLENGTH){
        bool IsFree = isFree(ptr);
        int chunkSize = getSize(ptr);

        if(chunkSize==0 || (IsFree && chunkSize >= size+8) ){//space for next metadata too
            setSize(ptr, size+8);
            setState(ptr, 0);
            //set up next metadata
            if(chunkSize==0)
                setSizeNext(ptr, MEMLENGTH - (size + 8));
            else 
                setSizeNext(ptr, chunkSize - (size+8));
            
            setStateNext(ptr, 1);
            return ptr+8;      
        }else{
            ptr= getNext(ptr);
        }
    }
    //not enough space 
    printf("Not enough space for %zu BYTES, requested in FILE %s at LINE %d\n", size, file, line);
    return NULL;
}
bool isPrecedingAndFree(char* curr, char* ptr){
    return (getNext(curr) == ptr) && (isFree(curr));
}
bool mergeBlocks(char* p1, char* p2){
    //merge to p1
    int size1 = getSize(p1);
    int size2 = getSize(p2);
    return (setSize(p1, size1+size2));
}
void* myfree(void* ptr,char* file, int line){
    if(!validPtr(ptr)){
        printf("Error: Attempted to free a pointer that was not obtained from heap, in FILE %s at LINE %d\n", file, line);
        return NULL;
    }
    char* curr = heapstart;
    char* realPtr = ptr - 8;

    while(curr< heapstart+MEMLENGTH){
        if(isPrecedingAndFree(curr, realPtr)){
            mergeBlocks(curr, realPtr);
            if(isFree(getNext(curr))){
                mergeBlocks(curr, getNext(curr));
            }
            //invalidate ptr?
            return NULL;
        }
        if(realPtr == curr){
            if(isFree(realPtr)){
                printf("Error: Double free, in FILE %s at LINE %d\n", file, line);
                return NULL;
            }
            if(isFree(getNext(curr))){
                mergeBlocks(curr, getNext(curr));
            }
            setState(realPtr, 1);
            //invalidate prr
            return NULL;
        }
        curr = getNext(curr);
    }
    printf("Error: Attempted to free a pointer that does not point to the start of an alloted chunk OR was previously freed, in FILE %s at LINE %d\n", file, line);
    return NULL;
}