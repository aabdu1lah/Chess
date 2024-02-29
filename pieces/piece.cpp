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

bool Piece::move(Piece* piece, PieceType enpassant) {
    if (getType() == NONE) return false;
    if (getType() == WPAWN || getType() == BPAWN) return movePawn(piece, enpassant);

    return false;
}

bool Piece::movePawn(Piece* piece, PieceType enpassant = NONE) {
    int rows = piece->getRow() - getRow();
    int cols = piece->getColumn() - getColumn();

    if (!moved) {
        if (piece->getColumn() == getColumn()) {
            if ((rows == -2 && getType() == WPAWN) || (rows == 2 && getType() == BPAWN) && piece->getType() == NONE) {
                moved = true;
                return true;
            }
        } else if ((rows == -2 && (cols == -1 || cols == 1) && getType() == WPAWN) || (rows == 2 && (cols == -1 || cols == 1) && getType() == BPAWN)) {
            if ((enpassant == WPAWN && getType() == BPAWN) || (enpassant == BPAWN && getType() == WPAWN)) {
                moved = true;
                return true;
            }
        }
    }

    if (piece->getColumn() == getColumn()) {
        if ((rows == -1 && getType() == WPAWN) || (rows == 1 && getType() == BPAWN) && piece->getType() == NONE) {
            moved = true;
            return true;
        }
    }

    return false;
}