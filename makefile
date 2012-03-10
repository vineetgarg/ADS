#####################################################
# MakeFile to compile Fibonacci Heap & Leftist Tree #
# @Author - Garg, Vineet
# Date Creatd - October 31, 2010
####################################################

#Main Target, Depends upon clean
all: clean
	gcc FibonacciHeap.c LeftistTree.c heap.c -o heap

#This target cleans the object & heap file
clean:
	rm -f *.o
	rm -f heap

#This target run the program in Random Mode
run: all 
	./heap -r
