#pragma once
#include <raylib-cpp-5.0.1\include\raylib-cpp.hpp>
#include "..\enums.h"

class Piece {
    const int width = 75;
    const int height = 75;
    PieceType type = NONE;
    bool selected = false;

    int row;
    int column;

    raylib::Texture2D texture;

public:
    Piece();
    Piece(const char* path, int r, int c);
    Piece(raylib::Image &image, int r, int c);
    
    void draw(int x, int y);

    void setRow(int r);
    void setColumn(int c);
    void setType(PieceType t);
    void setSelected(bool b);

    int getRow();
    int getColumn();
    PieceType getType();
    bool isSelected();
};