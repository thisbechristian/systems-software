//#ifndef MYMALLOC_H_INCLUDED
#define MYMALLOC_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#define MAX_SIZE (1 << 30)

typedef struct{
	char allocated;
	char size;
	struct Node* next;
	struct Node* prev;
}Node;

void my_buddy_init();
void* my_buddy_malloc(int size);
void* my_buddy_malloc_r(int size);
void my_free(void* ptr);
void my_free_r(void* buddy_to_free, int size);

