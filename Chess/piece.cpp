#include "piece.h"

Piece::Piece(enum Color color, enum Type type, char name, int x, int y) {
    this->color = color;
    this->type = type;
    this->name = name;
    this->x = x;
    this->y = y;
}

char Piece::getName() const {
    return name;
}

std::string Piece::to_string() const {
    std::string colorString;
    switch (color) {
    case WHITE:
        colorString = "White";
        break;
    case BLACK:
        colorString = "Black";
    }
    return std::string(colorString + " " + name + " (" + std::to_string(x) + ", " + std::to_string(y) + ").");
}

enum Color Piece::getColor() const {
    return color;
}

int Piece::getX() const {
    //std::cout << "x in piece: " << x << std::endl;
    return x;
}

int Piece::getY() const {
    return y;
}

int Piece::getType() const {
    return type;
}

void Piece::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

bool Piece::isFirstMove() const {
    return firstMove;
}

// wild boolean logic RAAAAHHHH
int Piece::move(int destX, int destY, Board* pBoard) {
    if (checkMoveIntercepted(destX, destY, pBoard) || kingVulnerable(destX, destY, pBoard))
        return -1;

    bool taking = pBoard->getPieceAt(destX, destY);

    switch (type) {
    case PAWN:
        if ((taking && abs(destY - y) == 1 && (destX == x + 1 || destX == x - 1)) ||
            (!taking && ((abs(destY - y) == 1 && destX == x || (firstMove && abs(destY - y) == 2 && destX == x))))) {
            return 0;
        }
        else break;

    case ROOK:
        // TODO: make sure to check if other pieces are in the way board-side
        if (destX == x || destY == y) {
            return 0;
        }
        else break;

    case KNIGHT:
        if (((destY == y + 1 || destY == y - 1) && (destX == x + 2 || destX == x - 2)) ||
            ((destY == y - 2 || destY == y + 2) && (destX == x + 1 || destX == x - 1))) {
            return 0;
        }
        else break;

    case BISHOP:
        // TODO: make sure to check if other pieces are in the way board-side
        if (abs(destX - x) == abs(destY - y)) {
            return 0;
        }
        else break;

    case QUEEN:
        // TODO: make sure to check Q no piece is in the way of move board-side
        if ((abs(destX - x) == abs(destY - y))
            || (destX == x || destY == y)) {
            return 0;
        }
        else break;

    case KING:
        if (abs(destX - x) <= 1 && abs(destY - y) <= 1) {
            return 0;
        }
        else break;
    }
    return -1;
}

bool Piece::diagonalInterceptCheck(int targetX, int targetY, Board* pBoard) const {
    int checkTargetX = targetX - x, checkTargetY = targetY - y;
    int stepX = checkTargetX > 0 ? 1 : -1, stepY = checkTargetY > 0 ? 1 : -1;
    // weird ass for loop
    for (int modX = stepX, modY = stepY; modX != checkTargetX;) {
        std::cout << "Checking if there is a piece at " << x + modX << " " << y + modY << std::endl;
        if (pBoard->getPieceAt(x + modX, y + modY))
            return false;
        std::cout << "No piece there." << std::endl;
        modX += stepX, modY += stepY;
    }
    return true;
}

// Checks if the suggested straight move is possible (true) or if the move is intercepted by another piece (false). 
bool Piece::straightInterceptCheck(int targetX, int targetY, int dirX, int dirY, Board* pBoard) const {
    if (dirX != 0) {
        for (int checkX = x + (dirX > 0 ? 1 : -1); checkX != targetX; checkX += (dirX > 0 ? 1 : -1)) {
            std::cout << "Checking if there is a piece at " << targetX << " " << targetY << std::endl;
            if (pBoard->getPieceAt(checkX, targetY))
                return false;
            std::cout << "No piece there." << std::endl;
        }
    }
    else if (dirY != 0) {
        for (int checkY = y + (dirY > 0 ? 1 : -1); checkY != targetY; checkY += (dirY > 0 ? 1 : -1)) {
            std::cout << "Checking if there is a piece at " << targetX << " " << checkY << std::endl;
            if (pBoard->getPieceAt(targetX, checkY))
                return false;
            std::cout << "No piece there." << std::endl;
        }
    }
    return true;
}

bool Piece::checkMoveIntercepted(int col, int row, Board* pBoard) {
    int dirX = col - x, dirY = row - y;

    bool pieceBetween = false;

    // check if the intended move is impossible because of a piece intercepting the move
    switch (type) {
    case ROOK:
        pieceBetween = !straightInterceptCheck(col, row, dirX, dirY, pBoard);
        break;
    case BISHOP:
        pieceBetween = !diagonalInterceptCheck(col, row, pBoard);
        break;
    case QUEEN:
        // check if move is diagonal or straight, same check as bishop and rook respectively
        if ((dirX != 0 && dirY == 0) || (dirX == 0 && dirY != 0))
            pieceBetween = !straightInterceptCheck(col, row, dirX, dirY, pBoard);
        else // check diagonal
            pieceBetween = !diagonalInterceptCheck(col, row, pBoard);
        break;
    }

    return pieceBetween;
}

// Does what it says.
// Returns a vector of all available destinations in {x, y} format.
std::vector<int[2]> Piece::calculateMovesForPiece(Board* pBoard) {
    std::vector<int[2]> moves;

    /*
    There is an efficient way and an inefficient way to do this
    I think I'm going to do the inefficient way first, since I can always come back and just rewrite this function
    So I'm going to loop through every possible destination on the board, and if everything checks out, the move is valid
    To do this, I have to restructure the move check algorithm in the pieces to integrate the intercept check inside the piece
    so that means I have to pass a pointer to the board so I can access the locations of the pieces.
    */

    // woah so simple and so horrible at the same time. O(n^2) :(
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            bool taking = (pBoard->getPieceAt(x, y));
            if (move(x, y, pBoard) == 0)
                moves.emplace_back(x, y);
        }
    }

    // I'll do the more complicated way later if there is one.

    return moves;
}

bool Piece::kingVulnerable(int destX, int destY, Board* pBoard) {
    int presX = x, presY = y;
    setPosition(destX, destY);

    std::vector<std::unique_ptr<Piece>>* pPieces = pBoard->getPieces();
    
    Piece* pKing;

    // find the king
    for (int i = 0; i < pPieces->size(); i++) {
        Piece* pPiece = pPieces->at(i).get();
        if (pPiece->getType() == KING && pPiece->getColor() == color)
            pKing = pBoard->getPieceAt(pPiece->getX(), pPiece->getY());
    }

    for (int i = 0; i < pPieces->size(); i++) {
        if (pPieces->at(i)->getColor() != color) {
            Piece* pPiece = pPieces->at(i).get();
            if (pPiece->move(pKing->getX(), pKing->getY(), pBoard) == 0)
                return false;
        }
    }

    // reset position
    setPosition(presX, presY);

    return true;
}