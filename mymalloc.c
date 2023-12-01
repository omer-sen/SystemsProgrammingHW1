#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.h"

#define FREE 1
#define ALOC 0
#define ROUNDUP8(x) (((x) + 7) & (-8))

static double memory[MEMLENGTH];
char *heapstart = (char *) memory;//heapstart will refer to the first byte of memory,
 
bool validPtr(char* ptr){
    return (ptr<heapstart+MEMLENGTH) && (ptr>=heapstart);
    //PPOINTER MUST ALSO BE AT THE START OF A PAYLOAD!!!
}
int getSize(char* ptr){
    int* p = (int*) ptr;
    return *p;

}
bool isFree(char*ptr){
    int* p = (int*) (ptr+4);
    return (*p==1);
}
void setSize(char* ptr, int size){
    int* p2 = (int*) ptr;
    *p2 = size;
}
void setState(char* ptr, int state){
    int* p2 = (int*) (ptr+4);
    *p2 = state;
}
int getSizeNext(char* ptr){
    int* p = (int*) getNext(ptr);
    //int *p = (int*) ptr;
    return *p;
    
}

bool isFreeNext(char* ptr){
    //ptr = getNext(ptr);
    int* p = (int*) (getNext(ptr)+4);
    return (*p==1);
}

void setSizeNext(char* ptr, int size){
    //ptr = getNext(ptr);
    int* p = (int*) getNext(ptr);
    *p=size;
}

void setStateNext(char* ptr, int state){
    //ptr = getNext(ptr);
    int* p = (int*) (getNext(ptr)+4);
    *p=state;
}

char* getNext(char* ptr){
    int* p = (int*) ptr;
    int size1 = *p;
    return ptr + size1;
}
void* mymalloc(size_t size, char* file, int line){
    if(size<=0){
        printf("Cannot allocate %zu bytes", size);
        return NULL;
    }
    size= ROUNDUP8(size); //metadata should always be a multiple of 8 bytes

    char* ptr = heapstart;
    while(ptr<heapstart +MEMLENGTH){
        bool IsFree = isFree(ptr);
        int chunkSize = getSize(ptr);

        if(chunkSize==0 || (IsFree && chunkSize >= size+16) ){//space for current+next metadata (8+8)
            setSize(ptr, size+8);
            setState(ptr, 0);
            //set up next metadata
            if(chunkSize==0)
                setSizeNext(ptr, MEMLENGTH - (size + 8));
            else{ 
                if(chunkSize - (size+8) == 8){ 
                //if chunkSize only has space for payload and new metadata (ie new metadata is useless), 
                // then just give that space to payload
                    setSize(ptr, getSize(ptr) + 8);
                    return ptr+8;
                }else
                    setSizeNext(ptr, chunkSize - (size+8));
            }
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
void mergeBlocks(char* p1, char* p2){
    //merge to p1
    int size1 = getSize(p1);
    int size2 = getSize(p2);
    setSize(p1, size1+size2);
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
    printf("Error: Attempted to free a pointer that does not point to the start of an alloted payload OR was previously freed, in FILE %s at LINE %d\n", file, line);
    return NULL;
}