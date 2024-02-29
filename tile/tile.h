#pragma once
#include <raylib-cpp-5.0.1\include\raylib-cpp.hpp>
#include "..\enums.h"

class Tile {
    int row;
    int column;
    int x;
    int y;
    PieceType type;

public:
    raylib::Vector2 getVector();

    void setType(PieceType t);
    void setX(int x);
    void setY(int y);
    void setRow(int r);
    void setColumn(int c);

    PieceType getType();
    int getX();
    int getY();
    int getRow();
    int getColumn();
};