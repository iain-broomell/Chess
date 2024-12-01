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
#include <sstream>

const std::string VERSION = "v0.1";
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

static void clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = { 0, 0 };
    DWORD dwCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, topLeft, &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, topLeft);
}

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
    int move(int destX, int destY, bool taking) const {
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
};

class Player {
private:
    std::string name;
    enum Color color;
    std::vector<std::unique_ptr<Piece>> capturedPieces;
    int piecesTaken = 0;
public:
    Player(const std::string& rName, enum Color iColor) {
        name = rName;
        color = iColor;
        capturedPieces.reserve(15);
    }
    std::string getName() const {
        return name;
    }
    enum Color getColor() {
        return color;
    }
    void addTakenPiece(Piece* pPiece) {
        capturedPieces.push_back(std::make_unique<Piece>(*pPiece));
    }
};

/* Uses a vector of pieces because it's easier than an array, but less performant when doing calculations and stuff.
Might switch to a more performant version later, or simply representing the entire board within a vector. */
// Big Ass Class
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
            4,  // rooks
            4,  // knights
            4,  // bishops
            2,  // queens
            2   // kings
        };

        // this is so verbose but wtf else am I supposed to do for this procedural stuff (fix maybe?)
        // order: pawn, rook, knight, bishop, queen, king
        char nameCharacters[6] = {
            'p', 'r', 'k', 'b', 'Q', 'K'
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

                enum Color color = isFirstHalf ? WHITE : BLACK;

                uint8_t whiteRow = typeIndex == 0 ? 1 : 0; // not pretty but not sure how else to do this efficiently
                uint8_t blackRow = typeIndex == 0 ? 6 : 7;

                uint8_t row = color == WHITE ? whiteRow : blackRow;

                uint8_t columnOffset = (iter == 0 || iter == countOfType[typeIndex] / 2 ? 0 : 
                    columnSteps[typeIndex] * (isFirstHalf ? iter : iter - countOfType[typeIndex] / 2));
                //std::cout << "maing piece with x of" << startColumn[typeIndex] + columnOffset << std::endl;
                pieces.emplace_back(std::make_unique<Piece>(
                    color, static_cast<Type>(typeIndex), nameCharacters[typeIndex], startColumn[typeIndex] + columnOffset, int(row)));
            }
        }
    }
    
    bool isInputValid(std::string in) {
        bool valid = true;

        if (in.length() > 0 && std::isalpha(in[0]) && std::tolower(in[0]) == 'q')
            return true;

        if (in.length() > 2) {
            std::cout << "Input too long." << std::endl;
            valid = false;
        } if (in.length() < 2) {
            std::cout << "Input too short." << std::endl;
            valid = false;
        }

        char char1 = in[0];
        char char2 = in[1];

        if (!std::isalpha(char1) || (char1 < 'a' || char1 > 'h')) {
            std::cout << "First input character must be alphabetical (a - h). Entered: " << char1 << "." << std::endl;
            valid = false;
        } if (!std::isdigit(char2) || (char2 - '0' < 1 || char2 - '0' > 8)) {
            std::cout << "Second input character must be numerical (1 - 8). Entered: " << char2 << "." << std::endl;
            valid = false;
        }

        return valid;
    }

    std::string getInput(std::string message) {
        std::cout << message;

        std::string in;
        std::cin >> in;

        if (!isInputValid(in))
            in = getInput(message);

        return in;
    }

    bool getYesNo(std::string message) {
        std::cout << message;

        std::string in;
        std::cin >> in;

        if (in.length() != 1) {
            std::cout << "Input must be single character y or n." << std::endl;
            in = getYesNo(message);
        }

        char yesOrNo = std::tolower(in[0]);
        switch (yesOrNo) {
        case 'y':
            return true;
        case 'n':
            return false;
        default:
            std::cout << "Input must be either y or n." << std::endl;
            return getYesNo(message);
        }
    }

    // Checks if the suggested diagonal move is possible (true) or if the move is intercepted by another piece (false).
    bool diagonalInterceptCheck(int originX, int originY, int targetX, int targetY) {
        int checkTargetX = targetX - originX, checkTargetY = targetY - originY;
        int stepX = checkTargetX > 0 ? 1 : -1, stepY = checkTargetY > 0 ? 1 : -1;
        // weird ass for loop
        for (int modX = stepX, modY = stepY; modX != checkTargetX;) {
            std::cout << "Checking if there is a piece at " << originX + modX << " " << originY + modY << std::endl;
            if (getPieceAt(originX + modX, originY + modY))
                return false;
            std::cout << "No piece there." << std::endl;
            modX += stepX, modY += stepY;
        }
        return true;
    }

    // Checks if the suggested straight move is possible (true) or if the move is intercepted by another piece (false). 
    bool straightInterceptCheck(int originX, int originY, int targetX, int targetY, int dirX, int dirY) {
        if (dirX != 0) {
            for (int checkX = originX + (dirX > 0 ? 1 : -1); checkX != targetX; checkX += (dirX > 0 ? 1 : -1)) {
                std::cout << "Checking if there is a piece at " << targetX << " " << targetY << std::endl;
                if (getPieceAt(checkX, targetY))
                    return false;
                std::cout << "No piece there." << std::endl;
            }
        }
        else if (dirY != 0) {
            for (int checkY = originY + (dirY > 0 ? 1 : -1); checkY != targetY; checkY += (dirY > 0 ? 1 : -1)) {
                std::cout << "Checking if there is a piece at " << targetX << " " << checkY << std::endl;
                if (getPieceAt(targetX, checkY))
                    return false;
                std::cout << "No piece there." << std::endl;
            }
        }
        return true;
    }

    void takePiece(Piece* pPiece, Player *pPlayer) {
        for (int index = 0; index < pieces.size(); index++) {
            if (pieces.at(index).get() == pPiece) {
                pieces.erase(pieces.begin() + index);
                pPlayer->addTakenPiece(pPiece);
            }
        }
    }

    std::string convertCharToLongName(char _name) {
        char charNames[6] = { 'p', 'r', 'k', 'b', 'q', 'K' };
        std::string longNames[6] = { "Pawn", "Rook", "Knight", "Bishop", "Queen", "King" };

        // if I organized them alphabetically I could do a binary search but I can't be bothered
        for (int i = 0; i < 6; i++) {
            if (_name == charNames[i])
                return longNames[i];
        }

        return "NOTFOUND";
    }

    bool checkMoveIntercepted(Piece *pPiece, int col, int row) {
        enum Type pieceType = static_cast<Type>(pPiece->getType());

        int x = pPiece->getX(), y = pPiece->getY();
        int dirX = col - x, dirY = row - y;

        bool pieceBetween = false;

        // check if the intended move is impossible because of a piece intercepting the move
        switch (pieceType) {
        case ROOK:
            pieceBetween = !straightInterceptCheck(x, y, col, row, dirX, dirY);
            break;
        case BISHOP:
            pieceBetween = !diagonalInterceptCheck(x, y, col, row);
            break;
        case QUEEN:
            // check if move is diagonal or straight, same check as bishop and rook respectively
            if ((dirX != 0 && dirY == 0) || (dirX == 0 && dirY != 0))
                pieceBetween = !straightInterceptCheck(x, y, col, row, dirX, dirY);
            else // check diagonal
                pieceBetween = !diagonalInterceptCheck(x, y, col, row);
            break;
        }

        return pieceBetween;
    }
public:
    Board() {
        init();
    }

    Piece* getPieceAt(int x, int y) {
        Piece* piece = nullptr;
        // there has to be a better more effecient way to look for the correct piece.
        // maybe I could do a binary search if the vector size was guarenteed constant
        // the search would be based on the type of the piece (pawns are always below 16, etc.)
        // O(n) complexity, not bad but worse than it could be. 
        for (int i = 0; i < pieces.size(); i++) {
            Piece* ptr = pieces[i].get();
            if (ptr->getY() == y && ptr->getX() == x)
                piece = ptr;
        }

        return piece;
    }

    // TODO: add a way to print the board flipped on y axis
    // switch to a cached board functionality later
    void printBoard(int selected[2][2], Player *pPlayer) {
        clearConsole();

        HANDLE hConsole; // for changing output color
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // change to macros
        int whiteColor = 15;
        int greenColor = 10;
        int redColor = 12;
        int yellowColor = 14;

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

            board[7 - y][x][0] = piece->getColor() == WHITE ? 'w' : 'b';
            board[7 - y][x][1] = piece->getName();
        }

        enum Color turn = pPlayer->getColor();
        // print each board slot
        for (int row = turn == WHITE ? 0 : 7; turn == WHITE ? row < 8 : row >= 0; row += turn == WHITE ? 1 : -1) {
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

                if (selected && 
                    ((row == selected[0][0] && col == selected[0][1]) || (row == selected[1][0] && col == selected[1][1]))) {
                    textColor += 8 * 16;
                }
                SetConsoleTextAttribute(hConsole, textColor);
                std::cout << board[row][col][1];
                SetConsoleTextAttribute(hConsole, whiteColor);
                std::cout << ' ';
            }
            std::cout << std::endl;
        }

        SetConsoleTextAttribute(hConsole, yellowColor);
        char letters[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
        std::cout << "  ";
        for (int i = 0; i < 8; i++) {
            std::cout << letters[i] << ' ';
        }
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, whiteColor);
    }

    // A mess of input checks to make sure you're doing the right thing (it sucks). 
    // TODO: encapsulate everything into another loop to prevent possible stack overflow
    void inputLoop(Player *pPlayer) {
        while (true) {
            enum Color turn = pPlayer->getColor();
            std::cout << "Player " << pPlayer->getName() << "'s turn..." << std::endl;
            std::string in = getInput(std::string("Input coordinates of piece (ex: d3) - "));

            char char1 = in[0];
            char char2 = in[1];

            // map input to correct coordinates
            int row = (char2 - '0') - 1;

            char columnChars[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
            int col = -1;

            // converts alpha char to index
            for (int i = 0; i < 8; i++) {
                if (char1 == columnChars[i])
                    col = i;
            }

            Piece* piece = getPieceAt(col, row);

            bool valid = true;

            if (!piece) {
                std::cout << "There is no piece at " << char1 << char2 << std::endl;
                valid = false;
            } if (piece && piece->getColor() != turn) {
                std::cout << "The piece at " << char1 << char2 << " is not your piece." << std::endl;
                valid = false;
            }

            if (!valid) {
                inputLoop(pPlayer);
                return;
            }

            bool destinationValid = false;

            int selected[2][2] = { {7 - piece->getY(), piece->getX()}, {7 - piece->getY(), piece->getX()} };

            bool firstIter = true;
            while (!destinationValid) {
                //printBoard(selected, pPlayer);

                if (!firstIter)
                    std::cout << "Move invalid for piece " << convertCharToLongName(piece->getName()) << "." << std::endl;

                firstIter = false;

                in = getInput(std::string("Input coordinates of destination (ex: d5), or 'q' to cancel - "));
                if (std::tolower(in[0]) == 'q') {
                    printBoard({}, pPlayer);
                    inputLoop(pPlayer);
                    return;
                }

                char1 = in[0];
                char2 = in[1];

                row = (char2 - '0') - 1;
                col = -1;

                for (int i = 0; i < 8; i++) {
                    if (char1 == columnChars[i])
                        col = i;
                }

                bool taking = false;

                // check if there is a piece at the destination and if it is the user's piece or not
                Piece* destPiece = getPieceAt(col, row);
                if (destPiece && destPiece->getColor() != turn)
                    taking = true;
                else if (destPiece) {
                    continue;
                }

                // repeat destination loop if the move is invalid
                if (piece->move(col, row, taking) != 0)
                    continue;

                enum Type pieceType = static_cast<Type>(piece->getType());

                int x = piece->getX(), y = piece->getY();
                int dirX = col - x, dirY = row - y;

                // repeat destination loop if move is intercepted by another piece
                if (checkMoveIntercepted(piece, col, row))
                    continue;

                destinationValid = true;

                // check if move leaves king exposed to attack (HOW TF DO I DO THIS AAAAAAAAAA)
                // I guess calculate all moves for other side? If any of the moves can hit the king, not legal move?
                // Sounds super performant :)

                // select the destination spot (flipped on y)
                selected[1][0] = 7 - row;
                selected[1][1] = col;

                printBoard(selected, pPlayer);

                bool moveConfirmed = getYesNo("Confirm move? (y/n) - ");

                if (moveConfirmed) {
                    piece->setPosition(col, row);
                    if (destPiece)
                        takePiece(destPiece, pPlayer);
                }
                else {
                    // try again cause they couldn't make up their mind
                    printBoard({}, pPlayer);
                    inputLoop(pPlayer);
                }
            }
        }
    }
};

static void printColorsAndStuff() {
    HANDLE hConsole; // for changing output color
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 0; i < 255; i++) {
        SetConsoleTextAttribute(hConsole, i);
        std::cout << i << std::endl;
    }
}

int main()
{
    std::cout << "Chess (" << VERSION << "), by Iain Broomell." << std::endl;
    std::cout << "btw... green goes first." << std::endl;

    Board board = Board();
    
    enum Color turn = BLACK;
    bool gameContinue = true;

    Player whitePlayer = Player("White", WHITE);
    Player blackPlayer = Player("Black", BLACK);

    Player *players[2] = { &whitePlayer, &blackPlayer };

    while (gameContinue) {
        Player* pPlayer = players[turn];
        board.printBoard({}, pPlayer);
        board.inputLoop(pPlayer);
        turn = turn == WHITE ? BLACK : WHITE;
    }
    return 0;
}