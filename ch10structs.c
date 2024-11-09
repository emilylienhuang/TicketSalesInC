//Chap 10 Structures

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 20
#define MAXEMPS 3

// Defining a structure
//Doesn't take up any memory until the structure is created
struct  employee{
	char firstName[NAME_SIZE];
	char lastName[NAME_SIZE];	
	unsigned int age;
	double hourlySalary;	
};

void enterEmployees(struct employee* employeeArr, int maxEmployees);



int main(void) 
{
	
	//PART 1 declare a struct
   	// declare	 	
	struct employee defaultEmployee;
	puts("\n\nPart 1 declare struct employee defaultEmployee");	
	
	puts("Print after declaration defaultEmployee\n");
	
	//What is stored?
	//A: garbage
	printf ("Employee's first name: %s\n", defaultEmployee.firstName);
	printf ("Employee's last name: %s\n", defaultEmployee.lastName);
	printf ("Employee's age = %d\n", defaultEmployee.age);
	printf ("Employee's hourly salary = %3.2f\n", defaultEmployee.hourlySalary);

	puts("\nInitialize to default values");
	//initialize
	strncpy (defaultEmployee.firstName, " ", NAME_SIZE); //get a string into another variable
	strncpy (defaultEmployee.lastName, " ", NAME_SIZE); //don't want to assign one string to another string (conflicting types addresses)
	defaultEmployee.age = 0; //assigns value to age
	defaultEmployee.hourlySalary = 0;

    puts("\nPrint after intialization of defaultEmployee");

	printf ("Employee's first name: %s\n", defaultEmployee.firstName);
	printf ("Employee's last name: %s\n", defaultEmployee.lastName);
	printf ("Employee's age = %d\n", defaultEmployee.age);
	printf ("Employee's hourly salary = %3.2f\n", defaultEmployee.hourlySalary);
 

	//Part 2
	puts("\n\nPart 2 Declare and Initalize struct employee defaultEmployee = { Joe, Smith, 23, 15}");	
	
	// declare and initialize two employees
	struct employee aEmployee = { "Stack", "Terror", 37, 60.00};
	struct employee bEmployee = { "Binary ", "Beasts",25, 50.00};
	
	puts("\nPrint after intializing employeea ");
		
	puts("Print after declaration defaultEmployee");
	printf ("aEmployee's first name: %s\n", aEmployee.firstName);
	printf ("aEmployee's last name: %s\n", aEmployee.lastName);
	printf ("aEmployee's age = %d\n", aEmployee.age);
	printf ("aEmployee's hourly salary = %3.2f\n", aEmployee.hourlySalary);
	printf ("sizeof  aEmployee.age = %d\n", sizeof(aEmployee.age));
	
	//compare members
	if (aEmployee.age > bEmployee.age) 
	{
		puts ("aEmployee age is greater than bEmployee age");
	}
	else if (aEmployee.age < bEmployee.age)
	{
		puts ("aEmployee age is less than bEmployee age");	
	}
	else
	{
		puts ("aEmployee age is = bEmployee age");
	}
	
	// memeber assignments
	bEmployee.hourlySalary = aEmployee.hourlySalary;
	printf ("Now bEmployee's hourly salary = %3.2f\n", bEmployee.hourlySalary);
	printf ("&bEmployee.hourlySalary = %p", &bEmployee.hourlySalary);
	
	//Part 3 Pointer to employee struct
	puts("\n\nPart 3 Declare and Initalize employeePtr ");	

	// Intialize employee pointer to aEmployee 
	struct employee *employeePtr = &aEmployee; 	//can create a pointer to point to different addresses of a structure.

	//STRUCTURES ARE PASSED BY VALUE
	
    // Arrow operators for pointers to structures 
    printf ("EmployeePtr first name -> %s\n", employeePtr->firstName); //-> dereferences and goes to whatever field you want
	printf ("EmployeePtr last name -> %s\n", employeePtr->lastName);
	printf ("EmployeePtr age -> %d\n", employeePtr->age);
	printf ("EmployeePtr hourly salary -> %3.2f\n", employeePtr->hourlySalary);

	// assign bEmployee address
	puts("\nAssigne employeePtr = &bEmployee and access with (*employeePtr). ");
	employeePtr = &bEmployee;
	// equivalent to arrow operators 
    printf ("(*employeePtr).firstName  %s\n", (*employeePtr).firstName); //(*employeePtr).firstName because of order of operations.
	printf ("(*employeePtr).lastName %s\n", (*employeePtr).lastName);
	printf ("(*employeePtr).age %d\n", (*employeePtr).age);
	printf ("(*employeePtr).hourlySalary %3.2f\n", (*employeePtr).hourlySalary);


	//part 4 passing array of structures
	puts("\n\nPart 4 Array of struct employee ");	
	// Intialize array of employees 
	struct employee employees[MAXEMPS];
	enterEmployees(employees, MAXEMPS);

	// create function to display employees
	//displayEmployees(employees, MAXEMPS);	
	
	
	return 0;
}

void enterEmployees(struct employee * employeeArr, int max) //*employeeArr must pass structs by refernece, they are default by value
{

	for (int i = 0; i < max; i++)
	{
		printf("\nEnter details of employees %d\n\n", i + 1);

		printf("%s", "Enter first name: ");
		scanf("%s", employeeArr[i].firstName); //firstName is already storing the address.

		printf("%s", "Enter last name: ");
		scanf("%s", employeeArr[i].lastName);

		printf("%s", "Enter age: ");
		scanf("%i", &employeeArr[i].age);

		printf("%s", "Enter Hourly Salary: ");
		scanf("%lf", &employeeArr[i].hourlySalary);

	}

}



