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
        if (mousePos.x <= 600 && mousePos.y <= 600) {
            currentPiece = getPiece(mousePos);
            if ((currentPiece->getParent() == W && turn == player_1) || (currentPiece->getParent() == B && turn == player_2)) {
                currentPiece->setSelected(true);
                pieceSelected = true;
            }
        }
    }

    if (mouse.IsButtonReleased(MOUSE_BUTTON_LEFT) && pieceSelected) {
        if (mousePos.x <= 600 && mousePos.y <= 600) {
            Piece* newPiece = getPiece(mousePos);
            if (currentPiece != newPiece) {
                move(currentPiece, newPiece);
                turn = turn == player_1 ? 2 : 1; 
            } else {
                currentPiece->setSelected(false);
            }
            pieceSelected = false;
            currentPiece = nullptr;
        }
    }
}

void Board::move(Piece* c, Piece* n) {
    c->setSelected(false);
    n->setSelected(false);

    PieceType t = c->getType();

    if (c == n) return;

    if (t == WPAWN || t == BPAWN) {
        movePawn(c, n);
    } 
    if (t == WROOK || t == BROOK) {
        moveRook(c, n);
    } 
    if (t == WBISHOP || t == BBISHOP) {
        moveBishop(c, n);
    }
    if (t == WKNIGHT || t == BKNIGHT) {
        moveKnight(c, n);
    }
    if (t == WKING || t == BKING) {
        moveKing(c, n);
    }
    if (t == WQUEEN || t == BQUEEN) {
        moveQueen(c, n);
    }
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
                    c->setMoved(true);
                    c->swap(n);

                    enpassantPiece->getTexture()->Unload();
                    enpassantPiece->setType(NONE);
                    enpassantPiece->setMoved(false);
                    enpassantPiece->setSelected(false);
                }
            } else if (dc == 0 and !moved) {
                c->setMoved(true);
                c->swap(n);
            }
        } else if (dr == -1 && dc == 0 && empty) {
            c->setMoved(true);
            c->swap(n);
        } else if (std::abs(dc) == 1 && dr == -1 && n->getParent() == B) {
            c->setMoved(true);
            c->capture(n);
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
                    c->setMoved(true);
                    c->swap(n);

                    enpassantPiece->getTexture()->Unload();
                    enpassantPiece->setType(NONE);
                    enpassantPiece->setMoved(false);
                    enpassantPiece->setSelected(false);
                }
            } else if (dc == 0 & !moved) {
                c->setMoved(true);
                c->swap(n);
            }
        } else if (dr == 1 && dc == 0 && empty) {
            c->setMoved(true);
            c->swap(n);
        } else if (std::abs(dc) == 1 && dr == 1 && n->getParent() == W) {
            c->setMoved(true);
            c->capture(n);
        }
    }
}

void Board::moveRook(Piece* c, Piece* n) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;
    
    PieceType t = c->getType();
    bool empty = true;

    if (dr != 0 && dc != 0) return;

    if (dc == 0) {
        if (dr < 0) {
            for (int i=cr-1; i>nr; i--) {
                if (pieces[i][cc]->getType() != NONE) {
                    empty = false;
                }
            }
        } else {
            for (int i=cr+1; i<nr; i++) {
                if (pieces[i][cc]->getType() != NONE) {
                    empty = false;
                }
            }
        }
    } else if (dr == 0) {
        if (dc < 0) {
            for (int i=cc-1; i>nc; i--) {
                if (pieces[cr][i]->getType() != NONE) {
                    empty = false;
                }
            }
        } else {
            for (int i=cc+1; i<nc; i++) {
                if (pieces[cr][i]->getType() != NONE) {
                    empty = false;
                }
            }
        }
    }

    if (empty) {
        if ((n->getParent() == W && t == BROOK) || n->getParent() == B && t == WROOK) {
            c->capture(n);
        } else if (n->getType() == NONE) {
            c->swap(n);
        }
    }
}

void Board::moveBishop(Piece* c, Piece* n) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;
    
    PieceType t = c->getType();
    bool empty = true;

    if (std::abs(dr) != std::abs(dc)) return;

    if (dc > 0 && dr > 0) {
        for (int i=cr+1, j=cc+1; i<nr && j<nc; i++, j++) {
            if (pieces[i][j]->getType() != NONE) {
                empty = false;
            }
        }
    } else if (dc < 0 && dr < 0) {
        for (int i=cr-1, j=cc-1; i>nr && j>nc; i--, j--) {
            if (pieces[i][j]->getType() != NONE) {
                empty =  false;
            }
        }
    } else if (dc > 0 && dr < 0) {
        for (int i=cr-1, j=cc+1; i>nr && j<nc; i--, j++) {
            if (pieces[i][j]->getType() != NONE) {
                empty =  false;
            }
        }
    } else if (dc < 0 && dr > 0) {
        for (int i=cr+1, j=cc-1; i<nr && j>nc; i++, j--) {
            if (pieces[i][j]->getType() != NONE) {
                empty =  false;
            }
        }
    }

    if (empty) {
        if ((n->getParent() == W && t == BBISHOP) || n->getParent() == B && t == WBISHOP) {
            c->capture(n);
        } else if (n->getType() == NONE) {
            c->swap(n);
        }
    }
}

void Board::moveKnight(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    PieceType t = c->getType();

    if (std::abs(dr) == 2 && std::abs(dc) == 1) {}
    else if (std::abs(dr) == 1 && std::abs(dc) == 2) {}
    else return;

    if ((t == WKNIGHT && n->getParent() == B) || (t == BKNIGHT && n->getParent() == W)) {
        c->capture(n);
    } else if (n->getType() == NONE) {
        c->swap(n);
    }
}

void Board::moveQueen(Piece* c, Piece* n) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;
    
    PieceType t = c->getType();
    bool empty = true;

    if (dc == 0) {
        if (dr < 0) {
            for (int i=cr-1; i>nr; i--) {
                if (pieces[i][cc]->getType() != NONE) {
                    empty = false;
                }
            }
        } else {
            for (int i=cr+1; i<nr; i++) {
                if (pieces[i][cc]->getType() != NONE) {
                    empty = false;
                }
            }
        }
    } else if (dr == 0) {
        if (dc < 0) {
            for (int i=cc-1; i>nc; i--) {
                if (pieces[cr][i]->getType() != NONE) {
                    empty = false;
                }
            }
        } else {
            for (int i=cc+1; i<nc; i++) {
                if (pieces[cr][i]->getType() != NONE) {
                    empty = false;
                }
            }
        }
    } else if (dc > 0 && dr > 0) {
        for (int i=cr+1, j=cc+1; i<nr && j<nc; i++, j++) {
            if (pieces[i][j]->getType() != NONE) {
                empty = false;
            }
        }
    } else if (dc < 0 && dr < 0) {
        for (int i=cr-1, j=cc-1; i>nr && j>nc; i--, j--) {
            if (pieces[i][j]->getType() != NONE) {
                empty = false;
            }
        }
    } else if (dc > 0 && dr < 0) {
        for (int i=cr-1, j=cc+1; i>nr && j<nc; i--, j++) {
            if (pieces[i][j]->getType() != NONE) {
                empty = false;
            }
        }
    } else if (dc < 0 && dr > 0) {
        for (int i=cr+1, j=cc-1; i<nr && j>nc; i++, j--) {
            if (pieces[i][j]->getType() != NONE) {
                empty = false;
            }
        }
    }

    if (empty) {
        if ((n->getParent() == W && t == BQUEEN) || n->getParent() == B && t == WQUEEN) {
            c->capture(n);
        } else if (n->getType() == NONE) {
            c->swap(n);
        }
    }
}

void Board::moveKing(Piece* c, Piece* n) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    PieceType t = c->getType();
    
    if (std::abs(dr) > 1 || std::abs(dc) > 1) return;
    
    if ((t == WKING && n->getParent() == B) || (t == BKING && n->getParent() == W)) {
        c->capture(n);
    } else if (n->getType() == NONE) {
        c->swap(n);
    }
}
