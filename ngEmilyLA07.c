/*
Emily Ng
CS2060
March 5, 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//we will define the size of the array of possible entries and declare the same array as a constant
#define NUMBER_OF_OPTIONS 4
const char entries[NUMBER_OF_OPTIONS] = { 'C', 'M', 'U', 'Q' };


char validateSeatLevel(char const array[], size_t sizeOfArray); //function takes in array and size of array. According to LPP, array will be passed in as a constant

int main(void) {

	char userEntry = validateSeatLevel(entries, NUMBER_OF_OPTIONS);

	return 0;
}

//This function validates the user entry of a character
char validateSeatLevel(char const array[], size_t sizeOfArray) {

	char userValue; //will keep track of value user enters
	bool foundValue = false; //will keep track of whether or not we found a valid entry

	do {
		puts("Enter your seat level: (C)ourtside, (M)id or (U)pper"); 
		scanf("%c", &userValue); //store user entry to variable
		while ((getchar()) != '\n'); //clear the buffer 

		userValue = toupper(userValue); //convert user entry to upper case

		//indexing through array and changing foundValue is a match has been made between value in array and user entry
		for (size_t i = 0; i < sizeOfArray; i++) { 
			if (array[i] == userValue) {
				foundValue = true;
			} //end inner if
		} //end for

		//if the entry has not been found valid yet, we will put out an error
		if (foundValue == false) {
			puts("Invalid entry");
		}// end if 

	} while (foundValue == false); //we will prompt the user until a valid entry is found

	return userValue; //we have found a valid entry from the user, we will return this value to the function calling this method. 

}
