#pragma once
#include <raylib-cpp-5.0.1\include\raylib-cpp.hpp>
#include "..\enums.h"

class Piece {
    const int width = 75;
    const int height = 75;

    raylib::Texture2D texture;
    PieceType type = NONE;
    PieceType parent = NONE;
    bool selected = false;
    bool moved = false;

    int row;
    int column;
    int x;
    int y;
    void setParent(PieceType t);

public:
    Piece();
    Piece(const char* path, int r, int c, int a, int b);
    Piece(int r, int c, int a, int b);
    Piece(Piece &p);

    void draw(int a, int b);
    void draw(raylib::Vector2 v);
    void swap(Piece* p);
    void capture(Piece* p);

    void setRow(int r);
    void setColumn(int c);
    void setType(PieceType t);
    void setSelected(bool b);
    void setMoved(bool m);
    void setX(int a);
    void setY(int a);

    PieceType getType();
    PieceType getParent();
    raylib::Texture2D* getTexture();
    bool isSelected();
    bool hasMoved();
    int getRow();
    int getColumn();
    int getX();
    int getY();
    int getWidth();
    int getHeight();

    Piece& operator=(const Piece& other);
};