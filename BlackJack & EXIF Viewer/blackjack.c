//Preprocessors
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Method Prototypes
void PrintScore(int[], int[], int f);
int CalculateScore(int a[]);
void DetermineWinner(int,int);
int StringEquals(char[], char[], int);
int GetRandomCard();

int main()
{

	//Declarations/Initializations
	int i = 0;
	int hitting = 1;
	int value = 0;
	int value2 = 0;
	int d_cards[2];
	int d_score = 0;
	int d_aces = 0;
	int p_cards[2];
	int p_score = 0;
	int p_aces = 0;
	char input[10];
	char hit[10] = "hit";
	char stand[10] = "stand";
	
	//Seed Random Number Generator
	srand((unsigned int)time(NULL));
	
	//Greet Player
	printf("Welcome to Blackjack\n");
	
	//Loop that gets the Dealers and Players first 2 cards to begin the game
	while(i < 2)
	{
		//Get value for cards
		value = GetRandomCard();
		value2 = GetRandomCard();
		
		//Keep track of how many aces the Dealer has
		if(value == 11)
			d_aces++;
			
		//Keep track of how many aces the Player has
		if(value2 == 11)
			p_aces++;

		d_cards[i] = value;
		p_cards[i] = value2;
		
		i++;
	}
	
	//Calculate Initial Scores for Dealer and Player
	d_score = CalculateScore(d_cards);
	p_score = CalculateScore(p_cards);
	
	//Print Initial Scores for Dealer and Player
	PrintScore(d_cards,p_cards,0);
	
	//If Player Draws 21 off the start of the game. Inform Player that he/she has won and end the game
	if(p_score == 21)
	{
		printf("You have 21! You WIN!!!\n");
	}
	
	
	else
	{
	
	//While the Player's Score is under 21. Allow him to hit until he stands or busts
	while(p_score <= 21 && hitting)
	{
		printf("Would you like to \"hit\" or \"stand\"? \n");
		fgets (input, 10, stdin);
	
		//When the Player Hits: Sum the Player's current cards and store the sum in p_card[0]. Draw a new card and store it in p_card[1]
		if(StringEquals(input,hit,3))
		{
			p_cards[0] += p_cards[1];
			value = GetRandomCard();
			
			//Keep track of how many aces the Player has
			if(value == 11)
				p_aces++;
			
			p_cards[1] = value;
			
			//If Player Busts check to see if he has any aces. If so then decrement his score by 10
			if(p_score > 21 && p_aces > 0)
			{
				p_cards[0] -= 10;
				p_aces--;
			}
			
			//Print Scores for Dealer and Player after Player Hits
			p_score = CalculateScore(p_cards);
			PrintScore(d_cards,p_cards, 0);
		}
	
		//When Player Stands: End the while loop
		else if(StringEquals(input,stand,5))
		{
			hitting = 0;
		}
		
	}
	
	//Print Scores for Dealer and Player to reveal Dealer's cards
	PrintScore(d_cards,p_cards, 1);
	
	//Determine whether the Dealer will hit/stand if the Player did not bust
	if(p_score <= 21)
	{
		//Hit while Dealer's Score is under 17
		//When the Dealer Hits: Sum the Dealer's current cards and store the sum in d_card[0]. Draw a new card and store it in d_card[1]
		while(d_score < 17)
		{
			d_cards[0] += d_cards[1];
			value = GetRandomCard();
			
			if(value == 11)
				d_aces++;
			
			d_cards[1] = value;
			
			//If Dealer Busts check to see if he has any aces. If so then decrement his score by 10
			if(d_score > 21 && d_aces > 0)
			{
				d_cards[0] -= 10;
				d_aces--;
			}
			
			//Print Scores for Dealer and Player after Dealer Hits
			d_score = CalculateScore(d_cards);
			PrintScore(d_cards,p_cards, 1);
		}
			
	}
	
	//Calculate Final Scores for Dealer and Player
	d_score = CalculateScore(d_cards);
	p_score = CalculateScore(p_cards);
	
	//Print Final Scores for Dealer and Player
	printf("Final Scores:\n\n");
	PrintScore(d_cards,p_cards, 1);
	DetermineWinner(d_score,p_score);
	
	}
	
	printf("GAME OVER!!! Thanks for playing Blackjack!\n");
}

//Function that takes two card arrays: d[] and p[] and uses their card values to print the Dealers and Players score
//It also takes an int f that serves as a flag whether to hide on of the Dealer's cards
//Hide one of Dealer's cards: f = 0
//Show both of Dealer's cards and score: f= 1
void PrintScore(int d[], int p[], int f)
{
	if(f == 0)
	{
		printf("The dealer:\n");
		printf(" ? + %d \n\n",d[1]);
	}
	
	else
	{
		printf("The dealer:\n");
		printf(" %d + %d = %d",d[0],d[1],CalculateScore(d));
	
		if(CalculateScore(d) > 21)
			printf(" BUSTED!\n");
			
		printf("\n\n");
		
	}
	
	printf("You:\n");
	printf(" %d + %d = %d",p[0],p[1],CalculateScore(p));
	
	if(CalculateScore(p) > 21)
		printf(" BUSTED!\n");
			
	printf("\n\n");
	
}

//Function that compares two Strings up to a maximum amount of characters (n)
//Returns 0 if they strings arent the same
//Returns 1 if the strings match 
int StringEquals(char a[], char b[], int n)
{
	int i;
	
	for(i = 0; i < n; i++)
	{
		if(a[i] != b[i])
			return 0;
	}
	
	return 1;
}

//Function that takes a card array a[] and sums its two card value to calculate the score
int CalculateScore(int a[])
{
	return (a[0] + a[1]);
}

//Function that returns a random card value 
int GetRandomCard()
{
	int low = 2;
	int high = 14;
	int value;
	
	value = rand() % (high - low + 1) + low;
	
	//Compensate for face cards. Substitute 12,13,14 for Jack,Queen,King
	if(value == 12 || value == 13 || value == 14)
		value = 10;
		
	return value;
}

//Function given two ints that determines the winner/loser of the game
//int d: dealers score
//int p: players score 
void DetermineWinner(int d, int p)
{
	
	if(d == 21 && p == 21)
	{
		printf("Dealer has 21! But You also have 21!! Its a DRAW!!!\n");
	}
	
	else if(d == 21)
	{
		printf("Dealer has 21! You LOSE!!!\n");
	}
	
	else if(p == 21)
	{
		printf("You have 21! You WIN!!!\n");
	}
	
	else if(d > 21)
	{
		printf("Dealer has BUSTED! You WIN!!!\n");
	}
	
	else if(p > 21)
	{
		printf("You have BUSTED! You LOSE!!!\n");
	}
	
	else if(d == p)
	{
		printf("Dealer has %d. You have %d: Its a DRAW!!!\n",d,p);
	}
	
	else if(d > p)
	{
		printf("Dealer has %d. You have %d: You LOSE!!!\n",d,p);
	}
	
	else if(d < p)
	{
		printf("Dealer has %d. You have %d: You WIN!!!\n",d,p);
	}
	
}