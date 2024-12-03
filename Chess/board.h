#pragma once

#include <windows.h>
#include <vector>
#include "piece.h"
#include "player.h"

/* Uses a vector of pieces because it's easier than an array, but less performant when doing calculations and stuff.
Might switch to a more performant version later, or simply representing the entire board within a vector. */
// Big Ass Class
// Maybe split this up into smaller parts later?
class Board {
private:
	std::vector<std::unique_ptr<Piece>> pieces;
	// Populates vector of pieces. Pass reference to pieces vector as a parameter in function call. 
	void populatePieces(std::vector<std::unique_ptr<Piece>>& pieces);
	bool isInputValid(std::string in);
	std::string getInput(std::string message);
	bool getYesNo(std::string message);
	void takePiece(Piece* pPiece, Player* pPlayer);
public:
	Board();
	Piece* getPieceAt(int x, int y);
	void printBoard(int selected[2][2], Player* pPlayer, Player* pPlayers[2]);
	// A mess of input checks to make sure you're doing the right thing (it sucks). 
	void inputLoop(Player* pPlayer, Player* pPlayers[2]);
	std::vector<std::unique_ptr<Piece>>* getPieces();
};