/*
Emily Ng
HW05
Due: Marchm 16, 2022
OS: Windows
This program set ups admin functions and uses pointers.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_ADMIN_PIN 5 //size taken in of user entry for admin pin
#define ADMIN_PIN "4932" //correct admin pin 
#define NUMBER_OPTIONS 3 //number of options for seats
const char* const seatLevel[NUMBER_OPTIONS] = { "(C)ourtside seats","(M)id Level seats","(U)pper Level seats" }; //array of pointers to character arrays indicating sections

bool adminSetUp(double pricesArray[], double* percentPtr); //function that allows the admin to set up the prices after entering pin
void setUpValues(const char* const seatLevelOrPercentage, double * const priceToFill, int min, int max); //function that establishes the values for seat and percentage based off user entry
bool isPinValid(char* const userEntry, unsigned int* const triesPtr); //function to verify admin pin
bool isNumber(char* const userEntry); //function that verifies if user entry is a number

int main(void) {

	//we will create an array to hold the different prices that the admin enters as well as a double to hold percentage to donate to charity and a pointer to that percentage. 
	double prices[NUMBER_OPTIONS];

	double percentageForCharity = 0;

	double *percentagePtr = &percentageForCharity;

	//we will check if the admin set up is successful. If it is, we will output the information established in the set up.
	bool adminSetUpSuccessful = adminSetUp(prices, percentagePtr);

	if (adminSetUpSuccessful) {
		printf("%s", "\nPrices for Seats\n");
		printf("%-25s%-10.2lf\n", seatLevel[0], prices[0]);
		printf("%-25s%-10.2lf\n", seatLevel[1], prices[1]);
		printf("%-25s%-10.2lf\n", seatLevel[2], prices[2]);
		printf("Percent Donated To Charity Per Purchase: %.2lf\n\n", *percentagePtr);
	}

	//we will indicate that we are ending the program 
	puts("Now exiting program");

	return 0;
}

//This function allows the admin to set up the values for seats and percentage if a valid pin is entered. 
bool adminSetUp(double pricesArray[], double * const percentPtr) {

	unsigned int attempts = 0; //keeps track of how many times the admin tries to enter the pin
	unsigned int *attemptsPtr = &attempts; //pointer to attempts
	char entry[SIZE_ADMIN_PIN]; //character array to hold user entry
	bool successful = false; //boolean if true means that the pin is valid, and we will set up the values for seat level and percentage
	do {
		puts("Enter four digit admin pin: "); //prompts the user to enter a pin
		fgets(entry, SIZE_ADMIN_PIN, stdin); //stores the value the user entered into entry
		while ((getchar()) != '\n'); //clear the buffer
		successful = isPinValid(entry, attemptsPtr); //validates pin
		if (successful) {

			//setting up values for seat level and percentage if the pin is valid.
			setUpValues(seatLevel[0], &pricesArray[0], 200, 500);
			setUpValues(seatLevel[1], &pricesArray[1], 75, 200);
			setUpValues(seatLevel[2], &pricesArray[2], 10, 75);
			setUpValues("percentage", percentPtr, 5, 25);

			attempts = 3; //end the loop after setting up values

		}
	

	} while (attempts < 3);

	return successful;
}

//this function sets up a value given a minimum and maximum. It also validates that user entry for this value is valid.
void setUpValues(const char* const seatLevelOrPercentage, double * const priceToFill, int min, int max) {

	double valueEntered;  //stores user entry

	printf("Enter a number for %s \nEntry must be greater than %d and less than or equal to %d.\n", seatLevelOrPercentage, min, max);
	int scannedValue = scanf("%lf", &valueEntered); //saves user entry to valueEntered
	while ((getchar()) != '\n'); //clear buffer

	//this checks if scanf was able to take in a double value. If invalid entry, we prompt the user for another entry
	while (scannedValue != 1) {
		puts("You did not enter a number.");
		printf("Enter a number for %s \nEntry must be greater than %d and less than or equal to %d.\n", seatLevelOrPercentage, min, max);
		scannedValue = scanf("%lf", &valueEntered);
		while ((getchar()) != '\n');
	}

	//this checks if the value entered is in the specified range, if not we reprompt the user for another entry and validate again if the value entered is numeric and in the specified range. 
	while (valueEntered <= min || valueEntered > max) {
		puts("Invalid numeric entry.");
		printf("Enter a number for %s\nEntry must be greater than %d and less than or equal to %d.\n", seatLevelOrPercentage, min, max);
		scannedValue = scanf("%lf", &valueEntered);
		while ((getchar()) != '\n');

		while (scannedValue != 1) {
			puts("You did not enter a number.");
			printf("Enter a number for %s\nEntry must be greater than %d and less than or equal to %d.\n", seatLevelOrPercentage, min, max);
			scannedValue = scanf("%lf", &valueEntered);
			while ((getchar()) != '\n');
		} //end inner while
	} //while to check valid numeric entry

	 *priceToFill = valueEntered; //we will put the valid value of valueEntered back where it belongs in the calling function.


} //setUpValues

//this function checks if the pin entered by the admin is valid. It uses a pointer to keep track of admin attempts
bool isPinValid(char* const userEntry, unsigned int * const triesPtr) {

	bool validPin = false; //boolean later returned to let us know if the admin entered a valid pin
	bool entryIsNumeric = isNumber(userEntry); //checks if userEntry is in fact a number
	
	//if user entry is not a number, we need to let the admin know. 
	if ((!entryIsNumeric)) {
		printf("%s\n", "You did not enter a number");
		(*triesPtr)++;
	}

	//the admin entered a number less than the needed length. We will let them know. 
	if (entryIsNumeric && (strlen(userEntry) < 4)) {
		printf("%s", "You did not enter enough numbers.\n");
		(*triesPtr)++;
	}

	//the user entered a valid pin, we will update our boolean keeping track of valid pin
	if (strcmp(userEntry, ADMIN_PIN) == 0) {
		validPin = true;
	}

	//the user entered enough numbers, but the pin is invalid. We will let them know. 
	if(strcmp(userEntry, ADMIN_PIN) != 0 && (strlen(userEntry) == 4) && entryIsNumeric) {
		puts("Invalid pin.");
		(* triesPtr)++;
	}
	return validPin;
}

//this function checks if the user entered a number
bool isNumber(char* const userEntry) {

	bool isNumeric = true; //boolean to return at end of function

	size_t i = 0; //keeps track of character array index

	while (isNumeric && (userEntry[i] != '\0')) { //loops while we have a number or until the end of the character string

		if (isdigit(userEntry[i]) == 0) { //result only equals 0 if entry is not numeric
			isNumeric = false; 
		}

		i++;
	}

	return isNumeric;
}



