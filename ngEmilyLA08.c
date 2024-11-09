/*
Name: Emily Ng
OS:Windows
Due: March 30, 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define NUM_SUITS 4 //number of suits of cards
#define SUIT_SIZE 12 //size of character array to hold a single suit value
#define FILE_PATH_SIZE 100 // number of characters allowed in file path including the null character
const char* const suitPtr[NUM_SUITS] = { "hearts", "diamonds", "clubs", "spades" }; //array of pointers that hold different suit values
const char filePath[FILE_PATH_SIZE] = {"C:\\Users\\17192\\Documents\\suitsFile.txt"}; //file path used to generate the file 

int main(void) {
	

	char suit[SUIT_SIZE]; //array that will hold the name of a suit

	FILE* filePtr; // pointer used to store file information, will allow us to access file information

	filePtr = fopen(filePath, "w"); //opens/creates suitsFile.txt to write to

	srand(time(NULL)); //seeding random number generation.

	for (unsigned int i = 0; i < 20; i++) { //looping 20 times

		unsigned int randomizedValue = (rand() % 4); //generates a random number between 0 and 3 to use as an index for suitPtr array to grab randomized suit value

		strcpy(suit, suitPtr[randomizedValue]); //copies randomized value from suitPtr array into suit array
		
		suit[0] = toupper(suit[0]); //makes first character in suit array upper case

		fputs(suit, filePtr); //write suit array to suitsFile

		fputs("\n", filePtr); //puts new line in suitsFile
	}

	fclose(filePtr); //closes the file

	printf("Your file is located at %s\n", filePath); //shows absolute file path where file is located

	return 0;

}

/*
For my solution, I spent a lot of time exploring C libraries and different functionalities.
I explored fig11_02 and fig11_06. I also got some assistance from the CS Center in getting the file open and started. 
First, I made sure the file opened successfully. 
Next, I wrote the for loop. I generated a random value and made sure that worked. 
I then copied the randomly chosen string for suit into the suit array. I made the first letter of the suit array upper case.
I continued to print the suit array to the file followed by a newline character. 
Finally, I closed the file and printed its path. 
*/