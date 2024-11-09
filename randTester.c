#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
	srand(time(NULL));

	int number = ( - 3 + (rand() % 11));
	printf("%d", number);
}