#include "raylib-cpp-5.0.1\include\raylib-cpp.hpp"
#include "vector"
#include "iostream"

enum PIECES {
    WKING,
    WQUEEN,
    WKNIGHT,
    WBISHOP,
    WROOK,
    WPAWN,
    BKING,
    BQUEEN,
    BKNIGHT,
    BBISHOP,
    BROOK,
    BPAWN,
};

class Piece {
    raylib::Texture2D texture;

    const int width = 75;
    const int height = 75;
    int currentX;
    int currentY;

public:
    // Piece() = default;
    Piece(const char* path, int x, int y) {
        raylib::Image image(path);
        image.Resize(width, height);
        texture.Load(image);

        currentX = x;
        currentY = y;
    }

    void draw() {
        texture.Draw(currentX, currentY);
    }

    void setX(int x) {
        currentX = x;
    }

    void setY(int y) {
        currentY = y;
    }

    int getX(int x) {
        return currentX;
    }

    int getY(int y) {
        return currentY;
    }
};

class Pawn : public Piece {
public:
    Pawn(const char* path, int x, int y) : Piece(path, x, y) {};
};

class Knight : public Piece {
public:
    Knight(const char* path, int x, int y) : Piece(path, x, y) {};
};

class King : public Piece {
public:
    King(const char* path, int x, int y) : Piece(path, x, y) {};
};

class Queen : public Piece {
public:
    Queen(const char* path, int x, int y) : Piece(path, x, y) {};
};

class Rook : public Piece {
public:
    Rook(const char* path, int x, int y) : Piece(path, x, y) {};
};

class Bishop : public Piece {
public:
    Bishop(const char* path, int x, int y) : Piece(path, x, y) {};
};

class Tile {
public:
    int row;
    int column;
    int x;
    int y;
    PIECES currentPiece;
};

class Board {
    raylib::Texture2D texture;
    Tile tiles[8][8];
    std::vector<Piece> pieces;

    const int width = 600;
    const int height = 600;
    const int dx = 75;
    const int dy = 75;

    const char* whitePawnPath = "assets\\white_pawn.png";
    const char* whiteRookPath = "assets\\white_rook.png";
    const char* blackPawnPath = "assets\\black_pawn.png";
    const char* blackRookPath = "assets\\black_rook.png";

public:
    Board(const char* path) {
        raylib::Image image(path);
        image.Resize(width, height);
        texture.Load(image);

        for (int y=0; y<600; y+=dy) {
            for (int x=0; x<600; x+=dx) {
                Tile* currentTile = &tiles[y/dy][x/dx];

                currentTile->row = y/dy;
                currentTile->column = x/dx;
                currentTile->x = x;
                currentTile->y = y;

                if (currentTile->row == 6) {
                    currentTile->currentPiece == WPAWN;
                    pieces.push_back(Pawn(whitePawnPath, x, y));
                } else if (currentTile->row == 0 && (currentTile->column == 0 || currentTile->column == 7)) {
                    currentTile->currentPiece == WROOK;
                    pieces.push_back(Rook(whiteRookPath, x, y));
                } else if (currentTile->row == 1) {
                    currentTile->currentPiece = BPAWN;
                    pieces.push_back(Pawn(blackPawnPath, x, y));
                } else if (currentTile->row == 7 && (currentTile->column == 0 || currentTile->column == 7)) {
                    currentTile->currentPiece == BROOK;
                    pieces.push_back(Rook(blackRookPath, x, y));
                }
            }
        }
    }

    void draw(int x, int y) {
        texture.Draw(x, y);
        for (int i=0; i<pieces.size(); i++) {
            pieces[i].draw();
        }
    }
};

int main() {
    int screenWidth = 600;
    int screenHeight = 600;

    const char* boardPath = "assets\\chess.png";

    raylib::Window window(screenWidth, screenHeight, "Chess - A Project");
    Board board(boardPath);
    
    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        BeginDrawing();
            window.ClearBackground();
            board.draw(0, 0);

        EndDrawing();
    }

    return 0;
}



