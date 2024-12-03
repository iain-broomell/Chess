#pragma once

#include <string>
#include <vector>
#include <memory>
#include "piece.h"

class Player {
private:
	std::string name;
	enum Color color;
	std::vector<std::unique_ptr<Piece>> capturedPieces;
	int piecesTaken;
	bool bot;
public:
	Player(const std::string& rName, enum Color iColor, bool isBot);
	std::string getName() const;
	enum Color getColor() const;
	void capturePiece(Piece* pPiece);
	void printCapturedPieces();
	std::vector<std::unique_ptr<Piece>> getCapturedPieces();
	bool isBot() const;
	void botMove(Board* pBoard);
};