#include "yahtzee.h"

int dice[NUMDICE];
int upper[NUMSEL];
int lower[NUMSEL];

int main(){

	int i;
	int choice1,choice2;
	int total;
	int round = 0;
	int roll = 0;
	char roll_again = 1;
	
	initializeSelection();
	
	while(round < NUMROUNDS){
	
	roll = 0;
	roll_again = 1;
	
	for(i = 0; i < NUMDICE; i++){
		dice[i] = rollDie();
	}
	roll++;
	
	printDice(roll);
	
	while(roll < MAXROLL && roll_again){
		
		int x;
		char buffer[25];
		
		printf("Which dice to reroll? ");

		fgets(buffer, sizeof(buffer), stdin);	
		
		i = 0;
		if(buffer[i] != ASCII0){
			
			while(i < sizeof(buffer)){
				if(buffer[i] > ASCII0 && buffer[i] < ASCII7){
					x = (buffer[i] - ASCII0) - 1;
					dice[x] = rollDie();
				}
				i++;
			}
		
			roll++;
			printDice(roll);
		
		}
		
		else
			roll_again = 0;
			
	}
	
	qsort(dice, NUMDICE, sizeof(int), compareInt);
	
	choice1 = getSelection(0);
	choice2 = getSelection(choice1);
	total = populateSelection(choice1, choice2);
	round++;
	if(round == NUMROUNDS){
		printf("Final Score:\n");
	}
	printScore(total);
	printf("\n");
	}
	
}

void printScore(int total){

	printf("Your score: %d\n",total);
	printf("Ones: %d \t\t\t\t\t Fours: %d\n",selectionScore(1,1),selectionScore(1,4));
	printf("Twos: %d \t\t\t\t\t Fives: %d\n",selectionScore(1,2),selectionScore(1,5));
	printf("Threes: %d \t\t\t\t\t Sixes: %d\n",selectionScore(1,3),selectionScore(1,6));
	printf("Upper Section Bonus: %d\n",selectionScore(1,7));
	printf("Three of a Kind: %d \t\t\t\t Four of a Kind: %d\n",selectionScore(2,1),selectionScore(2,2));
	printf("Small Straight: %d \t\t\t\t Large Strait: %d\n",selectionScore(2,4),selectionScore(2,5));
	printf("Full House: %d \t\t\t\t\t Yahtzee: %d\n",selectionScore(2,3),selectionScore(2,6));
	printf("Chance: %d\n",selectionScore(2,7));

}

int selectionScore(int c1, int c2){
	
	if(c1 == 1)
		return upper[c2-1] < 0 ? 0 : upper[c2-1];

	
	else
		return lower[c2-1] < 0 ? 0 : lower[c2-1];

	
}

int populateSelection(int c1, int c2){

	int i;
	int count,count2;
	int sum;
	int total;
	
	if(c1 == 1){
		count = 0;
		sum = 0;
		
		for(i = 0; i < NUMDICE; i++){
			if(dice[i] == c2)
				count++;
		}
		
		upper[c2-1] = (count*c2);
		
		for(i = 0; i < (NUMSEL-1); i++){
			sum = sum + selectionScore(1,i);
		}
		
		if(sum > (BONUS-1))
			upper[NUMSEL-1] = 35;	
	}
	
	else{
		sum = 0;
		count = 0;
		
		if(c2 == 1 || c2 == 2){
			for(i = 0; i < (NUMDICE-1); i++){
				if(dice[i] == dice[i+1])
					count++;
			}
			
			if(c2 == 1){
				if(count == 2){
					sum = 0;
					for(i = 0; i < NUMDICE; i++){
						sum = sum + dice[i];
					}
					
					lower[c2-1] = sum;
					
				}
				
				else{
					lower[c2-1] = 0;
				}
			}
			
			else if(c2 == 2){
				if(count == 3){
					sum = 0;
					for(i = 0; i < NUMDICE; i++){
						sum = sum + dice[i];
					}
					
					lower[c2-1] = sum;
					
				}
				
				else{
					lower[c2-1] = 0;
				}
			}
			
		}
		
		else if(c2 == 3){
			count2 = 0;
			
			for(i = 0; i < (NUMDICE-1); i++){
				if(dice[i] == dice[i+1])
					count++;
				else{
					count2 = count;
					count = 0;
				}
			
			if((count + count2) == 3)
				lower[c2-1] = FULL;
				
			else
				lower[c2-1] = 0;
			
			}
			
		}
		
		else if(c2 == 4 || c2 == 5){
			for(i = 0; i < (NUMDICE-1); i++){
				if(dice[i]+1 == dice[i+1])
					count++;
			}
			
			if(c2 == 4){
				if(count == 3)
					lower[c2-1] = SMALL;
				
				else
					lower[c2-1] = 0;
			}
			
			else if(c2 == 5){
				if(count == 3)
					lower[c2-1] = LARGE;
				
				else
					lower[c2-1] = 0;
			}

		}
		
		else if(c2 == 6){
			for(i = 0; i < (NUMDICE-1); i++){
				if(dice[i] == dice[i+1])
					count++;
			}
			
			if(count == 4)
				lower[c2-1] = YAHTZEE;
			else
				lower[c2-1] = 0;
		}
		
		else{
			sum = 0;
			for(i = 0; i < NUMDICE; i++){
				sum = sum + dice[i];
			}
					
			lower[c2-1] = sum;
		}
	
	
	}
	
	total = 0;
	for(i = 1; i <= NUMSEL; i++){
		total = total + selectionScore(1,i) + selectionScore(2,i);
	}
	
	return total;
}

int compareInt(const void* x, const void* y){

	int *a = x;
	int *b = y;
	
	return (*a-*b);

}

int getSelection(int choice){
	
	int choice2;
	int flag = 0;
	
	if(choice == 0){
		do{
		printf("Place dice into:\n");
		printf("1) Upper Section\n");
		printf("2) Lower Section\n");
		printf("Section? ");
		scanf("%d",&choice2);
		printf("\n");
		}while(choice2 > 2 || choice2 < 0);
	}
	
	if(choice == 1){
		do{
		if(flag)
			printf("Invalid Selection or Selection already used! Try Again\n");
		else
			flag = 1;
		printf("Place dice into:\n");
		printf("1) Ones\n");
		printf("2) Twos\n");
		printf("3) Threes\n");
		printf("4) Fours\n");
		printf("5) Fives\n");
		printf("6) Sixes\n");
		printf("Section? ");
		scanf("%d",&choice2);
		while(getchar() != '\n');
		printf("\n");
		}while((upper[choice2-1] != -1) || (choice2 > 6 || choice2 < 0));
	}
	
	if(choice == 2){
		do{
		if(flag)
			printf("Invalid Selection or Selection already used! Try Again\n");
		else
			flag = 1;
		printf("Place dice into:\n");
		printf("1) Three of a kind\n");
		printf("2) Four of a kind\n");
		printf("3) Full House\n");
		printf("4) Small Straight\n");
		printf("5) Large Straight\n");
		printf("6) Yahtzee!\n");
		printf("7) Chance!\n");
		printf("Section? ");
		scanf("%d",&choice2);
		while(getchar() != '\n');
		printf("\n");
		}while((lower[choice2-1] != -1) || (choice2 > 7 || choice2 < 0));
	}
	
	return choice2;	
}

void initializeSelection(){

	int i;
	for(i = 0; i < 7; i++){
		upper[i] = -1;
		lower[i] = -1;
	}
	
	upper[NUMSEL-1] = 0; // Upper Section Bonus
}

void printDice(int roll){
	int i;
	printf("Your roll #%d: \n",roll);
	for(i = 0; i < NUMDICE; i++){		
		printf("%d ",dice[i]);
	}
	printf("\n");
}

//Function that returns a random die value 
int rollDie(){
	int low = 1;
	int high = 6;
	int random,ret;
	char* buffer = malloc(sizeof(char));
	
	int f = open("/dev/dice",0);
	ret = read(f,buffer,1);
	
	if(ret >= 0){
		random = (int)(buffer[0] - 48);
	}
	
	else{
		printf("Not using syscall!\n");
		random = (rand() % high) + low;
	}
	
	free(buffer);
	
	return random;
}