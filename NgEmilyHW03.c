/*
Emily Ng
CS2060 TTh
OS: Windows
HW03
Due: 02/15/22
Description: This program helps calculate daily board costs for a paddle boarding company.
*/

//we will import some libraries we will use in our program.
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

//we will declare some global constants representing different charges and important hoursand entries to determine charge.
const double MINIMUM_CHARGE = 35.0;
const double ADDITIONAL_CHARGE = 10.0;
const double MAXIMUM_CHARGE = 75.0;
const int SENTINEL_VALUE= -1;
const unsigned int ZERO = 0;
const unsigned int SIX = 6;
const unsigned int MINIMUM_CHARGE_HOURS = 2;
const unsigned int MAX_HOURS = 10;

//we will write some function prototypes
void promptUserForEntry(void);
double validateScannedValueAsNumeric(int scannedValue, double givenValue);
double validateNumber(double givennValue);
double calculateHours(double time, double newTime);
double calculateCharge(double time);
void printCurrentOutput(int boards, double time, double cost);
void displayTotals(int boardNumber, double time, double runningCharge);

int main(void) {

	//we will initialize a board counter, hours counter, charge counter, and total charge counter.
	unsigned int boardCounter = 0;
	double hours = 0;
	double charge = 0;
	double totalCharge = 0;

	//we will also initialize a variable called entry that will track what the user enters and set it equal to 0
	double entry = 0;
	bool done = false;

	do {
		//we will ask the user for input 
		promptUserForEntry();
		int userEntry = scanf("%lf", &entry);
		while ((getchar()) != '\n');

		//we will validate that the user entered a numeric value
		entry = validateScannedValueAsNumeric(userEntry, entry);

		//we will validate that the user entered a numeric value within the appropriate range
		entry = validateNumber(entry);
		
		//we will change our boolean value given the value of entry to end the loop when necessary
		done = (entry == SENTINEL_VALUE);

		//now that entry is validated, we will perform some calculations until the user wants to exit the program 
		if(entry > ZERO && entry <= MAX_HOURS){

			//we will increment our counters and calculate charge
			hours = calculateHours(hours, entry);
			boardCounter++;
			charge = calculateCharge(entry);
			totalCharge += charge;

			//we will output some information.
			printCurrentOutput(boardCounter, entry, charge);

		} //end if

	} while (done == false);

	//we will print out the total
	puts("Total for the day");

	//First, the case the user only entered -1, so no boards were entered for the day.
	if (boardCounter == ZERO) {
		puts("No boards were rented today.");
	}
	else {

		//The user entered at least one board. We will display the totals for the day.
		displayTotals(boardCounter, hours, totalCharge);

	}
	return 0;
}

//we will create a function that asks the user for entry
void promptUserForEntry(void) {

	puts("Enter the number of hours the board was rented or enter -1 to quit.");

}

//we will create a function that validates that scannedValue is numeric and returns a numeric entry called givenValue
double validateScannedValueAsNumeric(int scannedValue, double givenValue) {

	while (scannedValue != 1) {
		puts("You did not enter a number");
		promptUserForEntry();
		scannedValue = scanf("%lf", &givenValue);
		while ((getchar()) != '\n');
	}

	return givenValue;

} //end validateScannedValueAsNumeric

//we will create a function to validate that the user entered a number in the specified range
double validateNumber(double givenValue) {


	while ((givenValue < SENTINEL_VALUE) || (givenValue > MAX_HOURS) || (givenValue == ZERO) || (givenValue < ZERO && givenValue > SENTINEL_VALUE)) {
	
		puts("The number of hours must be greater than 0 and not more than 10 hours.");
		promptUserForEntry();
		int scannedValue = scanf("%lf", &givenValue);
		while ((getchar()) != '\n');
		givenValue = validateScannedValueAsNumeric(scannedValue, givenValue);
	} 

	return givenValue;

} //end validateNumber

//we will create a function that calculates the hours
double calculateHours(double time, double newTime) {

	return (time += newTime);
}

//we will create a function that calculates the charge
double calculateCharge(double time) {

	//we will use a variable named timeRounded that rounds time up because we are always rounding time up to the nearest hour for calculations.
	double timeRounded = ceil(time);
	double output = 0;

	if (time > ZERO && time <= MINIMUM_CHARGE_HOURS) {

		output += MINIMUM_CHARGE;

	}
	else if (time > MINIMUM_CHARGE_HOURS && time <= SIX) {

		//we will do a calculation of the output if the charge is greater than the minimum charge and less than the maximum charge using the function below.
		output += ((MINIMUM_CHARGE)+((timeRounded-MINIMUM_CHARGE_HOURS) * ADDITIONAL_CHARGE));

	}
	else if (time > SIX && time <= MAX_HOURS) {

		output += MAXIMUM_CHARGE;

	} 

	return output;

} 

//we will create a function that prints out current output of the boards
void printCurrentOutput(int boards, double time, double cost) {
	
	printf("%-10s%-8s%-15s\n", "Boards", "Hours", "Charge");
	printf("%-10d%-8.2lf$%-15.2lf\n", boards, time, cost);
}


//we will create a function to display the totals when the user is done entering values.
void displayTotals(int boardNumber, double time, double runningCharge) {
	//we will output some information.
	printf("%-13s%-14s%s\n", "Boards", "Total Hours", "Total Charge");
	printf("%-13d%-14.2lf$%-15.2lf\n", boardNumber, time, runningCharge);
}
