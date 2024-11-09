/*
Emily Ng
CS2060 TTh
OS: Windows
HW04
Due: 02/24/2022
Description:This program simulates how canvas calculates grades using arrays.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STUDENTS 4
#define GRADE_CATEGORIES 5
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final "
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, .15 };


void displayCategories(const double gradeCategoryWeights[], size_t gradeCategories);
void collectGrades(int gradesArray[][GRADE_CATEGORIES], size_t pupils, size_t categories);
void displayGradesArray(const int gradesArray[][GRADE_CATEGORIES], size_t pupils, size_t categories);
void calculateFinalGrades(double finalGradesArray[], const int gradesArray[][GRADE_CATEGORIES], size_t pupils, size_t categories);
void calculateLetterGrades(char letterGrades[], const double finalGradesArray[], size_t pupils);
void displayFinalGrades(const char letterGrades[], const double finalGradesArray[], size_t pupils);
double calculateClassAverage(const double finalGradesArray[], size_t pupils);
void displayClassAverage(double average);

int main(void) {

	//We will display to our "canvas" user the grade categories and how to enter grades in for students.
	displayCategories(GRADE_CATEGORY_WEIGHT, GRADE_CATEGORIES);

	//We will declare an array for the grades and fill them with user inputted grades
	int arrayOfGrades[STUDENTS][GRADE_CATEGORIES];

	collectGrades(arrayOfGrades, STUDENTS, GRADE_CATEGORIES);


	//we will display the collected grades array
	displayGradesArray(arrayOfGrades, STUDENTS, GRADE_CATEGORIES);

	//we will declare an array to hold the final grades for students, and we will calculate the final grades for students
	double finalGrades[STUDENTS];

	calculateFinalGrades(finalGrades, arrayOfGrades, STUDENTS, GRADE_CATEGORIES);

	//we will declare and fill an array that holds the letter grades 
	char finalLetterGrades[STUDENTS];

	calculateLetterGrades(finalLetterGrades, finalGrades, STUDENTS);

	//we will display final grades and letter grades
	displayFinalGrades(finalLetterGrades, finalGrades, STUDENTS);

	//we will display and calculate class average
	double classAverage = calculateClassAverage(finalGrades, STUDENTS);

	displayClassAverage(classAverage);

		return 0;
}

//This function displays the grade categories and informs the user how to input grades.
void displayCategories(const double gradeCategoryWeights[], size_t gradeCategories) {

	printf("This program will calculate the grades for these categories\n%s\n", CATEGORIES);
	printf("The category weights are\n");

	for (size_t i = 0; i < gradeCategories; i++) {
		printf("Category %llu weight is %4.2f\n", (i+1), gradeCategoryWeights[i]);
	}

	puts("");

	printf("The correct order to enter grades for each student is: \n%s\n", CATEGORIES);
}


//This function collects user input to fill a 2D array of grade values
void collectGrades(int gradesArray[][GRADE_CATEGORIES], size_t pupils, size_t categories) {

	puts("");

	//This outer loop indexes through the rows in the 2D array represented by students
	for (size_t i = 0; i < pupils; i++) {


		//This inner loop indexes through the columns of the 2D  represented by grades
		for (size_t j = 0; j < categories; j++) {

			//We will tell the user to enter grades for each student and record their input at the specific location for the 2D array.
			printf("Enter the grade for each category for student %llu, category %llu: ", (i + 1), (j+1));
			scanf("%d", &gradesArray[i][j]);
		
		}

		puts("");

	}
	
}

//This function displays all the grades collected in the grades array by indexing through rows then columns
void displayGradesArray(const int gradesArray[][GRADE_CATEGORIES], size_t pupils, size_t categories) {

	puts("Grades entered for each student");
	puts("");

	//First, indexing through rows
	for (size_t i = 0; i < pupils; i++) {

		printf("Student %llu: ", i+1);
		//Now, indexing through columns
		for (size_t j = 0; j < categories; j++) {
			
			printf("%-5d", gradesArray[i][j]);
		}

		puts("");
	}
}

//This function calculates the final grades based on the weighted average for students.
void calculateFinalGrades(double finalGradesArray[], const int gradesArray[][GRADE_CATEGORIES], size_t pupils, size_t categories) {

	//First, indexing through the rows which represent students
	for (size_t i = 0; i < pupils; i++) {

		//we will keep track of each students cumulative grade using a counter
		double cumulativeGrade = 0;

		//then indexing through the columns represented by grades per cateogry
		for (size_t j = 0; j < categories; j++) {
			cumulativeGrade += (gradesArray[i][j] * GRADE_CATEGORY_WEIGHT[j]);
		}

		//we will put the cumulative grade per student into the finalGradesArray before moving to the next student.
		finalGradesArray[i] = cumulativeGrade;
	}
}

//This function calculates letter grades per student
void calculateLetterGrades(char letterGrades[], const double finalGradesArray[], size_t pupils) {

	//we will index through all cumulative grades per student, assign a letter value to each student, and save those letter values into an array.
	for (size_t i = 0; i < pupils; i++) {

		if (finalGradesArray[i] >= 90) {
			letterGrades[i] = 'A';
		}
		else if (finalGradesArray[i] >= 80 && finalGradesArray[i] < 90) {
			letterGrades[i] = 'B';
		}
		else if (finalGradesArray[i] >= 70 && finalGradesArray[i] < 80) {
			letterGrades[i] = 'C';
		}
		else if (finalGradesArray[i] >= 60 && finalGradesArray[i] < 70) {
			letterGrades[i] = 'D';
		}
		else {
			letterGrades[i] = 'F';
		}
	}
}


//This function displays the final grades and letter grades per student
void displayFinalGrades(const char letterGrades[], const double finalGradesArray[], size_t pupils) {

	puts("");

	printf("Final grades for students, respectively:\n");

	for (size_t i = 0; i < pupils; i++) {
		printf("Student %llu: %7.1f %c\n",(i+1),finalGradesArray[i],letterGrades[i]);
	}

	puts("");
}


//This function calculates the class average and returns it
double calculateClassAverage(const double finalGradesArray[], size_t pupils) {

	//sum will keep track of all student grades added together
	double sum = 0;

	for (size_t i = 0; i < pupils; i++) {
		sum += finalGradesArray[i];
	}

	//we will return sum/ number of students which gives us an average for the class.
	return (sum / pupils);
}

//This function displays the class average
void displayClassAverage(double average) {

	printf("Class average is %4.1f", average);
}