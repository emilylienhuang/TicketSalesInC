/*
* Name: Emily Ng
* Class: CS2060 T/Th
* HW07 Linked Lists
* Due: April 21, 2022
* This assignment uses user input to create a linked list of pets, display the linked list, and delete nodes.
* This assignment also uses structures to create Pets.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


#define NAME_LENGTH 50//why can't I use this in my structure?
#define USER_INPUT_LENGTH 15
#define PET_QUESTION "How many pets do you want to enter ? "
#define AGE_QUESTION "Enter age "

typedef struct pet {
	char name[NAME_LENGTH];
	unsigned int age;
	struct pet* nextPtr;
}Pet;

unsigned int convertUserEntry(const char question[]); //converts user entered string to number if possible and takes in user input. Use unsigned int because age and number of pets to enter cannot be negative
void fillLinkedList(unsigned int numEntered, Pet** headPtr); //function that fills a linked list with Pet structures
void displayLinkedList(Pet* headPtr); //displays the linked list of Pets
void deleteNode(Pet** headPtr, const char stringSearchFor[]); //deletes a node in the linked list of pets
void cleanName(char nameToClean[]); //puts user entered name in correct format

int main(void) {

	Pet* headPtr = NULL; //initializing headPtr to null value for linked list

	unsigned int numPets = convertUserEntry(PET_QUESTION); //asks user for number of pets to enter and saves answer to a variable

	fillLinkedList(numPets, &headPtr); //fills Pet linked list

	displayLinkedList(headPtr); //displays pet linked list

	bool done = false;

	//loops while user wants to delete pets from the list or the list is empty
	do {
		char answer;

		char userEntry[USER_INPUT_LENGTH];
		printf("\n%s ", "Do you want to remove a pet (y)es or (n)o?");
		fgets(userEntry, USER_INPUT_LENGTH, stdin);
		if (strchr(userEntry, '\n') == NULL) {
			//clears buffer if there is anything left in it
			while ((getchar()) != '\n');
		}

		answer = toupper(userEntry[0]);

		if (answer == 'Y') {
			//the user wants to delete a name, we will ask them for the name to delete, delete it from the linked list, and display the new linked list.
			char nameToDelete[USER_INPUT_LENGTH];
			printf("%s ", "Enter the name of who should be removed");
			fgets(nameToDelete, USER_INPUT_LENGTH, stdin);
			cleanName(nameToDelete);
			deleteNode(&headPtr, nameToDelete);
			displayLinkedList(headPtr);
		}

		done = (headPtr == NULL || answer == 'N');

	} while (!done);

}

//this function takes in user entry and returns its value as a valid integer.
unsigned int convertUserEntry(const char question[]) {

	bool valid = false; //represents if the user entered a valid string to represent a number or not
	char userInput[USER_INPUT_LENGTH]; //string that stored what the user will enter
	char* end; //will store any left over characters from user entry
	unsigned int value; //will be returned as the value to return once user input has been validated

	while (!valid) {

		printf("%s", question);
		fgets(userInput, USER_INPUT_LENGTH, stdin);

		//if there is no new line in userInput, we will clear the buffer
		if (strchr(userInput, '\n') == NULL) {
			while ((getchar()) != '\n');
		}

		//if there is a new line in userInput, then we will remove it
		if (userInput[strlen(userInput) - 1] =='\n'){
			userInput[strlen(userInput) - 1] = '\0';
		}

		long userNumber = strtol(userInput, &end, 10);

		if (end == userInput) {
			//the user didn't enter a number. We will let them know.
			puts("You did not enter a number");
		}
		else if ('\0' != *end) {
			printf("%s has extra characters %s\n", userInput, end);
		}
		else {
			value = (unsigned int)userNumber;
			valid = true;
		}

	}

	return value;
}

//this function fills the linked list with Pet
void fillLinkedList(unsigned int numEntered, Pet** headPtr) {

	printf("Enter the names and ages of %d pets\n", numEntered);

	//loops for as many pets as the user wants in the linked list.
	for (unsigned int i = 0; i < numEntered; i++) {

		char nameAdding[USER_INPUT_LENGTH];
		printf("%s", "Enter name ");
		fgets(nameAdding, USER_INPUT_LENGTH, stdin);
		cleanName(nameAdding);

		unsigned int ageAdding = convertUserEntry(AGE_QUESTION);

		Pet* nodeToAddPtr = malloc(sizeof(Pet)); //allocating memory for a Pet node to add to the linked list

		if (nodeToAddPtr != NULL) {
			//This block executes if there was memory able to be allocated for the creation of a node

			//creating node
			strcpy(nodeToAddPtr->name, nameAdding);
			nodeToAddPtr->age = ageAdding;
			nodeToAddPtr->nextPtr = NULL;

			//this block executes if we need to search for the correct position to add in the node alphabetically into the linked list. 
			Pet* previousPtr = NULL;
			Pet* currentPtr = *headPtr;

			while (currentPtr != NULL && strcmp(currentPtr->name, nodeToAddPtr->name) < 0) {
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextPtr;
			}

			if (previousPtr == NULL) {
				//this block executes if we are adding our node to add to the end of the list
				*headPtr = nodeToAddPtr;
			}
			else {
				//This block executes if we have to insert a node in the middle of the linked list.
				previousPtr->nextPtr = nodeToAddPtr;
			}
			nodeToAddPtr->nextPtr = currentPtr;
		}
		else {
			puts("No memory available for node creation.");
			
		}
	

	}

	}
		





//this function displays the linked list of pets in alphabetical order.
void displayLinkedList(Pet* listPtr) {

	if (listPtr != NULL) {

		Pet* currentPtr = listPtr;

		puts("");
		puts("The names in alphabetical order: ");

		while (currentPtr != NULL) {
			printf("%s is %u years old\n", currentPtr->name, currentPtr->age);
			currentPtr = currentPtr->nextPtr;
		}
	}
	else {
		puts("List is Empty");
	}
}

//this function deletes a node from the linked list. 
void deleteNode(Pet** headPtr, const char stringSearchFor[]) {

	Pet* previousPtr = NULL;
	Pet* currentPtr = *headPtr;
	if (*headPtr != NULL) {
		if (strcmp((*headPtr)->name, stringSearchFor) == 0) {
			*headPtr = (*headPtr)->nextPtr;
			free(currentPtr); //deallocating the memory taken up by the current node 
			currentPtr = NULL; //effectively "deleting" the node
		}
		else {

			while (currentPtr != NULL && strcmp(currentPtr->name, stringSearchFor) != 0) {
				//looping until currentPtr is null or currentPtr is pointing to a node to delete
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextPtr;
			}

			if (currentPtr == NULL) {
				//we did not find the pet name, so we will let the user know
				puts("Pet not in list");
			}
			else {
				//we found the pet that needs to be removed. We will remove it.
				previousPtr->nextPtr = currentPtr->nextPtr;
				free(currentPtr);
				currentPtr = NULL;
			}
		}
	}

}

//this function puts user entered name in its correct format
void cleanName(char nameToClean[]) {

	//clear the buffer if there are left over characters in it
	if (strchr(nameToClean, '\n') == NULL) {
		while (getchar() != '\n');
	}

	if (nameToClean[strlen(nameToClean) - 1] =='\n') {
		nameToClean[strlen(nameToClean) - 1] = '\0';
	}

	for (unsigned int i = 0; i < strlen(nameToClean); i++) {
		nameToClean[i] = tolower(nameToClean[i]);
	}
	nameToClean[0] = toupper(nameToClean[0]);
}