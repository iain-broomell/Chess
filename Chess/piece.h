#pragma once

#include <iostream>
#include <string>
#include "board.h"

const enum Type {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

const enum Color {
    WHITE,
    BLACK
};

class Piece {
private:
    char name;
    int x;
    int y;
    enum Type type;
    enum Color color;
    bool firstMove = true;

    // Checks if the suggested diagonal move is possible (true) or if the move is intercepted by another piece (false).
    bool diagonalInterceptCheck(int targetX, int targetY, Board* pBoard) const;

    // Checks if the suggested straight move is possible (true) or if the move is intercepted by another piece (false). 
    bool straightInterceptCheck(int targetX, int targetY, int dirX, int dirY, Board* pBoard) const;

    bool checkMoveIntercepted(int col, int row, Board* pBoard);

public:
    Piece(enum Color color, enum Type type, char name, int x, int y);
    char getName() const;
    std::string to_string() const;
    enum Color getColor() const;
    int getX() const;
    int getY() const;
    int getType() const;
    void setPosition(int x, int y);
    bool isFirstMove() const;
    bool kingVulnerable(int destX, int destY, Board* pBoard);
    int move(int destX, int destY, Board* pBoard);
    std::vector<int[2]> calculateMovesForPiece(Board* pBoard);
};