#ifndef _MYMALLOC_H
#define _MYMALLOC_H

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

void* mymalloc(size_t size, char* file, int line);
void* myfree(void* ptr,char* file, int line);
int getSize(char* ptr);
bool isFree(char*ptr);
bool setSize(char* ptr, int size);
bool setState(char* ptr, int state);
int getSizeNext(char* ptr);
bool isFreeNext(char* ptr);
bool setSizeNext(char* ptr, int size);
bool setStateNext(char* ptr, int state);
char* getNext(char* ptr);

#endif