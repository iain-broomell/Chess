#include "player.h"

Player::Player(const std::string& rName, enum Color iColor, bool isBot) {
    name = rName;
    color = iColor;
    capturedPieces.reserve(15);
    bot = isBot;
}

std::string Player::getName() const {
    return name;
}

enum Color Player::getColor() const{
    return color;
}

void Player::capturePiece(Piece* pPiece) {
    capturedPieces.push_back(std::make_unique<Piece>(*pPiece));
}

std::vector<std::unique_ptr<Piece>> Player::getCapturedPieces() {
    return capturedPieces;
}

void Player::printCapturedPieces() {
    for (int i = 0; i < capturedPieces.size(); i++) {
        std::cout << (capturedPieces.at(i)).get()->getName() << ' ';
    }
    std::cout << std::endl;
}

bool Player::isBot() const {
    return bot;
}

void Player::botMove(Board* pBoard) {
    std::vector<std::unique_ptr<Piece>> pieces;

    std::vector<std::unique_ptr<Piece>>* pBoardPieces = pBoard->getPieces();
    for (int i = 0; i < pBoardPieces->size(); i++) {
        if (pBoardPieces->at(i).get()->getColor() == color)
            pieces.push_back(pBoardPieces->at(i));
    }

    // TODO: pick random piece using random index

    std::unique_ptr<Piece> u_pPiece;

    std::vector<int[2]> moves = u_pPiece.get()->calculateMovesForPiece(pBoard);

    // TODO: pick a random move using random index

    int move[2] = {};
    
    if (u_pPiece.get()->move(move[0], move[1], pBoard))
        u_pPiece.get()->setPosition(move[0], move[1]);
}