/*
main.cpp
Programmed by Iain Broomell
A program which allows a user to play chess.
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory>

const std::string VERSION = "v0.1";
const enum Type {
    PAWN,
    CASTLE,
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
public:
    Piece(enum Color color, enum Type type, char name, int x, int y) {
        this->color = color;
        this->type = type;
        this->name = name;
        this->x = x;
        this->y = y;
    }
    ~Piece() {}
    char getName() const {
        return name;
    }

    std::string to_string() {
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

    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
    int getType() const {
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

    bool isFirstMove() const {
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
            else if (!taking && ((isFirstMove() && y + 2 != destY) || y + 1 != destY)) {
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

class Board {
private:
    void init() {
        /*
        Instead of this, have a constant array of piece types, iterate through the array rather
        than using i as the switch parameter. then, use a single function with a switch statement
        to populate the pieces vector.
        */
        std::vector<std::unique_ptr<Piece>> pieces;
        pieces.reserve(32);
        populatePieces(pieces);
    }

    // Populates vector of pieces. Pass reference to pieces vector as a parameter in function call. 
    void populatePieces(std::vector<std::unique_ptr<Piece>>& pieces) {
        // assumes every count in this list is an even number
        uint16_t countOfType[6] = {
            16, // pawns
            4,  // castles
            4,  // knights
            4,  // bishops
            2,  // queens
            2   // kings
        };

        // this is so verbose but wtf else am I supposed to do for this procedural stuff (fix maybe?)
        char nameCharacters[6] = {
            'p', // pawn
            'c', // castle
            'k', // knight
            'b', // bishop
            'q', // queen
            'K'  // king
        };

        uint8_t startColumn[6] = {
            0, // pawn
            0, // castle
            1, // knight
            2, // bishop
            3, // queen
            4  // king
        };

        uint8_t columnSteps[6] = {
            1, // pawn
            7, // castle
            5, // knight
            3, // bishop
            0, // queen
            0  // king
        };

        /*
        after test, need to fix columning. Columns are not accurately generated. Probably not adding the iteration in the constructor.
        */

        // bah more than O(n) complexity :(
        for (uint8_t typeIndex = 0; typeIndex < 6; typeIndex++) {
            for (uint16_t iter = 0; iter < countOfType[typeIndex]; iter++) {
                bool isFirstHalf = iter < countOfType[typeIndex] / 2;

                enum Color color = isFirstHalf ? WHITE : BLACK;

                uint8_t whiteRow = typeIndex == 0 ? 1 : 0; // not pretty but not sure how else to do this efficiently
                uint8_t blackRow = typeIndex == 0 ? 6 : 7;

                uint8_t row = color == WHITE ? whiteRow : blackRow;

                uint8_t columnOffset = (iter == 0 || iter == countOfType[typeIndex] / 2 ? 0 : 
                    columnSteps[typeIndex] * (isFirstHalf ? iter : iter - countOfType[typeIndex] / 2));

                pieces.emplace_back(std::make_unique<Piece>(
                    Piece(color, static_cast<Type>(typeIndex), nameCharacters[typeIndex], startColumn[typeIndex] + columnOffset, int(row))));
            }
        }
        
        // debug
        for (int i = 0; i < 32; i++) {
            std::cout << std::to_string(i) << " " << (*pieces[i]).to_string() << std::endl;
        }
    }
public:
    Board() {
        init();
    }
};

int main()
{
    std::cout << "Chess (" << VERSION << "), by Iain Broomell." << std::endl;

    Board board = Board();

    return 0;
}