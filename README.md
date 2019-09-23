# Pima CC CIS278 Week #10 
## Exercise 21.11 
### Hangman Game
C++ How to Program, 10/e, Deitel

Write a program that plays the game of Hangman. The program should pick a word (which is either coded directly into the program or read from a text file) and display the following:
```Text
  Guess the word: XXXXXX
```
Each X represents a letter. The user tries to guess the letters in the word. The appropriate response yes or no should be displayed after each guess. After each incorrect guess, display the diagram with another body part filled. After seven incorrect guesses, the user should be hanged. The display should look as follows:
```Text
     O
    /|\
     |
    / \
```
After each guess, display all user guesses. If the user guesses the word correctly, display
```Text
  Congratulations!!! You guessed my word. Play again? yes/no
```

Notes:
* Compiled with MS Visual Studio 2017 Community (v141).
* Submitted in partial fulfillment of the requirements of PCC CIS-278.
* Updated code per suggestions received on StackExchange Code Review [here](https://codereview.stackexchange.com/questions/229422/hangman-game-yahg)
