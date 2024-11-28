/*
main.cpp
Programmed by Iain Broomell
A program which allows a user to play chess.
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>

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

    std::string to_string() const {
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

    enum Color getColor() {
        return color;
    }
    int getX() const {
        //std::cout << "x in piece: " << x << std::endl;
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

    // wild boolean logic RAAAAHHHH
    int move(int destX, int destY, bool taking) {
        switch (type) {
        case PAWN:
            if ((taking && destY == y + 1 && (destX == x + 1 || destX == x - 1)) ||
                (!taking && ((destY == y + 1 && destX == x || (firstMove && destY == y + 2 && destX == x))))) {
                setPosition(destX, destY);
                return 0;
            }
            else break;

        case CASTLE:
            // TODO: make sure to check if other pieces are in the way board-side
            if (destX == x || destY == y) {
                setPosition(destX, destY);
                return 0;
            }
            else break;

        case KNIGHT:
            if (((destY == y + 1 || destY == y - 1) && (destX == x + 2 || destX == x - 2)) ||
                ((destY == y - 2 || destY == y + 2) && (destX == x + 1 || destX == x - 1))) {
                setPosition(destX, destY);
                return 0;
            }
            else break;

        case BISHOP:
            // TODO: make sure to check if other pieces are in the way board-side
            if (abs(destX - x) == abs(destY - y)) {
                setPosition(destX, destY);
                return 0;
            }
            else break;

        case QUEEN:
            // TODO: make sure to check no piece is in the way of move board-side
            if ((abs(destX - x) == abs(destY - y)) 
                || (destX == x || destY == y)) {
                setPosition(destX, destY);
                return 0;
            }
            else break;

        case KING:
            if (abs(destX - x) <= 1 && abs(destY - y) <= 1) {
                setPosition(destX, destY);
                return 0;
            }
            else break;
        }
        return -1;
    }
};

class Board {
private:
    std::vector<std::unique_ptr<Piece>> pieces;
    void init() {
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
        // order: pawn, castle, knight, bishop, queen, king
        char nameCharacters[6] = {
            'p', 'c', 'k', 'b', 'q', 'K'
        };
        uint8_t startColumn[6] = {
            0, 0, 1, 2, 3, 4  
        };
        uint8_t columnSteps[6] = {
            1, 7, 5, 3, 0, 0 
        };

        // bah more than O(n) complexity :(
        for (uint8_t typeIndex = 0; typeIndex < 6; typeIndex++) {
            for (uint16_t iter = 0; iter < countOfType[typeIndex]; iter++) {
                bool isFirstHalf = iter < countOfType[typeIndex] / 2;

                enum Color color = isFirstHalf ? BLACK : WHITE;

                uint8_t whiteRow = typeIndex == 0 ? 6 : 7; // not pretty but not sure how else to do this efficiently
                uint8_t blackRow = typeIndex == 0 ? 1 : 0;

                uint8_t row = color == WHITE ? whiteRow : blackRow;

                uint8_t columnOffset = (iter == 0 || iter == countOfType[typeIndex] / 2 ? 0 : 
                    columnSteps[typeIndex] * (isFirstHalf ? iter : iter - countOfType[typeIndex] / 2));
                //std::cout << "maing piece with x of" << startColumn[typeIndex] + columnOffset << std::endl;
                pieces.emplace_back(std::make_unique<Piece>(
                    Piece(color, static_cast<Type>(typeIndex), nameCharacters[typeIndex], startColumn[typeIndex] + columnOffset, int(row))));
            }
        }
    }
public:
    Board() {
        init();
    }

    std::vector<std::unique_ptr<Piece>> &getPieces() {
        return pieces;
    }

    // switch to a cached board functionality later
    void printBoard() {
        HANDLE hConsole; // for changing output color
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // change to macros
        int whiteColor = 7;
        int greenColor = 2;
        int redColor = 4;
        int yellowColor = 6;

        char board[8][8][2] = {};

        // loop through board, populate with placeholders
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                board[row][col][0] = '.';
                board[row][col][1] = '.';
            }
        }

        // loop through pieces, put name char and color char at coords
        for (int i = 0; i < pieces.size(); i++) {
            Piece* piece = pieces[i].get();

            // doesn't work because all returning the same value. Very confusing. 
            int x = piece->getX();
            int y = piece->getY();

            board[y][x][0] = piece->getColor() == WHITE ? 'w' : 'b';
            board[y][x][1] = piece->getName();
        }

        // print each board slot
        for (int row = 0; row < 8; row++) {
            SetConsoleTextAttribute(hConsole, yellowColor);
            std::cout << (8 - row) << ' ';
            for (int col = 0; col < 8; col++) {
                int textColor;
                switch (board[row][col][0]) {
                case 'b':
                    textColor = redColor;
                    break;
                case 'w':
                    textColor = greenColor;
                    break;
                default:
                    textColor = whiteColor;
                    break;
                }

                SetConsoleTextAttribute(hConsole, textColor);
                std::cout << board[row][col][1] << ' ';
            }
            std::cout << std::endl;
        }

        SetConsoleTextAttribute(hConsole, yellowColor);
        char letters[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'h', 'i' };
        std::cout << "  ";
        for (int i = 0; i < 8; i++) {
            std::cout << letters[i] << ' ';
        }
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, whiteColor);
    }
};

int main()
{
    std::cout << "Chess (" << VERSION << "), by Iain Broomell." << std::endl;
    std::cout << "btw... green goes first." << std::endl;

    Board board = Board();
    board.printBoard();

    return 0;
}