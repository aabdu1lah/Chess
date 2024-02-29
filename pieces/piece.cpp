#include "piece.h"

Piece::Piece() = default;
Piece::Piece(const char* path, int r, int c) {
    raylib::Image image(path);
    image.Resize(width, height);
    texture.Load(image);

    row = r;
    column = c;
}

Piece::Piece(raylib::Image &image, int r, int c) {
    texture.Load(image);

    row = r;
    column = c;
}

void Piece::draw(int x, int y) {
    texture.Draw(x, y);
}

void Piece::setRow(int r) {
    row = r;
}

    void Piece::setColumn(int c) {
    column = c;
}

void Piece::setType(PieceType t) {
    type = t;
}

void Piece::setSelected(bool b) {
    selected = b;
}

int Piece::getRow() {
    return row;
}

int Piece::getColumn() {
    return column;
}

PieceType Piece::getType() {
    return type;
}

bool Piece::isSelected() {
    return selected;
}