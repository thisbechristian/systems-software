#include<stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "mymalloc.h"

int main(){
	
	int* a;
	double* b;
	char*c;
	int*d;
	
	a = my_buddy_malloc(1073741824);
	my_free(a);
	a = my_buddy_malloc(sizeof(int));
	b = my_buddy_malloc(1073741824);
	b = my_buddy_malloc(sizeof(double));
	c = my_buddy_malloc(100*sizeof(char));
	d = my_buddy_malloc(sizeof(int));
	
	*a = 150000;
	*b = 99.9995;
	strcpy(c,"Hello! :)");
	*d = 123;

	printf("a: %d, b: %lf c: %s d: %d\n",*a,*b,c,*d);
	
	my_free(c);
	my_free(b);
	my_free(a);
	
	a = my_buddy_malloc(sizeof(int));
	b = my_buddy_malloc(sizeof(double));
	c = my_buddy_malloc(100*sizeof(char));
	d = my_buddy_malloc(sizeof(int));
		
	*a = 25;
	*b = 23.3333;
	strcpy(c,"Goodbye! :)");
	*d = 321;

	printf("a: %d, b: %lf c: %s d: %d\n",*a,*b,c,*d);
	
	my_free(a);
	my_free(b);
	my_free(c);
	
}