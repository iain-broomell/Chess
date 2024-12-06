#include "board.h"

static void clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = { 0, 0 };
    DWORD dwCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, topLeft, &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, topLeft);
}

static std::string convertCharToLongName(char _name) {
    char charNames[6] = { 'p', 'r', 'k', 'b', 'q', 'K' };
    std::string longNames[6] = { "Pawn", "Rook", "Knight", "Bishop", "Queen", "King" };

    // if I organized them alphabetically I could do a binary search but I can't be bothered
    for (int i = 0; i < 6; i++) {
        if (_name == charNames[i])
            return longNames[i];
    }

    return "NOTFOUND";
}

Board::Board() {
    pieces.reserve(32);
    populatePieces(pieces);
}

void Board::populatePieces(std::vector<std::unique_ptr<Piece>>& pieces) {
    // assumes every count in this list is an even number
    uint16_t countOfType[6] = {
        16, // pawns
        4,  // rooks
        4,  // knights
        4,  // bishops
        2,  // queens
        2   // kings
    };

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
    // bah nesting :(
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
                color, static_cast<Type>(typeIndex), nameCharacters[typeIndex], startColumn[typeIndex] + columnOffset, int(row)));
        }
    }
}

bool Board::isInputValid(std::string in) {
    bool valid = true;

    if (in.length() > 0 && std::isalpha(in[0]) && std::tolower(in[0]) == 'q')
        return true;

    if (in.length() != 2) {
        std::cout << "Input not correct length (must be two characters)." << std::endl;
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

std::string Board::getInput(std::string message) {
    std::string in;

    bool inputValid = false;

    while (!inputValid) {
        std::cout << message;

        std::cin >> in;

        if (!isInputValid(in))
            continue;

        inputValid = true;
    }

    return in;
}

bool Board::getYesNo(std::string message) {
    bool inputValid = false;
    bool yes = false;

    while (!inputValid) {
        std::cout << message;

        std::string in;
        std::cin >> in;

        if (in.length() != 1) {
            std::cout << "Input must be single character y or n." << std::endl;
            continue;
        }

        char yesOrNo = std::tolower(in[0]);
        switch (yesOrNo) {
        case 'y':
            yes = true;
            inputValid = true;
        case 'n':
            yes = false;
            inputValid = true;
        default:
            std::cout << "Input must be either y or n." << std::endl;
            continue;
        }
    }

    return yes;
}

void Board::takePiece(Piece* pPiece, Player* pPlayer) {
    for (int index = 0; index < pieces.size(); index++) {
        if (pieces.at(index).get() == pPiece) {
            pieces.erase(pieces.begin() + index);
            pPlayer->capturePiece(pPiece);
        }
    }
}

// switch to a cached board functionality later
void Board::printBoard(int selected[2][2], Player* pPlayer, Player* pPlayers[2]) {
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

    pPlayers[turn == WHITE ? 1 : 0]->printCapturedPieces();
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
    pPlayer->printCapturedPieces();
}

// split this up into smaller parts later!
void Board::inputLoop(Player* pPlayer, Player* pPlayers[2]) {
    bool moveConfirmed = false;
    while (!moveConfirmed) {
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

        // I have to do inline construction or else I have to make it a static function
        Piece* piece = Piece::getPieceAt(col, row, &pieces);

        bool valid = true;

        if (!piece) {
            std::cout << "There is no piece at " << char1 << char2 << std::endl;
            valid = false;
        } if (piece && piece->getColor() != turn) {
            std::cout << "The piece at " << char1 << char2 << " is not your piece." << std::endl;
            valid = false;
        }

        if (!valid) {
            continue;
        }

        bool destinationValid = false;

        int selected[2][2] = { {7 - piece->getY(), piece->getX()}, {7 - piece->getY(), piece->getX()} };

        bool firstIter = true;
        while (!destinationValid) {
            printBoard(selected, pPlayer, pPlayers);

            if (!firstIter)
                std::cout << "Move invalid for piece " << convertCharToLongName(piece->getName()) << "." << std::endl;

            firstIter = false;

            in = getInput(std::string("Input coordinates of destination (ex: d5), or 'q' to cancel - "));
            if (std::tolower(in[0]) == 'q') {
                printBoard({}, pPlayer, pPlayers);
                break;
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
            Piece* destPiece = Piece::getPieceAt(col, row, &pieces);
            if (destPiece && destPiece->getColor() != turn)
                taking = true;
            else if (destPiece) {
                continue;
            }

            // repeat destination loop if the move is invalid
            if (piece->move(col, row, &pieces) != 0)
                continue;

            destinationValid = true;

            // select the destination spot (flipped on y)
            selected[1][0] = 7 - row;
            selected[1][1] = col;

            printBoard(selected, pPlayer, pPlayers);

            moveConfirmed = getYesNo("Confirm move? (y/n) - ");

            if (moveConfirmed) {
                piece->setPosition(col, row);
                if (destPiece)
                    takePiece(destPiece, pPlayer);
            }
            else {
                // try again cause they couldn't make up their mind
                printBoard({}, pPlayer, pPlayers);
                break;
            }
        }
    }
}

std::vector<std::unique_ptr<Piece>>* Board::getPieces() {
    return &pieces;
}