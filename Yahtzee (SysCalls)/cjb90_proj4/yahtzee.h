//Include Guard
#ifndef YAHTZEE_H

//Preprocessors
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//Method Prototypes
int rollDie();
void printDice(int roll);
void printScore(int total);
int selectionScore(int c1, int c2);
int populateSelection(int c1, int c2);
int compareInt(const void* x, const void* y);
int getSelection(int choice);
void initializeSelection();

//Defines
#define YAHTZEE_H
#define NUMDICE 5
#define MAXROLL 3
#define NUMSEL 7
#define ASCII0 48
#define ASCII7 55
#define FULL 25
#define SMALL 30
#define LARGE 40
#define YAHTZEE 50
#define BONUS 63
#define NUMROUNDS 13

#endif