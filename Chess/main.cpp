#include "piece.h"

const std::string VERSION = "v0.1";

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
    
    enum Color turn = WHITE;
    bool gameContinue = true;

    Player whitePlayer = Player("White", WHITE, false);
    Player blackPlayer = Player("Black", BLACK, true);

    Player *players[2] = { &whitePlayer, &blackPlayer };

    while (gameContinue) {
        Player* pPlayer = players[turn];

        board.printBoard({}, pPlayer, players);

        if (!pPlayer->isBot())
            board.inputLoop(pPlayer, players);
        else
            pPlayer->botMove(&board);

        turn = turn == WHITE ? BLACK : WHITE;
    }
    return 0;
}