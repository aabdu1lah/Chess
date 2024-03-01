#include "piece.h"

Piece::Piece() = default;
Piece::Piece(const char* path, int r, int c, int a, int b) {
    raylib::Image image(path);
    image.Resize(width, height);
    texture.Load(image);

    row = r;
    column = c;
    x = a;
    y = b;
}

Piece::Piece(raylib::Image &image, int r, int c, int a, int b) {
    texture.Load(image);

    row = r;
    column = c;
    x = a;
    y = b;
}

void Piece::draw(int a, int b) {
    if (a == -1) a = x;
    if (b == -1) b = y;
    texture.Draw(a, b);
}

void Piece::draw(raylib::Vector2 v) {
    texture.Draw(v.x, v.y);
}

void Piece::setRow(int r) {
    row = r;
}

void Piece::setColumn(int c) {
    column = c;
}

void Piece::setX(int a) {
    x = a;
}

void Piece::setY(int a) {
    y = a;
}

void Piece::setType(PieceType t) {
    type = t;
}

void Piece::setSelected(bool b) {
    selected = b;
}

void Piece::setMoved(bool m) {
    moved = m;
}


int Piece::getRow() {
    return row;
}

int Piece::getColumn() {
    return column;
}

int Piece::getX() {
    return x;
}

int Piece::getY() {
    return y;
}

PieceType Piece::getType() {
    return type;
}

bool Piece::isSelected() {
    return selected;
}

bool Piece::hasMoved() {
    return moved;
}

int Piece::getWidth() {
    return width;
}

int Piece::getHeight() {
    return height;
}

void Piece::setTexture(raylib::Texture2D &t) {
    texture = std::move(t);
}

raylib::Texture2D* Piece::getTexture() {
    return &texture;
}

void Piece::swap(Piece* p) {
    PieceType temptype = p->getType();
    p->setType(getType());
    setType(temptype);
    
    raylib::Image i;
    raylib::Image i2;

    i.Load(*p->getTexture());
    i2.Load(*getTexture());

    p->getTexture()->Unload();
    getTexture()->Unload();

    getTexture()->Load(i);
    p->getTexture()->Load(i2);
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