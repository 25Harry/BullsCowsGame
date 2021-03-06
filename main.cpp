// BullCowGame.cpp : Defines the entry point for the console application.
/* This is console executable, that makes use of BullCow class .
This acts as the view in a MVC pattern, and is responsible for all user interaction.
For game logic, see the FBullCowGame class.
*/
// future ideas: add a menu for difficulty options, exit game from middle of game.
#pragma once

#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "FBullCowGame.h"

//to make Unreal friendly
using FText = std::string;
using int32 = int;

//function prototypes as outside of class
void PrintIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
bool AskToPlayAgain();
FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

					 // the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);

	return 0; // exit the application
}
void SetConsoleColour(WORD* h, DWORD Colour)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &Info);
	*h = Info.wAttributes;
	SetConsoleTextAttribute(hStdout, Colour);
}
void ResetConsoleColour(WORD h)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), h);
}
void PrintIntro()
{
	WORD Attributes = 0;
	SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	std::cout << "						 ___                          ___       ____    ____    ____      ___	 _____             ___     " << std::endl;
	std::cout << "						|   |  |    |  |      |      |         |    |  |    |  |    |    |      |     |  |     |  |		  " << std::endl;
	std::cout << "						|___|  |    |  |      |      |___      |____|  |    |  |    |    |      |     |  |     |  |___      " << std::endl;
	std::cout << "						|   |  |    |  |      |          |     |    |  |    |  |    |    |      |     |  |  |  |      |  " << std::endl;
	std::cout << "						|___|  |____|  |____  |____  ____|     |    |  |    |  |___/     |____  |_____|  |__|__|  ____|  " << std::endl;
	std::cout << "" << std::endl;
	std::cout << std::endl;
	std::cout << "								WELCOME TO BULLs & COWs, a money chase word puzzle game.\n" << std::endl;
	std::cout << std::endl;
	std::cout << "									          }   {            ___ " << std::endl;
	std::cout << "									          (o o)           (o o) " << std::endl;
	std::cout << "									   /-------\\ /             \\ /-------\\ " << std::endl;
	std::cout << "									  / | BULL |O               O| COW  | \\ " << std::endl;
	std::cout << "									 *  |-,--- |                 |------|  * " << std::endl;
	std::cout << "									    ^      ^                 ^      ^ " << std::endl;
	std::cout << std::endl;
	std::cout << "									  BULL refers to            COW refers to" << std::endl;
	std::cout << "									 correct letter in         correct letter in" << std::endl;
	std::cout << "									  correct place              wrong place " << std::endl;
	std::cout <<  std::endl;
	std::cout << std::endl;
	ResetConsoleColour(Attributes);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	std::cout << "STORYLINE:\n";
	std::cout << "1. You have a cryptex, which is basically a security apparatus in which you have to put up a secret password for it to open.\n";
	std::cout << "2. Inside that cryptex is a paper, which includes an address where a huge amount of money is dumped.\n";
	std::cout << "3. You are given 10 chances to guess the right password, and the address and the money will be yours.\n";
	std::cout << "4. Otherwise the cryptex will destroy that address forever.\n";
	std::cout << std::endl;
	
	std::cout << "RULES:\n";
	std::cout << "1. You are given " << BCGame.GetMaxTries() << " chances to guess the right password.\n";
	std::cout << "2. The password is a " << BCGame.GetHiddenWordLength(); 
	std::cout  << " letter isogram(word with no repeating letters).\n";
	std::cout << "3. After each guess, you will be told how many bulls and cows were in that last guess.\n";
	std::cout << "4. BULL refers to correct letter in correct place and COW refers to correct letter in wrong place.\n";
	std::cout << std::endl;
	
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram which is the password to this cryptex? Let's find out.\n";
	std::cout << std::endl;
	return;
}

// plays a single game to completion
void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	// loop asking for guesses while the game
	// is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();

		// submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		WORD Attributes = 0;
		SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries();
		std::cout << ". Enter your guess: ";
		std::getline(std::cin, Guess);

		// check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again(y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WON! Your reward awaits at 221b Baker Street, London.\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
	}
}
