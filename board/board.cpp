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

            Tile* currentTile = new Tile();
            Piece* currentPiece = nullptr;

            currentTile->setRow(row);
            currentTile->setColumn(column);
            currentTile->setX(x);
            currentTile->setY(y);
            currentTile->setType(NONE);

            if (currentTile->getRow() == 1) {
                currentTile->setType(BPAWN);
                currentPiece = new Piece(blackPawnPath, row, column);
            } else if (currentTile->getRow() == 6) {
                currentTile->setType(WPAWN);
                currentPiece = new Piece(whitePawnPath, row, column);
            } else if (currentTile->getRow() == 7) {
                if (currentTile->getColumn() == 0 || currentTile->getColumn() == 7) {
                    currentTile->setType(WROOK);
                    currentPiece = new Piece(whiteRookPath, row, column);
                } else if (currentTile->getColumn() == 1 || currentTile->getColumn() == 6) {
                    currentTile->setType(WKNIGHT);
                    currentPiece = new Piece(whiteKnightPath, row, column);
                } else if (currentTile->getColumn() == 2 || currentTile->getColumn() == 5) {
                    currentTile->setType(WBISHOP);
                    currentPiece = new Piece(whiteBishopPath, row, column);
                } else if (currentTile->getColumn() == 3) {
                    currentTile->setType(WQUEEN);
                    currentPiece = new Piece(whiteQueenPath, row, column);
                } else if (currentTile->getColumn() == 4) {
                    currentTile->setType(WKING);
                    currentPiece = new Piece(whiteKingPath, row, column);
                }
            } else if (currentTile->getRow() == 0) {
                if (currentTile->getColumn() == 0 || currentTile->getColumn() == 7) {
                    currentTile->setType(BROOK);
                    currentPiece = new Piece(blackRookPath, row, column);
                } else if (currentTile->getColumn() == 1 || currentTile->getColumn() == 6) {
                    currentTile->setType(BKNIGHT);
                    currentPiece = new Piece(blackKnightPath, row, column);
                } else if (currentTile->getColumn() == 2 || currentTile->getColumn() == 5) {
                    currentTile->setType(BBISHOP);
                    currentPiece = new Piece(blackBishopPath, row, column);
                } else if (currentTile->getColumn() == 3) {
                    currentTile->setType(BQUEEN);
                    currentPiece = new Piece(blackQueenPath, row, column);
                } else if (currentTile->getColumn() == 4) {
                    currentTile->setType(BKING);
                    currentPiece = new Piece(blackKingPath, row, column);
                }
            }

            if (currentPiece == nullptr) currentPiece = new Piece();

            BoardSquare* square = new BoardSquare();
            currentPiece->setType(currentTile->getType());
            currentPiece->setColumn(currentTile->getColumn());
            currentPiece->setRow(currentTile->getRow());
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

BoardSquare* Board::getHoverSquare(raylib::Vector2 vector) {
    for (auto &row : squares) {
        for (auto &cell : row) {
            if (vector.x >= cell->getTile()->getX() && vector.x <= cell->getTile()->getX() + dx) {
                if (vector.y >= cell->getTile()->getY() && vector.y <= cell->getTile()->getY() + dy) {
                    return cell;
                }
            }
        }
    }

    return nullptr;
}

BoardSquare* Board::getSquare(int row, int column) {
    return squares[row][column];
}

void Board::draw(int x, int y) {
    mousePos = mouse.GetPosition();
    enpassant = NONE;

    texture.Draw(x, y);
    for (auto &row : squares) {
        for (auto &cell : row) {
           if (!cell->getPiece()->isSelected()) cell->draw(-1, -1);
        }
    }

    if (mouse.IsButtonPressed(MOUSE_LEFT_BUTTON) && !pieceSelected) {
        currentSquare = getHoverSquare(mousePos);
        if (currentSquare->getTile()->getType() != NONE) {
            currentSquare->getPiece()->setSelected(true);
            pieceSelected = true;
        }
    }

    if (pieceSelected) {
        currentSquare->draw(mousePos.GetX(), mousePos.GetY());
    }

    if (mouse.IsButtonReleased(MOUSE_BUTTON_LEFT) && pieceSelected) {
        BoardSquare* newSquare = getHoverSquare(mousePos);
        currentSquare->getPiece()->setSelected(false);

        if (currentSquare->getPiece()->getType() == WPAWN || currentSquare->getPiece()->getType() == BPAWN) {
            enpassant = getSquare(newSquare->getTile()->getRow()-1, newSquare->getTile()->getColumn())->getTile()->getType();
        }

        if (currentSquare->getPiece()->move(newSquare->getPiece(), enpassant)) {
            currentSquare->getPiece()->setColumn(newSquare->getPiece()->getColumn());
            currentSquare->getPiece()->setRow(newSquare->getPiece()->getRow());
            newSquare->setPiece(currentSquare->getPiece());

            newSquare->getTile()->setType(currentSquare->getTile()->getType());
            currentSquare->setPiece(new Piece());
            currentSquare->getPiece()->setColumn(currentSquare->getTile()->getColumn());
            currentSquare->getPiece()->setRow(currentSquare->getTile()->getRow());
            currentSquare->getTile()->setType(NONE);
        }
        
        pieceSelected = false;
        currentSquare = nullptr;
    }
}