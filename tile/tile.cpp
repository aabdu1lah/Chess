#include "tile.h"

raylib::Vector2 Tile::getVector() {
    raylib::Vector2 vector;
    vector.SetX(x);
    vector.SetY(y);
    return vector;
}

void Tile::setType(PieceType t) {
    type = t;
}

void Tile::setX(int a) {
    x = a;
}
 
void Tile::setY(int a) {
    y = a;
}

void Tile::setRow(int r) {
    row = r;
}

void Tile::setColumn(int c) {
    column = c;
}

PieceType Tile::getType() {
    return type;
}

int Tile::getX() {
    return x;
}

int Tile::getY() {
    return y;
}

int Tile::getRow() {
    return row;
}

int Tile::getColumn() {
    return column;
}
