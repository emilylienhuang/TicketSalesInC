/*
Emily Ng
Learning Activity 4
Due: February 7, 2022
*/

#include <stdio.h>

int sum(int number1, int number2);
void printSum(int sum);

int main(void) {

	/*
	Code used to check functions:
	int number = sum(-6, -4);

	printSum(number);
	*/
	

	return 0;

}

int sum(int number1, int number2) {

	return number1 + number2;
}

void printSum(int result) {

	printf("The sum is %d ", result);
}