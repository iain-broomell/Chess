#pragma once

#include <iostream>
#include <string>
#include <vector>

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

struct xy {
    int x;
    int y;
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
    bool diagonalInterceptCheck(int targetX, int targetY, std::vector<std::unique_ptr<Piece>>* pPieces);

    // Checks if the suggested straight move is possible (true) or if the move is intercepted by another piece (false). 
    bool straightInterceptCheck(int targetX, int targetY, int dirX, int dirY, std::vector<std::unique_ptr<Piece>>* pPieces);

    bool checkMoveIntercepted(int col, int row, std::vector<std::unique_ptr<Piece>>* pPieces);

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
    bool kingVulnerable(int destX, int destY, std::vector<std::unique_ptr<Piece>>* pPieces);
    int move(int destX, int destY, std::vector<std::unique_ptr<Piece>>* pPieces);
    std::vector<xy> calculateMovesForPiece(std::vector<std::unique_ptr<Piece>>* pPieces);
    static Piece* getPieceAt(int x, int y, std::vector<std::unique_ptr<Piece>>* pPieces);
};