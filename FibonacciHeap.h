#include <stdio.h>
#include <stdlib.h>
#include<string.h>
//#include <cstdlib>

//Structure of Fibbonacci Heap's node
struct f_node{
    int degree; //Degree of a node
	int data; //Priority
    int childCut; //Child cut of a node. It can either be true(1), false(0) or undefined(-1)
    struct f_node *child, *leftSibling, *rightSibling, *parent; //Child, Left Sibling, Right Sibling & Parent pointer
};




//for Logging
//enum LOG {DEBUG, ERROR, INFO};

//for Mode
//enum MODE {RANDOM, USERINPUT};

//Function declarations
struct f_node *insert_MinFHeap(struct f_node*, int);
int print_MinFHeap(struct f_node*);
struct f_node *meld_MinFHeap(struct f_node*, struct f_node*);
struct f_node *deleteMin_MinFHeap(struct f_node*);
struct f_node *pairwiseCombine_MinFHeap(struct f_node*, struct f_node*);
struct f_node *cascading(struct f_node*, struct f_node*); //Cascading function to Min Fibonacci Heap
struct f_node *removeMinFHeap(struct f_node*, int index); //To remove any node from Min Fibonacci Heap
struct f_node *decreaseKey(struct f_node*, int index, int key); //It decrease the data of a node by value key
struct f_node *intializeFMinHeap(struct f_node*, int[], int);



//Global variables
//enum LOG log = DEBUG; //Setting logging level to DEBUG
//enum MODE mode = USERINPUT;

