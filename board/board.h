#pragma once
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

    const int player_1 = 1;
    const int player_2 = 2;
    int turn = 1;

    raylib::Texture2D texture;
    Piece* pieces[8][8];

    Piece* whiteKing;
    Piece* blackKing;

    Piece* lastState[2] = {nullptr, nullptr};
    Piece* currentState[2] = {nullptr, nullptr};
    bool captured = false;

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

    void draw(int x, int y);
    void undo();
    
    bool underCheck(int player);
    bool isStalemate(int player);
    bool isCheckmate(int player);

    bool move(Piece* c, Piece* n, bool check = false);
    bool movePawn(Piece* c, Piece* n, bool check);
    bool moveRook(Piece* c, Piece* n, bool check);
    bool moveBishop(Piece* c, Piece* n, bool check);
    bool moveKnight(Piece* c, Piece* n, bool check);
    bool moveQueen(Piece* c, Piece* n, bool check);
    bool moveKing(Piece* c, Piece* n, bool check);

    int getTurn();
};