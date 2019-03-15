/*
 * production.c
 *
 *  Created on: February 1, 2019
 *      Author: Jasmine Duerk
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#include <string.h>

// For testing (will be set to the number of generations that were played so that it can be compared to a number in a test)
int howManyGens = -1;

/*
 * numGens returns the number of generations (for testing)
 * @return An int representing the number of generations
 */
int numGens()
{
	return howManyGens;
}

bool isTest = false;
/*
 * testPrintOverride makes it so that boards are not shown during testing
 * @param bool testing, a boolean representing if it is a test case or not
 * @return Nothing, because it sets the boolean isTest to true if it is a test
 */
void testPrintOverride(bool testing){
	isTest = testing;
}

/**
 * clearBoard clears an array by setting everything to 'o'
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @param char* board, the location of the upper left of the petri dish
 * @return There is none, because it just resets an array
 */
void clearBoard(int nRows, int nCols, char* board) {
	//For all rows
	for (int row=0; row<nRows; row++)
		//For all columns
		for (int col=0; col<nCols; col++)
			//sets the entire board to 'o' which is a blank space
			*(board + (row*nCols) + col) = 'o';
}

/**
 * production reads from the file and handles gameplay, including checking for end conditions
 * @param int argc, the number of arguments passed into the program
 * @param char* argv, the array of arguments passed into the program
 * @return a boolean representing if production completed successfully
 */
bool production(int argc, char* argv[])
{

	bool results = false; //true if production completes successfully
	bool ok2; //for opening file
	bool done = false; //true if done
	int nRows=-1; //number of rows
	int nCols = -1; //number of columns
	int gens = 0; //number of generations to play
	int howManyLinesInFile = 0; //the number of lines in the file
	int maximumWidth = 0; //the maximum width of the file
	char filename[100]; //the name of the file

	//For the whole filename array
	for(int i= 0; i<100; i++)
	{
		//The whole filename array is being set to NUL
		filename[i]='\0';
	}

	char print = 'n'; //'y' if print each generation
	char pause = 'n'; //'y' if waits for user input after each generation

	//get the NR NC gens input [print] [pause], Usage as needed.
	if(argc<5 || argc>8)//not all mandatory args provided or there are too many args provided
	{
		usage();
		done=true;
	}
	else if (argc == 7) //if there are 7 args provided
	{
		pause = argv[6][0]; //pause is arg 6 from the command line input
	}
	if (!done && (argc >= 6)) //if there are greater than 6 args provided
	{
		print = argv[5][0]; //print is arg 5 from the command line input
	}

	if(!done)//must be greater than or equal to 5, so get the mandatory vals
	{

		char* ptr=0; //pointer to use for strtol
		long nr_l = strtol(argv[1],&ptr,10);//get the NR
		nRows = (int)nr_l;

		if(nRows<1)
		{//There must be at least 1 row
			printf("Usage: Rows should be greater than 0, received %d.\n",nRows);
			done = true;
		}

		ptr=0;
		long nc_l = strtol(argv[2],&ptr,10);//get the NC
		nCols = (int)nc_l;
		if(nCols<1)
		{//There must be at least 1 column
			printf("Usage: Columns should be greater than 0, received %d.\n",nCols);
			done = true;
		}

		ptr=0;
		long ng_l = strtol(argv[3],&ptr,10);//get the gens
		gens = (int)ng_l;
		if(gens<1)
		{//There must be at least 1 generation
			printf("Usage: Generations should be greater than 0, received %d.\n",gens);
			done = true;
		}


		strcpy(filename, argv[4]);
		//now we have the command line
		//Let's read the input file
		FILE* fp = fopen(filename, "r"); //we try to read it
		if (fp != false)
		{//it opened, yay!

			if (!isTest) printf("Opened %s.\n",filename);
			ok2 = true;
			//can we read the data?
			char oRow[100];


			//Let's find out how many lines there are, and
			//find out the maximum width
			bool doneReadingFile = false;
			while(!doneReadingFile) //While the file is not done being read
			{
				oRow[0]='\0';
				fscanf(fp, "%s", oRow);
				if(strlen(oRow)!=0)//there was something there
				{
					howManyLinesInFile++;
					if(strlen(oRow) > maximumWidth){ //If oRow is larger than the maximumWidth, maximumWidth must be increased to accommodate oRow
						maximumWidth = strlen (oRow);
					}
				}
				else
				{
					//Close the file when done with reading it
					doneReadingFile = true;
					fclose(fp);

				}

			}

		}//can read filename
		else
		{
			puts("Cannot find that file,");
			done = true;
		}
		if(!done) //If we can find the file, we are not done, and we must initialize the boards
		{

			//Initialize the arrays
			char A[nRows][nCols];
			char B[nRows][nCols];
			char C[nRows][nCols];
			char* old_p=&A[0][0];
			char* new_p=&B[0][0];
			char* other_p=&C[0][0];

			//Initialize the boards to be 'o'
			clearBoard(nRows, nCols, old_p);
			clearBoard(nRows, nCols, new_p);
			clearBoard(nRows, nCols, other_p);


			FILE* fp = fopen(filename, "r");//we read it before, we expect we can read it again

			readFileIntoArray(nRows, nCols, howManyLinesInFile,  maximumWidth, old_p, fp);

			howManyGens = generate(gens,  nRows,  nCols,  old_p, new_p, other_p, print, pause);
			if (!isTest) printf("Ran %d generations\n", howManyGens); //If (!isTest) ensures that it only prints if it is not a test case
		}

	}//not done
	results = !done;
	return results;

}



/**
 * PlayOne carries out one generation
 * @param unsigned int nr, the number of rows in the petri dish
 * @param unsigned int nc, the number of columns in the petri dish
 * @param char* Old, the location of the upper left of starting petri dish
 * @param char* New, the location of the upper left of the ending petri dish
 * @return There is none, because results are in New array
 */
void PlayOne (unsigned int nr, unsigned int nc, char* Old, char* New)
{
	int nRows = nr; //number of rows
	int nCols = nc; //number of columns

	//loop over all locations in the new dish
	//figure out whether occupied or not
	for(int row=0;row<nRows;row++) //For every row
	{
		for(int col=0; col<nCols; col++) //For every column
		{
			//There are three rules,
			// use occupied in old
			//use neighbors in old
			bool occupied = true;
			occupied = (getLetter(row,col,nCols, Old) == 'x');

			//If the cell is occupied and the number of neighbors is less than two or greater than three, the organism in the cell dies
			if(occupied && ((HowManyNeighbors(row, col, nRows, nCols, Old) < 2) || (HowManyNeighbors(row, col, nRows, nCols, Old) > 3)))
			{//Death rule
				*(New + (row * nCols) + col)='o';
			}
			//Otherwise, if the cell is occupied, the organism survives
			else if(occupied)
			{//Survival rule
				*(New + (row * nCols) + col)='x';
			}
			//If the cell is unoccupied but has three neighbors, an organism will be born
			else if(HowManyNeighbors(row, col, nRows, nCols, Old) == 3)
			{//Birth rule
				*(New + (row * nCols) + col)='x';

			}
			//Or else, nothing happens to the cell
			else
			{
				*(New + (row * nCols) + col)='o';
			}
		}
	}
}

/**
 * usage prints how to input the information needed to play
 */
void usage(void)
{
	puts("Usage: HW2 NR NC gens input [print] [pause]");
}

/**
 * getLetter finds the letter at a location of the array
 * @param int row, the row of the place in the array to get the letter from
 * @param int col, the column of the place in the array to get the letter from
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @param char* Old, the location of the upper left of the petri dish
 * @return char, the letter that is at the specified location of the array
 */
char getLetter(int row, int col, int nCols, char* Old)
{
	return *(Old+ (row*nCols)+col);
}

/**
 * HowManyNeighbors finds the number of neighbors at the specified location of the array
 * @param int row, the row of the place in the array to get the number of neighbors from
 * @param int col, the column of the place in the array to get the number of neighbors from
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @param char* Old, the location of the upper left of the petri dish
 * @return There is none, because it just resets an array
 */
int HowManyNeighbors(int row, int col, int nRows, int nCols, char* Old)
{
	int howManyN = 0; //The number of neighbors

	//there could be as many as 8 neighbors
	//cells on an edge or corner have fewer neighbors
	//we will search for neighbors clockwise from NorthWest

	if (row>0) //If it is not in the top row, we can look north
	{
		if(col>0) //If it is not the leftmost column, we can look west
		{
			if(getLetter(row-1, col-1,nCols, Old)=='x')//NW
			{
				howManyN++;
			}
		}
		if(getLetter(row-1, col,nCols, Old)=='x')//N
		{
			howManyN++;
		}
		if(col<(nCols-1)) //If it is not all the way on the right of the board, we can look east
		{
			if(getLetter(row-1, col+1,nCols, Old)=='x')//NE
			{
				howManyN++;
			}
		}
	}//can look north
	if(col>0) //If it is not the leftmost column, we can look west
	{
		if(getLetter(row, col-1,nCols, Old)=='x')//W
		{
			howManyN++;
		}
	}
	if(col<(nCols-1)) //If it is not all the way on the right of the board, we can look east
	{
		if(getLetter(row, col+1,nCols, Old)=='x')//E
		{
			howManyN++;
		}
	}
	if(row<nRows-1) //If it is not the bottom row of the board, we can look south
	{
		if(col>0) //If it is not the leftmost column, we can look west
		{
			if(getLetter(row+1, col-1,nCols, Old)=='x')//SW
			{
				howManyN++;
			}
		}
		if(getLetter(row+1, col,nCols, Old)=='x')//S
		{
			howManyN++;
		}
		if(col<(nCols-1)) //If it is not the rightmost column, we can look east
		{
			if(getLetter(row+1, col+1,nCols, Old)=='x')//SE
			{
				howManyN++;
			}
		}
	}//can look south

	return howManyN;
}

/**
 * readFileIntoArray reads the file and inputs the information into an array
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @param int howManyLinesInFile, the number of lines in the file
 * @param int maximumWidth, the maximum width of the array
 * @param char* ar_p, the location of the upper left of the array that the information is being put into
 * @param FILE* fp, the file that is being read
 * @return There is none, because it puts the information from the file onto an array
 */
void readFileIntoArray(int nRows, int nCols, int howManyLinesInFile, int maximumWidth, char* ar_p, FILE* fp)
{
	for(int row = 0; row< nRows; row++) //For every row

	{
		for(int col = 0; col<nCols; col++) //For every column
		{
			*(ar_p+(row*nCols)+col) = 'o'; //First set everything to 'o' to initialize the board

		}
	}

	for(int frow=0; frow< howManyLinesInFile; frow++) //For every row of the board

	{
		char fromFile[maximumWidth];

		for(int i = 0; i<maximumWidth; i++) //For the width of the board
		{
			fromFile[i]='o';
		}
		fscanf(fp, "%s", fromFile);
		for(int fcol=0; fcol<maximumWidth; fcol++) //For every column of the file
		{
			if(fromFile[fcol]=='x') //If the file reads 'x', set the value in the array to 'x'
			{
				int targetRow = frow+(nRows-howManyLinesInFile)/2; //centers the board
				int targetCol = fcol+(nCols-maximumWidth)/2; //centers the board
				*(ar_p+(targetRow*nCols)+targetCol) = 'x';
			}
		}
	}
}

/**
 * generate creates a generation
 * @param int gens, the number of generations to output
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @param char* old_p, the location of the upper left of the old petri dish
 * @param char* new_p, the location of the upper left of the new petri dish
 * @param char* other_p, the location of the upper left of a third temp petri dish
 * @param char print, 'y' if generate prints each generation
 * @param char pause, 'y' if generate waits for user input between each generation
 * @return An int representing the generations completed
 */
int generate(int gens,  int nRows,  int nCols,  char* old_p, char* new_p, char* other_p, char print, char pause)
{
	int g = 0;
	bool allOrganismsDead = false;
	bool patternRepeated = false;
	bool done = false;
	bool firstTime = true;

	for(int gensDone = 0; !done && (gensDone<gens); gensDone++) //For every generation
	{
		if(!anyX(old_p, nRows, nCols)) //If there are no organisms left alive
		{//all organisms are dead
			allOrganismsDead =  true;
			done = true;
			if (!isTest) puts("All organisms dead."); //if (!iTest) used to only print if it is not a test case
			printThis(nRows, nCols, old_p);
		}
		PlayOne(nRows, nCols, old_p, new_p);
		g++;
		if (sameContent(old_p, new_p, nRows, nCols)) //if the pattern repeated for multiple generations
		{
			patternRepeated = true;
			if (!isTest) puts("Pattern repeated in one generation.");//if (!iTest) used to only print if it is not a test case
			printThis(nRows, nCols, old_p);
			done = true;
		}
		if(firstTime) //If it is the first generation
		{
			firstTime = false; //Set firstTime variable to false because it is no longer the first generation after this generation
		}
		else
		{
			if (sameContent(other_p, new_p, nRows, nCols)) //if the pattern is alternating between two for multiple generations
			{
				patternRepeated = true;
				if (!isTest) puts("Pattern repeated after two generations.");//if (!iTest) used to only print if it is not a test case
				printThis(nRows, nCols, other_p);
				done= true;

			}
		}
		if(!done) //If the program has not terminated because none of the termination conditions were met
		{
			if(print=='y') //If the input from command line wants every generation to be printed
			{
				puts("New generation");
				printThis(nRows, nCols, new_p);
			}
			if(pause=='y') //If the input from the command line wants to wait for user input after each generation is printed
			{
				puts("Paused waiting for input.");
				getc(stdin);//just waits for user input
			}

			//Rotate boards
			char* temp = other_p;
			other_p = old_p;
			old_p = new_p;
			new_p = temp;

			//Resets the new board
			clearBoard(nRows, nCols, new_p);
		}
	}//end of generations

	return g;
}

/**
 * anyX determines if there are any X's in the petri dish
 * @param char* arr, the upper left of the petri dish array
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @return A boolean representing if there are any X's in the petri dish
 */
bool anyX(char* arr, int nRows, int nCols)
{
	bool any = false; //true if there are any alive organisms in the petri dish
	for(int row=0; !any && (row<nRows); row++) //For every row
	{
		for(int col=0; !any && (col< nCols); col++) //For every column
		{
			if(getLetter(row, col, nCols, arr) == 'x') //If there is an organism at the specified cell
			{
				any=true;
			}
		}
	}
	return any;
}

/**
 * generate creates a generation
 * @param int gens, the number of generations to output
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @return An int representing the generations completed
 */
bool sameContent(char* one_p, char* another_p, int nRows, int nCols)
{
	bool same = true; //for all the comparisons that have been made so far

	for(int row=0; same && (row<nRows); row++) //For every row
	{
		for(int col=0; same && (col< nCols); col++) //For every column
		{
			if(getLetter(row, col, nCols, one_p) != getLetter(row, col, nCols, another_p)) //If the letter at that cell is not the same for both arrays
			{
				same=false;
			}
		}
	}
	return same;
}

/**
 * printThis prints a generation
 * @param int nRows, the number of rows in the petri dish
 * @param int nCols, the number of columns in the petri dish
 * @param char* old_p, the location of the upper left of the petri dish
 * @return Nothing, because it prints out a generation
 */
void printThis(int nRows, int nCols, char* old_p)
{
	//prints out the board only if it is not a test case
	if (!isTest)
	{
		// For each row
		for (int row=0; row<nRows; row++) {
			// Print each column position...
			for (int col=0; col<nCols; col++) {
				//Print x if there is an organism, blank space if there is no organism
				//printf ("%c", (getLetter(row, col, nCols, old_p)));
				printf ("%c", (*(old_p + (row*nCols) + col) == 'x') ? 'x' : ' ');
			}
			printf ("\n"); // End of row
		}
		printf ("\n"); // Put space between board prints
	}
}
