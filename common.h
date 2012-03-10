
#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>

#define MAX_OPERATIONS  5000 //Maximum random operations

//Constants
#define F_MAXNODE 15000 //Maximum number of nodes in Fibonacci Heap
#define PATH = "C:\\read\input.txt" //Path of Input file

enum LOGLEVEL {INFO, ERROR, DEBUG};
enum MODE {RANDOM, USERINPUT};


//Structure for a Queue element
 struct q_element{
	struct f_node *element; //This will contain the Fibonacci Heap node
	struct q_element *next; //Pointer to next element
}; //This will be HEAD & TAIL for queue


 //Queue function's declarations
int q_enqueue(struct f_node *);
struct f_node *q_dequeue();
int is_q_empty();

//Main funtions's declarations
void userInputMode(char *, char *);
void randomMode();
void randomPermutation(int[], int);

int D; //Selector
#endif
