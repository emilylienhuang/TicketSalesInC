/*
Emily Ng
Iteration01
Due: April 1, 2022
OS: Windows
Description: This program allows an admin to set up sales for a Nuggets basketball game where a percentage of sales is donated to charity after entering a valid pin in three or less attempts.
The admin sets the price for courtside, mid level, and upper level seats given a minimum value that the price must be greater than and a maximum value that the price must be less than or equal to.
The admin also sets up a percentage of the sales to be donated to charity in a specified range not including the minimum but including the maximum.
Then, the program enters a sales mode for users to buy tickets. Users are able to purchase courtside, mid level or upper level seats at prices the admin has set.
Finally, the admin can end the program by entering the pin and sales totals will print including grand totals donated to charity, grand total earned, total tickets and total cost of tickets per seat level.
*/

#include <float.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_ADMIN_PIN 5 //size taken in of user entry for admin pin
#define ADMIN_PIN "4932" //correct admin pin 
#define NUM_ADMIN_ATTEMPTS 4
#define NUMBER_OPTIONS_SEATS 3 //number of options for seats
#define NUMBER_OF_ENTRY_OPTIONS 4 //number of things the user can enter in the program without getting an error
#define NUMBER_TICKET_OPTIONS 4 //number of ticket sale options
#define ZIP_LENGTH 6
#define USER_INPUT_SIZE 2
#define SIZE_NUMERIC_ENTRY 25
#define SIZE_USER_VALIDATION 2
#define FILE_PATH_LENGTH 100

const char DENVER_NUGGETS[] = "Denver Nuggets";
const char DIRECTORY_PATH[] = "C:\\CS2060Files\\";
const char RECEIPTS_FILE_NAME[] = "receipt.txt";
const char SALES_SUMMARY_FILE_NAME[] = "salesSummary.txt";
const char* const SEAT_LEVEL_PTR[NUMBER_OPTIONS_SEATS] = { "(C)ourtside","(M)id Level","(U)pper Level" }; //array of pointers to character arrays indicating sections
const char* const TICKET_LEVELS_PTR[NUMBER_TICKET_OPTIONS] = { "1: Single Pack (1 ticket)","2: Double Pack (2 tickets)", "3: Triple Pack (3 tickets)", "4: Family Pack (4 tickets)" }; //array holding the strings of ticket levels
const char ENTRIES[NUMBER_OF_ENTRY_OPTIONS] = { 'C', 'M', 'U', 'Q' };  //array holding the valid entries that the user can enter
const char DEFAULT_ENTRY = 'Z'; //character representing the user either hasn't entered a character or has not yet entered Q. A place holder value to help keep the loop going if admin pin is invalid the second time.
const int MIN_COURTSIDE_PRICE = 200; //minimum price of courtside tickets must be greater than this number
const int MAX_COURTSIDE_PRICE = 500; // maximum price of courtside tickets
const int MIN_MIDLEVEL_PRICE = 75; //minimum price of mid level tickets must be greater than this number
const int MAX_MIDLEVEL_PRICE = 200; //maximum price of midlevel seats
const int MIN_UPPERLEVEL_PRICE = 10; //upper level seats price must be greater than this number
const int MAX_UPPERLEVEL_PRICE = 75; //maximum price of upper level seats
const int MIN_PERCENT = 10; //percent to charity must be greater than this number
const int MAX_PERCENT = 30; //highest percent that can be donated to charity
const int MIN_NUM_TICKETS = 1; //minimum number of tickets user can purchase
const int MAX_NUM_TICKETS = 4; //maximum number of tickets user can purchase
const char Y = 'Y'; //represents user enters yes
const char N = 'N'; //represents user enters no

void printReceiptToConsole(const char team[], const char seatLevel[], const char ticketLevel[], const double total, const double moneyDonated);
void printToReceiptsFile(const char filePath[], const char teamName[], const char seatLevel[], const char ticketLevel[], double currentTotalSpent, double currentDonatedToCharity);
void getFilePath(char* finalPath, const char* directoryPath, const char* fileName); //function to make and return a file path
char validateYOrN(char entry[]);
bool adminSetUp(double pricesArray[], double* const percentPtr, bool* const validatedOnce); //function that allows the admin to set up the prices after entering pin
bool setUpValues(char* entry, double* const priceToFillPtr, int min, int max); //function that establishes the values for seat and percentage based off user entry
bool isPinValid(char* const userEntry, unsigned int* const triesPtr); //function to verify admin pin
bool isNumber(char* const userEntry); //function that verifies if user entry is a number
char validateSeatLevel(); //function that validates user entry of seat level
int validateTicketNumber(int min, int max); //function that validates the number of tickets the user entered. 
void validateZipCode(); //function that validates zipcode
void displayTotals(const char* const seat, double price, int amountSold, double salesTotal, double charityTotal); //function that displays totals per seat option

int main(void) {

	char receiptsFilePath[FILE_PATH_LENGTH];
	getFilePath(receiptsFilePath, DIRECTORY_PATH, RECEIPTS_FILE_NAME);

	//we will create an array to hold the different prices that the admin enters as well as a double to hold percentage to donate to charity and a pointer to that percentage. 
	double prices[NUMBER_OPTIONS_SEATS];
	double percentageForCharity = 0;

	unsigned int courtsideCounter = 0; //counts number of courtside seats
	unsigned int midCounter = 0; //counts number of mid level seats
	unsigned int upperCounter = 0; //counts number of upper level seats

	double totalCostCourtside = 0; //keeps track of the total cost accumulated by courtside seat purchases
	double totalCostMid = 0; //keeps track of the total cost accumulated by mid level seat purchases
	double totalCostUpper = 0; //keeps track of the total cost accumulated by upper level seat purchases

	double moneyDonatedC = 0; //keeps track of the money donated to charity from courtside seat purchases
	double moneyDonatedM = 0; //keeps track of the money donated to charity from mid level seat purchases
	double moneyDonatedU = 0; //keeps track of the money donated to charity from upper level seat purchases

	double totalTicketSales = 0; //keeps track of total cost of ticket sales
	double totalDonated = 0; //tracks total amount of money donated to charity

	bool adminPinHasBeenValidated = false; //boolean to represent if the admin pin has been validated a first time

	//we will check if the admin set up is successful. If it is, we will output the information established in the set up.
	bool adminSetUpSuccessful = adminSetUp(prices, &percentageForCharity, &adminPinHasBeenValidated);

	char enteredSeatLevel = DEFAULT_ENTRY; //will represent seat level, must be initialized to a value that is not C,M, U, or Q, so is initialized to default entry.

	//We will begin the program if the user has entered the admin pin successfully a first time and the user entered seat level does not equal Q
	while (adminSetUpSuccessful && enteredSeatLevel != ENTRIES[3]) {

		//displays a menu:
		printf("\n\n\nYou can purchase tickets to the Nuggets game and %.2lf%s of your ticket purchase will go to charity.\n", percentageForCharity, "%");
		printf("%-15s%-15s\n", "Level", "Price per person");
		printf("%-15s$%-15.2lf\n", SEAT_LEVEL_PTR[0], prices[0]);
		printf("%-15s$%-15.2lf\n", SEAT_LEVEL_PTR[1], prices[1]);
		printf("%-15s$%-15.2lf\n", SEAT_LEVEL_PTR[2], prices[2]);

		enteredSeatLevel = validateSeatLevel(); //validates entered seat level and stores value returned in enteredSeatLevel

			//if option is C,M, or U the program will continue to ask user for the ticket number. If ticket number is valid, then the program will perform some computations
		if (enteredSeatLevel == ENTRIES[0] || enteredSeatLevel == ENTRIES[1] || enteredSeatLevel == ENTRIES[2]) {

			int ticketsRequest = validateTicketNumber(MIN_NUM_TICKETS, MAX_NUM_TICKETS);

			double currentTotal = 0; //running total of current charge
			unsigned int typeOfTicket; //represents courtside, mid level, or upper level seat as an integer value
			double currentDonatedMoneyToCharity = 0; //running total of amount of money donated to charity

			if (enteredSeatLevel == ENTRIES[0]) { //if seat selection is courtside
				typeOfTicket = 0; // 0 means courtside
				printf("Your cost is $%.2lf. ", prices[typeOfTicket] * ticketsRequest); //displays price due
				totalCostCourtside += prices[typeOfTicket] * ticketsRequest; //increment total spent on courtside tickets
				currentTotal = prices[typeOfTicket] * ticketsRequest; //sets current total of money spent on this purchase
				currentDonatedMoneyToCharity = (percentageForCharity / 100) * currentTotal; //sets current total of money donated to charity
				moneyDonatedC += currentDonatedMoneyToCharity; //increments total money donated to charity from courtside seat purchases
				courtsideCounter += ticketsRequest; //increments number of courtside seats sold by number of current tickets requested
			}
			else if (enteredSeatLevel == ENTRIES[1]) { //repeats the process in first if block for mid level seats
				typeOfTicket = 1;
				printf("Your cost is $%.2lf. ", prices[typeOfTicket] * ticketsRequest);
				totalCostMid += prices[typeOfTicket] * ticketsRequest;
				currentTotal = prices[typeOfTicket] * ticketsRequest;
				currentDonatedMoneyToCharity = (percentageForCharity / 100) * currentTotal;
				moneyDonatedM += currentDonatedMoneyToCharity;
				midCounter += ticketsRequest;
			}
			else if (enteredSeatLevel == ENTRIES[2]) { //repeats the process in first if block for upper level seats
				typeOfTicket = 2;
				printf("Your cost is $%.2lf. ", prices[typeOfTicket] * ticketsRequest);
				totalCostUpper += prices[typeOfTicket] * ticketsRequest;
				currentTotal = prices[typeOfTicket] * ticketsRequest;
				currentDonatedMoneyToCharity = (percentageForCharity / 100) * currentTotal;
				moneyDonatedU += currentDonatedMoneyToCharity;
				upperCounter += ticketsRequest;
			}

			totalTicketSales += currentTotal; //increments total ticket sales cost by the current running total
			totalDonated += currentDonatedMoneyToCharity; //increments total donated money donated by the current amount of money donated to charity

			validateZipCode(); //we validate the user's entry for zipcode

			char validation = DEFAULT_ENTRY;
			while (validation != Y && validation != N) {
				char userReceiptRequest[SIZE_USER_VALIDATION];
				printf("Thank you for purchasing your tickets. $%.2lf of the ticket sales will go to charity.\n", currentDonatedMoneyToCharity);
				puts("Do you want a receipt (y)es or (n)o?"); //prompt the user to if they want a receipt and validate the request
				fgets(userReceiptRequest, SIZE_USER_VALIDATION, stdin);
				validation = validateYOrN(userReceiptRequest);
			}

			if (validation == Y) { //if user wants a receipt, then we print one for the current transaction
				printToReceiptsFile(receiptsFilePath, DENVER_NUGGETS, SEAT_LEVEL_PTR[typeOfTicket], TICKET_LEVELS_PTR[ticketsRequest - 1], currentTotal, currentDonatedMoneyToCharity);
				printReceiptToConsole(DENVER_NUGGETS, SEAT_LEVEL_PTR[typeOfTicket], TICKET_LEVELS_PTR[ticketsRequest - 1], currentTotal, currentDonatedMoneyToCharity);
				
			}


		}

		if (enteredSeatLevel == ENTRIES[3]) { //if the user enters q we will propt for the admin pin

			bool adminEnteredCorrectPinAfterFirstTime = adminSetUp(prices, &percentageForCharity, &adminPinHasBeenValidated);

			if (!adminEnteredCorrectPinAfterFirstTime) { //if the user fails to enter a correct pin in less than 3 tries, we will reset the value of enteredSeatLevel to continue looping.
				enteredSeatLevel = DEFAULT_ENTRY;
			}


		}

	}
	if (!adminSetUpSuccessful) {
		//we will indicate that we are ending the program because the admin failed to enter the correct pin a first time
		puts("Unsuccessful admin log in attempt. Now exiting program.");
	}
	else { //we will display sales totals if the admin was able to successfully log in to the program before entering q and the admin pin again
		printf("\n\n\n%-15s%-16s%-15s%-16s%s\n", "Level", "Price", "Sold", "Sales Total", "Charity Total");
		displayTotals(SEAT_LEVEL_PTR[0], prices[0], courtsideCounter, totalCostCourtside, moneyDonatedC);
		displayTotals(SEAT_LEVEL_PTR[1], prices[1], midCounter, totalCostMid, moneyDonatedM);
		displayTotals(SEAT_LEVEL_PTR[2], prices[2], upperCounter, totalCostUpper, moneyDonatedU);

		printf("\nTotal Ticket Sales: $%.2lf\n", totalTicketSales);
		printf("Total Raised for Charity: $%.2lf", totalDonated);

	}


	return 0;
}


void printReceiptToConsole(const char team[], const char seatLevel[], const char ticketLevel[], const double total, const double moneyDonated) {
	printf("\n\n%s Charity Ticket Receipt\n", team);
	printf("%s\n", seatLevel);
	printf("%s\n", ticketLevel);
	printf("Total Cost: $%.2lf\n", total);
	printf("Donation to Charity: $%.2lf\n\n\n", moneyDonated);
}

void printToReceiptsFile(const char filePath[], const char teamName[], const char seatLevel[], const char ticketLevel[], double currentTotalSpent, double currentDonatedToCharity) {

	FILE* filePtr;

	if ((filePtr = fopen(filePath, "a")) != NULL) {
		
		fprintf(filePtr,"%s Charity Ticket Receipt\n", teamName);
		fprintf(filePtr,"%s\n", seatLevel);
		fprintf(filePtr,"%s\n", ticketLevel);
		fprintf(filePtr,"Total Cost: $%.2lf\n", currentTotalSpent);
		fprintf(filePtr, "Donation to Charity: $%.2lf\n\n\n", currentDonatedToCharity);
		
	}
	else {
		puts("Receipt could not be printed to file");
	}
	fclose(filePtr);
}

//This function generates a file path
void getFilePath(char* finalPath, const char* directoryPath, const char* fileName) {

	strncpy(finalPath, directoryPath, FILE_PATH_LENGTH);
	strncat(finalPath, fileName, FILE_PATH_LENGTH);

}

//This function validates if the user answered y for yes or n for no. If they did not, we will tell them they had invalid entry
char validateYOrN(char entry[]) {

	if (strchr(entry, '\n') == NULL) {
		while ((getchar()) != '\n');
	}

	entry[0] = toupper(entry[0]);

	if (entry[0] != Y && entry[0] != N) {
		puts("Invalid entry");
	}

	return entry[0];
}

//This function allows the admin to set up the values for seats and percentage if a valid pin is entered. 
bool adminSetUp(double pricesArray[], double* const percentPtr, bool* const validatedPinOnce) {

	unsigned int attempts = 0; //keeps track of how many times the admin tries to enter the pin
	char entry[SIZE_ADMIN_PIN]; //character array to hold user entry
	bool successful = false; //boolean if true means that the pin is valid, and we will set up the values for seat level and percentage
	do {

		if (*validatedPinOnce == false) {
			puts("Enter admin pin to set up charity ticket sales ");
		}
		else {
			puts("Enter four digit admin pin: "); //prompts the user to enter a pin
		}

		fgets(entry, SIZE_ADMIN_PIN, stdin); //stores the value the user entered into entry

		if (strchr(entry, '\n') == NULL) {
			while ((getchar()) != '\n'); //clear buffer
		}

		successful = isPinValid(entry, &attempts); //validates pin
		if (successful && !(*validatedPinOnce)) {

			//setting up values for seat level and percentage if the pin is valid and pin has been entered for the first time.
			char userInput[SIZE_NUMERIC_ENTRY];
			bool setUpCourtside = false;
			while (!setUpCourtside) {

				printf("Enter a number for %s \nEntry must be greater than or equal to %d and less than or equal to %d.\n", SEAT_LEVEL_PTR[0], MIN_COURTSIDE_PRICE, MAX_COURTSIDE_PRICE);
				fgets(userInput, SIZE_NUMERIC_ENTRY, stdin);

				if (strchr(userInput, '\n') == NULL) {
					while ((getchar()) != '\n'); //clear buffer
				}

				setUpCourtside = setUpValues(userInput, &pricesArray[0], MIN_COURTSIDE_PRICE, MAX_COURTSIDE_PRICE);
			}

			bool setUpMid = false;
			while (!setUpMid) {

				printf("Enter a number for %s \nEntry must be greater than or equal to %d and less than or equal to %d.\n", SEAT_LEVEL_PTR[1], MIN_MIDLEVEL_PRICE, MAX_MIDLEVEL_PRICE);
				fgets(userInput, SIZE_NUMERIC_ENTRY, stdin);

				if (strchr(userInput, '\n') == NULL) {
					while ((getchar()) != '\n'); //clear buffer
				}

				setUpMid = setUpValues(userInput, &pricesArray[1], MIN_MIDLEVEL_PRICE, MAX_MIDLEVEL_PRICE);
			}
			
			bool setUpUpper = false;
			while (!setUpUpper) {

				printf("Enter a number for %s \nEntry must be greater than or equal to %d and less than or equal to %d.\n", SEAT_LEVEL_PTR[2], MIN_UPPERLEVEL_PRICE, MAX_UPPERLEVEL_PRICE);
				fgets(userInput, SIZE_NUMERIC_ENTRY, stdin);

				if (strchr(userInput, '\n') == NULL) {
					while ((getchar()) != '\n'); //clear buffer
				}

				setUpUpper = setUpValues(userInput, &pricesArray[2], MIN_UPPERLEVEL_PRICE, MAX_UPPERLEVEL_PRICE);
			}

			bool setUpPercent = false;
			while (!setUpPercent) {

				printf("Enter a number for %s \nEntry must be greater than or equal to %d and less than or equal to %d.\n", "percentage", MIN_PERCENT, MAX_PERCENT);
				fgets(userInput, SIZE_NUMERIC_ENTRY, stdin);

				if (strchr(userInput, '\n') == NULL) {
					while ((getchar()) != '\n'); //clear buffer
				}

				setUpPercent = setUpValues(userInput, percentPtr, MIN_PERCENT, MAX_PERCENT);
			}

			attempts = NUM_ADMIN_ATTEMPTS; //end the loop after setting up values
			*validatedPinOnce = true; //pin has been validated for the first time and the values for the program have been set up, we will set that boolean equal to true
		}

		if (successful && (*validatedPinOnce)) { //this if statement represents the case that the pin has been validated once in less than 3 attempts and the admin successfully entered the pin again, so we will stop the loop.
			attempts = NUM_ADMIN_ATTEMPTS;
		}


	} while (attempts < NUM_ADMIN_ATTEMPTS); //looping until the admin has had at most three chances to enter the correct pin

	return successful;
}

//this function sets up a value given a minimum and maximum. It also validates that user entry for this value is valid. 
bool setUpValues(char* entry, double* const priceToFillPtr, int min, int max) {

	bool successful = false;
	char* end; //this will hold any left over characters in the buffer

	errno = 0;


	double value = strtod(entry, &end); //converts user entered string to double, end takes in leftover characters left in the buffer

	entry[strlen(entry) - 1] = '\0'; //gets rid of new line character if there is one

	if (entry == end) {
		//this block executes if there was not a decimal value found because the address stored in end is the same as the address of entry[0]
		fprintf(stderr, "%s: not a decimal number \n\n", entry);
	}
	else if ('\0' != (*end)) {
		//this block executes if end points to a character that is not the null or the new line character which means that the user entered more than just a number
		fprintf(stderr, "%s: extra characters at end of input: %s\n\n", entry, end);

	}
	else if ((DBL_MIN == value || DBL_MAX == value) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type double\n", entry);
	}
	else if (value < min || value > max) {
		//This block executes if the user enters an invalid decimal value
		fprintf(stderr, "%s is out of bounds.\n\n", entry);

	}
	else {
		//The user has entered a valid number, we will update the value stored in priceToFillPtr and exit the loop
		*priceToFillPtr = value;
		successful = true;
	}


	return successful;



} //setUpValues

//this function checks if the pin entered by the admin is valid. It uses a pointer to keep track of admin attempts
bool isPinValid(char* const userEntry, unsigned int* const triesPtr) {

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
	if (strcmp(userEntry, ADMIN_PIN) != 0 && (strlen(userEntry) == 4) && entryIsNumeric) {
		puts("Invalid pin.");
		(*triesPtr)++;
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


//This function validates the user entry of a character
char validateSeatLevel() {

	char userValue[USER_INPUT_SIZE]; //will keep track of value user enters
	bool foundValue = false; //will keep track of whether or not we found a valid entry
	bool validatedSeatLevel = false; //keeps track if the user has validated the seat level they would like to purchase
	int userSelectionAsNumber = -1; //represents the user's selection as a numeric value

	do {
		printf("Enter your seat level: %s, %s or %s\n", SEAT_LEVEL_PTR[0], SEAT_LEVEL_PTR[1], SEAT_LEVEL_PTR[2]);
		fgets(userValue, USER_INPUT_SIZE, stdin);

		if (strchr(userValue, '\n') == NULL) {
			while ((getchar()) != '\n'); //clear the buffer 
		}

		userValue[0] = toupper(userValue[0]); //convert user entry to upper case

		if (userValue[0] == ENTRIES[0]) { //changes the value of userSelection number depending on the character the user enters. this case if user enters c
			userSelectionAsNumber = 0;
		}
		else if (userValue[0] == ENTRIES[1]) { //if user enters m
			userSelectionAsNumber = 1;
		}
		else if (userValue[0] == ENTRIES[2]) { //if user enters u
			userSelectionAsNumber = 2;
		}

		//indexing through array and changing foundValue is a match has been made between value in array and user entry
		for (size_t i = 0; i < NUMBER_OF_ENTRY_OPTIONS; i++) {
			if (ENTRIES[i] == userValue[0]) {
				foundValue = true;
			} //end inner if
		} //end for

		//if the entry has not been found valid yet, we will put out an error
		if (userValue[0] != ENTRIES[0] && userValue[0] != ENTRIES[1] && userValue[0] != ENTRIES[2] && userValue[0] != ENTRIES[3]) {
			puts("Invalid entry");
		}// end if 

		if (foundValue && (!validatedSeatLevel) && userValue[0] != ENTRIES[3] && userSelectionAsNumber != -1) { //we will validate the user's seat selcetion

			char validation = DEFAULT_ENTRY;
			while (validation != Y && validation != N) {
				char userValidation[SIZE_USER_VALIDATION];
				printf("Is selection %s correct (y)es or (n)o?\n", SEAT_LEVEL_PTR[userSelectionAsNumber]);
				fgets(userValidation, SIZE_USER_VALIDATION, stdin);
				validation = validateYOrN(userValidation);
			}

			if (validation == Y) { //the user validated their selection. We will end the loop.
				validatedSeatLevel = true;
			}

			if (validation == N) {
				userSelectionAsNumber = -1;
			}

		}

		if (foundValue && userValue[0] == ENTRIES[3]) { //we will break out of the loop if the user selected q or Q
			validatedSeatLevel = true;
		}


	} while (!foundValue || !validatedSeatLevel); //we will prompt the user until a valid entry is found

	return userValue[0]; //we have found a valid entry from the user, we will return this value to the function calling this method. 

}

//this function validates the number of tickets the user enters and returns that number of tickets as an integer value
int validateTicketNumber(int min, int max) {

	bool validTicketSelection = false; //boolean representing if the ticket selection is valid
	char userEntry[SIZE_NUMERIC_ENTRY];
	char* end;
	errno = 0;
	int value;

	do {
		puts("Enter 1, 2, 3 or 4 to select your package"); //prompts the user to enter a ticket selection and taked in the value to ticketPtr. Clear buffer.
		printf("%s\n", TICKET_LEVELS_PTR[0]);
		printf("%s\n", TICKET_LEVELS_PTR[1]);
		printf("%s\n", TICKET_LEVELS_PTR[2]);
		printf("%s\n", TICKET_LEVELS_PTR[3]);

		fgets(userEntry, SIZE_NUMERIC_ENTRY, stdin);
		if (strchr(userEntry, '\n') == NULL) {
			while ((getchar()) != '\n');
		}
		userEntry[strlen(userEntry) - 1] = '\0';

		long userNumber = strtol(userEntry, &end, 10);

		if (end == userEntry) {
			//if this block executes, then there was not a number found in the first index of userEntry, so end is pointing to userEntry[0] address and so is userEntry
			fprintf(stderr, "%s: not a decimal number \n", userEntry); //indicates that we are printing an error message to the console

		}
		else if ('\0' != ( * end)) {
			//this block executes if end points to a character that is not the null or the new line character. 
			//This block executes if there were characters entered after a number 
			fprintf(stderr, "%s: extra characters at end of input: %s\n", userEntry, end);

		}
		else if ((LONG_MIN == userNumber || LONG_MAX == userNumber) && ERANGE == errno) {
			//This block executes if there was overflow, so the minimum value able to be stored in a long or the maximum value able to be stored in a long equals userNumber.
			//ERANGE will exactly equal errno if there was overflow because the value of errno will be set to the value of ERANGE in that case. 
			// LONG_MIN == userNumber || LONG_MAX == userNumber could be true without overflow, but the condition  ERANGE == errno would clarify if there was overflow or not
			fprintf(stderr, "%s out of range of type long\n", userEntry);
		}
		else if ((userNumber <= INT_MIN || userNumber >= INT_MAX)) {
			//This block executes if userNumber is going to result min integer  or max integer overflow in long to int conversion.
			fprintf(stderr, "%s out of range of type int\n", userEntry);
		}
		else if(userNumber < min || userNumber > max) {
			//user entered a number outside of correct range. We will let them know.
			fprintf(stderr, "%ld is out of bounds\n", userNumber);
		}
		else {

			char validation = DEFAULT_ENTRY;
			while (validation != Y && validation != N) {
				char userValidation[SIZE_USER_VALIDATION];
				printf("Is selection %ld correct (y)es or (n)o ? \n", userNumber);
				fgets(userValidation, SIZE_USER_VALIDATION, stdin);
				validation = validateYOrN(userValidation);
			}

			if (validation == Y) {  
				//the user entered a valid number of tickets and verified their selection. We will end our loop
				validTicketSelection = true;
				value = (int) userNumber;
			}
		}

	} while (!validTicketSelection);

	return value;

}

//this function validates the zipcode from user entry.
void validateZipCode() {

	bool zipValid = false; //boolean representing we were able to take in a valid zipcode
	char zipArray[SIZE_NUMERIC_ENTRY];

	do { //loop that prompts the user to enter a valid zipcode until one is entered given a minimum and maximum zipcode value
		puts("Enter your 5 digit zipcode");
		fgets(zipArray, SIZE_NUMERIC_ENTRY, stdin);

		//if there was a new line character left in the buffer, we will clear it out.
		if (strchr(zipArray, '\n') == NULL) {
			while ((getchar()) != '\n');
		}

		zipArray[strlen(zipArray) - 1] = '\0';

		bool validNumber = isNumber(zipArray);

		if (strlen(zipArray) < ZIP_LENGTH - 1) {
			puts("You did not enter enough characters.");
		}

		if (!validNumber) {
			puts("You did not enter a number.");
		}

		zipValid = (strlen(zipArray) == ZIP_LENGTH - 1 && validNumber);

	} while (!zipValid);
}


void displayTotals(const char* const seat, double price, int amountSold, double salesTotal, double charityTotal) { //displays sales totals per seat level when called.

	printf("%-15s$%-15.2f%-15d$%-15.2f$%-15.2f\n", seat, price, amountSold, salesTotal, charityTotal);
}

