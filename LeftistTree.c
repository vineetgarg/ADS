#include "LeftistTree.h"
#include "common.h"
/*********************************************************************************************
Filename: MinLeftistTree
Description: It implements the Min Leftist Tree data strucutre
Operations: Insert, Print, Meld, DeleteMin
Author: Garg,Vineet
Date: October 21, 2010
**********************************************************************************************/
extern int log;
extern int mode;

struct leftistQueue *LHEAD=NULL, *LTAIL=NULL;
//int main()
//{
//	struct node *root=NULL; //Root of Leftist Tree
//    int operation, randomData, D;
//
//	for(operation=1;operation<=MAX_OPERATIONS; operation++){
//			if(log == DEBUG || log == INFO)
//				printf("\nPeforming operation #%d\n", operation);
//			D = rand() % 2;
//			randomData = rand() % 10000; //Data's range is b/w 0-9999
//			if(D == 0){
//				if(log == DEBUG || log == INFO)
//					printf("Deleting MIN............\n");
//				root = deleteMinLeftistTree(root);
//			}
//			else{
//				if(log == DEBUG || log == INFO)
//					printf("Inserting %d............\n", randomData);
//				//Insert
//				root = insertMinLeftistTree(root,  randomData);
//			}
//		}
//
//    return 0;
//}

/*************************************************************************************
 * Function: insertMinLeftistTree()
 * Description:	This function inserts a new node in Min Leftist Tree
 * Return: It returns a pointer to the root of new tree.
 * Input: root of tree & data of node to be inserted
 ************************************************************************************/
struct node *insertMinLeftistTree(struct node *root, int data){
	
    
	//Create a new node
    struct node *newNode=NULL;
    newNode = (struct node *)malloc(sizeof(struct node));

	if(log == DEBUG)
			printf("\nInitiating Insert of Min Leftist Tree....\n");

    //Initialize data members of newly created node.
    if(newNode != NULL){
		newNode->data = data;
		newNode->leftchild = NULL;
		newNode->rightchild = NULL;
		newNode->shortest = 1; //shortest value of a node is at least 1
		if(log == DEBUG)
			printf("insertMinLeftistTree()-Created new Node %d!\n", newNode->data);
	}

	else{ //If there is not sufficient memory
		if(log == DEBUG || INFO)
			printf("insertMinLeftistTree()-Sorry, Not sufficient memory to create new node!\n");
		return root;
	}

    if(root == NULL){     //If there does not exist any node.
        root = newNode;         //Then make the newly created node root.
		if(log == DEBUG)
			printf("insertMinLeftistTree()-Created first node %d.\n", root->data);
    }

    else{       //If there already exists more than one node
		if(log == DEBUG)
			printf("insertMinLeftistTree()-Melding %d & %d\n", root->data, newNode->data);
        root = meldMinLeftistTree(root, newNode); //Then meld the newly created node with the existed tree.
	}

	if(log == DEBUG || INFO)
			printf("Finishing Insertion for Min Leftist Tree...\n");

	return root;
}


/*************************************************************************************
 * Function: meldMinLeftistTree()
 * Description:	This functions melds two given Min Leftist Tree
 * Return: It returns the pointer to root of melded tree
 * Input: It takes pointer to roots of both the trees.
 ************************************************************************************/
struct node *meldMinLeftistTree(struct node *root1, struct node *root2){
	if(log == DEBUG)
			printf("\nmeldMinLeftistTree()-Melding %d & %d...\n", root1->data, root2->data);

    if(root1->data < root2->data){

		if(root1->rightchild == NULL){
			root1->rightchild = root2;   //If root1 do not have right child then simply make root2 a right child
			if(log == DEBUG)
				printf("meldMinLeftistTree()-Inserted %d as right child of%d\n", root2->data, root1->data);
		}
        else
			root1->rightchild = meldMinLeftistTree(root1->rightchild, root2); //If root1 has a right child, then meld it with root2 and make the resultant tree a right child of root1

        calSValue(root1);   //Update the s-value
		if(log == DEBUG)
			printf("meldMinLeftistTree()-Updated shortest value of %d to %d\n", root1->data, root1->shortest);

        root1 = ensureShortest(root1);  //Ensure s-value property
        calSValue(root1);   //Update the s-value. It might have changed after ensuring s-value property
		if(log == DEBUG)
			printf("meldMinLeftistTree()-Updated shortest value of %d to %d after ensuring\n", root1->data, root1->shortest);

        return root1;
    }

    else{
		if(root2->rightchild == NULL){
			root2->rightchild = root1;  //If root2 do not have right child then simply make root1 a right child
			if(log == DEBUG)
				printf("meldMinLeftistTree()-Inserted %d as right child of%d\n", root1->data, root2->data);
		}

        else
			root2->rightchild = meldMinLeftistTree(root1, root2->rightchild); //If root2 has a left child, then meld it with root1 and make the resultant tree a leftchild of root2

        //root2->shortest = (1 + (root2->leftchild->shortest)); //Update the s-value
        calSValue(root2);   //Update the s-value
		if(log == DEBUG)
			printf("meldMinLeftistTree()-Updated shortest value of %d to %d\n", root2->data, root2->shortest);

        root2 = ensureShortest(root2); //Ensure s-value property
        calSValue(root2);   //Update the s-value.It might have changed after ensuring s-value property
		if(log == DEBUG)
			printf("meldMinLeftistTree()-Updated shortest value of %d to %d after ensuring\n", root2->data, root2->shortest);

        return root2;
    }
}


//This is a dummy function to print the nodes of Leftist tree in pre-order
int printNodes(struct node *root){
	FILE *outputFile; //File to output the tree
	struct node *printThis=NULL, *levelInd=NULL;

	

	if(root == NULL){ //If tree is empty
		printf("Leftist Tree is empty!\n");
		return 0;
	}

	else{ //If tree is not empty
		outputFile = fopen("result_left.txt", "w"); // Open file
		if(outputFile != NULL){ //If file is created successfuly
		if(root->leftchild != NULL)
			levelInd = root->leftchild;
		else if(root->rightchild != NULL)
			levelInd = root->rightchild;

		if(leftistEnqueue(root)){ //enqueue the root
			while( !isLeftistQueueEmpty()){
				printThis = leftistDequeue();
					if(printThis == levelInd){
						//printf("\n"); //Next Level starts so print new line
						fprintf(outputFile, "%c", '\n'); //Printf new Line
						levelInd = NULL;	//Change the indicator
					}
					//printf("%d ", printThis->data); //Print data & space
					fprintf(outputFile, "%d ", printThis->data);

					//enqueue all children
					if(printThis->leftchild != NULL){
						if(levelInd == NULL)
							levelInd = printThis->leftchild;
						leftistEnqueue(printThis->leftchild);
						}

						if(printThis->rightchild != NULL){
							if(levelInd == NULL)
								levelInd = printThis->rightchild;
							leftistEnqueue(printThis->rightchild);
						}

					}
				}
		fprintf(outputFile, "%c", '\n');
		fclose(outputFile); //close the file
	}

		else //If file is not created successfuly
			printf("Can not create output file! Please try again...\n");
	}

	return 1;
}


/*************************************************************************************
 * Function: ensureShortest()
 * Description:	This functions ensures the shortest value property of Min Leftist Tree
 * Return: pointer to the node after ensuring its s-value
 * Input: Pointer to a node
 ************************************************************************************/
struct node *ensureShortest(struct node *theNode){
	

    struct node *saveChild=NULL;
    int leftChildShortest;

	if(log == DEBUG)
		printf("\nEnsuring Shortest Value of %d....\n", theNode->data);

    //if left child is NULL or if there is an external node
	if(theNode->leftchild == NULL){
        leftChildShortest = 0;
		if(log == DEBUG){
			printf("ensureShortest()-Left Child of % is an external node\n", theNode->data);
			printf("ensureShortest()-s-value of Left Child of % is %d\n", theNode->data, leftChildShortest);
		}
	}
    //if right child exists
    else{
		leftChildShortest = theNode->leftchild->shortest;
		if(log == DEBUG)
			printf("ensureShortest()-s-value of Left Child of %d is %d\n", theNode->data, leftChildShortest);
	}

    //Swap the children if s-value of right child is greater than the s-value of right child
	if(theNode->rightchild->shortest > leftChildShortest){
        saveChild = theNode->leftchild;
        theNode->leftchild = theNode->rightchild;
        theNode->rightchild = saveChild;
		if(log == DEBUG)
			printf("ensureShortest()- Swaped the left & right child of %d\n", theNode->data);

    }

	if(log == DEBUG)
		printf("Finshing Ensuring......\n");

    return theNode;
}


/*************************************************************************************
 * Function: deleteMin()
 * Description:	This functions delete the minimum element of Min Leftist Tree
 * Return: Pointer to the tree after deleting the min element
 * Input: Pointer to the root of the tree
 ************************************************************************************/
struct node *deleteMinLeftistTree(struct node *root){
	if(log == DEBUG || INFO)
		printf("\nInitiating Delete Min of Leftist Tree ......\n");

	if(root != NULL){ //Checking to print the DEBUG/INFO value
		if(log == DEBUG || INFO)
		printf("Deleting %d......\n", root->data);
	}

    if(root == NULL){ //If Leftist tree is empty
		if(log == DEBUG || INFO)
			printf("deleteMinLeftistTree()-Sorry, there is no node to be deleted!\n");
        root = root;
    }

	
	//If Leftist tree is not empty
    else if(root->leftchild != NULL && root->rightchild != NULL){
		if(log == DEBUG)
			printf("deleteMinLeftistTree()-Melding %d & %d!\n", root->leftchild->data, root->rightchild->data);
        root = meldMinLeftistTree(root->leftchild, root->rightchild);
	}

	else if(root->leftchild == NULL && root->rightchild != NULL){
		if(log == DEBUG)
			printf("deleteMinLeftistTree()-%d's leftchild is NULL and right child is %d\n", root->data, root->rightchild->data);
		root = root->rightchild;
		if(log == DEBUG)
			printf("deleteMinLeftistTree()-New root is %d\n", root->data);
	}

	else if(root->leftchild !=NULL && root->rightchild == NULL){
		if(log == DEBUG)
			printf("deleteMinLeftistTree()-%d's right child is NULL and left child is %d\n", root->data, root->leftchild->data);
        root = root->leftchild;
		if(log == DEBUG)
			printf("deleteMinLeftistTree()-New root is %d\n", root->data);
	}

	else{ //Right & Left Child are NULL
		root = NULL;
		if(log == DEBUG)
			printf("Both right & left child of Leftist Tree are null\n");
	}

	if(log == DEBUG)
			printf("Finishing Delete Min of Leftist Tree....\n");
	return root;
}

//This function calculates the s-value of a node.
//Input: pointer to node
//Return: N/A
void calSValue(struct node *p){
	if(p->rightchild == NULL)
        p->shortest = 1;
    else
		p->shortest = (1 + (p->rightchild->shortest));
}







/*************************************************************************************
 * Function:	leftistEnqueue()
 * Description:	This function enqueue a Leftist Tree node into queue
 * Return:	1 if element is inserted successfuly,  0 - if it is not able to insert
 * Input: Leftist Tree node
 ************************************************************************************/
int leftistEnqueue(struct node *ele){
	struct leftistQueue *p; //Create a new element
	p = (struct leftistQueue *)malloc(sizeof(struct leftistQueue)); //Alocate memory for new element

	//If there is sufficient memory, Assign initial values
	if(p != NULL){
		p->element = ele;
		p->next = NULL;

		//If queue is empty
		if(isLeftistQueueEmpty())
			LHEAD = LTAIL = p;
		else{ //If queue is not empty
			LTAIL->next = p;
			LTAIL = p;
		}
		//DEBUGGING
		//printf("%d", ele->data);
		return 1; //Element is inserted successfully
	}

	//If there isn't sufficient memory
	else{
		printf("Sorry, There isn't sufficient memory to put this node into queue. Please try again\n");
		return 0; //Element was not inserted
	}
}

/*************************************************************************************
 * Function:	leftistDequeue()
 * Description:	This function dequeue a Leftist Tree node from queue
 * Return: If queue is not empty then it returns the Leftist Tree Node else it returns NULL
 * Input: N/A
 ************************************************************************************/
struct node *leftistDequeue(){
	struct leftistQueue *temp;

	//if the queue is empty, print error message and return NULL
	if(isLeftistQueueEmpty()){
		printf("Sorry,  There is no element is queue!\n");
		return NULL;
	}
	else{
		temp = LHEAD;
		LHEAD = LHEAD->next;
		return temp->element;

	}
}


/*************************************************************************************
 * Function:	isLeftistQueueEmpty()
 * Description:	This function checks if the queue is empty or not
 * Return:	1 - if queue is empty,  0 - if queue is not empty
 * Input: N/A
 ************************************************************************************/
int isLeftistQueueEmpty(){
	if(LHEAD == NULL)
		return 1;
	else
		return 0;
}


struct node* initializeLeftistTree(struct node *root, int elements[], int noOfElements){
	int i;
	if(log == DEBUG)
		printf("\nInitializing Leftist Tree....\n");

	for(i=0;i<noOfElements; i++)
		root = insertMinLeftistTree(root, elements[i]);
	return root;
}