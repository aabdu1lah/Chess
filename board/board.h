#pragma once
#include "..\boardSquare\boardSquare.h"

class Board {
    const char* whitePawnPath = "assets\\white_pawn.png";
    const char* whiteRookPath = "assets\\white_rook.png";
    const char* whiteKnightPath = "assets\\white_knight.png";
    const char* whiteKingPath = "assets\\white_king.png";
    const char* whiteQueenPath = "assets\\white_queen.png";
    const char* whiteBishopPath = "assets\\white_bishop.png";

    const char* blackPawnPath = "assets\\black_pawn.png";
    const char* blackRookPath = "assets\\black_rook.png";
    const char* blackKnightPath = "assets\\black_knight.png";
    const char* blackKingPath = "assets\\black_king.png";
    const char* blackQueenPath = "assets\\black_queen.png";
    const char* blackBishopPath = "assets\\black_bishop.png";

    const int dx = 75;
    const int dy = 75;

    const int boardWidth = 600;
    const int boardHeight = 600;

    raylib::Texture2D texture;
    BoardSquare* squares[8][8];

    raylib::Mouse mouse = raylib::Mouse();
    raylib::Vector2 mousePos;

    PieceType currentType;
    PieceType enpassant = NONE;
    BoardSquare* currentSquare;
    bool pieceSelected = false;

public:
    Board(const char* path);
    ~Board();
    BoardSquare* getHoverSquare(raylib::Vector2 vector);
    BoardSquare* getSquare(int row, int column);
    void draw(int x, int y);
};