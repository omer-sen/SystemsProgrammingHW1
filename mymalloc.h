#ifndef _MYMALLOC_H
#define _MYMALLOC_H

#include <stdbool.h>

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

extern char* heapstart;
#define MEMLENGTH 2048

void* mymalloc(size_t size, char* file, int line);
void* myfree(void* ptr,char* file, int line);
int getSize(char* ptr);
bool isFree(char*ptr);
void setSize(char* ptr, int size);
void setState(char* ptr, int state);
int getSizeNext(char* ptr);
bool isFreeNext(char* ptr);
void setSizeNext(char* ptr, int size);
void setStateNext(char* ptr, int state);
char* getNext(char* ptr);
bool isPrecedingAndFree(char* curr, char* ptr);
void mergeBlocks(char* p1, char* p2);
#endif