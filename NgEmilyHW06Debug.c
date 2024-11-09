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
#define WORD_LENGTH 20
#define NUM_WORDS 5
#define NOUN "noun"
#define ADJECTIVE "adjective"

const char* MADLIB_FILE = "madlib.txt";
const char* PREPOSITION_FILE = "prepositions.txt";
const char* ARTICLE_FILE = "articles.txt";
const char* VERB_FILE = "verbs.txt";
const char* DIRECTORY_PATH = "C:\\CS2060Files\\";


void getFilePath(char* finalPath, const char* directoryPath, const char* fileName);
bool getWordsFromFile(const char* path, char words[][WORD_LENGTH], int numberWords);
void getUserWords(char word[][WORD_LENGTH], const char* wordType, int numberWords);
bool writeSentenceToFile(const char* path, const char article[][WORD_LENGTH], const char verb[][WORD_LENGTH], const char preposition[][WORD_LENGTH], const char noun[][WORD_LENGTH], const char adjective[][WORD_LENGTH], int numberWords, int numberSentences);

int main(void) {

	//we will generate the file paths for prepositions, articles and verbs. We will also create three word arrays to store the values. 
	char prepositionsFilePath[STRING_LENGTH];
	getFilePath(prepositionsFilePath, DIRECTORY_PATH, PREPOSITION_FILE);

	char articlesFilePath[STRING_LENGTH];
	getFilePath(articlesFilePath, DIRECTORY_PATH, ARTICLE_FILE);

	char verbsFilePath[STRING_LENGTH];
	getFilePath(verbsFilePath, DIRECTORY_PATH, VERB_FILE);

	char prepositions[NUM_WORDS][WORD_LENGTH];

	char articles[NUM_WORDS][WORD_LENGTH];

	char verbs[NUM_WORDS][WORD_LENGTH];

	char nouns[NUM_WORDS][WORD_LENGTH];

	char adjectives[NUM_WORDS][WORD_LENGTH];

	bool gotWordsFromPrepFile = getWordsFromFile(prepositionsFilePath, prepositions, NUM_WORDS);

	bool gotWordsFromArticlesFile = getWordsFromFile(articlesFilePath, articles, NUM_WORDS);

	bool gotWordsFromVerbsFile = getWordsFromFile(verbsFilePath, verbs, NUM_WORDS);

	if (gotWordsFromPrepFile && gotWordsFromArticlesFile && gotWordsFromVerbsFile) {

		getUserWords(nouns, NOUN, NUM_WORDS);

		getUserWords(adjectives, ADJECTIVE, NUM_WORDS);

		char madLibFilePath[STRING_LENGTH] = { " " };
		getFilePath(madLibFilePath, DIRECTORY_PATH, MADLIB_FILE);

		 bool hasBeenWritten = writeSentenceToFile(madLibFilePath, articles, verbs, prepositions, nouns, adjectives, NUM_WORDS, NUMBER_SENTENCES);

		 if (hasBeenWritten) {
			 puts("Sentences have been written to file.");
		 }
	}
	else {
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
	bool gotWords = false;

	if ((filePtr = fopen(path, "r")) != NULL) {
		const char delim[2] = " "; //how we are going to parse the file words for tokenizing

		char string[STRING_LENGTH];

		fgets(string, STRING_LENGTH, filePtr); //stores the words from file into a string

		char* token = strtok(string, delim); //creates a token and initializes it to the first tokenized string.

		unsigned int i = 0;

		while (token != NULL) { //loop used to walk through other tokens 

			strncpy(words[i], token, WORD_LENGTH); //sets words array at given value equal to the token
			token = strtok(NULL, delim); //grabs the next token for the next loop iteration?

			i++;
		}

		gotWords = true;

	}


	fclose(filePtr);

	return gotWords;
}

//This function collects five words from the user and stores them into an array.
void getUserWords(char word[][WORD_LENGTH], const char* wordType, int numberWords) {

	printf("Enter %d %ss\n", numberWords, wordType);

	for (size_t i = 0; i < numberWords; i++) { //indexing through the number of words to be collected (rows)

		char userInput[WORD_LENGTH];

		printf("Enter %s: ", wordType);
		fgets(userInput, WORD_LENGTH, stdin);

		if (strchr(userInput, '\n') == NULL) { //if the user enteres too many characters we must clear the buffer.
			while ((getchar() != '\n')); //clear the buffer
		}

		userInput[strlen(userInput) - 1] = '\0'; //deletes the new line character at the end of the string.

		strcpy(word[i], userInput); //puts valid user input into the array.
	}

	puts("");
}

//This function writes sentences to a given file.
bool writeSentenceToFile(const char* path, const char article[][WORD_LENGTH], const char verb[][WORD_LENGTH], const char preposition[][WORD_LENGTH], const char noun[][WORD_LENGTH], const char adjective[][WORD_LENGTH], int numberWords, int numberSentences) {

	srand((unsigned int)time(NULL));

	FILE* filePtr;

	bool hasBeenWritten = false;

	if ((filePtr = fopen(path, "w")) != NULL) {

		for (size_t i = 0; i < numberSentences; i++) {

			char sentence[STRING_LENGTH] = {" "};

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

			sentence[0] = toupper(sentence[0]);

			hasBeenWritten = (fprintf(filePtr, "%s\n", sentence) > 0); //checks to see if the information was successfully written to the file. 



		}
	}
	else {
		puts("Sentences could not be written to file.");
	}

	fclose(filePtr);

	return hasBeenWritten;
}