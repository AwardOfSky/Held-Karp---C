#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LIMIT 10
#define N 16

int memory_allocated = 0;

typedef struct s_node {
    int key;
    int value;
    struct s_node *next;
} s_node;

s_node **arr;

void create(void) {
    printf("Allocating Inicial array: %ld\n", sizeof(*arr) * N);
    memory_allocated += sizeof(*arr) * N;
    arr = (s_node **)malloc(sizeof(*arr) * N);
    for(int i = 0; i < N; i++) {
	arr[i] = NULL;
    }
    for(int i = 0; i < N; i++) {
	//printf("valor de arr[%d]: %ld\n", i, (long int)arr[i]);
    }
}

void put(int key, int value) {
    int index = rand() & (N - 1);
    //printf("We are getting to this function!\n");
    printf("Allocating element: %ld\n", sizeof(s_node));
    memory_allocated += sizeof(s_node);
    s_node *temp = (s_node*)malloc(sizeof(s_node));
    temp->key = key;
    temp->value = value;
    //printf("iuhgdyygwi %d\n", index);

    /* if(arr[index] == NULL) { */
    /* 	printf("tyhe fuck is going on here?\n"); */
    /* } else { */
    /* 	temp->next = arr[index]; */
    /* } */
    
    temp->next = arr[index];
    //printf("iuhgdwi\n");
    arr[index] = temp;
}

void weird_function(void) {
    for(int i = 0; i < LIMIT; i++) {
	put(1, 2);	
    }
}

void cleanup(void) {
    
    for(int i = 0; i < N; i++) {
    	printf("Size of element %d: %ld\n", i, sizeof(arr[i]));
    	memory_allocated -= sizeof(arr[i]);
    	free(arr[i]);
    }
   
    printf("Deallocating final array: %ld\n", sizeof(arr));
    memory_allocated -= sizeof(arr);
    free(arr);
}

int main() {
    srand(time(NULL));
    create();
    printf("We are gonna insert in the list\n");
    weird_function();
    printf("Total memory allocated: %d\nDeallocating memory\n",
	   memory_allocated);
    cleanup();
    printf("Remaning memory: %d\n", memory_allocated);
    return 0;
}
