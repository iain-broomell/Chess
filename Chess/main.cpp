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
    Piece(enum Type type, const std::string &name, int x, int y) {
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
        return type;
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

        switch (type) {
        case PAWN:
            if (taking && !((x + 1 == destX || x - 1 == destX) && y + 1 == destY)) {
                return -1;
            }
            else if (!taking && ((isFirstMove() && y + 2 != destY) || (!isFirstMove() && y + 1 != destY))) {
                return -1;
            }

            setPosition(destX, destY);
            break;
        case CASTLE:
            break;
        case KNIGHT:
            break;
        case BISHOP:
            break;
        case QUEEN:
            break;
        case KING:
            break;
        }
        return 0;
    }
};

int main()
{
    // init board
    
    std::cout << "Chess (" << VERSION << "), by Iain Broomell." << std::endl;

    Piece pawn = Piece(PAWN, "White Pawn", 0, 0);
    return 0;
}