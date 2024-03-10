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

Piece::Piece(int r, int c, int a, int b) {
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

    if (type >=2 && type <= 7) {
        setParent(W);
    } else if (type >= 8 && type <= 13) {
        setParent(B);
    }
}

void Piece::setSelected(bool b) {
    selected = b;
}

void Piece::setMoved(bool m) {
    moved = m;
}

void Piece::setParent(PieceType t) {
    parent = t;
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

PieceType Piece::getParent() {
    return parent;
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

raylib::Texture2D* Piece::getTexture() {
    return &texture;
}

void Piece::swap(Piece* p) {
    PieceType temptype = p->getType();
    p->setType(getType());
    setType(temptype);
    
    if (p->getTexture()->IsReady()) {
        raylib::Image i;
        raylib::Image i2;

        i.Load(*p->getTexture());
        i2.Load(*getTexture());

        p->getTexture()->Unload();
        getTexture()->Unload();

        getTexture()->Load(i);
        p->getTexture()->Load(i2);
    } else {
        raylib::Image i;
        i.Load(*getTexture());
        getTexture()->Unload();
        p->getTexture()->Load(i);
    }
    

    bool t = p->hasMoved();
    p->setMoved(hasMoved());
    setMoved(t);
}
