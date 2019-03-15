/*
 * tests.c
 *
 *  Created on: February 1, 2019
 *      Author: Jasmine Duerk
 */
#include <stdbool.h>
#include "tests.h"
#include "production.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strncmp

/**
 *
 * tests organizes all the test cases and prints out if the tests passed
 */
bool tests(void)
{
	bool results=false;
	puts("During tests");
	testPrintOverride(true); //stops the board from printing during testing
	bool ok1 = testReadInput();
	if(ok1)puts("Found and read the test file.");
	bool ok2 = testMakeArrays();
	if(ok2)puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOne();
	if(ok3)puts("playOne is ok.");
	bool ok4 = testExitAfterRepeatingTwo();
	if(ok4)puts("Exit after alternating two boards is ok.");
	bool ok5 = testExitAfterRepeatingOnce();
	if(ok5)puts("Exit after repeating one board is ok.");
	bool ok6 = testExitAfterDeath();
	if(ok6)puts("Exit after death is ok.");
	bool ok7 = testHowManyNeighbors();
	if(ok7)puts("HowManyNeighbors is ok.");
	bool ok8 = testGetLetter();
	if(ok8)puts("getLetter is ok.");
	bool ok9 = testAnyX();
	if(ok9)puts("anyX is ok.");
	bool ok10 = testSameContent();
	if(ok10)puts("sameContent is ok.");


	puts("end of tests");
	testPrintOverride(false); //turns the board printing back on after testing
	results = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7 && ok8 && ok9 && ok10;
	printf("tests returning %d.\n",results);
	return results;
}

/**
 * testReadInput tests that a file can be opened and read, and that if a file does not exist, the program notices
 */
bool testReadInput(void)
{
	//the plan is:
	//create a file
	//make sure it can be opened.
	//try a file that should not exist
	//check that we notice it does not exist

	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	//
	//Here goes: create a file
	FILE* fp;
	fp = fopen("/tmp/LifeInput.txt", "w"); //we write it first for later reading
	if (fp != false)
	{//it opened, yay!
		//file containing a sequence of lines,
		//each consisting of a sequence of 'x' and 'o' characters,
		//indicating the occupied and unoccupied cells
		//of the initial configuration, e.g.
		//oxo
		//xox
		//xox
		//oxo
		int w1 = fprintf(fp, "oxo\n");//returns the number of chars excluding null
		int w2 = fprintf(fp, "xox\n");
		int w3 = fprintf(fp, "xox\n");
		int w4 = fprintf(fp, "oxo\n");
		if ((w1 == w2) && (w2 == w3) && (w3 == w4) && (w4 == 4))
		{
			ok1 = true;
		}
		fclose(fp);
	}
	fp = fopen("/tmp/LifeInput.txt", "r"); //we try to read it
	if (fp != false)
	{//it opened, yay!
		ok2 = true;
		//can we read the correct data?
		char oRow[4]; //test case, we know a string of 3 chars terminated by null
		int count =  fscanf(fp, "%s", oRow);
		//printf("First row is %s\n", oRow);
		if(0==strncmp(oRow, "oxo", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Second row is %s\n", oRow);
		if(0==strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Third row is %s\n", oRow);
		if(0==strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Fourth row is %s\n", oRow);
		if(0==strncmp(oRow, "oxo", 4))
		{//got the right data
			ok3 = true;
		}
	}
	results = ok1 && ok2 && ok3;
	return results;
}

/**
 * testMakeArrays tests if making an array works
 */
bool testMakeArrays(void)
{
	bool results = false;
	bool ok1 = false;
	//we need to allocate and populate arrays
	//In this test case we set our arrays to 4 rows x3 columns
	int nRows = 4;
	int nCols = 4;
	char boardBefore[4][4]={
			{'o','x','o','\0'},
			{'x','o','x','\0'},
			{'x','o','x','\0'},
			{'o','x','o','\0'}
	};

	char boardAfter[nRows][nCols];
	//here's the test
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			boardAfter[row][col]=boardBefore[row][col];
		}
	}
	ok1 = true;
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if (boardAfter[row][col]!=boardBefore[row][col])
			{
				ok1=false;
			}
		}
	}

	results = ok1;
	return results;
}

/**
 * testPlayOne tests that the PlayOne function works
 */
bool testPlayOne(void)
{
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;


	int nRows = 4;
	int nCols = 3;
	char boardBefore[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};
	char correctBoardAfter[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};

	char boardAfter[nRows][nCols];
	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter[row][col]!=boardBefore[row][col])
			{//error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results;

}

/*
 * Test that the program terminates after boards begin alternating between two
 */
bool testExitAfterRepeatingTwo()
{
	int argc = 7;
	char* argv[7];
	argv[0] = "Test"; //Program name
	argv[1] = "10"; //Number of rows
	argv[2] = "10"; //Number of columns
	argv[3] = "100"; //Maximum number of generations to play
	argv[4] = "./testcases/blinker.txt"; //Input test file
	argv[5] = "n"; //Don't print in test
	argv[6] = "n"; //Don't pause in test

	bool ok = production(argc, argv);
	//The program should terminate after two generations because it alternates between two boards
	ok &= (numGens() == 2);
	return ok;
}

/*
 * Test that the program terminates after the boards begin to repeat themselves
 */
bool testExitAfterRepeatingOnce()
{
	int argc = 7;
	char* argv[7];
	argv[0] = "Test"; //Program name
	argv[1] = "10"; //Number of rows
	argv[2] = "10"; //Number of columns
	argv[3] = "100"; //Maximum number of generations to play
	argv[4] = "./testcases/block.txt"; //Input test file
	argv[5] = "n"; //Don't print in test
	argv[6] = "n"; //Don't pause in test

	bool ok = production(argc, argv);
	//The program should terminate after one generation because it repeats
	ok &= (numGens() == 1);
	return ok;
}

/*
 * Test that the program terminates after all organisms have died
 */
bool testExitAfterDeath()
{
	int argc = 7;
	char* argv[7];
	argv[0] = "Test"; //Program name
	argv[1] = "50"; //Number of rows
	argv[2] = "50"; //Number of columns
	argv[3] = "200"; //Maximum number of generations to play
	argv[4] = "./testcases/diehard.txt"; //Input test file
	argv[5] = "n"; //Don't print in test
	argv[6] = "n"; //Don't pause in test

	bool ok = production(argc, argv);
	//All organisms die after 131 generations
	ok &= (numGens() == 131);
	return ok;
}
/*
 * Test that HowManyNeighbors function works
 */
bool testHowManyNeighbors()
{
	//ok will be set to true if the test passes
	bool ok = false;
	char board[4][3]={
			{'x','x','o'},
			{'o','x','o'},
			{'o','o','o'},
			{'o','o','o'}
	};
	char* aBoard = &board[0][0];
	if (HowManyNeighbors(0, 0, 4, 3, aBoard) == 2)
	{
		ok = true;
	}
	return ok;
}

/*
 * Test that the getLetter function works
 */
bool testGetLetter()
{
	//ok will be set to true if the test passes
	bool ok = false;
	char board[4][3]={
			{'x','x','o'},
			{'o','x','o'},
			{'o','o','o'},
			{'o','o','o'}
	};
	char* aBoard = &board[0][0];
	if (getLetter(0, 0, 3, aBoard) == 'x')
	{
		ok = true;
	}
	return ok;
}

/*
 * Test that the anyX function works
 */
bool testAnyX()
{
	//ok will be set to true if the test passes
	bool ok = false;
	char board[4][3]={
			{'x','x','o'},
			{'o','x','o'},
			{'o','o','o'},
			{'o','o','o'}
	};
	char* aBoard = &board[0][0];
	if (anyX(aBoard, 4, 3))
	{
		ok = true;
	}
	return ok;
}

/*
 * Test that the sameContent function works
 */
bool testSameContent()
{
	//ok will be set to true if the test passes
	bool ok = false;
	char board[4][3]={
			{'x','x','o'},
			{'o','x','o'},
			{'o','o','o'},
			{'o','o','o'}
	};
	char* aBoard = &board[0][0];
	char* anotherBoard = &board[0][0];
	if (sameContent(aBoard, anotherBoard, 4, 3))
	{
		ok = true;
	}
	return ok;
}


