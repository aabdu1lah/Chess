#include "boardSquare.h"

BoardSquare::BoardSquare() {
    tile = nullptr;
    piece = nullptr;
};
BoardSquare::BoardSquare(Tile* t, Piece* p) {
    tile = t;
    piece = p;
}

void BoardSquare::setTile(Tile* t) {
    tile = t;
}

void BoardSquare::setPiece(Piece* p) {
    piece = p;
}

Tile* BoardSquare::getTile() {
    return tile;
}

Piece* BoardSquare::getPiece() {
    return piece;
}

void BoardSquare::draw(int x, int y) {
    if (x == -1) x = tile->getX();
    if (y == -1) y = tile->getY();
    piece->draw(x, y);
}