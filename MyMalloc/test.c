#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/mman.h>

typedef struct{
	intptr_t address;
	struct Node* next;
}Node;

Node* array[10];

int main(){
	
	Node* test;
	int* a;
	double* b;
	void* base;
	
	base = mmap(NULL, (1 << 30), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
	
	a = (base+500);
	
	*a = 100;
	
	int i =0;
	int size = 4;
	while(size >= (1 << i))
	{
		printf("1 << %d is: %d\n",i,(1 << i));
		i++;
	}
	
	printf("i: %d \n",i);
}


