#include <stdio.h>
#include <stdlib.h>

//Inlcuding node file
//#include <node.c>

//Structure definition for Leftist Tree node
//Represents a node to be used in Min Leftist Tree
struct node {
	int data; //Contains the priority
    int shortest; //It contains the shortest(x) value
    struct node *leftchild, *rightchild; //Pointers to Left Child & Right Child
};


//Structure for a Queue element to handle Leftist node during Print
 struct leftistQueue{
	struct node *element; //This will contain the Fibonacci Heap node
	struct leftistQueue *next; //Pointer to next element
}; //This will be HEAD & TAIL for queue


 //Queue function's declarations
int leftistEnqueue(struct node *);
struct node *leftistDequeue();
int isLeftistQueueEmpty();



//Leftist Tree Funtion declarations
struct node *insertMinLeftistTree(struct node*, int);
struct node *meldMinLeftistTree(struct node*, struct node*);
int printNodes(struct node*);
struct node *ensureShortest(struct node*);
struct node *deleteMinLeftistTree(struct node*);
void calSValue(struct node*);
struct node *initializeLeftistTree(struct node*, int[], int);

//Global Variable
//enum LOG {DEBUG, INFO, ERROR};
//enum LOG log;
//int MAX_OPERATIONS = 5000;
