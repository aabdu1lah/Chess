#pragma once
// #include "..\boardSquare\boardSquare.h"
#include "..\pieces\piece.h"

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

    const char* transparentImagePath = "assets\\transparent.png";

    const int dx = 75;
    const int dy = 75;

    const int boardWidth = 600;
    const int boardHeight = 600;

    raylib::Texture2D texture;
    Piece* pieces[8][8];

    raylib::Mouse mouse = raylib::Mouse();
    raylib::Vector2 mousePos;

    PieceType currentType;
    Piece* currentPiece;
    bool pieceSelected = false;

public:
    Board(const char* path);
    ~Board();

    Piece* getPiece(raylib::Vector2 vector);
    Piece* getPiece(int row, int column);
    raylib::Texture2D* toTexture(const char* path, int width, int height);
    bool emptySpacesInBetween(PieceType t, Piece* c, Piece* n);

    void draw(int x, int y);
    void move(Piece* c, Piece* n);
    void movePawn(Piece* c, Piece* n);
};