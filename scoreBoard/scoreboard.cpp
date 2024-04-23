#include "scoreBoard.h"

void ScoreBoard::draw(int x, int y) {
    cyan.DrawRectangle(x, y, boardWidth, boardHeight);
    turnText.SetText(TextFormat("Current Player = %01i", turn));
    turnText.Draw(x + 290, y + 40);
}

void ScoreBoard::setTurn(int x) {
    turn = x;
}