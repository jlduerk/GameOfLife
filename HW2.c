/*
 ============================================================================
 Name        : HW2.c
 Author      : Jasmine Duerk
 Created on  : February 1, 2019
 Description : Game of Life
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tests.h"
#include "production.h"

/**
 * main runs production and the tests for the Game Of Life
 * @param int argc, the number of arguments passed into the program
 * @param char* argv, the array of arguments passed into the program
 * @return An int indicating the successful execution of the program
 */
int main(int argc, char* argv[])
{
	bool ok=false;

	puts("!!!Hi all. We're doing HW2!!!");
	if(tests())
	{
		ok=production(argc, argv);
		if (ok)
		{
			puts("Game of life seems to have worked.");
		}
		else
		{
			puts("Game of life seems not to have worked.");
		}
	}
	else
	{
		puts("Something went wrong in the tests.");
	}
	return EXIT_SUCCESS;
}
