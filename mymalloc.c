

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
 
 struct MetaData {
    size_t size;
    int is_allocated;
    struct MetaData* next;
};

bool validPtr(char* ptr){
    return (ptr<heapstart+MEMLENGTH) && (ptr>=heapstart);
}
int getSize(char* ptr){
    int* p = (int*) ptr;
    return *p;

}
bool isFree(char*ptr){
    int* p = (int*) (ptr+4);
    return (*p==1);
}
bool setSize(char* ptr, int size){
    int* p2 = (int*) ptr;
    *p2 = size;
    return  (*p2 = size);
}
bool setState(char* ptr, int state){
    int* p2 = (int*) (ptr+4);
    return (*p2 = state);
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

bool setSizeNext(char* ptr, int size){
    //ptr = getNext(ptr);
    int* p = (int*) getNext(ptr);
    return (*p=size);
}

bool setStateNext(char* ptr, int state){
    //ptr = getNext(ptr);
    int* p = (int*) (getNext(ptr)+4);
    return (*p=state);
}

char* getNext(char* ptr){
    int* p = (int*) ptr;
    int size1 = *p;
    return ptr + size1;
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

void* mymalloc(size_t size, char* file, int line) {
    
    size_t aligned_size = ROUNDUP8(size);

    struct MetaData* current = (struct MetaData*)memory;
    
    if(!(current->size > 0)){
        current->size=MEMLENGTH;
    }
    while(current != NULL){
        if(!current->is_allocated && current->size >= aligned_size){
            current->is_allocated = 1; 
            current->size = aligned_size;
            if(current->next == NULL){
                
                current->next = (struct MetaData*) current + aligned_size;
                current->next->is_allocated = FREE;
                current->next->size = MEMLENGTH - (aligned_size + sizeof(struct MetaData));
            }
            
            return (void*)current + sizeof(struct MetaData);
            
        }
        current = current->next;
    }

    return NULL;
}

void* myfree(void* ptr,char* file, int line){
    if(!validPtr(ptr)){
        printf("Error: Attempted to free a pointer that was not obtained from heap, in FILE %s at LINE %d\n", file, line);
        return NULL;
    }
    char* curr = heapstart;
    char* realPtr = ptr - sizeof(struct MetaData);

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