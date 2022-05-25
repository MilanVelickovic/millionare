#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Game switch ('1' - yes, '0' - no).
int theGameIsOn = -1;

// FLAGS
// Help that user can use.
// 1 - Ask the audience, 2 - Phone a friend, 3 - 50/50, 4 - Change the question
// If the value of help is 1, that means that the user has not used it yet.
int askTheAudience = 1, phoneAFriend = 1, fiftyfifty = 1, changeTheQuestion = 1;

// Rewards (The rewards are expressed in Serbian money).
// Milestones (quaranteed rewards): Question 5 (5000), Question 10 (150000), Question 15 (5000000).
int rewards[] = { 500, 1000, 1500, 2500, 5000, 10000, 20000, 40000, 80000, 150000, 300000, 600000, 1250000, 2500000, 5000000 };

typedef struct Questions {
	char title[70];
	char correctAnswer[30];
	char incorrectAnswerA[30];
	char incorrectAnswerB[30];
	char incorrectAnswerC[30];
} Question;

void loadQuestionsFromFile(Question* questions) {
	FILE* file = fopen("questions.txt", "r");
	if (file == NULL) {
		printf("\n**[ERROR]: Pitanja nisu uspesno ucitana u igru... :(\nPokusaj ponovo!\n");
	}
	else {
		int firstCounter = 0;
		while (!feof(file)) {
			char line[200];
			fgets(line, 200, file);

			Question question;
			// Separting the read line from the file into parameters
			int secondCounter = 0;
			char* token = strtok(line, "*");
			while (token != NULL) {
				if (secondCounter == 0) {
					strcpy(question.title, token);
					secondCounter++;
				}
				else if (secondCounter == 1) {
					strcpy(question.correctAnswer, token);
					secondCounter++;
				}
				else if (secondCounter == 2) {
					strcpy(question.incorrectAnswerA, token);
					secondCounter++;
				}
				else if (secondCounter == 3) {
					strcpy(question.incorrectAnswerB, token);
					secondCounter++;
				}
				else {
					// Here we are removing the last character of the token, ie. '\ n', because a new line character is appended at the end of every file line.
					token[strlen(token) - 1] = '\0';
					strcpy(question.incorrectAnswerC, token);
					secondCounter = 0;
				}
				token = strtok(NULL, "*");
			}
			// Adding guestion
			questions[firstCounter] = question;
			firstCounter++;
		}
	}
	fclose(file);
}

void showWelcomeMessage() {
	printf("----------------------------------------------------------------------------\n");
	printf("------------------------------ KVIZ MILIONER -------------------------------\n");
	printf("----------------------------------------------------------------------------\n");
	printf("****************************************************************************\n");
}

// HELP FUNCTIONS --------------------------------------------------- START POINT

// Function checkAvailabeHelp returns 1 if a choosen help is available, and 0 if it's not.
int checkAvailableHelp(int optionNumber) {
	if (optionNumber == 6) {
		return askTheAudience == 1 ? 1 : 0;
	}
	else if (optionNumber == 7) {
		return phoneAFriend == 1 ? 1 : 0;
	}
	else if (optionNumber == 8) {
		return fiftyfifty == 1 ? 1 : 0;
	}
	else {
		return changeTheQuestion == 1 ? 1 : 0;
	}
}

void disableHelp(int optionNumber) {
	if (optionNumber == 6) {
		askTheAudience = 0;
	}
	else if (optionNumber == 7) {
		phoneAFriend = 0;
	}
	else if (optionNumber == 8) {
		fiftyfifty = 0;
	}
	else {
		changeTheQuestion = 0;
	}
}

// This function is required in askTheAudience function
double calculatePercent(int number, int total) {
	return (100 * number) / total;
}

void askTheAudienceHelp(char*** possibleAnswers) {
	/* LOGIC
				 | 100 people | 84 people | 63 people | 23 people |
				 |------------|-----------|-----------|-----------|
		sum: 100 | rnd: 16    | rnd: 21   | rnd: 40   | leave 23  | <-
				 |------------|-----------|-----------|-----------|
				 | res: 84    | res: 63   | res: 23   |     /     |
	*/
	int people = 100, total = 100;

	srand(time(NULL));
	int randA = 1 + (rand() % people);
	people = people - randA;
	int randB = 1 + (rand() % people);
	people = people - randB;
	int randC = 1 + (rand() % people);
	int randD = people - randC;

	printf(" [1]: %s [%.1f%%]\n", possibleAnswers[0], calculatePercent(randA, total));
	printf(" [2]: %s [%.1f%%]\n", possibleAnswers[1], calculatePercent(randB, total));
	printf(" [3]: %s [%.1f%%]\n", possibleAnswers[2], calculatePercent(randC, total));
	printf(" [4]: %s [%.1f%%]\n", possibleAnswers[3], calculatePercent(randD, total));
}

void phoneAFriendHelp(char** possibleAnswers) {
	srand(time(NULL));
	int randomNumber = 1 + (rand() % 4);

	for (int i = 0; i < 4; i++) {
		if ((i + 1) == randomNumber) {
			printf(" [%d]: %s [Zdravo! Hmmm... Mislim da je ovo odgovor. Nisam bas siguran.]\n", (i + 1), possibleAnswers[i]);
		}
		else {
			printf(" [%d]: %s \n", (i + 1), possibleAnswers[i]);
		}
	}
}

void fiftyFiftyHelp(char** possibleAnswers, char** correctAnswer) {
	int indexOfTheCorrectAnswer;
	for (int i = 0; i < (sizeof(possibleAnswers) / sizeof(char)); i++) {
		if (strcmp(possibleAnswers[i], correctAnswer) == 0) {
			indexOfTheCorrectAnswer = i;
		}
	}

	int randomNumber = 0;

	do {
		srand(time(NULL));
		int randomNumber = rand() % 4;
	} while (randomNumber == indexOfTheCorrectAnswer);

	for (int i = 0; i < 4; i++) {
		if (i == indexOfTheCorrectAnswer || i == randomNumber) {
			printf(" [%d]: %s [<--]\n", (i + 1), possibleAnswers[i]);
		}
		else {
			printf(" [%d]: %s \n", (i + 1), possibleAnswers[i]);
		}
	}

}

// HELP FUNCTIONS --------------------------------------------------- END POINT
// INFO FUNCTIONS --------------------------------------------------- START POINT

void showHelp() {
	printf("Dostupna pomoc:\n");
	if (askTheAudience == 1) {
		printf("  6 - Pomoc publike ");
	}
	if (phoneAFriend == 1) {
		printf(" 7 - Pozovi prijatelja ");
	}
	if (fiftyfifty == 1) {
		printf(" 8 - 50/50 ");
	}
	if (changeTheQuestion == 1) {
		printf(" 9 - Promeni pitanje");
	}
	if (askTheAudience == 0 && phoneAFriend == 0 && fiftyfifty == 0 && changeTheQuestion == 0) {
		printf("  Iskoristili ste sve.");
	}
}

void showMoney(int rewardIndex) {
	printf("U kasi imate %ddin.", rewards[rewardIndex]);
}

void showInfo(int rewardIndex) {
	printf("\n****************************************************************************\n");
	showHelp();
	printf("\n");
	if (rewardIndex != 0) {
		printf("\n");
		showMoney(rewardIndex - 1);
		printf("\n");
	}
	printf("\n****************************************************************************\n");
}

// INFO FUNCTIONS --------------------------------------------------- END POINT
// ANSWERS FUNCTIONS ------------------------------------------------ START POINT

void shufflePossibleAnswers(char** answers, int size) {
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		int randomNumber = rand() % size;
		int temp = answers[i];
		answers[i] = answers[randomNumber];
		answers[randomNumber] = temp;
	}
}

int checkPlayerAnswer(int indexOfCorrectAnswer) {
	int answer = -1;
	printf("[VAS ODGOVOR]: ");
	scanf("%d", &answer);
	while (answer < 1 || answer == 5 || answer > 9) {
		printf("** ( Kao odgovor unesite brojeve od 1 - 4)\n[VAS ODGOVOR] : ");
		scanf("%d", &answer);
	}
	printf("----------------------------------------------------------------------------\n");
	if (answer == indexOfCorrectAnswer + 1) {
		return 1;
	}
	else if (answer >= 6 && answer <= 9) {
		return answer;
	}
	else {
		return 0;
	}
}

int indexOfTheCorrectAnswer(char** possibleAnswers, int size, char* correctAnswer) {
	int i = 0;
	while (i < size) {
		if (strcmp(possibleAnswers[i], correctAnswer) == 0) {
			return i;
		}
		i++;
	}
}

void showAnswers(char** possibleAnswers, int option, char* correctAnswer) {
	if (option == 0 || option == 9) {
		// option == 0 -> shows all answers (standard)
		// option == 9 -> shows all answers (changed question)
		printf(" [1]: %s \n", possibleAnswers[0]);
		printf(" [2]: %s \n", possibleAnswers[1]);
		printf(" [3]: %s \n", possibleAnswers[2]);
		printf(" [4]: %s \n", possibleAnswers[3]);
	}
	else if (option == 6) {
		// option == 6 -> Ask the audience
		askTheAudienceHelp(possibleAnswers);
	}
	else if (option == 7) {
		// option == 7 -> Phone a friend
		phoneAFriendHelp(possibleAnswers);
	}
	else {
		// option == 8 -> 50/50
		fiftyFiftyHelp(possibleAnswers, correctAnswer);
	}
}

// ANSWERS FUNCTIONS ------------------------------------------------ END POINT

int showQuestionAndCheckAnswer(int questionNumber, Question* question, int helpOption) {
	// Adding answers to the array
	char* possibleAnswers[4] = { question->correctAnswer, question->incorrectAnswerA, question->incorrectAnswerB, question->incorrectAnswerC };
	// Shuffling answers
	shufflePossibleAnswers(&possibleAnswers, 4);
	// PRINTF FUNCTIONS
	printf("|[ PITANJE %d ]| ", questionNumber);
	printf("* %s\n", question->title);
	printf("* Vrednost: %ddin.\n\n", rewards[questionNumber - 1]);
	showAnswers(possibleAnswers, helpOption, question->correctAnswer);
	printf("\n----------------------------------------------------------------------------\n");
	// 1st: Calling indexOfTheCorrectAnswer function. Return value is a value of index of correctAnser in possibleAnswers array.
	// 2nd: Function checkPlayerAnswer is comparing player's answer and returned value of indexOfTheCorrectAnswer function.
	// 3rd: If compared values are equal, the user is able to approach next question.
	return checkPlayerAnswer(indexOfTheCorrectAnswer(possibleAnswers, 4, question->correctAnswer));
}

void showMessage(int counter, int milestoneIndex) {
	printf("\n****************************************************************************\n");
	if (counter == 15) {
		printf("*                    CESTITAMO! POSTALI STE MILIONER!!!                    *\n");
		printf("*                        OSVOJENA SUMA: %ddin                         *\n", rewards[counter - 1]);
	}
	else {	
		printf("*                        OSVOJENA SUMA: %ddin                         *\n", rewards[milestoneIndex] == 500 ? 0 : rewards[milestoneIndex]);	
	}
	printf("****************************************************************************\n\n\n");
}

void main() {
	showWelcomeMessage();

	printf("Zelite li da pokrenete igru? (1 - Da / 0 - Ne)\n");
	while (theGameIsOn < 0 || theGameIsOn > 1) {
		printf("[VAS ODGOVOR]: ");
		if (scanf("%d", &theGameIsOn) != 1) {
			theGameIsOn = -1;
		}
	}

	if (theGameIsOn == 1) {
		// Reserving a memmory block in heap for 16 questions.
		Question* questions = (Question*)malloc(sizeof(Question) * 17);
		loadQuestionsFromFile(questions);
		printf("\nIgra uskoro pocinje...\n\n");

		int milestoneIndex = 0;
		int counter = 0;
		int currentQuestion = counter;
		int helpOption = 0;

		while (counter < 15) {
			showInfo(counter);
			int answerValue = showQuestionAndCheckAnswer(counter + 1, &questions[currentQuestion], helpOption);
			helpOption = 0;
			if (answerValue != 1) {
				if (answerValue == 0) {
					printf("*** NETACAN Odgovor ***\n");
					// Value of 16 will not meet the requirement of our while loop.
					counter = 16;
				}
				else {
					if (checkAvailableHelp(answerValue) == 1) {
						helpOption = answerValue;
						// If statement represents the use of "Change the question" help
						if (helpOption == 9) {
							currentQuestion = 15;
						}
						disableHelp(answerValue);
					}
					else {
						printf("**************************************\n");
						printf("* Odabrana pomoc je vec iskoriscena! *\n");
						printf("**************************************\n");
					}

				}
			}
			else {
				printf("*** TACAN Odgovor ***\n");
				counter++;
				if (counter % 5 == 0) {
					if (counter == 5) {
						milestoneIndex += 4;
					}
					else {
						milestoneIndex += 5;
					}
				}
				currentQuestion = counter;
			}

		}
		showMessage(counter, milestoneIndex);
		free(questions);
	}
}