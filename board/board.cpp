#include "board.h"

Board::Board(const char* path) {
    raylib::Image image(path);
    image.Resize(boardWidth, boardHeight);
    texture.Load(image);

    int row, column;

    for (int y=0; y<600; y+=75) {
        for (int x=0; x<600; x+=75) {
            row = y/dy;
            column = x/dx;

            currentTile = new Tile();
            currentPiece = nullptr;

            currentTile->setRow(row);
            currentTile->setColumn(column);
            currentTile->setX(x);
            currentTile->setY(y);
            currentTile->setType(NONE);

            if (currentTile->getRow() == 1) {
                currentTile->setType(BPAWN);
                currentPiece = new Pawn(blackPawnPath, row, column);
            } else if (currentTile->getRow() == 6) {
                currentTile->setType(WPAWN);
                currentPiece = new Pawn(whitePawnPath, row, column);
            } else if (currentTile->getRow() == 7) {
                if (currentTile->getColumn() == 0 || currentTile->getColumn() == 7) {
                    currentTile->setType(WROOK);
                    currentPiece = new Rook(whiteRookPath, row, column);
                } else if (currentTile->getColumn() == 1 || currentTile->getColumn() == 6) {
                    currentTile->setType(WKNIGHT);
                    currentPiece = new Knight(whiteKnightPath, row, column);
                } else if (currentTile->getColumn() == 2 || currentTile->getColumn() == 5) {
                    currentTile->setType(WBISHOP);
                    currentPiece = new Bishop(whiteBishopPath, row, column);
                } else if (currentTile->getColumn() == 3) {
                    currentTile->setType(WQUEEN);
                    currentPiece = new Queen(whiteQueenPath, row, column);
                } else if (currentTile->getColumn() == 4) {
                    currentTile->setType(WKING);
                    currentPiece = new King(whiteKingPath, row, column);
                }
            } else if (currentTile->getRow() == 0) {
                if (currentTile->getColumn() == 0 || currentTile->getColumn() == 7) {
                    currentTile->setType(BROOK);
                    currentPiece = new Rook(blackRookPath, row, column);
                } else if (currentTile->getColumn() == 1 || currentTile->getColumn() == 6) {
                    currentTile->setType(BKNIGHT);
                    currentPiece = new Knight(blackKnightPath, row, column);
                } else if (currentTile->getColumn() == 2 || currentTile->getColumn() == 5) {
                    currentTile->setType(BBISHOP);
                    currentPiece = new Bishop(blackBishopPath, row, column);
                } else if (currentTile->getColumn() == 3) {
                    currentTile->setColumn(BQUEEN);
                    currentPiece = new Queen(blackQueenPath, row, column);
                } else if (currentTile->getColumn() == 4) {
                    currentTile->setType(BKING);
                    currentPiece = new King(blackKingPath, row, column);
                }
            }

            if (currentPiece == nullptr) currentPiece = new Piece();

            BoardSquare* square = new BoardSquare();
            currentPiece->setType(currentTile->getType());
            square->setPiece(currentPiece);
            square->setTile(currentTile);
            squares[row][column] = square;
        }
    }
}

Board::~Board() {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if (squares[i][j]->getPiece() != nullptr) {
                try {
                    delete squares[i][j]->getPiece();
                } catch (char*) {}
            }
            if (squares[i][j]->getTile() != nullptr) {
                try{
                    delete squares[i][j]->getTile();
                } catch (char*) {}
            }
        }
    }
}

// Tile* getHoverTile(raylib::Vector2 vector) {
//     for (auto &tilerow : tiles) {
//         for (auto& tilecol : tilerow) {
//             if (vector.x >= tilecol.getX() && vector.x <= tilecol.getX() + dx) {
//                 if (vector.y >= tilecol.getY() && vector.y <= tilecol.getY() + dy) {
//                     return &tilecol;
//                 }
//             }
//         }
//     }

//     return nullptr;
// }

void Board::draw(int x, int y) {
    mousePos = mouse.GetPosition();

    texture.Draw(x, y);
    for (auto &row : squares) {
        for (auto &cell : row) {
            cell->draw(-1, -1);
        }
    }

    // if (mouse.IsButtonPressed(MOUSE_LEFT_BUTTON) && !pieceSelected) {
    //     currentTile = getHoverTile(mousePos);
    //     for (int i=0; i<pieces.size(); i++) {
    //         if (currentTile->hasPiece(&pieces[i])) {
    //             pieces[i].setSelected(true);
    //             pieceSelected = true;
    //             currentPiece = &pieces[i];
    //         }
    //     }
    // }

    // if (pieceSelected) {
    //     currentPiece->draw(mousePos.GetX(), mousePos.GetY());
    // }

    // if (mouse.IsButtonReleased(MOUSE_BUTTON_LEFT) && pieceSelected) {
    //     currentTile = getHoverTile(mousePos);
    //     if (currentTile->getType() == NONE) {
    //         currentTile->setType(currentPiece->getType());
    //         currentTile->setPiece(currentPiece);
    //         currentPiece->getTile()->setType(NONE);
    //         currentPiece->getTile()->setPiece(nullptr);
    //         currentPiece->setTile(currentTile);
    //     }            

    //     pieceSelected = false;
    //     currentPiece->setSelected(false);
    //     currentPiece = nullptr;
    // }
}