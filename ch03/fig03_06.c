// Fig. 3.6: fig03_06.c
// Class average program with counter-controlled iteration
#include <stdio.h>

// function main begins program execution
int main( void )
{
   unsigned int counter = 0; // number of grade to be entered next
   int grade = 0; // grade value
   int total = 0; // sum of grades entered by user
   float average = 0; // average of grades
   
   // initialization phase
   total = 0; // initialize total
   counter = 1; // initialize loop counter
   
   // processing phase
   while ( counter <= 10 ) { // loop 10 times
      printf( "%s", "Enter grade: " ); // prompt for input
      scanf( "%d", &grade ); // read grade from user
      total = total + grade; // add grade to total
      counter = counter + 1; // increment counter
   } // end while
   
   // termination phase
   average = (float) total / 10; // integer division

   printf( "Class average is %.2f\n", average ); // display result
} // end function main

/**************************************************************************
 * (C) Copyright 1992-2013 by Deitel & Associates, Inc. and               *
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

