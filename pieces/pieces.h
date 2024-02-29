#pragma once
#include "piece.h"

class Pawn : public Piece {
public:
    Pawn(const char* path, int r, int c) : Piece(path, r, c) {};
};

class Knight : public Piece {
    const int width = 70;
    const int height = 70;

public:
    Knight(const char* path, int r, int c) : Piece(path, r, c) {};
};

class King : public Piece {
public:
    King(const char* path, int r, int c) : Piece(path, r, c) {};
};

class Queen : public Piece {
public:
    Queen(const char* path, int r, int c) : Piece(path, r, c) {};
};

class Rook : public Piece {
public:
    Rook(const char* path, int r, int c) : Piece(path, r, c) {};
};

class Bishop : public Piece {
public:
    Bishop(const char* path, int r, int c) : Piece(path, r, c) {};
};