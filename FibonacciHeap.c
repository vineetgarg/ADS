/*********************************************************************************************
Filename: FibbonacciHeap
Description: It implements the Fibbonacci Heap data strucutre
Operations: Insert, Print, Meld, DeleteMin, Remove, DecreaseKey, Cascading, Pairwise Combining
Author: Garg,Vineet
Date: October 21, 2010
**********************************************************************************************/

#include "FibonacciHeap.h" //Including header file 
#include "common.h"

int CurHeapNodeCount = 0; //Current Heap Node Count
struct f_node *refArray[F_MAXNODE]; //Ref Array to hold the reference of each node
struct f_node* degreeTable[F_MAXNODE] = {0} ; //degreeTable is used to implement table during Pairwise Combine operation
struct q_element *HEAD=NULL, *TAIL=NULL;
extern int log;
extern int mode;




/*************************************************************************************
 * Function: insert_MinFHeap()
 * Description:	This function inserts a new element into Fibbonacci Heap
 * Return: Pointer to the MIN of Fibonacci Heap
 * Input: Pointer to the Min of Fibbonacci Heap & data of element to be inserted
 ************************************************************************************/
struct f_node *insert_MinFHeap(struct f_node *min, int data){
	struct f_node *newNode = NULL;
	
	if(log == DEBUG)
		printf("\nInitiating Insert...\n");
	
	

	if(CurHeapNodeCount == F_MAXNODE){
		if(log == DEBUG )
			printf("insert_MinFHeap()-MAXIMUM LIMIT REACHED! This Fibonacci Heap can hold maximum number of %d nodes and there are already %d nodes in it",  F_MAXNODE, CurHeapNodeCount);
	}

	else{ //If Fibonacci can accomdate more nodes
   
		newNode = (struct f_node *)malloc(sizeof(struct f_node)); //Create new node

		// Assign Values
		if(newNode != NULL){
			newNode->data = data;
			newNode->degree = 0;
			newNode->childCut = -1; //undefined
			newNode->leftSibling = newNode->rightSibling = newNode; //Point left & right Sibling to node itself
			newNode->parent = NULL;
			newNode->child = NULL;
			
			
			//Save address of this node in refArray. Only in User Input Mode
			if(mode == USERINPUT)
				refArray[CurHeapNodeCount++] = newNode;
			
			if(log == DEBUG)
				printf("insert_MinFHeap()-Stored newly created node %d in refArray at %d\n", newNode->data, (CurHeapNodeCount-1));

		}

		//Not sufficient memory
		else{
			if(log == DEBUG )
				printf("insert_MinFHeap()-Sorry, Not sufficient memory to create new node!\n");
			return min;
		}

		//If it is the first element
		if(min == NULL){
			min=newNode;
			if(log == DEBUG )
				printf("insert_MinFHeap()-Created first element %d of Fibonacci Heap\n", min->data);
		}
			
	
		//If it is not the first element
		else{
			if(log == DEBUG)
				printf("insert_MinFHeap()-Melding Fibonacci Heap %d & new node %d\n", min->data, newNode->data);
			min = meld_MinFHeap(min, newNode); //Meld both min tree
		}
						
	}

	if(log == DEBUG )
		printf("Finishing Inserting...........\n");
	return min;
}


/*************************************************************************************
 * Function: print_MinFHeap()
 * Description:	This function prints the Fibbonacci Heap in Level Order
 * Return: 1 if successful, 0 if unsuccessful
 * Input: Pointer to the root of Fibbonacci Heap
 ************************************************************************************/
int print_MinFHeap(struct f_node *min){
	struct f_node *level_ind = NULL; //It indicates new level
	//printThis stores the node to be printed, child stores the children, nextChildInd is for the indicator of next Child, newTreeInd is to indicate new Tree
	struct f_node *printThis=NULL, *child=NULL,  *nextChildInd=NULL, *newTreeInd=NULL;
	FILE *outputFileFHeap;
	
	if(log == DEBUG  )
		printf("\nInitiating Print of Fibonacci Heap...\n");

	if(min == NULL){ //If Heap is empty
		if(log == DEBUG  )
			printf("print_MinFHeap()-Sorry, There is not node to print!\n");
		return 0;
	}


	else{ //If Heap has element
		outputFileFHeap = fopen("result_bino.txt", "w"); //Create a new file
		if(outputFileFHeap != NULL){
		newTreeInd = min;
		do{
			level_ind = min->child;
			//printf("####\n"); //New Tree starts
			fprintf(outputFileFHeap, "\n###\n"); //Print New Tree

			if(q_enqueue(min)){
				while( !is_q_empty()){
					printThis = q_dequeue();
					if(printThis == level_ind){
						//printf("\n"); //Next Level starts so print new line
						fprintf(outputFileFHeap, "%c", '\n');
						level_ind = NULL;	//Change the indicator
					}
					//printf("%d ", printThis->data); //Print data & space
					fprintf(outputFileFHeap, "%d ", printThis->data);

					//enqueue all children
					child = printThis->child;
					if(child != NULL){
                        nextChildInd = child;
                        if(level_ind == NULL) //If this is the first node of next level
                            level_ind = child;

                        do{
                            if(q_enqueue(child))
                                child = child->rightSibling;
                            else
                                return 0; //Enqueue unsuccessful
                        } while(child != nextChildInd);

					}


				}
			}

			else{ //If enqueue was unsuccessful
				if(log == DEBUG)
					printf("print_MinFHeap()-Enqueue was unsuccessful for %d\n", min->data); 
				return 0;
			}
			min = min->rightSibling; //The next tree
			printf("\n");


		}while(min != newTreeInd);

		if(log == DEBUG)
			printf("Finishing Printing.....\n");
		fprintf(outputFileFHeap, "%c", '\n'); //Printf new line in the end
		fclose(outputFileFHeap);

		return 1; //Nodes printed successfully
	} //Close- if file was created

		else
			printf("Can not create file!! Please try again\n");
	}

}

/*************************************************************************************
 * Function: meld_MinFHeap()
 * Description:	This function melds two Fibbonacci Min Heap
 * Return: Pointer MIN to the melded tree
 * Input: Pointer to Two Fibbonacci Min Heaps
 ************************************************************************************/
struct f_node *meld_MinFHeap(struct f_node *min1, struct f_node *min2){
	struct f_node *min1RightSibling=NULL, *min2RightSibling=NULL; //To hold Min's rightSibling
	
	if(log == DEBUG  )
		printf("\nInitiating Melding....\n");
	
	if(min1 == NULL || min2 == NULL){
		if(log == DEBUG){
			if(min1 == NULL)
				printf("meld_MinFHeap()-min1 is NULL!\n");
			else
				printf("meld_MinFHeap()-min2 is NULL!\n");
		}
	}
	else {
		if(log == DEBUG )
			printf("meld_MinFHeap()-Melding %d & %d\n", min1->data, min2->data);
		
		min1RightSibling = min1->rightSibling;
		min2RightSibling = min2->rightSibling;

		min1->rightSibling = min2RightSibling;
		min2RightSibling->leftSibling = min1;

		min2->rightSibling = min1RightSibling;
		min1RightSibling->leftSibling = min2;

		//If the data of new node is less than MIN then it will be the new min
		if(min1->data > min2->data){
			min1 = min2;
			
			/*******************************************************************************************************************************/
			if(log == DEBUG)
				printf("meld_MinFHeap()-MIN pointer changed to min2 %d\n", min2->data);
			/*******************************************************************************************************************************/
		}
	}

	/*******************************************************************************************************************************/
	if(log == DEBUG  )
		printf("Finishing Melding....\n");
	/*******************************************************************************************************************************/

	return min1;
}


/*************************************************************************************
 * Function: deleteMin_MinFHeap()
 * Description:	This function deletes the minimum mode from Fibbonacci Heap
 * Return: Min pointer to Fibonacci heap after deleting its MIN node
 * Input: Pointer to the root of Fibbonacci Heap
 ************************************************************************************/
struct f_node *deleteMin_MinFHeap(struct f_node *min){
	struct f_node *min_temp=NULL, *saveMin=NULL;
	int minInRefArray = 0;

	if(log == DEBUG  )
		printf("\nInitiating deleteMin.....\n");

	saveMin = min; //saving MIN to be deleted later from refArray

	//If there is no node
	if(min == NULL){
		if(log == DEBUG  )
			printf("deleteMin_MinFHeap()-Sorry, There is no node to be deleted\n");
	}

	//If there is only one tree
	else if(min->rightSibling == min){
		if(log == DEBUG)
			printf("deleteMin_MinFHeap()-There is only one tree in Heap\n");
		min = min->child;
		
		if(min != NULL){
			if(log == DEBUG)
				printf("deleteMin_MinFHeap()-Pairwise combining MIN %d\n", min->data);
            min = pairwiseCombine_MinFHeap(min, NULL);  
		}
	}

	//If there are more than one tree
	else{
		if(log == DEBUG)
			printf("deleteMin_MinFHeap()-There are more than one tree. Deleting %d\n", min->data);

		//Pull out the subtree containing MIN
		min->leftSibling->rightSibling = min->rightSibling;
		min->rightSibling->leftSibling = min->leftSibling;

		min_temp = min->rightSibling; //Keeping min_temp pointer to Heap after pulling out MIN
		if(log == DEBUG)
			printf("deleteMin_MinFHeap()-Pulled out subtree at MIN %d which created another subtree %d\n", min->data, min_temp->data);
		
		//Discard minimum node
		min = min->child;
		if(log == DEBUG){
			if(min != NULL)
				printf("deleteMin_MinFHeap()-Discarded the node & left with subtree %d\n", min->data);
			else
				printf("deleteMin_MinFHeap()-Discarded the node & left with NULL subtree");
		}
		//Pairwise Combine
		min = pairwiseCombine_MinFHeap(min, min_temp);
	}

	if(mode == USERINPUT){ //Only in User Input mode
		if(saveMin != NULL){
			if(log == DEBUG)
				printf("deleteMin_MinFHeap()-Finding where MIN element %d is stored in refArray\n", saveMin->data);
			
			//Find where saveMin is stored in refArray
			while(refArray[minInRefArray++] != saveMin);

			if(log == DEBUG){
				printf("deleteMin_MinFHeap()-MIN element %d is stored in refArray at %d\n", saveMin->data, minInRefArray-1);
				printf("deleteMin_MinFHeap()-Copying last element %d of refArray to %d position\n", refArray[CurHeapNodeCount-1]->data, (minInRefArray-1));
			}
			
			//Copy the last element of refArray to deleted position
			refArray[--minInRefArray] = refArray[CurHeapNodeCount-1];
			refArray[CurHeapNodeCount-1] = NULL;
			CurHeapNodeCount--; //Decrease the current Heap Node Count

			if(log == DEBUG)
				printf("deleteMin_MinFHeap()-There are %d element left in Fibonacci Heap\n", CurHeapNodeCount);
		}
	}
	if(log == DEBUG  )
		printf("Finishing deleteMin.....\n");

	return min;
}


/*************************************************************************************
 * Function: pairwiseCombine_MinFHeap()
 * Description:	This function perform pairwise combine operation on two Fibbonacci Heap
 * Return:	pointer to MIN after performing pairwise combine
 * Input: Two Fibonacci Heaps
 ************************************************************************************/
struct f_node *pairwiseCombine_MinFHeap(struct f_node *heap1, struct f_node *heap2){
	struct f_node *heap1Ind = NULL,  *heap2Ind=NULL; //Indicators
	struct f_node *Cparent=NULL, *Cchild=NULL, *Cparentchild1=NULL, *Cparentchild2=NULL, *saveMinNext=NULL ; //pointers to hold various nodes while combining two trees
	struct f_node *new_min=NULL; //To hold new MIN
	int tableIndex, moveForward=1;
	
	if(log == DEBUG  )
		printf("\nInitiating Pairwise Combing..............\n");

	//Pairwise combine trees of heap1
		heap1Ind = heap1;
		if(heap1 != NULL){
			if(log == DEBUG)
				printf("pairwiseCombine_MinFHeap()-Pairwise Combining %d\n", heap1->data);

			do{
			    if(log == DEBUG)
					printf("pairwiseCombine_MinFHeap()-Processing %d\n", heap1->data);

			    if(moveForward == 1)
                    saveMinNext = heap1->rightSibling; //Save heap1's next because it will be changed
				
				//If there does not exist any tree equal to heap1's degree
				if(degreeTable[heap1->degree] == NULL){
				    degreeTable[heap1->degree] = heap1;
					degreeTable[heap1->degree]->rightSibling = degreeTable[heap1->degree]->leftSibling = degreeTable[heap1->degree]; //Cutting off this from main queue.
					
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-No tree of degree equal to node %d's degree %d exists\n", heap1->data, heap1->degree);
					
					heap1 = saveMinNext;
					moveForward = 1;
				}

				else{ //If there is a tree with degree equal to heap1's degree
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-A tree with degree equal to node %d's degree %d exists\n", heap1->data, heap1->degree);
				    
					//if heap1 is smaller than heap1 will be parent
				    if(heap1->data < degreeTable[heap1->degree]->data){
                        Cparent = heap1;
                        Cchild = degreeTable[heap1->degree];						
				    }
                    else { //else degreeTable's will be parent
                        Cparent = degreeTable[heap1->degree];
                        Cchild = heap1;
                    }

					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Node %d will be parent & Node %d will be child\n", Cparent->data, Cchild->degree);

					//If parent do not have any child, then point its child pointer to child
					if(Cparent->child == NULL){
                        Cparent->child = Cchild;
                         Cchild->leftSibling = Cchild->rightSibling = Cchild; //This is the only child, so left & right Sibling will point to itself
                    
						if(log == DEBUG)
							printf("pairwiseCombine_MinFHeap()-Made Node %d child of Node %d by changing child pointer\n", Cchild->data, Cparent->degree);
					}

					else{ //If parent have children
						//Insert the tree in parent's children queue
						Cparentchild1 = Cparent->child;     //Cparentchild1 points to parent's child
						Cparentchild2 = Cparent->child->rightSibling;   //Cparentchild2 points to child's right sibling

						Cparentchild1->rightSibling = Cchild;
						Cchild->leftSibling = Cparentchild1;

						Cchild->rightSibling = Cparentchild2;
						Cparentchild2->leftSibling = Cchild;

						if(log == DEBUG)
							printf("pairwiseCombine_MinFHeap()-Node %d already have children. Putting node %d in its children queue\n", Cparent->degree, Cchild->data);
					}

					
					Cchild->parent = Cparent; //Set the parent pointer of inserted child to parent
					Cchild->childCut = 0; //Set the childCut of newly combined tree to false
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Changed the parent pointer of %d to point to %d\n", Cchild->data, Cparent->degree);
					
					degreeTable[Cparent->degree] = NULL; //Delete the tree from table
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Changed the %d position of degreeTable to NULL\n", Cparent->degree);

					(Cparent->degree)++; //Increase the degree of min by one
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Changed the degree of %d to %d\n", Cparent->data, Cparent->degree);
                    
					Cparent->leftSibling = Cparent->rightSibling = Cparent;

					
					moveForward = 0; // Do not move forward in main queue
                    heap1 = Cparent;

				}
			}while(heap1 != heap1Ind || moveForward == 0); //Keep checking all the trees in min heap
		}

		//Pairwise combine trees of min_temp heap
		heap2Ind = heap2;
		
		if(heap2 != NULL){
			if(log == DEBUG)
				printf("pairwiseCombine_MinFHeap()-Pairwise Combining %d\n", heap2->data);

			do{
				if(log == DEBUG)
					printf("pairwiseCombine_MinFHeap()-Processing %d\n", heap2->data);

			    if(moveForward == 1)
                    saveMinNext = heap2->rightSibling; //Save min_temp's next because it will be changed
				
				//If there does not exists any tree equal to min_temp's degree
				if(degreeTable[heap2->degree] == NULL){
					degreeTable[heap2->degree] = heap2;
					degreeTable[heap2->degree]->rightSibling = degreeTable[heap2->degree]->leftSibling = degreeTable[heap2->degree]; //Cutting off this from main queue.
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-No tree of degree equal to node %d's degree %d exists\n", heap2->data, heap2->degree);
					
					heap2 = saveMinNext;
					moveForward = 1;
				}

                else{ //If there is a tree with degree equal to min_temp's degree
				    if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-A tree with degree equal to node %d's degree %d exists\n", heap2->data, heap2->degree);
					
					
					//if min is larger than mini will be parent
				    if(heap2->data < degreeTable[heap2->degree]->data){
                        Cparent = heap2;
                        Cchild = degreeTable[heap2->degree];
				    }
                    else { //else degreeTable's will be parent
                        Cparent = degreeTable[heap2->degree];
                        Cchild = heap2;

                    }
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Node %d will be parent & Node %d will be child\n", Cparent->data, Cchild->degree);

					//If parent do not have any child, then point its child pointer to child
					if(Cparent->child == NULL){
                        Cparent->child = Cchild;
                        Cchild->leftSibling = Cchild->rightSibling = Cchild; //This is the only child, so left & right Sibling will point to itself
						
						if(log == DEBUG)
							printf("pairwiseCombine_MinFHeap()-Made Node %d child of Node %d by changing child pointer\n", Cchild->data, Cparent->degree);
					}

					else{ //If parent have children
						//Insert the tree in parent's children queue
						Cparentchild1 = Cparent->child;     //Cparentchild1 points to parent's child
						Cparentchild2 = Cparent->child->rightSibling;   //Cparentchild2 points to child's right sibling

						Cparentchild1->rightSibling = Cchild;
						Cchild->leftSibling = Cparentchild1;

						Cchild->rightSibling = Cparentchild2;
						Cparentchild2->leftSibling = Cchild;

						if(log == DEBUG)
							printf("pairwiseCombine_MinFHeap()-Node %d already have children. Putting node %d in its children queue\n", Cparent->degree, Cchild->data);
					}
					
					Cchild->parent = Cparent; //Set the parent pointer of inserted child to parent
					Cchild->childCut = 0; //Set the childCut of newly combined tree to 0
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Changed the parent pointer of %d to point to %d\n", Cchild->data, Cparent->degree);
					
					degreeTable[Cparent->degree] = NULL; //Delete the tree from table
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Changed the %d position of degreeTable to NULL\n", Cparent->degree);
					
					(Cparent->degree)++; //Increase the degree of min by one
					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Changed the degree of %d to %d\n", Cparent->data, Cparent->degree);

                    //Pull back this from queue
                    Cparent->leftSibling = Cparent->rightSibling = Cparent;

                    moveForward = 0; // Do not move forward in main queue
                    heap2 = Cparent;				
				}			
			}while(heap2 != heap2Ind || moveForward == 0 ); //Keep checking all the trees in min_temp heap
		}

		//Collect remaining trees and find min
		if(log == DEBUG)
				printf("pairwiseCombine_MinFHeap()-Collecting trees from table\n");
		for(tableIndex=0; tableIndex<=F_MAXNODE; tableIndex++){
		    if(degreeTable[tableIndex] != NULL){
				if(log == DEBUG)
					printf("pairwiseCombine_MinFHeap()-Collecting %d\n", degreeTable[tableIndex]->data);

				if(new_min == NULL){
					new_min = degreeTable[tableIndex];
					new_min->leftSibling = new_min->rightSibling = new_min; //This is the only tree in queue so right & left sibling will point to itself

					if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-New Min is Null. Making %d New Min\n", degreeTable[tableIndex]->data);
                }

				else{
				    if(log == DEBUG)
						printf("pairwiseCombine_MinFHeap()-Melding %d & %d\n", new_min->data,degreeTable[tableIndex]->data);

					new_min = meld_MinFHeap(new_min, degreeTable[tableIndex]);				
				}


			}
			degreeTable[tableIndex] = NULL; //reseting enteries to NULL
		}

		if(log == DEBUG  )
			printf("Finishing Pairwise Combine....\n");
		return new_min;
	}



/*************************************************************************************
 * Function: removeMinFHeap()
 * Description:	This function removes any node from Min Fibbonacci Heap
 * Return:	Min pointer to Min Fibbonacci Heap after removing the node
 * Input: Min pointer to Fibonacci Heap, pointer to the node to be deleted, index of refArray
 * Assumption - theNode is not NULL
 ************************************************************************************/
	struct f_node *removeMinFHeap(struct f_node *min, int index){
		struct f_node *saveParent=NULL, *theNode=NULL;

		if(log == DEBUG  )
			printf("\nInitiating removeMin operation......\n");

		if(min == NULL){ //If Fibonacci Heap is empty
			if(log == DEBUG  )
				printf("removeMinFHeap() - Fibonacci Heap is Empty!\n");
		}

		else if (index >= F_MAXNODE || refArray[index] == NULL){ //If index is out of bound or node does not exits
			if(log == DEBUG  )
				printf("removeMinFHeap() - This nodes does not exists in Heap!\n");
		}

		
		else{ //If node exists
			theNode = refArray[index];
				
			//If theNode is MIN then use deleteMin operation		
			if(theNode == min){

				/*******************************************************************************************************************************/
				if(log == DEBUG)
					printf("This node %d is the MIN %d, so using deleteMin to remove it\n", theNode->data, min->data);
				/*******************************************************************************************************************************/
			
			min = deleteMin_MinFHeap(min);
			}
		
			else{
			
				/*******************************************************************************************************************************/
				if(log == DEBUG  )
					printf("\nRemoving %d from Min Fibonaaci Heap %d...\n", theNode->data, min->data);
				/*******************************************************************************************************************************/
				//Pull the subtree at theNode
			
				//If subtree is the child of its parent
				if(theNode->parent != NULL){ //If it is not root
					if(theNode->parent->child == theNode){
				
						/*******************************************************************************************************************************/
						if(log == DEBUG)
							printf("%d is the child of its parent %d\n", theNode->data, theNode->parent->data);
						/*******************************************************************************************************************************/

						if(theNode->rightSibling == theNode) //If its the only child
							theNode->parent->child = NULL;
						else	 //Make next node the child of subtree's parent's child
							theNode->parent->child = theNode->rightSibling;
					}

					//Decreasing degree of node's parent be one
					/*******************************************************************************************************************************/
					if(log == DEBUG)
						printf("Decreasing degree of node %d parent from %d to %d\n", theNode->data, theNode->parent->degree, (theNode->parent->degree)-1);
					/*******************************************************************************************************************************/
					theNode->parent->degree--;
				}
				//Pulling out the subtree from its list
				/*******************************************************************************************************************************/
				if(log == DEBUG)
					printf("Pulling out the subtree located at node %d by equating left & right sibling of %d & %d\n", theNode->data, theNode->leftSibling->data, theNode->rightSibling->data);
				/*******************************************************************************************************************************/
				theNode->leftSibling->rightSibling = theNode->rightSibling;
				theNode->rightSibling->leftSibling = theNode->leftSibling;

				//Save its parent
				saveParent = theNode->parent;
				//Discard the node
				theNode = theNode->child;

				if(theNode != NULL){ //If it had child
					theNode->childCut = -1; //childCut will be zero because it will be root node
					theNode->parent = NULL; //Parent will be NULL
					min = meld_MinFHeap(min, theNode); //Meld the node with min
				}

				//Cascade
				if(saveParent != NULL) //if it is not root
					min = cascading(min, saveParent);
			}

			if(mode == USERINPUT){
				//Copy the last element of refArray to deleted position
				refArray[index] = refArray[CurHeapNodeCount-1];
				refArray[CurHeapNodeCount-1] = NULL;
				CurHeapNodeCount--; //Decrease the current Heap Node Count

				/*******************************************************************************************************************************/
				if(log == DEBUG){
					//printf("removeMinFHeap()-Copied the last element and the element at %d position is %d\n", index, ((refArray[index])->data));
					printf("removeMinFHeap()-There are %d element left in Fibonacci Heap\n", CurHeapNodeCount);
				}
				/*******************************************************************************************************************************/
			}

			/*******************************************************************************************************************************/
			if(log == DEBUG  )
				printf("Finishing remove.....\n");
			/*******************************************************************************************************************************/
		}
		return min;
	}



/*************************************************************************************
 * Function: cascading()
 * Description:	This function implements Cascading for a Min Fibbonacci Heap
 * Return:	Min pointer to cascaded Min Fibbonacci Heap
 * Input: Min pointer to Fibonacci Heap, pointer to subtree
 ************************************************************************************/
	struct f_node *cascading(struct f_node *min, struct f_node *subtree){
		struct f_node *tempParent; //to save subtree's parent

		/*******************************************************************************************************************************/
		if(log == DEBUG  )
			printf("\nInitiating Cascading of Min %d at subtree %d...\n", min->data, subtree->data);
		/*******************************************************************************************************************************/

		if(min == NULL || subtree == NULL){
			/*******************************************************************************************************************************/
			if(log == DEBUG  ){
				if(min == NULL)
					printf("Fibonacci Heap is NULL!\n");
				if(subtree == NULL)
					printf("Subtree is NULL!\n");
			}
			/*******************************************************************************************************************************/
		}

		else{
			while(subtree->childCut == 1){ // Until we reach at root(childCut value is -1) or node whose childCut value is false (0)
				//Pull the subtree
				//If subtree is the child of its parent
				if(subtree->parent->child == subtree){
					if(log == DEBUG)
						printf("%d is the only child of its parent %d\n", subtree->data, subtree->parent->data);
					if(subtree->rightSibling == subtree) //If its the only child
						subtree->parent->child = NULL;
					else	 //Make next node the child of subtree's parent's child
						subtree->parent->child = subtree->rightSibling;
				}

				//Decreasing degree of subtree's parent be one
				/*******************************************************************************************************************************/
				if(log == DEBUG)
					printf("Decreasing degree of subtree's parent from %d to %d\n", subtree->parent->degree, (subtree->parent->degree)-1);
				/*******************************************************************************************************************************/
				subtree->parent->degree--;
				
				//Pulling out the subtree from its list
				subtree->leftSibling->rightSibling = subtree->rightSibling;
				subtree->rightSibling->leftSibling = subtree->leftSibling;

				//Change the childCut, parent, leftSibling & rightSibling of subtree
			
				/*******************************************************************************************************************************/
				if(log == DEBUG){
					printf("Changing childcut %d of node %d to -1\n", subtree->childCut, subtree->data);
					printf("Changing Right Sibling & Left Sibling of node %d to point to itself i.e. to %d\n", subtree->data, subtree->data);
					printf("Parent pointer of node %d is pointing to node %d.Changing it to NULL\n", subtree->data, subtree->parent->data);
				}
				/*******************************************************************************************************************************/
				subtree->childCut = -1; //Because subtree will be root
				subtree->rightSibling = subtree->leftSibling = subtree;
				tempParent = subtree->parent; //save the subtree's parent
				subtree->parent = NULL;

				/*******************************************************************************************************************************/
				if(log == DEBUG)
					printf("Melding Min %d and subtree %d\n", min->data, subtree->data );
				/*******************************************************************************************************************************/
				//Meld subtree with min
				min = meld_MinFHeap(min, subtree); 
			
				//Go to next parent
				subtree = tempParent;
	
			}

			//If the parent is not root then change its childCut value to true becaue we have just pulled out its subtree
			if(subtree->childCut == 0)
				subtree->childCut = 1;

			if(log == DEBUG  )
				printf("Cascading finishing...\n");
		}
		return min;
	}


/*************************************************************************************
 * Function: decreaseKey()
 * Description:	This function decreases the data of a node by value key in Min Fibbonacci Heap
 * Return:	It returns a min pointer to the heap after decreasing the value
 * Input: Min pointer, Pointer to node, Key
 * Assumption: theNode is not NULL & key is a positive value
 ************************************************************************************/
	struct f_node *decreaseKey(struct f_node *min, int index, int key){
		struct f_node *saveParent=NULL, *theNode=NULL;
		
		if(log == DEBUG  )
			printf("\nInitiating decreaseKey operation......\n");

		if(min == NULL){ //If Fibonacci Heap is empty
			if(log == DEBUG  )
				printf("decreaseKey() - Fibonacci Heap is Empty!\n");
		}

		else if (index >= F_MAXNODE || refArray[index] == NULL){ //If index is out of bound or node does not exists
			if(log == DEBUG  )
				printf("decreaseKey() - This nodes does not exists in Heap!\n");
		}
		
		else{ //If node exists
			
			theNode = refArray[index];
			
			if(log == DEBUG  )
				printf("Decreasing Key of %d......\n", theNode->data);

			theNode->data = (theNode->data - key); // Decreasing the data by value key
			/*******************************************************************************************************************************/
			if(log == DEBUG)
				printf("decreasekey() - Decreased key of the node by %d to %d\n", key, theNode->data);
			/*******************************************************************************************************************************/
			
			
			if(theNode->parent != NULL){
				if(theNode->parent->data > theNode->data){
					/*******************************************************************************************************************************/
					if(log == DEBUG)
						printf("decreasekey() - Decreased key of the node %d is smaller than its parent's key %d\n", theNode->data, theNode->parent->data);
					/*******************************************************************************************************************************/
		
					//Pull out the subtree at theNode
					//If subtree is the child of its parent
					if(theNode->parent->child == theNode){
						/*******************************************************************************************************************************/
						if(log == DEBUG)
							printf("decreasekey() - %d is the child of its parent %d\n", theNode->data, theNode->parent->data);
						/*******************************************************************************************************************************/

						if(theNode->rightSibling == theNode) //If its the only child
							theNode->parent->child = NULL;
						else	 //Make next node the child of subtree's parent's child
							theNode->parent->child = theNode->rightSibling;
					}

					//Decreasing degree of node's parent be one
					theNode->parent->degree--;
					/*******************************************************************************************************************************/
					if(log == DEBUG)
						printf("decreasekey() - Decreased degree of node %d's parent %d by one and it is now %d\n", theNode->data, theNode->parent->data, theNode->parent->degree);
					/*******************************************************************************************************************************/
										
					
					//Pulling out the subtree from its list
					theNode->leftSibling->rightSibling = theNode->rightSibling;
					theNode->rightSibling->leftSibling = theNode->leftSibling;

					/*******************************************************************************************************************************/
					if(log == DEBUG)
						printf("decreasekey() - Pulled out the subtree located at node %d by equating left & right sibling of %d & %d\n", theNode->data, theNode->leftSibling->data, theNode->rightSibling->data);
					/*******************************************************************************************************************************/

					//Save its parent
					saveParent = theNode->parent;
			
					theNode->childCut = -1; //childCut will be zero because it will be root node
					theNode->parent = NULL; //Parent will be NULL
					theNode->rightSibling  = theNode->leftSibling = theNode; // Right & Left sibling of the node will point to itself
					min = meld_MinFHeap(min, theNode); //Meld the node with min
			
					//Cascade
					min = cascading(min, saveParent);
				}
			}

			else if(theNode->data < min->data){
				min = theNode;
				
				/*******************************************************************************************************************************/
				if(log == DEBUG)
					printf("decreasekey() - Node %d is smaller than Min, so MIN is changed to this node\n", theNode->data);
				/*******************************************************************************************************************************/
			}

			

		}
		
		/*******************************************************************************************************************************/
		if(log == DEBUG)
			printf("Finishing Decrease Key....\n");
		/*******************************************************************************************************************************/

		return min;
	}



/*************************************************************************************
 * Function:	q_enqueue()
 * Description:	This function enqueue a Fibbonacci Heap node into queue
 * Return:	1 if element is inserted successfuly,  0 - if it is not able to insert
 * Input: Fibbonacci Heap node
 ************************************************************************************/
int q_enqueue(struct f_node *ele){
	struct q_element *p; //Create a new element
	p = (struct q_element *)malloc(sizeof(struct q_element)); //Alocate memory for new element

	//If there is sufficient memory, Assign initial values
	if(p != NULL){
		p->element = ele;
		p->next = NULL;

		//If queue is empty
		if(is_q_empty())
			HEAD = TAIL = p;
		else{ //If queue is not empty
			TAIL->next = p;
			TAIL = p;
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
 * Function:	q_dequeue()
 * Description:	This function dequeue a Fibbonacci Heap node from queue
 * Return: If queue is not empty then it returns the Fibbonacci Node else it returns NULL
 * Input: N/A
 ************************************************************************************/
struct f_node *q_dequeue(){
	struct q_element *temp;

	//if the queue is empty, print error message and return NULL
	if(is_q_empty()){
		printf("Sorry,  There is no element is queue!\n");
		return NULL;
	}
	else{
		temp = HEAD;
		HEAD = HEAD->next;
		return temp->element;

	}
}


/*************************************************************************************
 * Function:	q_empty()
 * Description:	This function checks if the queue is empty or not
 * Return:	1 - if queue is empty,  0 - if queue is not empty
 * Input: N/A
 ************************************************************************************/
int is_q_empty(){
	if(HEAD == NULL)
		return 1;
	else
		return 0;
}


struct f_node *intializeFMinHeap(struct f_node *min, int elements[], int numberofElements){
	int i;
	if(log == DEBUG)
		printf("\nInitializing Fibonacci Heap....\n");

	for(i=0;i<numberofElements; i++)
		min = insert_MinFHeap(min, elements[i]);
	return min;
}
