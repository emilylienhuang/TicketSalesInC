//Emily Ng
//homework 04_13

#include <stdio.h>

int main(void) {

	//we will initialize product to 1
	unsigned int product = 1;

	//we will use a for loop to calculate the product of the odd integers between 1 and 15
	for (unsigned int i = 1; i <= 15; i += 2) {
		product *= i;
	}

	//we will print out the result
	printf("The product of the odd integers between 1 and 15 is %u", product);

	return 0;
}