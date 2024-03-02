#include "board.h"
#include <cmath>

Board::Board(const char* path) {
    raylib::Image image(path);
    image.Resize(boardWidth, boardHeight);
    texture.Load(image);

    int row, column;

    for (int y=0; y<600; y+=75) {
        for (int x=0; x<600; x+=75) {
            row = y/dy;
            column = x/dx;

            currentPiece = nullptr;

            if (row == 1) {
                currentPiece = new Piece(blackPawnPath, row, column, x, y);
                currentPiece->setType(BPAWN);
            } else if (row == 6) {
                currentPiece = new Piece(whitePawnPath, row, column, x, y);
                currentPiece->setType(WPAWN);
            } else if (row == 7) {
                if (column == 0 || column == 7) {
                    currentPiece = new Piece(whiteRookPath, row, column, x, y);
                    currentPiece->setType(WROOK);
                } else if (column == 1 || column == 6) {
                    currentPiece = new Piece(whiteKnightPath, row, column, x, y);
                    currentPiece->setType(WKNIGHT);
                } else if (column == 2 || column == 5) {
                    currentPiece = new Piece(whiteBishopPath, row, column, x, y);
                    currentPiece->setType(WBISHOP);
                } else if (column == 3) {
                    currentPiece = new Piece(whiteQueenPath, row, column, x, y);
                    currentPiece->setType(WQUEEN);
                } else if (column == 4) {
                    currentPiece = new Piece(whiteKingPath, row, column, x, y);
                    currentPiece->setType(WKING);
                }
            } else if (row == 0) {
                if (column == 0 || column == 7) {
                    currentPiece = new Piece(blackRookPath, row, column, x, y);
                    currentPiece->setType(BROOK);
                } else if (column == 1 || column == 6) {
                    currentPiece = new Piece(blackKnightPath, row, column, x, y);
                    currentPiece->setType(BKNIGHT);
                } else if (column == 2 || column == 5) {
                    currentPiece = new Piece(blackBishopPath, row, column, x, y);
                    currentPiece->setType(BBISHOP);
                } else if (column == 3) {
                    currentPiece = new Piece(blackQueenPath, row, column, x, y);
                    currentPiece->setType(BQUEEN);
                } else if (column == 4) {
                    currentPiece = new Piece(blackKingPath, row, column, x, y);
                    currentPiece->setType(BKING);
                }
            }
            if (currentPiece == nullptr) currentPiece = new Piece(transparentImagePath, row, column, x, y);
            pieces[row][column] = currentPiece;
        }
    }
}

Board::~Board() {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            delete pieces[i][j];
        }
    }
}

Piece* Board::getPiece(raylib::Vector2 vector) {
    int row = vector.GetY() / dy;
    int col = vector.GetX() / dx;

    return pieces[row][col];
}

Piece* Board::getPiece(int row, int column) {
    return pieces[row][column];
}

void Board::draw(int x, int y) {
    mousePos = mouse.GetPosition();

    texture.Draw(x, y);
    for (auto &row : pieces) {
        for (auto &cell : row) {
           if (!cell->isSelected()) cell->draw(-1, -1);
        }
    }

    if (pieceSelected) currentPiece->draw(mousePos);

    if (mouse.IsButtonPressed(MOUSE_LEFT_BUTTON) && !pieceSelected) {
        currentPiece = getPiece(mousePos);
        if (currentPiece->getType() != NONE) {
            currentPiece->setSelected(true);
            pieceSelected = true;
        }
    }

    if (mouse.IsButtonReleased(MOUSE_BUTTON_LEFT) && pieceSelected) {
        Piece* newPiece = getPiece(mousePos);
        move(currentPiece, newPiece);

        pieceSelected = false;
        currentPiece = nullptr;
    }
}

void Board::move(Piece* c, Piece* n) {
    c->setSelected(false);
    n->setSelected(false);

    PieceType t = c->getType();

    if (t == WPAWN || t == BPAWN) {
        movePawn(c, n);
    } else if (t == WROOK || t == BROOK || t == WQUEEN || t == BQUEEN) {
        moveRook(c, n);
    }
}

bool Board::emptySpacesInBetween(Piece* c, Piece* n) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;

    PieceType t = c->getType();

    if (t == WPAWN) {
        cr = std::max(c->getRow(), n->getRow());
        nr = std::min(c->getRow(), n->getRow());

        for (int i=cr-1; i>=nr; i--) {
            if (pieces[i][cc]->getType() != NONE) {
                return false;
            }
        }
    }
    
    if (t == BPAWN) {
        nr = std::max(c->getRow(), n->getRow());
        cr = std::min(c->getRow(), n->getRow());

        for (int i=cr+1; i<=nr; i++) {
            if (pieces[i][cc]->getType() != NONE) {
                return false;
            }
        }
    }

    if (t == WROOK || t == BROOK || t == WQUEEN || t == BQUEEN) {
        if (dc == 0) {
            if (dr < 0) {
                for (int i=cr-1; i>=nr; i--) {
                    if (pieces[i][cc]->getType() != NONE) {
                        return false;
                    }
                }
            } else {
                for (int i=cr+1; i<=nr; i++) {
                    if (pieces[i][cc]->getType() != NONE) {
                        return false;
                    }
                }
            }
        } else {
            if (dc < 0) {
                for (int i=cc-1; i>=nc; i--) {
                    if (pieces[cr][i]->getType() != NONE) {
                        return false;
                    }
                }
            } else {
                for (int i=cc+1; i<=nc; i++) {
                    if (pieces[cr][i]->getType() != NONE) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Board::movePawn(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    if (!c->hasMoved()) {
        if (dr == -2 && c->getType() == WPAWN) {
            if (emptySpacesInBetween(c, n) && c->getColumn() == n->getColumn()) {
                c->swap(n);
                n->setMoved(true);
            }            
        } else if (dr == 2 && c->getType() == BPAWN) {
            if (emptySpacesInBetween(c, n) && c->getColumn() == n->getColumn()) {
                c->swap(n);
                n->setMoved(true);
            }
        }
    }

    if (emptySpacesInBetween(c, n) && c->getColumn() == n->getColumn()) {
        if ((dr == -1 && c->getType() == WPAWN) || (dr == 1 && c->getType() == BPAWN)) {
            c->swap(n);
            n->setMoved(true);
        }
    }
}

void Board::moveRook(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    if (dr != 0 && dc != 0) return;

    if (emptySpacesInBetween(c, n)) {
        c->swap(n);
    }
}