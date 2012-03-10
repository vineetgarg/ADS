#include <time.h>
#include"common.h" //Inlcuding Common Header file
#include "FibonacciHeap.h" //Including Fibonacci Header file
#include "LeftistTree.h" //Including Leftist Tree Header file

int log = INFO; //Log level is DEBUG, Change it to INFO before submitting
int mode = USERINPUT; //Default Input Mode is USERINPUT

struct f_node *MIN = NULL; //MIN of Fibonacci Heap
struct node *ROOT = NULL; //ROOT of Leftist Tree

/*************************************************************************************
 * Function:	main()
 * Description:	This is the entry point of Program
 * Return: 0-Success, 1-Unsucessful
 * Input: N/A
 ************************************************************************************/
int main(int argc, char *argv[])
{
	char *inputFilePath = NULL; //User will input the path

	if(argc > 3 || argc < 2){ //Number of arguments are invalid
		printf("Invalid number of arguments! Please try again\n");
	}
	else if(argc == 2){ // Randome Mode
		if(strcmp(argv[1], "-r") == 0){
			mode = RANDOM;
			randomMode();
			//Run in radom mode
			
		}
		else{
			printf("Invalid argument! Please try again\n");
			
		}
	}

	else if(argc == 3){ //User Input Mode
		inputFilePath = argv[2]; //Get the filepath
		if(strcmp(argv[1], "-il") == 0 || strcmp(argv[1], "-if") == 0)
			//User Input Mode
			userInputMode(argv[1], inputFilePath);
		else
			printf("Invalid argument! Please try again\n");
	
	}


		
	//printNodes(ROOT);
    return 0;
}

/*************************************************************************************
 * Function: userInputMode()
 * Description:	This is the function to handle User Input Mode
 * Return: 0-Success, 1-Unsucessful
 * Input: scheme - Fibonacci Heap or Leftist Tree, filePath - path of input file
 ************************************************************************************/
void userInputMode(char *scheme, char *filePath){	
	//struct f_node *f_root=NULL; //Root of Fibbonacci Heap pointer to Min tree
	//int operation, randomData; //To be used in Random Operation Generator
	FILE *inputFile; // User input File
	
	                                         	
	char operationUserInput[3] = {0}; //Operation in user input mode
	int userInputData, index, decreaseKeyBy; //Data in user input mode
	int fibonacciScheme, leftistScheme;

	inputFile = fopen(filePath, "r"); //Open input file in read mode

	if(strcmp(scheme, "-il") == 0){ //If scheme is Leftist Tree
		fibonacciScheme = 0;
		leftistScheme = 1;
	}
	else{ //If scheme is Fibonacci Heap
		fibonacciScheme = 1;
		leftistScheme = 0;
	}

		
		if(inputFile != NULL){ //If file is opened successfully
			if(log == DEBUG)
					printf("\nInitiating User mode Input...\n");
			
			while(strcmp(operationUserInput, "*") != 0){
				fscanf(inputFile, "%s", operationUserInput); //Scan first word
				
				//Delete Min Operation
				if(strcmp(operationUserInput, "D") == 0){
					if(fibonacciScheme){ //If it is fibonacci Scheme
						if(log == DEBUG)
							printf("\nDeleting Min of Fibonacci Heap....\n");
						MIN = deleteMin_MinFHeap(MIN);
					}
					else{ //If it is Leftist Tree
						if(log == DEBUG)
							printf("\nDeleting Min of Leftist Tree....\n");
						ROOT = deleteMinLeftistTree(ROOT);
					}
					
					
				}

				//Insert Operation
				else if(strcmp(operationUserInput, "I") == 0){
					fscanf(inputFile, "%d", &userInputData); //Read data

					if(fibonacciScheme){ //If it is fibonacci scheme
						if(log == DEBUG)
							printf("Inserting %d in Fibonacci Heap....\n", userInputData);
						MIN = insert_MinFHeap(MIN, userInputData);
					}
					else{ //If it is Leftist Tree
						if(log == DEBUG)
							printf("\nInserting %d in Leftist Tree....\n", userInputData);
						ROOT = insertMinLeftistTree(ROOT, userInputData);
					}
				}

				//Decrease Key
				else if(strcmp(operationUserInput, "DK") == 0){
					fscanf(inputFile, "%d", &index); //Read index
					fscanf(inputFile, "%d", &decreaseKeyBy); //Read decreasekeyby
					if(fibonacciScheme){
						if(log == DEBUG)
							printf("\nDecreasing Key of node at %d by %d....\n", index, decreaseKeyBy);
						MIN = decreaseKey(MIN, index, decreaseKeyBy);
					}
				}
				
				//Remove mode from Fibonacci Heap
				else if(strcmp(operationUserInput, "R") == 0){
					fscanf(inputFile, "%d", &index); //Read index

					if(log == DEBUG )
						printf("\nRemoving Key of node at %d....\n", index);
					MIN = removeMinFHeap(MIN, index);
				}
				
			}

			if(fibonacciScheme){
				if(!print_MinFHeap(MIN))
					printf("There was some problem in printing nodes!! Please try once again\n");
			}
			else{
				if(!printNodes(ROOT))
					printf("There was some problem in printing nodes!! Please try once again\n");
			}
		}

		else{
			if(log == DEBUG || log == INFO)
				printf("Sorry, File is not accessible!\n");
		}
	
}



/*************************************************************************************
 * Function: randomMode()
 * Description:	This is the function to handle User Input Mode
 * Return: 0-Success, 1-Unsucessful
 * Input: N/A
 ************************************************************************************/
void randomMode(){
	int inputCase1[5000], operation, randomData, iCase, nTime;
	int ipCase[7] = {100, 500, 1000, 2000, 3000, 4000, 5000};
	clock_t Start, Time, fTime=0;

	printf("Calculating Processing Time for Fibonacci Heap & Leftist Tree...\n");

	//Fibonacci Heap
	for(iCase = 0; iCase<7; iCase++){
		fTime=0;
		for(nTime=0; nTime<5; nTime++){
			randomPermutation(inputCase1, iCase); //Generating Random Permutation of Inputs
			MIN = intializeFMinHeap(MIN, inputCase1, iCase); //Initializing Fibbonacci Heap
	
			Start = clock(); //Start time

			//Operation Generator Sequence
				for(operation=1;operation<=MAX_OPERATIONS; operation++){
					if(log == DEBUG)
						printf("\nPeforming operation #%d\n", operation);
					D = rand() % 2;
					randomData = rand() % 10000; //Data's range is b/w 0-9999
					if(D == 0){
						if(log == DEBUG)
							printf("Deleting MIN............\n");
						MIN = deleteMin_MinFHeap(MIN);
					}
					else{
						if(log == DEBUG)
							printf("Inserting %d............\n", randomData);
						//Insert
						MIN = insert_MinFHeap(MIN,  randomData);
					}
				}


			Time = clock() - Start;
			fTime +=(Time/5000L);
		}
		printf("********************************************************************\n");
		printf("Time taken for input size %d in Fibonacci Heap is %ldms: \n", ipCase[iCase], (fTime/5L));
		printf("********************************************************************\n");
	}


	//Leftist Tree
	for(iCase = 0; iCase<7; iCase++){
		fTime=0;
		for(nTime=0; nTime<5; nTime++){
			randomPermutation(inputCase1, iCase); //Generating Random Permutation of Inputs
			ROOT = initializeLeftistTree(ROOT, inputCase1, iCase); //Initializing Fibbonacci Heap
	
			Start = clock(); //Start time

			//Operation Generator Sequence
				for(operation=1;operation<=MAX_OPERATIONS; operation++){
					if(log == DEBUG)
						printf("\nPeforming operation #%d\n", operation);
					D = rand() % 2;
					randomData = rand() % 10000; //Data's range is b/w 0-9999
					if(D == 0){
						if(log == DEBUG)
							printf("Deleting MIN............\n");
						ROOT = deleteMinLeftistTree(ROOT);
					}
					else{
						if(log == DEBUG)
							printf("Inserting %d............\n", randomData);
						//Insert
						ROOT = insertMinLeftistTree(ROOT, randomData);
					}
				}


			Time = clock() - Start;
			fTime +=(Time/5000L);
		}
		printf("********************************************************************\n");
		printf("Time taken for input size %d in Leftist Tree is %ldms: \n", ipCase[iCase], (fTime/5L));
		printf("********************************************************************\n");
	}


}


void randomPermutation(int elements[], int numberOfElements){
	int temp, index, randomIndex, i;

	if(log == DEBUG)
		printf("\nGenerating Random permutation of elements....\n");

	for(i=0;i<numberOfElements; i++) //Initialize element array
		elements[i] = i;
	
	for(index=0; index<numberOfElements; index++){
		randomIndex = rand()%(numberOfElements);
		temp = elements[index];
		elements[index] = elements[randomIndex];
		elements[randomIndex] = temp;
	}
}

