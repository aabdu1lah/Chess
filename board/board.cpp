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
            if (currentPiece == nullptr) currentPiece = new Piece(row, column, x, y);
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
    } 
    if (t == WROOK || t == BROOK || t == WQUEEN || t == BQUEEN) {
        moveRook(c, n);
    } 
    if (t == WBISHOP || t == BBISHOP || t == WQUEEN || t == BQUEEN) {
        moveBishop(c, n);
    }
    if (t == WKNIGHT || t == BKNIGHT) {
        moveKnight(c, n);
    }
    if (t == WKING || t == BKING) {
        moveKing(c, n);
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

    if (t == WBISHOP || t == BBISHOP || t == WQUEEN || t == BQUEEN) {
        if (std::abs(dr) == std::abs(dc)) {
            if (dc > 0 && dr > 0) {
                for (int i=cr+1, j=cc+1; i<nr && j<nc; i++, j++) {
                    if (pieces[i][j]->getType() != NONE) {
                        return false;
                    }
                }
            } else if (dc < 0 && dr < 0) {
                for (int i=cr-1, j=cc-1; i>nr && j>nc; i--, j--) {
                    if (pieces[i][j]->getType() != NONE) {
                        return false;
                    }
                }
            } else if (dc > 0 && dr < 0) {
                for (int i=cr-1, j=cc+1; i>nr && j<nc; i--, j++) {
                    if (pieces[i][j]->getType() != NONE) {
                        return false;
                    }
                }
            } else if (dc < 0 && dr > 0) {
                for (int i=cr+1, j=cc-1; i<nr && j>nc; i++, j--) {
                    if (pieces[i][j]->getType() != NONE) {
                        return false;
                    }
                }
            }
        }
    }

    if (t == WROOK || t == BROOK || t == WQUEEN || t == BQUEEN) {
        if (dc == 0) {
            if (dr < 0) {
                for (int i=cr-1; i>nr; i--) {
                    if (pieces[i][cc]->getType() != NONE) {
                        return false;
                    }
                }
            } else {
                for (int i=cr+1; i<nr; i++) {
                    if (pieces[i][cc]->getType() != NONE) {
                        return false;
                    }
                }
            }
        } else if (dr == 0) {
            if (dc < 0) {
                for (int i=cc-1; i>nc; i--) {
                    if (pieces[cr][i]->getType() != NONE) {
                        return false;
                    }
                }
            } else {
                for (int i=cc+1; i<nc; i++) {
                    if (pieces[cr][i]->getType() != NONE) {
                        return false;
                    }
                }
            }
        }
    }

    if (t == WKNIGHT && n->getParent() == W) {
        return false;
    }
    if (t == BKNIGHT && n->getParent() == B) {
        return false;
    }

    return true;
}

void Board::movePawn(Piece* c, Piece* n) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;
    
    PieceType t = c->getType();
    bool empty = true;
    bool moved = c->hasMoved();

    if (t == WPAWN) {
        cr = std::max(c->getRow(), n->getRow());
        nr = std::min(c->getRow(), n->getRow());

        for (int i=cr-1; i>=nr; i--) {
            if (pieces[i][cc]->getType() != NONE) {
                empty = false;
            }
        }

        if (dr == -2 and empty) {
            if (std::abs(dc) == 1 && !moved) {
                Piece* enpassantPiece = getPiece(c->getRow() - 1, c->getColumn() + dc);
                if (enpassantPiece->getParent() == B) {
                    c->swap(n);
                    n->setMoved(true);

                    enpassantPiece->getTexture()->Unload();
                    enpassantPiece->setType(NONE);
                    enpassantPiece->setMoved(false);
                    enpassantPiece->setSelected(false);
                }
            } else if (dc == 0) {
                c->swap(n);
                n->setMoved(true);
            }
        } else if (dr == -1 && dc == 0 && empty) {
            c->swap(n);
            n->setMoved(true);
        }
    }
    
    if (t == BPAWN) {
        nr = std::max(c->getRow(), n->getRow());
        cr = std::min(c->getRow(), n->getRow());

        for (int i=cr+1; i<=nr; i++) {
            if (pieces[i][cc]->getType() != NONE) {
                empty = false;
            }
        }

        if (dr == 2 and empty) {
            if (std::abs(dc) == 1 && !moved) {
                Piece* enpassantPiece = getPiece(c->getRow() + 1, c->getColumn() + dc);
                if (enpassantPiece->getParent() == W) {
                    c->swap(n);
                    n->setMoved(true);

                    enpassantPiece->getTexture()->Unload();
                    enpassantPiece->setType(NONE);
                    enpassantPiece->setMoved(false);
                    enpassantPiece->setSelected(false);
                }
            } else if (dc == 0) {
                c->swap(n);
                n->setMoved(true);
            }
        } else if (dr == 1 && dc == 0 && empty) {
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

void Board::moveBishop(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    if (std::abs(dr) != std::abs(dc)) return;

    if (emptySpacesInBetween(c, n)) {
        c->swap(n);
    }
}

void Board::moveKnight(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    if (std::abs(dr) == 2 && std::abs(dc) == 1) {}
    else if (std::abs(dr) == 1 && std::abs(dc) == 2) {}
    else return;

    if (emptySpacesInBetween(c, n)) {
        c->swap(n);
    }
}

void Board::moveKing(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    if (std::abs(dr) > 1 || std::abs(dc) > 1) return;

    if (n->getType() == NONE) {
        c->swap(n);
    }    
}
