#include "..\pieces\piece.h"
// #include "..\tile\tile.h"

class BoardSquare {
    Tile* tile;
    Piece* piece;

public:
    BoardSquare();
    BoardSquare(Tile* t, Piece* p);

    void draw(int x, int y);

    void setTile(Tile* t);
    void setPiece(Piece* p);
    Tile* getTile();
    Piece* getPiece();
};