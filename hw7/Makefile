#compiler and flags
CC = g++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra -g

#linking together
cTreeTest: CTree.o CTreeTest.o
	$(CC) -o cTreeTest CTreeTest.o CTree.o

tTreeTest: TTreeTest.o
	$(CC) -o tTreeTest TTreeTest.o

#compiling for cTreeTest
cTreeTest.o: CTreeTest.cpp CTree.h
	$(CC) $(CFLAGS) -c CTreeTest.cpp

CTree.o: CTree.cpp CTree.h 
	$(CC) $(CFLAGS) -c CTree.cpp

#compiling for tTreeTest
TTreeTest.o: TTreeTest.cpp Tree.h
	$(CC) $(CFLAGS) -c TTreeTest.cpp

#remove files that were made
clean:
	rm -f *.o cTreeTest
	rm -f *.o tTreeTest
