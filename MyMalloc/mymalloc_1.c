#include "mymalloc.h"

int MAX_SIZE = (1 << 30);
int initialize_flag;
Node* free_buddytable[31];
Node* used_buddytable[31];
void* base;

void my_buddy_init(){
	Node* max_free = (Node*)malloc(sizeof(Node));
	base = mmap(NULL, MAX_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
	max_free->address = base;
	max_free->prev = NULL;
	max_free->next = NULL;
	free_buddytable[30] = max_free;
}


void* my_buddy_malloc(int size){
	
	if(size <= MAX_SIZE)
	{
		int i;
		
		if(!initialize_flag){
			my_buddy_init();
			initialize_flag = 1;
		}
	
		for(i = 0; i < 30 && (1 << i) < size; i++);
		
		Node* free_location = my_buddy_malloc_r(i);
	
		if(free_location != NULL){
		
			if(used_buddytable[i] == NULL){
				free_location->prev = NULL;
				free_location->next = NULL;
				used_buddytable[i] = free_location;
			}
		
			else{
				used_buddytable[i]->prev = free_location;
				free_location->next = used_buddytable[i];
				free_location->prev = NULL;
				used_buddytable[i] = free_location;
			}
		
			return free_location->address;	
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
	Node* root;
	
	if(size <= 30){
		
		if(free_buddytable[size] != NULL){
			buddy_free = free_buddytable[size];
			if(buddy_free->next != NULL){
				root = free_buddytable[size]->next;
				root->prev = NULL;
			}
			free_buddytable[size] = free_buddytable[size]->next;
			return buddy_free;
		}
		
		else{
			buddy1 = my_buddy_malloc_r(size+1);
			if(buddy1 != NULL){
			buddy2 = (Node*)malloc(sizeof(Node));
			buddy2->address = ((buddy1->address-base) ^ (1 << size)) + base;
			buddy2->prev = NULL;
			buddy2->next = NULL;
			free_buddytable[size] = buddy2;
			
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
	int found = 0;
	Node* buddy_to_free = NULL;
	Node* curr_buddy;
	Node* prev_buddy;
	Node* next_buddy;
	
	if(initialize_flag){
		
		for(i = 0; i <= 30 && !found; i++){
		
			if(used_buddytable[i] != NULL){
			
				curr_buddy = used_buddytable[i];
				
				do{
					if(curr_buddy->address == ptr){
						
						buddy_to_free = curr_buddy;
						found = 1;
						size = i;
						
						if(curr_buddy->prev == NULL)
							used_buddytable[i] = NULL;
								
						else{
							prev_buddy = curr_buddy->prev;
							next_buddy = curr_buddy->next;
							next_buddy->prev = prev_buddy;
							prev_buddy->next = next_buddy;
						}
					}
					
				}while(curr_buddy->next != NULL && !found);	
			}
		}
		
		if(buddy_to_free != NULL){
			
			if(free_buddytable[size] == NULL){
				buddy_to_free->next = NULL;
				buddy_to_free->prev = NULL;
				free_buddytable[size] = buddy_to_free;
			}
			
			else{
				my_free_r(buddy_to_free,size);
			}
			
		}
		
		else{
			printf("Address given was previously freed or not dynamically allocated!\n");
		}
		
			
	}
	
	else{
		printf("Heap is Empty. No memory to free!\n");
	}

}

void my_free_r(void* free_buddy, int size){
	
	int found = 0;
	void* buddy1_address;
	void* buddy2_address;
	Node* buddy_to_free;
	Node* curr_buddy;
	Node* prev_buddy;
	Node* next_buddy;
		
	buddy_to_free = (Node*)free_buddy;
	curr_buddy = free_buddytable[size];
	buddy1_address = buddy_to_free->address;
	buddy2_address = ((buddy1_address-base) ^ (1 << size)) + base;
	
	if(free_buddytable[size] != NULL){
	
	do{
		
		if(curr_buddy->address == buddy2_address){		
			found = 1;		
			if(curr_buddy->prev == NULL)
				used_buddytable[size] = NULL;
								
			else{
				prev_buddy = curr_buddy->prev;
				next_buddy = curr_buddy->next;
				next_buddy->prev = prev_buddy;
				prev_buddy->next = next_buddy;
			}
		}
			
	}while(curr_buddy->next != NULL && !found);	
	
	}
	
	if(found)
		my_free_r(buddy_to_free, (size+1));
		
	else{
		buddy_to_free->next = free_buddytable[size];
		buddy_to_free->prev = NULL;
 		free_buddytable[size] = buddy_to_free;
	}
	
}