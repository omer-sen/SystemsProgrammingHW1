#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mymalloc.h"
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
bool test2(){
    char* arr[120];
    for(int i=0; i<120; i++){
        arr[i] = malloc(1);
        if(arr[i]==NULL){
            return false;
        }
    }
    for(int i=0; i<120; i++){
        free(arr[i]);
    }
    return memCleared() ==1;
}

bool test3(){
    int random_number;
    char* arr[120];
    int counter = 0;
    while(counter<120){
        random_number = rand() % 2;
        if(random_number == 1){
            arr[counter] = malloc(1);
            if(arr[counter]==NULL){
                return false;
            }
            counter++;
        }else{
            if(counter>0 && arr[counter] != NULL){
                counter--;
                free(arr[counter]);
            }
        }
        
    }
    for(int i=0; i<120; i++){
        if(arr[i] != NULL)
            free(arr[i]);
    }
    return memCleared() ==1;
}

bool test4(){
    char* arr[120];
    for(int i=0; i<120; i++){
        arr[i] = malloc(1);
    }
    for(int i=0; i<120; i++){
        free(arr[i]);
    }
    char *ptr = malloc(MEMLENGTH-16);
    free(ptr);
    return memCleared() == 1;
}

bool test5(){
    int random_number;
    for(int i=0; i<120; i++){
        random_number = rand() % 2;
        if(random_number == 1){ 
            char *ptr = malloc(MEMLENGTH-16);
            free(ptr);
        }else{
            char *ptr = malloc(1);
            free(ptr);
        }
    }
    return memCleared() == 1;
}

int main(){


    double timeSum =0;
    
    //test1
    for(int i = 0; i < 50; i++) {
        clock_t start = clock();
        if(test1()==false){
            printf("error");
            return EXIT_SUCCESS;
        }
        clock_t end = clock();
        double time = (double) (end-start) / CLOCKS_PER_SEC;
        timeSum+=time;
    }
    double avgTime =timeSum/50;
    printf("Average time for test1: %f\n", avgTime);
    //clear memory

    //test2
    timeSum=0;
    for(int i = 0; i < 50; i++) {
        clock_t start = clock();
        if(test2()==false){
            printf("error");
            return EXIT_SUCCESS;
        }
        clock_t end = clock();
        double time = (double) (end-start) / CLOCKS_PER_SEC;
        timeSum+=time;
    }
    avgTime =timeSum/50;
    printf("Average time for test2: %f\n", avgTime);

     //test3
    timeSum=0;
    for(int i = 0; i < 50; i++) {
        clock_t start = clock();
        if(test3()==false){
            printf("error");
            return EXIT_SUCCESS;
        }
        clock_t end = clock();
        double time = (double) (end-start) / CLOCKS_PER_SEC;
        timeSum+=time;
    }
    avgTime =timeSum/50;
    printf("Average time for test3: %f\n", avgTime);

    //test4
    timeSum=0;
    for(int i = 0; i < 50; i++) {
        clock_t start = clock();
        if(test4()==false){
            printf("error");
            return EXIT_SUCCESS;
        }
        clock_t end = clock();
        double time = (double) (end-start) / CLOCKS_PER_SEC;
        timeSum+=time;
    }
    avgTime =timeSum/50;
    printf("Average time for test4: %f\n", avgTime);

    //test5
    timeSum=0;
    for(int i = 0; i < 50; i++) {
        clock_t start = clock();
        if(test5()==false){
            printf("error");
            return EXIT_SUCCESS;
        }
        clock_t end = clock();
        double time = (double) (end-start) / CLOCKS_PER_SEC;
        timeSum+=time;
    }
    avgTime =timeSum/50;
    printf("Average time for test5: %f\n", avgTime);
    
    return EXIT_SUCCESS;
}
