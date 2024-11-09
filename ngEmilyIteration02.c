/*
Emily Ng
Iteration02
Due: April 29, 2022
OS: Windows
Description: This program allows an admin to set up sales for a Nuggets basketball game where a percentage of sales is donated to charity after entering a valid pin in four or less attempts.
The admin enters seat levels, prices for seat levels, and a percentage for charity.  Seat levels are ordered from highest to lowest prices.
Then, the program enters a sales mode for users to buy tickets. 
Users are able to purchase tickets based on the seat levels and prices the admin has set.
Finally, the admin can end the program by entering the pin and sales totals will print.
ENTER q or Q to begin ending the program.
*/
#include <float.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define DIGITS_IN_ADMIN_PIN 4
#define SIZE_ADMIN_PIN 15 //size taken in of user entry for admin pin
#define ADMIN_PIN "4932" //correct admin pin 
#define NUM_ADMIN_ATTEMPTS 4
#define NUMBER_OPTIONS_SEATS 3 //number of options for seats
#define NUMBER_OF_ENTRY_OPTIONS 4 //number of things the user can enter in the program without getting an error
#define NUMBER_TICKET_OPTIONS 4 //number of ticket sale options
#define ZIP_LENGTH 5 //length of zipcode (5 numbers)
#define USER_INPUT_SIZE 2 //defines size for a user choice selection of y or n
#define SIZE_NUMERIC_ENTRY 25 //size of string for numeric entries
#define FILE_PATH_LENGTH 100 //length of string that will hold a file path
#define SEATLEVEL_NAME_LENGTH 50 //length of string that will hold the name of a seat level
#define PRICE "price"
#define PERCENT "percentage"


const int MIN_PERCENT = 10;
const int MAX_PERCENT = 30;
const int MIN_PRICE = 10;
const int MAX_PRICE = 500;
const char DEFAULT_ENTRY = 'Z'; //fills character value, acts as a placeholder for a char variable until the user enters a value to fill it.
const char Y = 'Y'; //represents user enters yes
const char N = 'N'; //represents user enters no
const char* const TICKET_LEVELS_PTR[NUMBER_TICKET_OPTIONS] = { "1: Single Pack (1 ticket)","2: Double Pack (2 tickets)", "3: Triple Pack (3 tickets)", "4: Family Pack (4 tickets)" }; //array holding the strings of ticket levels
const unsigned int MIN_TICKETS = 1;
const unsigned int MAX_TICKETS = 4;
const char DENVER_NUGGETS[] = "Denver Nuggets";
const char DIRECTORY_PATH[] = "C:\\CS2060Files\\"; //file path to a folder
const char RECEIPTS_FILE_NAME[] = "receipt.txt";
const char SALES_SUMMARY_FILE_NAME[] = "salesSummary.txt";

//structure that represents a seat level and the properties of a seat level.
typedef struct seat {
	char name[SEATLEVEL_NAME_LENGTH];
	double price;
	unsigned int totalSold;
	double totalDonated;
	double totalSpent;
	struct seat* nextSeatLevelPtr;
}SeatLevel;

void getFilePath(char* finalPath, const char* const directoryPath, const char* const fileName); //generates a file path
bool isPinValid(); //validates admin pin
bool isNumber(const char* const userEntry); //function that determines if the user enters a string
void createSeatLevelLinkedList(SeatLevel** headPtr); //generates a linked list of seat levels based on admin entry
bool setUpPrice(const char typeValueToSet[], int min, int max, double* const valueToSet); //sets prices for seat levels and percent for charity
char validateYOrN(char entry[]); //validates if the user entered y for yes or n for no
void cleanName(char nameToClean[]); //puts user enters strings in consistent formatting for use in the program
void displaySeatLevelLinkedList(SeatLevel* listPtr, double percentage); //displays linked list of seat levels
SeatLevel* validateSeatLevel(SeatLevel* headPtr, double percentage); //validates that the user selected a valid seat level option or wants to quit the program.
unsigned int validateTicketNumber(int min, int max); //validates the number of tickets the user wants to purchase
void addToSalesRecords(SeatLevel* seatLevelChosen, unsigned int ticketNum, double percent, double* const totalDue, double* const totalForCharity); //talies up information based on seat level from user purchase
void validateZipCode(); //validates user entered zip code
void printReceiptToConsole(const char team[], SeatLevel* seatLevel, const char ticketLevel[], double total, double moneyDonated);
void printToReceiptsFile(const char filePath[], const char teamName[], SeatLevel* seatLevel, const char ticketLevel[], double currentTotalSpent, double currentDonatedToCharity);
void printSalesSummaryToConsole(SeatLevel* listPtr, const char team[], double charityPercent, double totalTicketSales, double totalDonated);
void printSalesSummaryToFile(const char filePath[], SeatLevel* listPtr, const char team[], double charityPercent, double totalTicketSales, double totalDonated);

int main(void) {

	//generates the file paths for the receipts file and sales summary files
	char receiptsFilePath[FILE_PATH_LENGTH];
	getFilePath(receiptsFilePath, DIRECTORY_PATH, RECEIPTS_FILE_NAME);

	char salesTotalSummaryPath[FILE_PATH_LENGTH];
	getFilePath(salesTotalSummaryPath, DIRECTORY_PATH, SALES_SUMMARY_FILE_NAME);

	SeatLevel* headSeatLevelPtr = NULL; //creates a head pointer that will be used to create a seat level linked list.
	bool hasEnded = false; //boolean representing if the program has been terminated
	bool pinIsValid = isPinValid(); // boolean checking if admin has entered correct admin pin to begin setting up sales mode.

	if (pinIsValid) {

		createSeatLevelLinkedList(&headSeatLevelPtr); //creates seat level linked list

		//sets percentage for charity
		double percentForCharity;
		bool setPercentage = false;
		while (!setPercentage) {
			setPercentage = setUpPrice(PERCENT, MIN_PERCENT, MAX_PERCENT, &percentForCharity);
		}

		double totalCollected = 0; //total money spent while in sales mode
		double totalDonated = 0; //total money donated in sales mode

		do {

			SeatLevel* chosenSeatLevel = validateSeatLevel(headSeatLevelPtr, percentForCharity); //checks if user enters a valid selection for seat level or has indicated that they want to quit.

			if (chosenSeatLevel != NULL) {
				//the user has selected a valid seat level, we will continue sales mode and ask them whick ticket package they would like to purchase.
				printf("Your seat level is %s and the price per seat is %.2lf\n", chosenSeatLevel->name, chosenSeatLevel->price);
				unsigned int numTickets = validateTicketNumber(MIN_TICKETS, MAX_TICKETS);
				double currentCharged = 0; //current balance due in current transaction
				double currentDonated = 0; //current amount donated in current transaction
				addToSalesRecords(chosenSeatLevel, numTickets, percentForCharity, &currentCharged, &currentDonated); //changes values in seat level chosen based off of user's current transaction
				//increment the total collected and the total donated by the current spent and the current donated
				totalCollected += currentCharged;
				totalDonated += currentDonated;
				printf("Your cost is $%.2lf.\n", currentCharged);
				validateZipCode();

				//asks user if they want a receipt until a valid entry, then continues to next sale
				char validation = DEFAULT_ENTRY;
				while (validation != Y && validation != N) {
					char userReceiptRequest[USER_INPUT_SIZE];
					printf("Thank you for purchasing your tickets. $%.2lf of the ticket sales will go to charity.\n", currentDonated);
					puts("Do you want a receipt (y)es or (n)o?"); //prompt the user to if they want a receipt and validate the request
					fgets(userReceiptRequest, USER_INPUT_SIZE, stdin);
					validation = validateYOrN(userReceiptRequest);
				}

				if (validation == Y) { //if user wants a receipt, then we print one for the current transaction
					printToReceiptsFile(receiptsFilePath, DENVER_NUGGETS, chosenSeatLevel, TICKET_LEVELS_PTR[numTickets - 1], currentCharged, currentDonated);
					printReceiptToConsole(DENVER_NUGGETS, chosenSeatLevel, TICKET_LEVELS_PTR[numTickets- 1], currentCharged, currentDonated);
				}


			}
			else{
				//we will end sales mode because the admin has successfully ended the program
				puts("Sales mode has ended.");
				hasEnded = true;
			}
		} while (!hasEnded);

		//print summary of sales mode to console and to file.
		printSalesSummaryToConsole(headSeatLevelPtr, DENVER_NUGGETS, percentForCharity,totalCollected, totalDonated);
		printSalesSummaryToFile(salesTotalSummaryPath, headSeatLevelPtr, DENVER_NUGGETS, percentForCharity, totalCollected, totalDonated);
	}
	else {
		puts("Unsuccessful admin log in attempt. The program will now end.");
	}
	return 0;
}

//This function generates a file path
void getFilePath(char* finalPath, const char* const directoryPath, const char* const fileName) {

	strncpy(finalPath, directoryPath, FILE_PATH_LENGTH);
	strncat(finalPath, fileName, FILE_PATH_LENGTH);

}

//this function checks if the pin entered by the admin is valid. It uses a pointer to keep track of admin attempts
bool isPinValid() {

	unsigned int attempts = 0; //keeps track of admin attempts to enter a valid pin
	bool validPin = false; //boolean later returned to let us know if the admin entered a valid pin
	
	do {
		char userEntry[SIZE_ADMIN_PIN];
		puts("Enter four digit admin pin: "); //prompts the user to enter a pin
		fgets(userEntry, SIZE_ADMIN_PIN, stdin);
		attempts++;

		//if there's some characters left in the buffer, we will clear the buffer
		if (strchr(userEntry, '\n') == NULL) {
			while ((getchar()) != '\n');
		}

		//if there's a new line character in the user entry collected, we will replace it with the null character.
		if (userEntry[strlen(userEntry) - 1] =='\n') {
			userEntry[strlen(userEntry) - 1] = '\0';
		}

		bool entryIsNumeric = isNumber(userEntry); //checks if userEntry is in fact a number

		//if user entry is not a number, we need to let the admin know. 
		if ((!entryIsNumeric)) {
			printf("%s\n", "You did not enter a number");
		}

		//the admin entered a number less than the needed length. We will let them know. 
		if (entryIsNumeric && (strlen(userEntry) < DIGITS_IN_ADMIN_PIN)) {
			printf("%s", "You did not enter enough numbers.\n");
		}
		 
		//the admin entered too many numbers, we will let them know.
		if (entryIsNumeric && (strlen(userEntry) > DIGITS_IN_ADMIN_PIN)) {
			printf("%s", "You entered too many numbers.\n");
		}

		//the user entered a valid pin, we will update our boolean keeping track of valid pin
		if (strcmp(userEntry, ADMIN_PIN) == 0) {
			validPin = true;
			attempts = NUM_ADMIN_ATTEMPTS; //ending the loop
		}

		//the user entered enough numbers, but the pin is invalid. We will let them know. 
		if (strcmp(userEntry, ADMIN_PIN) != 0 && (strlen(userEntry) == 4) && entryIsNumeric) {
			puts("Invalid pin.");
		}
	} while (attempts < NUM_ADMIN_ATTEMPTS);

	return validPin;
}

//this function checks if the user entered a number
bool isNumber(const char* const userEntry) {

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

//this function creates a linked list of seat levels based on the admin's entry
void createSeatLevelLinkedList(SeatLevel** headPtr) {
	unsigned int pass = 0; //indicated if we are in the first time of running through the loop
	char userWantsToContinue = DEFAULT_ENTRY; // character the admin will enter to represent that they do or don't want to continue adding seat levels.
	do {
		if (pass == 0) {
			puts("Enter the seat level names and prices");
			pass++;
			userWantsToContinue = Y;
		}
		else {
			//asks the admin if they want to enter another seat level and price until they enter a y or an n
			bool validatedCorrectly = false;
			while (!validatedCorrectly) {
				printf("%s", "Do you want to add another seat level and price, (y)es or (n)o?");
				char userValidation[USER_INPUT_SIZE];
				fgets(userValidation, USER_INPUT_SIZE, stdin);
				userWantsToContinue = validateYOrN(userValidation);
				validatedCorrectly = (userWantsToContinue == Y || userWantsToContinue == N);
			}
		}

		if (userWantsToContinue == Y) {
			//the admin wants to add a seat level, we will ask them for a name and a price.
			printf("%s", "Enter seat level name ");
			char nameAdding[SEATLEVEL_NAME_LENGTH];
			fgets(nameAdding, SEATLEVEL_NAME_LENGTH, stdin);
			cleanName(nameAdding);

			double priceAdding;
			bool setPriceAdding = false;
			while (!setPriceAdding) {
				//loops until admin enters a valid price
				setPriceAdding = setUpPrice(PRICE, MIN_PRICE, MAX_PRICE, &priceAdding);
			}

			SeatLevel* nodeToAddPtr = malloc(sizeof(SeatLevel)); //allocates memory for a node of the linked list
			if (nodeToAddPtr != NULL) {
				//continue adding seat level into linked list
				//This block executes if there was memory able to be allocated for the creation of a node

			//creating node
				strcpy(nodeToAddPtr->name, nameAdding);
				nodeToAddPtr->price = priceAdding;
				nodeToAddPtr->totalSold = 0;
				nodeToAddPtr->totalDonated = 0;
				nodeToAddPtr->totalSpent = 0;
				nodeToAddPtr->nextSeatLevelPtr = NULL;

				SeatLevel* previousPtr = NULL;
				SeatLevel* currentPtr = *headPtr;

				if (*headPtr == NULL) {
					//if there is nothing in the linked list, we will set our node to add to the head of the linked list
					*headPtr = nodeToAddPtr;
				}
				else {
					while (currentPtr != NULL && nodeToAddPtr->price < currentPtr->price) {
						//moves previous and current until we have found the correct position of node to add in the linked list.
						previousPtr = currentPtr;
						currentPtr = currentPtr->nextSeatLevelPtr;
					}

					if (currentPtr == NULL) {
						//this block executes if we are adding our node to add to the end of the list
						previousPtr->nextSeatLevelPtr = nodeToAddPtr;
					}
					else if (currentPtr == *headPtr) {
						//the insertion needs to be at the head of the linked list. we will place node to add pointer at the head
						*headPtr = nodeToAddPtr;
					}
					else {
						//This block executes if we have to insert a node in the middle of the linked list.
						previousPtr->nextSeatLevelPtr = nodeToAddPtr;
					}

					//we will set node to add's next seat level field to the value stored in current pointer.
					nodeToAddPtr->nextSeatLevelPtr = currentPtr;
				}
				}
			}

		} while (userWantsToContinue == Y);
	} 


	//this function sets up a value given a minimum and maximum. It also validates that user entry for this value is valid. 
	bool setUpPrice(const char typeValueToSet[],int min, int max, double * const valueToSet) {

		char entry[SIZE_NUMERIC_ENTRY]; //will save user input as a string
		char* end; //this will hold any left over characters in the buffer
		double value; //will store the translated value from strtod
		errno = 0; //constant whose value will change based off what happens during strtod
		bool successful = false; //represents if valueToSet was successfully set
		
			printf("Enter a number for %s \nEntry must be greater than or equal to %d and less than or equal to %d.\n", typeValueToSet, min, max);
			fgets(entry, SIZE_NUMERIC_ENTRY, stdin);

			if (strchr(entry, '\n') == NULL) {
				while ((getchar()) != '\n'); //clear buffer if there are characters left over in the buffer
			}

			value = strtod(entry, &end); //converts user entered string to double, end takes in leftover characters left in the buffer

			if (entry[strlen(entry) - 1] =='\n') {
				entry[strlen(entry) - 1] = '\0'; //gets rid of new line character if there is one
			}

			if (entry == end) {
				//this block executes if there was not a decimal value found because the address stored in end is the same as the address of entry[0]
				fprintf(stderr, "%s: not a decimal number \n\n", entry);
			}
			else if ('\0' != (*end)) {
				//this block executes if end points to a character that is not the null character which means that the user entered more than just a number
				fprintf(stderr, "%s: extra characters at end of input: %s\n\n", entry, end);

			}
			else if ((DBL_MIN == value || DBL_MAX == value) && ERANGE == errno) {
				//this block executes if there was overflow.
				fprintf(stderr, "%s out of range of type double\n", entry);
			}
			else if (value < min) {
				//This block executes if the user enters a value less than the minimum value
				fprintf(stderr, "%s is less than %d.\n\n", entry, min);

			}
			else if (value > max) {
				//this block executes if the user enters a value greater than the maximum value
				fprintf(stderr, "%s is greater than %d.\n\n", entry, max);
			}
			else {
				//The user has entered a valid number, we will change the value to set and return true.
				successful = true;
				(*valueToSet) = value;
			}

	
		return successful;
	} //setUpPrice

	//This function validates if the user answered y for yes or n for no. If they did not, we will tell them they had invalid entry
	char validateYOrN(char entry[]) {

			if (strchr(entry, '\n') == NULL) {
				while ((getchar()) != '\n'); //clears buffer if characters left in it
			}

			entry[0] = toupper(entry[0]); 

			if (entry[0] != Y && entry[0] != N) {
				puts("Invalid entry");
			}

		return entry[0];
	}

	//this function puts user entered name in its correct format
	void cleanName(char nameToClean[]) {

		//clear the buffer if there are left over characters in it
		if (strchr(nameToClean, '\n') == NULL) {
			while (getchar() != '\n');
		}

		if (nameToClean[strlen(nameToClean) - 1] =='\n') {
			//changes new line character to null character if there is one.
			nameToClean[strlen(nameToClean) - 1] = '\0';
		}

		for (unsigned int i = 0; i < strlen(nameToClean); i++) {
			nameToClean[i] = tolower(nameToClean[i]);
		}
		nameToClean[0] = toupper(nameToClean[0]);

		//nameToClean will come out of this function with the first letter capitalized, all other characters lower case.
	}

	//this function displays the linked list of seat levels from highest price to lowest
	void displaySeatLevelLinkedList(SeatLevel* listPtr, double percentage) {

		if (listPtr != NULL) {

			SeatLevel * currentPtr = listPtr;

			puts("");
			printf("Here are the availablie seat levels and prices. %.2lf%% of your purchase will be donated to charity.\n", percentage);

			while (currentPtr != NULL) {
				printf("%s: %.2lf\n",currentPtr->name, currentPtr->price);
				currentPtr = currentPtr->nextSeatLevelPtr;
			}
		}
		else {
			puts("List is Empty");
		}
	}

	//this function returns a pointer to a node in the linked list of seat levels based on user selection
	SeatLevel* getSeatLevel(SeatLevel* headPtr, char  seatLevelSearchingFor[], double percent){
		
		SeatLevel* foundPtr = headPtr; //will store the address of the seat level node that the user wants to purchase tickets from
		bool found = false; //represents finding the seat level the user is looking for or not
		do {
			displaySeatLevelLinkedList(headPtr, percent);

			//takes in user input for seat level selection and puts it in proper format
			printf("Enter your seat level name you want to select ");
			fgets(seatLevelSearchingFor, SEATLEVEL_NAME_LENGTH, stdin);

			if (strchr(seatLevelSearchingFor,'\n') == NULL){
				while ((getchar()) != '\n'); //clear the buffer 
			}

			cleanName(seatLevelSearchingFor);

			if (strcmp(seatLevelSearchingFor, "Q") == 0) {
				//the admin wants to end salesmode. We will end the loop and return null 
				foundPtr = NULL;
				found = true;
			}
			else {

				if (headPtr != NULL) {

					//we will search for the seat level the user has selected until we find the seat level, or the seat level cannot be found.
					SeatLevel* currentPtr = headPtr;

					while (currentPtr != NULL && strcmp(currentPtr->name, seatLevelSearchingFor) != 0) {
						currentPtr = currentPtr->nextSeatLevelPtr;
					}

					if (currentPtr == NULL) {
						puts("");
						puts("Seat Level not found.");
						foundPtr = NULL;
					}
					else {
						//we found the seat level the user wants. We will return a pointer to the seat level.
						foundPtr = currentPtr;
						found = true;
					}

				}
				else {
					//this block executes if there are no seat levels in the linked list. 
					puts("Our apologies. No more seats available for purchase.");
				}


			}
		} while (!found);

		return foundPtr; 
	}

	//This function validates the user entry of a character
	SeatLevel * validateSeatLevel(SeatLevel *headPtr, double percentage){

		
		bool validatedSeatLevel = false; //keeps track if the user has validated the seat level they would like to purchase
		SeatLevel* seatLevelChosenPtr = NULL; //will store a pointer to the seat level the user selects.
		bool adminEnd = false; //represents if the admin wants to end the program

		do {
			
			//asks user for seat level
			char userEntry[SEATLEVEL_NAME_LENGTH];
			seatLevelChosenPtr = getSeatLevel(headPtr, userEntry, percentage); //valid seat level user wants to collect. This value is null if admin wants to quit

			if (seatLevelChosenPtr != NULL) {

				//asks user to validate their choice of seat level until they enter y or n
				char validation = DEFAULT_ENTRY;
				do{
					char userValidation[USER_INPUT_SIZE];
					printf("Is selection %s correct (y)es or (n)o?\n", userEntry);
					fgets(userValidation, USER_INPUT_SIZE, stdin);
					validation = validateYOrN(userValidation);
				} while ((validation != Y && validation != N));

				if (validation == Y) { //the user validated their selection. We will end the loop.
					validatedSeatLevel = true;
				}
			}
			else {
				//the user has chosen quit. We will ask for admin pin
				adminEnd = isPinValid();
			}

		} while (!validatedSeatLevel && !adminEnd); //we will prompt the user until a valid entry is found

		return seatLevelChosenPtr; //we have found a valid entry from the user, we will return this value to the function calling this method. 

	}

	//this function validates the number of tickets the user enters and returns that number of tickets as an integer value
	unsigned int validateTicketNumber(int min, int max) {

		bool validTicketSelection = false; //boolean representing if the ticket selection is valid
		char* end; //will store the characters if any left over from the conversion of strtol
		errno = 0; //variable whose value will change based on the value taken in by strtol
		unsigned int value; //will store the value to be returned by the function once the user enters a valid positive integer.s

		do {
			puts("Enter 1, 2, 3 or 4 to select your package"); //prompts the user to enter a ticket selection and taked in the value to ticketPtr. Clear buffer.
			printf("%s\n", TICKET_LEVELS_PTR[0]);
			printf("%s\n", TICKET_LEVELS_PTR[1]);
			printf("%s\n", TICKET_LEVELS_PTR[2]);
			printf("%s\n", TICKET_LEVELS_PTR[3]);

			//takes in user entry and cleans it
			char userEntry[SIZE_NUMERIC_ENTRY];
			fgets(userEntry, SIZE_NUMERIC_ENTRY, stdin);
			if (strchr(userEntry, '\n') == NULL) {
				while ((getchar()) != '\n');
			}

			if (userEntry[strlen(userEntry) - 1] == '\n') {
				userEntry[strlen(userEntry) - 1] = '\0';
			}
			
			//converts user entered string to a long
			long userNumber = strtol(userEntry, &end, 10);

			if (end == userEntry) {
				//if this block executes, then there was not a number found in the first index of userEntry, so end is pointing to userEntry[0] address and so is userEntry
				fprintf(stderr, "%s: not a decimal number \n", userEntry); //indicates that we are printing an error message to the console

			}
			else if ('\0' != (*end)) {
				//this block executes if end points to a character that is not the null character. 
				//This block executes if there were characters entered after a number 
				fprintf(stderr, "%s: extra characters at end of input: %s\n", userEntry, end);

			}
			else if ((LONG_MIN == userNumber || LONG_MAX == userNumber) && ERANGE == errno) {
				//This block executes if there was overflow, so the minimum value able to be stored in a long or the maximum value able to be stored in a long equals userNumber.
				//ERANGE will exactly equal errno if there was overflow because the value of errno will be set to the value of ERANGE in that case. 
				// LONG_MIN == userNumber || LONG_MAX == userNumber could be true without overflow, but the condition  ERANGE == errno would clarify if there was overflow or not
				fprintf(stderr, "%s out of range of type long\n", userEntry);
			}
			else if ((userNumber < 0 || userNumber >= UINT_MAX)) {
				//This block executes if userNumber is going to result min integer  or max integer overflow in long to int conversion.
				fprintf(stderr, "%s out of range of type unsigned int\n", userEntry);
			}
			else if (userNumber < min || userNumber > max) {
				//user entered a number outside of correct range. We will let them know.
				fprintf(stderr, "%ld is out of bounds of ticket package options.\n", userNumber);
			}
			else {

				//the user has entered a valid selection. We will ask them to validate it.
				char validation = DEFAULT_ENTRY;
				while (validation != Y && validation != N) {
					char userValidation[USER_INPUT_SIZE];
					printf("Is selection %ld correct (y)es or (n)o ? \n", userNumber);
					fgets(userValidation, USER_INPUT_SIZE, stdin);
					validation = validateYOrN(userValidation);
				}

				if (validation == Y) {
					//the user entered a valid number of tickets and verified their selection. We will end our loop
					validTicketSelection = true;
					value = (unsigned int)userNumber;
				}
			}

		} while (!validTicketSelection);

		return value;

	}

	//this function alters the values stored inside a seat level structure after the user clarifies what they are purchasing.
	void addToSalesRecords(SeatLevel * seatLevelChosen, unsigned int ticketNum, double percent, double *const totalDue, double * const totalForCharity) {
		*totalDue = (seatLevelChosen->price) * ticketNum; //sets the current total amount due
		*totalForCharity = (*totalDue) * (percent / 100); //sets the current amount donated to charity for the given sale

		seatLevelChosen->totalSold += ticketNum; //incrementing the number of tickets sold in given seat level
		seatLevelChosen->totalSpent += *totalDue; //incrementing the total amount spent per seat level
		seatLevelChosen->totalDonated += *totalForCharity; //incrementing the total amount donated to charity per seat level.
	}

	//this function validates the zipcode from user entry.
	void validateZipCode() {

		bool zipValid = false; //boolean representing we were able to take in a valid zipcode
		char zipArray[SIZE_NUMERIC_ENTRY]; //stores the user's entry for zip code as a string

		do { //loop that prompts the user to enter a valid zipcode until one is entered given a minimum and maximum zipcode value
			puts("Enter your 5 digit zipcode");
			fgets(zipArray, SIZE_NUMERIC_ENTRY, stdin);

			//we will clean up the user's entry for zip code.
			if (strchr(zipArray, '\n') == NULL) {
				while ((getchar()) != '\n');
			}

			if (zipArray[strlen(zipArray) - 1] == '\n') {
				zipArray[strlen(zipArray) - 1] = '\0';
			}
			

			bool validNumber = isNumber(zipArray); //checks if user entered a number

			if (strlen(zipArray) < ZIP_LENGTH) {
				puts("You did not enter enough characters.");
			}

			if (!validNumber) {
				puts("You did not enter a number.");
			}

			if (strlen(zipArray) > ZIP_LENGTH && validNumber) {
				puts("You entered too many numbers.");
			}

			zipValid = (strlen(zipArray) == ZIP_LENGTH && validNumber);

		} while (!zipValid);
	}

	//this function prints a receipt to the console
	void printReceiptToConsole(const char team[], SeatLevel * seatLevel, const char ticketLevel[], double total, double moneyDonated) {
		printf("\n\n%s Charity Ticket Receipt\n", team);
		printf("%s\n", seatLevel->name);
		printf("%s\n", ticketLevel);
		printf("Total Cost: $%.2lf\n", total);
		printf("Donation to Charity: $%.2lf\n\n\n", moneyDonated);
	}

	//this function prints a receipt to a file
	void printToReceiptsFile(const char filePath[], const char teamName[], SeatLevel * seatLevel, const char ticketLevel[], double currentTotalSpent, double currentDonatedToCharity) {

		FILE* filePtr;

		if ((filePtr = fopen(filePath, "a")) != NULL) {
			//writes a receipt to a file if able to open it
			fprintf(filePtr, "%s Charity Ticket Receipt\n", teamName);
			fprintf(filePtr, "%s\n", seatLevel->name);
			fprintf(filePtr, "%s\n", ticketLevel);
			fprintf(filePtr, "Total Cost: $%.2lf\n", currentTotalSpent);
			fprintf(filePtr, "Donation to Charity: $%.2lf\n\n\n", currentDonatedToCharity);

		}
		else {
			puts("Receipt could not be printed to file");
		}
		fclose(filePtr);
	}
	
	//this function prints the sales summary to the console after sales mode is complete
	void printSalesSummaryToConsole(SeatLevel * listPtr, const char team[], double charityPercent, double totalTicketSales, double totalDonated) {
		

		if (listPtr != NULL) {
			//prints out seat level information if there are seat levels in the linked list
			fprintf(stdout, "%s\n","-----------------------------------------------------------------------------------------------------------");
			fprintf(stdout, "Summary of %s Ticket Sales with %.0lf%% Charity Donation:\n", team, charityPercent);
			fprintf(stdout, "%s\n", "-----------------------------------------------------------------------------------------------------------");

			printf("%-15s%-16s%-15s%-16s%s\n", "Level", "Price", "Sold", "Sales Total", "Charity Total");
			SeatLevel* currentPtr = listPtr;
			while (currentPtr != NULL) {
				fprintf(stdout, "%-15s$%-15.2f%-15d$%-15.2f$%-15.2f\n", currentPtr->name, currentPtr->price, currentPtr->totalSold, currentPtr->totalSpent, currentPtr->totalDonated);
				currentPtr = currentPtr->nextSeatLevelPtr;
			}
			printf("\nTotal Ticket Sales: $%.2lf\n", totalTicketSales);
			printf("Total Raised for Charity: $%.2lf\n", totalDonated);
		}
		else {
			puts("No seat level sales to show.");
		}

	}

	//this function prints sales summary to a file after sales mode is complete.
	void printSalesSummaryToFile(const char filePath[], SeatLevel* listPtr, const char team[], double charityPercent, double totalTicketSales, double totalDonated) {

		FILE* filePtr;
		if ((filePtr = fopen(filePath, "w")) != NULL) {
			//will write to file if we could open it
			fprintf(filePtr, "%s\n", "-----------------------------------------------------------------------------------------------------------");
			fprintf(filePtr, "Summary of %s Ticket Sales with %.0lf%% Charity Donation:\n", team, charityPercent);
			fprintf(filePtr, "%s\n", "-----------------------------------------------------------------------------------------------------------");
			fprintf(filePtr, "%-15s%-16s%-15s%-16s%s\n", "Level", "Price", "Sold", "Sales Total", "Charity Total");

			if (listPtr != NULL) {
				//will print out seat level information if there are seat levels in the linked list.
				SeatLevel* currentPtr = listPtr;
				while (currentPtr != NULL) {
					fprintf(filePtr, "%-15s$%-15.2f%-15d$%-15.2f$%-15.2f\n", currentPtr->name, currentPtr->price, currentPtr->totalSold, currentPtr->totalSpent, currentPtr->totalDonated);
					currentPtr = currentPtr->nextSeatLevelPtr;
				}
			}
			else {
				puts("No seat level sales to show.");
			}

			fprintf(filePtr, "\nTotal Ticket Sales: $%.2lf\n", totalTicketSales);
			fprintf(filePtr, "Total Raised for Charity: $%.2lf\n", totalDonated);
			fclose(filePtr);

		}
		else {
			puts("Sales summary could not be written to file.");
		}

		
	}