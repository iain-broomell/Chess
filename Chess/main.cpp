/*
main.cpp
Programmed by Iain Broomell
A program which allows a user to play chess.
*/

#include <iostream>

const std::string VERSION = "v0.1";
const enum Type {
    PAWN,
    CASTLE,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

class Piece {
private:
    std::string name;
    int x;
    int y;
    enum Type type;
    bool firstMove = true;
public:
    Piece(enum Type type, std::string name, int x, int y) {
        this->type = type;
        this->name = name;
        this->x = x;
        this->y = y;
    }
    ~Piece() { std::cout << name << " was destroyed" << std::endl; }
    std::string getName() {
        return name;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getType() {
        return this->type;
    }
    int* getXY() {
        int position[2] = { x, y };
        return position;
    }
    void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool isFirstMove() {
        return firstMove;
    }

    int move(int destX, int destY, bool taking) {
        int x = getX();
        int y = getY();

        // if piece is a PAWN
        if (type == PAWN) {
            if (taking && !((x + 1 == destX || x - 1 == destX) && y + 1 == destY)) {
                return -1;
            }
            else if (!taking && ((isFirstMove() && y + 2 != destY) || (!isFirstMove() && y + 1 != destY))) {
                return -1;
            }

            setPosition(destX, destY);
            return 0;
        }

        // if piece is a CASTLE
        if (type == CASTLE) {

        }

        // if piece is a KNIGHT
        if (type == KNIGHT) {

        }

        // if piece is a BISHOP
        if (type == BISHOP) {

        }

        // if piece is a QUEEN
        if (type == QUEEN) {

        }

        // if piece is a KING
        if (type == KING) {

        }
    }
};

int main()
{
    // init board
    
    std::cout << "Chess (" << VERSION << "), by Iain Broomell." << std::endl;

    Piece pawn = Piece(PAWN, "White Pawn", 0, 0);
    return 0;
}