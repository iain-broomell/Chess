/*
Program: Chess.cpp
Programmed by Iain Broomell
A program which allows a user to play chess.
*/

#include <iostream>

const std::string VERSION = "v0.1";

class Piece {
private:
    int id;
    std::string name;
    int x;
    int y;
public:
    Piece(std::string name, int x, int y) {
        this->name = name;
        this->x = x;
        this->y = y;
    }
    ~Piece() { std::cout << name << " was destroyed" << std::endl; }
    int getId() {
        return id;
    }
    std::string getName() {
        return name;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int* getXY() {
        int position[2] = { x, y };
        return position;
    }
    void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Pawn : Piece {
private:
    bool firstMove = true;
public:
    bool isFirstMove() {
        return firstMove;
    }

    int move(int destX, int destY, bool taking) {
        int x = getX();
        int y = getY();

        if (taking && !((x + 1 == destX || x - 1 == destX) && y + 1 == destY)) {
            return -1;
        }
        else if (!taking && ((isFirstMove() && y + 2 != destY) || (!isFirstMove() && y + 1 != destY))) {
            return -1;
        }

        setPosition(destX, destY);
        return 0;
    }
};

int main()
{
    // !TODO!: find a better way. not very memory effecient.
    // board init goes here

    std::cout << "Chess (" << VERSION << "), by Iain Broomell." << std::endl;
    return 0;
}