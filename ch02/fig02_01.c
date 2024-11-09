// Fig. 2.1: fig02_01.c
// A first program in C 
#include <stdio.h>
int main(void) {

	// initialize three integers:
	int length = 0;
	int width = 0;
	int area = 0;
	int resultOfLengthDividedByWidth = 0;

	//prompt user for a length and save response to length variable
	printf("Enter the length\n");
	scanf("%d", &length);

	//prompt user for a width and save response to width variable
	printf("Enter the width\n");
	scanf("%d", &width);

	//calculate area and assign it to the area variable
	area = calculateArea(length, width);

	//print the information length, width, and area
	printf("Length: %d width: %d and area is %d\n", length, width, area);

	//reassign resultOfLengthDividedByWidth
	resultOfLengthDividedByWidth = lengthDividedByWidth(length, width);

	

	//print resultOfLengthDividedByWidth
	printf("%d\n",resultOfLengthDividedByWidth);

	return 0;

} // end function main 

// create a function to calculate area given a length and width.
int calculateArea(int length, int width) {

	return length * width;

}

//find length/width
int lengthDividedByWidth(int length, int width) {

	return length / width;
}


/**************************************************************************
 * (C) Copyright 1992-2015 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/
