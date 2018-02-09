/*************************************************************************
* Title: Hangman Game
* File: CIS278_Week10_21.11.cpp
* Author: James Eli
* Date: 1/17/2018
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
*   01/17/2018: Initial release. JME
*************************************************************************/
#include <iostream>  // cin/cout/endl
#include <string>    // string
#include <list>      // list
#include <ctime>     // time for random seed
#include <random>    // random
#include <cctype>    // isalpha/tolower
#include <algorithm> // transform/count

using namespace std;

// Get single character user input.
char getUserInput(string prompt)
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

	return input;
}

// Check if guessed character has any matches in word.
int checkMatch(const string& s, string& d, const char c)
{
	// Replace 'X' with (guess) matching character.
	transform(s.begin(), s.end(), d.begin(), d.begin(),
		[&](char s, char d) { if (s == c) { return c; } else return d; }
	);
	// Return number of characteres replaced.
	return count(d.begin(), d.end(), c);
}

// Check if character is already stored in list of guesses.
bool checkLetter(list<char>& ltrs, const char c, const string response)
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
	const int numWords{ 30 };
	// Maximum guesses before the user is hung.
	const unsigned maxGuesses{ 7 };

	// Lambda random number generator [0...numWords].
	auto rng = [&]() -> unsigned { return int(numWords * rand() / (RAND_MAX + 1.0)); };
	// Seed the rng.
	srand(static_cast<unsigned>(time(0))); rng(); 
	// Display program purpose.
	cout << "Let's play a game of Hangman!\n";

	do {
		// List of words to guess (ten each 5, 6 & 7 character long).
		string words[numWords] = {
			"blimp", "inuit", "roach", "ankle", "stogy",
			"could", "mover", "lovey", "snide", "parch", 
			"queazy", "quartz", "quacky", "jockey", "jinxed",
			"hijack", "coccyx", "jacked", "jackal", "jumped",
			"primate", "humidor", "pismire", "hypoxia", "alcohol",
			"located", "ectopic", "crochet", "polymer", "worsted"
		};
		// Characters make up the gallows.
		string gallows[] =
		{
			"  |\n  |\n  |\n  |",                // empty gallows
			"  |  O\n  |\n  |\n  |",             // +head
			"  |  O\n  | /\n  |\n  |",           // +left arm
			"  |  O\n  | /|\n  |\n  |",          // +body
			"  |  O\n  | /|\\\n  |\n  |",        // +right arm
			"  |  O\n  | /|\\\n  |  |\n  |",     // +torso
			"  |  O\n  | /|\\\n  |  |\n  | /",   // +left leg
			"  |  O\n  | /|\\\n  |  |\n  | / \\" // +right leg (complete)
		};

		list<char> guessedLetters;
		unsigned numWrongGuesses{ 0 };
		unsigned numCorrectLetters{ 0 };
		unsigned rn{ rng() };
		const size_t numLetters{ words[rn].size() };
		string clue = { string(words[rn].size(), 'X') };

		guessedLetters.clear();

		// Loop until word is guesses or user is hung.
		while (numCorrectLetters < numLetters && numWrongGuesses < maxGuesses) {
			// Display clue and get user guess.
			char g = getUserInput("Guess the word: " + clue + "\n");

			if (isalpha(g)) 
				if (int matchedLetters = checkMatch(words[rn], clue, tolower(g)))
					// Good guess.
					numCorrectLetters += (checkLetter(guessedLetters, g, "Good guess!\n") ? matchedLetters : 0);
				else
					// Bad guess.
					numWrongGuesses += (checkLetter(guessedLetters, g, "Wrong guess!\n") ? 1 : 0);

			// Display results of guess.
			cout << "   ____\n  |  |\n" << gallows[numWrongGuesses] << "\n _|_____\n";
			cout << "Letters guessed: ";
			for (char c : guessedLetters)
				cout << c << " ";
			cout << "\n\n";
		}

		// Display result of game.
		if (numCorrectLetters == numLetters)
			cout << "Congratulations!!! You guessed my word. ";
		else
			cout << "You've been hung!!! ";

	} while (getUserInput("Play again? yes/no ") == 'y');

	return 0;
}

