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
*   09/22/2019: Updated per stackexchange/code review suggestions. JME
*************************************************************************/
#include <iostream>  // std::cin/cout/endl
#include <iterator>  // ostream iterator
#include <string>    // std::string
#include <vector>    // vector
#include <random>    // random
#include <cctype>    // isalpha/tolower
#include <algorithm> // transform/count

using std::cout;

constexpr unsigned TOP{ 8 };
constexpr unsigned BASE{ 9 };
constexpr unsigned GALLOWS_SIZE{ 10 };
constexpr unsigned MAX_GUESSES{ 7 };
constexpr size_t NUM_WORDS{ 15 };

// Sample list of words to guess (five each 5, 6 & 7 character long).
std::vector<std::string> words
{
	"blimp", "inuit", "roach", "ankle", "could",
	"visual", "quartz", "studio", "jockey", "hijack",
	"located", "alcohol", "crochet", "polymer", "humidor"
};

class Gallows
{
	const std::string gallows[GALLOWS_SIZE] =
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

  public:
	void show(const int stage)
	{
		if (stage >= 0 && stage < GALLOWS_SIZE)
			std::cout << gallows[TOP] << gallows[stage] << gallows[BASE];
	}
};

// Prompt and get a single character from user.
char getCharInput(const std::string prompt)
{
	char c;

	cout << prompt;
	// Input from user.
	std::cin.get(c);

	if (std::cin.peek() != '\n')
		// More than a single char input.
		c = 0;

	// Eat any remaining chars and LF.
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return tolower(c);
}

// Check if character is already stored in list of guesses.
bool checkLetter(std::vector<char>& guessedLetters, const char c, const std::string response)
{
	if (std::find(guessedLetters.begin(), guessedLetters.end(), c) == guessedLetters.end())
	{
		cout << response;
		guessedLetters.push_back(c);
		return true;
	}
	else
		cout << "You already guessed \"" << c << "\"\n";

	return false;
}

void playHangman()
{
	Gallows gallows;
	std::uniform_int_distribution<unsigned long> dist(0, NUM_WORDS); // Random number generation [0...NUM_WORDS].
	std::random_device rd;
	std::mt19937 mt(rd());                                           // Non-deterministic 32-bit seed.
	unsigned remainingGuesses{ MAX_GUESSES };                        // Number of guesses remianing.
	unsigned randNum{ dist(mt) };		                         // Random number distribution [inclusive, inclusive].
	size_t remainingLetters{ words[randNum].size() };                // Number un-guessed letters remaining in word.
	std::string clue = { std::string(words[randNum].size(), 'X') };  // Display word clue.
	std::vector<char> guessedLetters;                                // List of guessed letters.

	// Empty the list.
	guessedLetters.clear();

	// Loop until word is guessed or user gets hung.
	while (remainingLetters && remainingGuesses)
	{
		// Display clue and get the user's guess.
		char guess = getCharInput("Guess the word: " + clue + "\n");

		if (isalpha(guess))
			// Replace 'X' with matching character.
			std::transform(words[randNum].cbegin(), words[randNum].cend(), clue.begin(), clue.begin(),
				[=](char s, char d) { return (s == guess ? guess : d); });

		// Check if guessed character had any matches in word.
		if (int matchedLetters = std::count(clue.cbegin(), clue.cend(), guess))
			// Correct guess, decrement remaining letters by proper amount.
			remainingLetters -= (checkLetter(guessedLetters, guess, "Good guess!\n") ? matchedLetters : 0);
		else
			// Wrong guess, decrement number of remaining guesses.
			remainingGuesses -= (checkLetter(guessedLetters, guess, "Wrong guess!\n") ? 1 : 0);

		// Display results of guess.
		gallows.show(remainingGuesses);
		cout << "Letters guessed: ";
		std::copy(guessedLetters.begin(), guessedLetters.end(), std::ostream_iterator<char>(std::cout, " "));
		cout << "\n\n";
	}

	// Display result of game.
	if (!remainingLetters)
		cout << "Congratulations!!! You guessed my word \"" << clue << "\" with " << guessedLetters.size() << " guesses!\n";
	else
		cout << "You've been hung!!!\n";
}

int main()
{
	// Display program purpose.
	cout << "Let's play a game of Hangman!\n\n";

	do { playHangman(); } while (tolower(getCharInput("Play again? y/n ")) == 'y');

	return 0;
}
