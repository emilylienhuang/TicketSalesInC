#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define LENGTH 100

void validateInt(const char* buff);

int main(void) {
	
	char inputStr[LENGTH]; //define a string to hold user input
	fgets(inputStr, LENGTH, stdin); //fill the string to hold user input with input from keyboard
	validateInt(inputStr);

	return 0;
}

void validateInt(const char* buff) {

	char* end; //declares a pointer that will store the values left over in buff that are not numbers
	errno = 0; //a variable that will store a value based on what happens in the strtol call. If strtol gets a range error this value will change.
	long intTest = strtol(buff, &end, 10); //saves value from  strtol returned as a long.
	//strtol takes the string in buff and tries to convert it to a long. If there are extra characters in buff, they will be stored in end. 10 indicates base 10 or the decimal system.

	if (end == buff) {
		//if this block executes, then there was not a number found in the first index of buff, so end points to buff[0] which indicates that the user did not enter a decimal number.
		//This block will execute for code even like: abc4.5d
		fprintf(stderr, "%s: not a decimal number \n", buff); //indicates that we are printing an error message to the console

	}
	else if ('\0' != *end && '\n' != *end) {
		//this block executes if end points to a character that is not the null or the new line character. 
		//This block executes if there were characters entered after a number like: 23.5abc
		fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);

	}
	else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
		//This block executes if there was overflow, so the minimum value able to be stored in a long or the maximum value able to be stored in a long equals intTest.
		//ERANGE will exactly equal errno if there was overflow because the value of errno will be set to the value of ERANGE in that case. 
		// LONG_MIN == intTest || LONG_MAX == intTest could be true without overflow, but the condition  ERANGE == errno would clarify if there was overflow or not
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	else {

		//This block executes if the user entered a valid number.
		printf("%ld is integer value ", intTest);
	}
}