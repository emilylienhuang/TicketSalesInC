/*
Name: Emily Ng
Due: Feb 16, 2022
Learning Activity 05
This program demonstrates my learning on 1D arrays.
*/

#define SIZE 10

#include <stdio.h>
#include <stdbool.h>

void initializeArray(unsigned int numArray[]);
void displayArray(const unsigned int numArray[]);
unsigned int sumElems(const unsigned int numArray[]);


int main(void) {

	int numbers[SIZE];
	initializeArray(numbers);
	displayArray(numbers);
	unsigned int sumNumbers = sumElems(numbers);
	printf("\nSum is %d\n", sumNumbers);

}

//This function initializes the array and fills the array with values the user enters.
void initializeArray(unsigned int numArray[]) {

	bool fullArray = false;
	unsigned int counter = 0;

	//a do while loop will be used to fill values in the array. No need to validate as specified in class.
	do {
		printf("Enter an integer to fill array at index [%d]\n", counter);
		scanf("%d", &numArray[counter]);
		counter++;

		if (counter == SIZE) {
			fullArray = true;
		}

	} while (fullArray == false);
	

}


//this method displays the values in the array. The keyword const is used so the values in the array cannot be changed inside the scope of the function.
void displayArray(const unsigned int numArray[]) {

	puts("");

	for (size_t i = 0; i < SIZE; i++) {
		printf("array at index [%d] is %d\n", i, numArray[i]);
	}
}

//this method sums the elements in the array. The keyword const is used so the values in the array cannot be altered in the scope of the function.
unsigned int sumElems(const unsigned int numArray[]) {
	
	unsigned int sum = 0;

	for (size_t i = 0; i < SIZE; i++) {
		sum += numArray[i];
	}

	return sum;
}