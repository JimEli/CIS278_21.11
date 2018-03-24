/*************************************************************************
* Title: Hangman Game
* File: CIS278_Week10_21.11.cpp
* Author: James Eli
* Date: 2/9/2018
*
* Exercise 21.11 on page 896, Hangman Game
* Write a program that plays the game of Hangman. The program should pick a
* word (which is either coded directly into the program or read from a text
* file) and display the following:
*   Guess the word: XXXXXX
* Each X represents a letter. The user tries to guess the letters in the
* word. The appropriate response yes or no should be displayed after each
* guess. After each incorrect guess, display the diagram with another body
* part filled. After seven incorrect guesses, the user should be hanged. The
* display should look as follows:
*     O
*    /|\
*     |
*    / \
* After each guess, display all user guesses. If the user guesses the word
* correctly, display
*	Congratulations!!! You guessed my word. Play again? yes/no
*
* Notes:
*  (1) Compiled with MS Visual Studio 2017 Community (v141).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-278.
*************************************************************************
* Change Log:
*   02/09/2018: Initial release. JME
*************************************************************************/
#include <iostream>  // cin/cout/endl
#include <string>    // string
#include <vector>    // vector
#include <ctime>     // time for random seed
#include <random>    // random
#include <cctype>    // isalpha/tolower
#include <algorithm> // transform/count

using namespace std;

// Get single character user input.
char getUserInput(const string prompt)
{
	char input;

	cout << prompt;
	// Input from user.
	cin.get(input);
	if (cin.peek() != '\n')
		// More than a single char input.
		input = 0;
	// Eat any remaining chars and LF.
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return tolower(input);
}

// Check if character is already stored in list of guesses.
bool checkLetter(vector<char>& ltrs, const char c, const string response)
{
	if (find(ltrs.begin(), ltrs.end(), c) == ltrs.end())
	{
		cout << response;
		ltrs.push_back(c);
		return true;
	}
	else
		cout << "You already guessed \"" << c << "\"\n";
	return false;
}

int main()
{
	// Number of words.
	const unsigned numWords{ 15 };
	// Lambda random number generator [0...numWords].
	auto rng = [=]() -> unsigned { return int(numWords * rand() / (RAND_MAX + 1.0)); };
	// Seed the rng.
	srand(static_cast<unsigned>(time(0))); rng();
	do {
		// Sample list of words to guess (five each 5, 6 & 7 character long).
		string words[numWords] = {
			"blimp", "inuit", "roach", "ankle", "could",
			"visual", "quartz", "studio", "jockey", "hijack",
			"located", "alcohol", "crochet", "polymer", "humidor"
		};
		// Characters making up the gallows.
		string gallows[] =
		{
			"  |  O\n  | /|\\\n  |  |\n  | / \\", // right leg (complete)
			"  |  O\n  | /|\\\n  |  |\n  | /",    // left leg
			"  |  O\n  | /|\\\n  |  |\n  |",      // torso
			"  |  O\n  | /|\\\n  |\n  |",         // right arm
			"  |  O\n  | /|\n  |\n  |",           // body
			"  |  O\n  | /\n  |\n  |",            // left arm
			"  |  O\n  |\n  |\n  |",              // head
			"  |\n  |\n  |\n  |",                 // empty gallows
			"   ____\n  |  |\n",                  // gallows top
			"\n _|_____\n"                        // gallows base
		};

		vector<char> guessedLetters;      // List of guessed letters.
		guessedLetters.clear();         // Empty the list.
		unsigned remainingGuesses{ 7 }; // Number of guesses remianing.
		unsigned rn{ rng() };           // Get a random number.
		size_t remainingLetters{ words[rn].size() };     // Number un-guessed letters remaining in word.
		string clue = { string(words[rn].size(), 'X') }; // Display word clue.

		// Display program purpose.
		cout << "Let's play a game of Hangman!\n\n";

		// Loop until word is guessed or user is hung.
		while (remainingLetters && remainingGuesses) {
			// Display clue and get a user guess.
			char g = getUserInput("Guess the word: " + clue + "\n");

			if (isalpha(g)) {
				// Replace 'X' with matching character.
				transform(words[rn].cbegin(), words[rn].cend(), clue.begin(), clue.begin(),
					[=](char s, char d) { if (s == g) { return g; } else return d; });
			}

			// Check if guessed character had any matches in word.
			if (int matchedLetters = count(clue.cbegin(), clue.cend(), g))
				// Correct guess, decrement remaining letters by proper amount.
				remainingLetters -= (checkLetter(guessedLetters, g, "Good guess!\n") ? matchedLetters : 0);
			else
				// Wrong guess, decrement number of remaining guesses.
				remainingGuesses -= (checkLetter(guessedLetters, g, "Wrong guess!\n") ? 1 : 0);

			// Display results of guess.
			const unsigned TOP{ 8 }, BASE{ 9 };
			cout << gallows[TOP] << gallows[remainingGuesses] << gallows[BASE];
			cout << "Letters guessed: ";
			for (char c : guessedLetters)
				cout << c << " ";
			cout << "\n\n";
		}

		// Display result of game.
		if (!remainingLetters)
		{
			cout << "Congratulations!!! You guessed my word \"" << clue << "\" ";
			cout << "with " << guessedLetters.size() << " guesses!\n";
		}

		else
			cout << "You've been hung!!!\n";

	} while (tolower(getUserInput("Play again? yes/no ")) == 'y');

	return 0;
}
