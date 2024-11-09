/*
Name: Emily Ng
OS: Windows
Due: April 9, 2022
Description: This program uses random number generation and user input to create sentences from words in a file.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define STRING_LENGTH 100
#define NUMBER_SENTENCES 5
#define NUMBER_WORDS 5
#define WORD_LENGTH 20
#define NOUN "noun"
#define ADJECTIVE "adjective"

const char* MADLIB_FILE = "madlib.txt";
const char* PREPOSITION_FILE = "prepositions.txt";
const char* ARTICLE_FILE = "articles.txt";
const char* VERB_FILE = "verbs.txt";
const char* DIRECTORY_PATH = "C:\\CS2060Files\\";


void getFilePath(char* finalPath, const char* directoryPath, const char* fileName); //function to make and return a file path
bool getWordsFromFile(const char* path, char words[][WORD_LENGTH], int numberWords); //function to retrieve words from a file and place them into a 2D character array
void getUserWords(char word[][WORD_LENGTH], const char* wordType, int numberWords); //function called to retrieve words from a user and put them in a character array
bool writeSentenceToFile(const char* path, const char article[][WORD_LENGTH], const char verb[][WORD_LENGTH], const char preposition[][WORD_LENGTH], const char noun[][WORD_LENGTH], const char adjective[][WORD_LENGTH], int numberWords, int numberSentences);
//The above function writes sentences to a file.

int main(void) {

	//we will generate the file paths for prepositions, articles and verbs. We will also create three word arrays to store the values. 
	char prepositionsFilePath[STRING_LENGTH];
	getFilePath(prepositionsFilePath, DIRECTORY_PATH, PREPOSITION_FILE);

	char articlesFilePath[STRING_LENGTH];
	getFilePath(articlesFilePath, DIRECTORY_PATH, ARTICLE_FILE);

	char verbsFilePath[STRING_LENGTH];
	getFilePath(verbsFilePath, DIRECTORY_PATH, VERB_FILE);

	char prepositions[NUMBER_WORDS][WORD_LENGTH];

	char articles[NUMBER_WORDS][WORD_LENGTH];

	char verbs[NUMBER_WORDS][WORD_LENGTH];

	char nouns[NUMBER_WORDS][WORD_LENGTH];

	char adjectives[NUMBER_WORDS][WORD_LENGTH];

	//We will now get words from the prepositions, articles, and verbs files and place the words into our 2D word arrays

	bool gotWordsFromPrepFile = getWordsFromFile(prepositionsFilePath, prepositions, NUMBER_WORDS);

	bool gotWordsFromArticlesFile = getWordsFromFile(articlesFilePath, articles, NUMBER_WORDS);

	bool gotWordsFromVerbsFile = getWordsFromFile(verbsFilePath, verbs, NUMBER_WORDS);

	//if we successfully got words from each file, we will ask the user to enter 5 nouns and 5 adjectives.
	if (gotWordsFromPrepFile && gotWordsFromArticlesFile && gotWordsFromVerbsFile) {

		getUserWords(nouns, NOUN, NUMBER_WORDS);

		getUserWords(adjectives, ADJECTIVE, NUMBER_WORDS);

		//Now, we will generate the file path for the mad libs file
		char madLibFilePath[STRING_LENGTH] = { " " };
		getFilePath(madLibFilePath, DIRECTORY_PATH, MADLIB_FILE);

		//We will write sentences to the mad lib file using our word arrays
		bool hasBeenWritten = writeSentenceToFile(madLibFilePath, articles, verbs, prepositions, nouns, adjectives, NUMBER_WORDS, NUMBER_SENTENCES);

		//if we were able to write words to a file, we will let the user know where the file is located; otherwise, we will let them know we failed to write sentences to the file.
		if (hasBeenWritten) {
			printf("Madlibs file: %s", madLibFilePath);
		}
		else {
			puts("Failed to write sentences to madlib.txt");
		}
	}
	else {
		//Here, we failed to retireve either the prepositions, articles, or verbs, or alll three from the files. We will let the user know.
		puts("Failed to retrieve words from files.");
	}


}

//This function generates a final file path based on an input directory path and file name.
void getFilePath(char* finalPath, const char* directoryPath, const char* fileName) {

	strncpy(finalPath, directoryPath, STRING_LENGTH);
	strncat(finalPath, fileName, STRING_LENGTH);

}

//This function opens a file to read the words as a string then gets each word and puts it in an array
bool getWordsFromFile(const char* path, char words[][WORD_LENGTH], int numberWords) {

	FILE* filePtr; //pointer used to open the file
	bool gotWords = false; //boolean representing successfule completion of a task

	//we will collect words from file and put them in a 2D array if the file was able to be opened.
	if ((filePtr = fopen(path, "r")) != NULL) {
		const char delim[2] = " "; //how we are going to parse the file words for tokenizing

		char string[STRING_LENGTH]; //will store the words read in from the file

		fgets(string, STRING_LENGTH, filePtr); //stores the words from file into a string

		char* token = strtok(string, delim); //creates a token and initializes it to the first tokenized string.

		unsigned int i = 0; //will keep track of the index of the rows in our 2D words array

		while (token != NULL) { //loop used to walk through other tokens 

			strncpy(words[i], token, WORD_LENGTH); //sets words array at given value equal to the token
			token = strtok(NULL, delim); //grabs next token from string

			i++;
		}

		gotWords = true; //successfully accomplished task, so we will set the bool equal to true

	}
	else {
		puts("Unable to open file path");
	}


	fclose(filePtr);

	return gotWords;
}

//This function collects five words from the user and stores them into an array.
void getUserWords(char word[][WORD_LENGTH], const char* wordType, int numberWords) {

	printf("Enter %d %ss\n", numberWords, wordType); //prompt user for entry

	for (size_t i = 0; i < numberWords; i++) { //indexing through the number of words to be collected (rows)

		char userInput[WORD_LENGTH]; //will store the user input

		printf("Enter %s: ", wordType);
		fgets(userInput, WORD_LENGTH, stdin); 

		if (strchr(userInput, '\n') == NULL) { //if the user enteres too many characters we must clear the buffer.
			while ((getchar() != '\n')); //clear the buffer
		}

		userInput[strlen(userInput) - 1] = '\0'; //deletes the new line character at the end of the string if there is one.

		strcpy(word[i], userInput); //puts valid user input into the array.
	}

	puts("");
}

//This function writes sentences to a given file.
bool writeSentenceToFile(const char* path, const char article[][WORD_LENGTH], const char verb[][WORD_LENGTH], const char preposition[][WORD_LENGTH], const char noun[][WORD_LENGTH], const char adjective[][WORD_LENGTH], int numberWords, int numberSentences) {

	srand((unsigned int)time(NULL)); //seeds random number generation

	FILE* filePtr;

	bool hasBeenWritten = false; //boolean representing successfully writing sentences to file.

	if ((filePtr = fopen(path, "w")) != NULL) {

		puts("Creating sentences."); //we will let the user know that we are beginning to create sentences.

		//we will write sentences to a file using random number generation and our arrays.
		for (size_t i = 0; i < numberSentences; i++) {

			char sentence[STRING_LENGTH];

			unsigned int randArticle1 = rand() % 5;
			unsigned int randAdj1 = rand() % 5;
			unsigned int randNoun1 = rand() % 5;
			unsigned int randVerb = rand() % 5;
			unsigned int randPrep = rand() % 5;
			unsigned int randArticle2 = rand() % 5;
			unsigned int randAdj2 = rand() % 5;
			unsigned int randNoun2 = rand() % 5;

			sprintf(sentence, "%s %s %s %s %s %s %s %s.",
				article[randArticle1],
				adjective[randAdj1],
				noun[randNoun1],
				verb[randVerb],
				preposition[randPrep],
				article[randArticle2],
				adjective[randAdj2],
				noun[randNoun2]);

			sentence[0] = toupper(sentence[0]); //capitalize the first letter of the sentence. 

			fprintf(filePtr, "%s\n", sentence); // we will write the sentence to a file


		}
		hasBeenWritten = true; //we have written the sentences to the file. We will set hasBeenWritten to true.
	}
	else {
		puts("File could not be opened for writing sentences.");
	}

	fclose(filePtr);

	return hasBeenWritten;
}