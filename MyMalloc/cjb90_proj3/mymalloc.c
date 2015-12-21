#include "mymalloc.h"

int initialize_flag;
void* buddytable[31];
void* base;

void my_buddy_init(){
	base = mmap(NULL, MAX_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
	Node* max_free = (Node*)base;
	max_free->allocated = 0;
	max_free->size = 30;
	max_free->prev = NULL;
	max_free->next = NULL;
	buddytable[30] = base;
}


void* my_buddy_malloc(int size){
	
	if(size <= MAX_SIZE)
	{
		int i;
		Node* free_location;
		Node* nextbuddy;
		
		if(!initialize_flag){
			my_buddy_init();
			initialize_flag = 1;
		}
	
		for(i = 0; i < 30 && (1 << i) < size; i++);
		
		if(i < 5)
			i = 5;
		else if(i == 5)
			i = 6;
		
		free_location = (Node*)my_buddy_malloc_r(i);
	
		if(free_location != NULL){                 
			free_location->allocated = 1;
			free_location->size = i;
			return (void*)(free_location) + 0x2;
		}
	
		else{
			printf("Not enough memory for given size.\n");
			return NULL;
		}
	
	}
		
	else{
		printf("Not enough memory for given size.\n");
		return NULL;
	}
	
	
}

void* my_buddy_malloc_r(int size){

	Node* buddy_free;
	Node* buddy1;
	Node* buddy2;
	Node* nextbuddy;
	Node* prevbuddy;

	if(size <= 30){

		if(buddytable[size] != NULL){
		
			buddy_free = (Node*)buddytable[size];
			if(buddy_free->prev == NULL){
				nextbuddy = buddy_free->next;
				if(nextbuddy != NULL)
					nextbuddy->prev = NULL;
				buddytable[size] = nextbuddy;
			}
			
			else if(buddy_free->prev != NULL){
				prevbuddy = buddy_free->prev;
				nextbuddy = buddy_free->next;
				prevbuddy->next = nextbuddy;
				if(nextbuddy != NULL)
					nextbuddy->prev = prevbuddy;
			}

			return buddy_free;
		}
		
		else{
			buddy1 = (Node*)my_buddy_malloc_r(size+1);
			if(buddy1 != NULL){
				buddy2 = (Node*)((((void*)buddy1 - base) ^ (1 << size)) + base);
				buddy2->next = NULL;
				buddy2->prev = NULL;
				buddy2->size = size;
				buddy2->allocated = 0;
				buddytable[size] = buddy2;
			}
			return buddy1;
		}
	}
	
	else
		return NULL;

}


void my_free(void *ptr){

	int i;
	int size;
	Node* buddy_to_free;
	Node* curr_buddy;
	Node* prev_buddy;
	Node* next_buddy;
	ptr = ptr-0x2;

	if(initialize_flag){
	
		buddy_to_free = (Node*)ptr;
		size = buddy_to_free->size;
		
		if(buddy_to_free->allocated == 1 && size <= 30){
			my_free_r(buddy_to_free,size);
		}
		
		else{
			printf("Address given was previously freed or not dynamically allocated!\n");
		}
		
			
	}
	
	else{
		printf("Heap is Empty. No memory to free!\n");
	}

}

void my_free_r(void* buddy_to_free, int size){

	Node* buddy1;
	Node* buddy2;
	Node* nextbuddy;
	Node* prevbuddy;
	
	buddy1 = (Node*)(buddy_to_free);
	buddy2 = (Node*)((((void*)buddy1-base) ^ (1 << size)) + base);
	
	if(size < 30 && buddy2->allocated == 0){
			
		if(buddy2->prev == NULL){
			nextbuddy = buddy2->next;
			if(nextbuddy != NULL)
				nextbuddy->prev = NULL;
			buddytable[size] = nextbuddy;
		}
			
		else if(buddy2->prev != NULL){
			prevbuddy = buddy2->prev;
			nextbuddy = buddy2->next;
			prevbuddy->next = nextbuddy;
			if(nextbuddy != NULL)
				nextbuddy->prev = prevbuddy;
		}
		
		my_free_r(buddy1, (size+1));
		
		return;
	}
		
	else{
		buddy1->allocated = 0;
		buddy1->size = size;
		buddy1->prev = NULL;
		
		if(size == 30){
			buddy1->next = NULL;
		}
		
		else{
			nextbuddy = buddytable[size];
			buddy1->next = nextbuddy;
			if(nextbuddy != NULL)
				nextbuddy->prev = buddy1;
		}
		
		buddytable[size] = buddy1;
		return;
	}	
}
