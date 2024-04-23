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
                    whiteKing = currentPiece;
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
                    blackKing = currentPiece;
                }
            }
            if (currentPiece == nullptr) currentPiece = new Piece(row, column, x, y);
            pieces[row][column] = currentPiece;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete pieces[i][j]; // Delete each pointer
        }
    }

    delete lastState[0];
    delete lastState[1];
}

Piece* Board::getPiece(raylib::Vector2 vector) {
    int row = vector.GetY() / dy;
    int col = vector.GetX() / dx;

    return pieces[row][col];
}

Piece* Board::getPiece(int row, int column) {
    return pieces[row][column];
}

int Board::getTurn() {
    return turn;
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

    if (isStalemate(turn)) return;
    if (isCheckmate(turn)) return;

    if (mouse.IsButtonPressed(MOUSE_LEFT_BUTTON) && !pieceSelected) {
        if (mousePos.x <= boardWidth && mousePos.y <= boardHeight) {
            currentPiece = getPiece(mousePos);
            if ((currentPiece->getParent() == W && turn == player_1) || (currentPiece->getParent() == B && turn == player_2)) {
                currentPiece->setSelected(true);
                pieceSelected = true;
            }
        }
    }

    if (mouse.IsButtonReleased(MOUSE_BUTTON_LEFT) && pieceSelected) {
        if (mousePos.x <= boardWidth && mousePos.y <= boardHeight) {
            Piece* newPiece = getPiece(mousePos);
            if (!underCheck(turn)) {
                if (currentPiece != newPiece) {
                    bool moved = move(currentPiece, newPiece);
                    if (moved) turn = ((turn == player_1) ? player_2 : player_1); 
                }
            } else {
                if (currentPiece != newPiece) {
                    bool moved = move(currentPiece, newPiece);
                    if (moved && underCheck(turn)) {
                        std::cout << "under";
                        undo();
                    } else {
                        turn = ((turn == player_1) ? player_2 : player_1); 
                    }
                }
            }
            currentPiece->setSelected(false);
            pieceSelected = false;
            currentPiece = nullptr;
        }
    }
}

bool Board::underCheck(int player) {
    Piece* king = player == 1 ? whiteKing : blackKing;

    for (auto &row : pieces) {
        for (auto &piece : row) {
            if ((player == 1 && piece->getParent() == B) || (player == 2 && piece->getParent() == W)) {
                if (move(piece, king, true)) return true;
            }
        }
    }

    return false;
}

bool Board::isStalemate(int player) {
    if (underCheck(player)) return false;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* currentPiece = getPiece(row, col);
            if ((player == player_1 && currentPiece->getParent() == W) || (player == player_2 && currentPiece->getParent() == B)) {
                for (int targetRow = 0; targetRow < 8; targetRow++) {
                    for (int targetCol = 0; targetCol < 8; targetCol++) {
                        Piece* targetPiece = getPiece(targetRow, targetCol);
                        if (move(currentPiece, targetPiece, true)) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool Board::isCheckmate(int player) {
    if (!underCheck(player)) return false;

    Piece* king = player == player_1 ? whiteKing : blackKing;
    int kingRow = king->getRow();
    int kingCol = king->getColumn();

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* targetPiece = getPiece(row, col);
            bool moved = move(king, targetPiece);
            if (moved) {
                if (!underCheck(player)) {
                    undo();
                    return false;
                }
                undo();
            }
        }
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* blockingPiece = getPiece(row, col);
            if ((player == player_1 && blockingPiece->getParent() == W) || (player == player_2 && blockingPiece->getParent() == B)) {
                for (int targetRow = 0; targetRow < 8; targetRow++) {
                    for (int targetCol = 0; targetCol < 8; targetCol++) {
                        Piece* targetSquare = getPiece(targetRow, targetCol);
                        bool moved = move(blockingPiece, targetSquare);
                        if (moved) {
                            if (!underCheck(player)) {
                                undo();
                                return false;
                            }
                            undo();
                        }
                    }
                }
            }
        }
    }

    return true;
}

void Board::undo() {
    currentState[0]->getTexture()->Unload();
    currentState[1]->getTexture()->Unload();
    
    *currentState[0] = *lastState[0];
    *currentState[1] = *lastState[1];

    delete lastState[0];
    delete lastState[1];

    lastState[0] = nullptr;
    lastState[1] = nullptr;

    captured = false;
}

bool Board::move(Piece* c, Piece* n, bool check) {
    if (!check) {
        c->setSelected(false);
        n->setSelected(false);
    }
    PieceType t = c->getType();

    if (c == n) return false;

    if (t == WPAWN || t == BPAWN) {
        return movePawn(c, n, check);
    } 
    if (t == WROOK || t == BROOK) {
        return moveRook(c, n, check);
    } 
    if (t == WBISHOP || t == BBISHOP) {
        return moveBishop(c, n, check);
    }
    if (t == WKNIGHT || t == BKNIGHT) {
        return moveKnight(c, n, check);
    }
    if (t == WKING || t == BKING) {
        return moveKing(c, n, check);
    }
    if (t == WQUEEN || t == BQUEEN) {
        return moveQueen(c, n, check);
    }

    return false;
}

bool Board::movePawn(Piece* c, Piece* n, bool check) {
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
                    if (!check) {
                        c->setMoved(true);
                        c->swap(n);

                        enpassantPiece->getTexture()->Unload();
                        enpassantPiece->setType(NONE);
                        enpassantPiece->setMoved(false);
                        enpassantPiece->setSelected(false);
                    }
                    return true;
                }
            } else if (dc == 0 and !moved) {
                if (!check) {
                    c->setMoved(true);
                    c->swap(n);
                }
                return true;
            }
        } else if (dr == -1 && dc == 0 && empty) {
            if (!check) {
                c->setMoved(true);
                c->swap(n);
            }
            return true;
        } else if (std::abs(dc) == 1 && dr == -1 && n->getParent() == B) {
            if (!check) {
                c->setMoved(true);
                c->capture(n);
            }
            return true;
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
                    if (!check) {
                        lastState[0] = new Piece(*c);
                        lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                        captured = false;

                        c->setMoved(true);
                        c->swap(n);

                        enpassantPiece->getTexture()->Unload();
                        enpassantPiece->setType(NONE);
                        enpassantPiece->setMoved(false);
                        enpassantPiece->setSelected(false);
                    }
                    return true;
                }
            } else if (dc == 0 & !moved) {
                if (!check) {
                    lastState[0] = new Piece(*c);
                    lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                    captured = false;
                    
                    c->setMoved(true);
                    c->swap(n);
                }
                return true;
            }
        } else if (dr == 1 && dc == 0 && empty) {
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                captured = false;
                    
                c->setMoved(true);
                c->swap(n);
            }
            return true;
        } else if (std::abs(dc) == 1 && dr == 1 && n->getParent() == W) {
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                captured = true;
                    
                c->setMoved(true);
                c->capture(n);
            }
            return true;
        }
    }

    return false;
}

bool Board::moveRook(Piece* c, Piece* n, bool check) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;
    
    PieceType t = c->getType();
    bool empty = true;

    if (dr != 0 && dc != 0) return false;

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
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);
                currentState[0] = c;
                currentState[1] = n;
                captured = true;

                c->capture(n);
            }
            return true;
        } else if (n->getType() == NONE) {
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);
                currentState[0] = c;
                currentState[1] = n;
                captured = false;

                c->swap(n);
            }
            return true;
        }
    }

    return false;
}

bool Board::moveBishop(Piece* c, Piece* n, bool check) {
    int cr = c->getRow();
    int cc = c->getColumn();
    int nr = n->getRow();
    int nc = n->getColumn();

    int dr = nr - cr;
    int dc = nc - cc;
    
    PieceType t = c->getType();
    bool empty = true;

    if (std::abs(dr) != std::abs(dc)) return false;

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
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                captured = true;

                c->capture(n);
            }
            return true;
        } else if (n->getType() == NONE) {
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                captured = false;

                c->swap(n);
            }
            return true;
        }
    }
    
    return false;
}

bool Board::moveKnight(Piece* c, Piece* n, bool check) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    PieceType t = c->getType();

    if (!(std::abs(dr) == 2 && std::abs(dc) == 1) && !(std::abs(dr) == 1 && std::abs(dc) == 2)) return false;

    if ((t == WKNIGHT && n->getParent() == B) || (t == BKNIGHT && n->getParent() == W)) {
        if (!check) {
            lastState[0] = new Piece(*c);
            lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
            captured = true;

            c->capture(n);
        }
        return true;
    } else if (n->getType() == NONE) {
        if (!check) {
            lastState[0] = new Piece(*c);
            lastState[1] = new Piece(*n);
            currentState[0] = c;
            currentState[1] = n;
            captured = false;

            c->swap(n);
        }
        return true;
    }

    return false;
}

bool Board::moveQueen(Piece* c, Piece* n, bool check) {
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
    } else if (std::abs(dr) == std::abs(dc)) {
        if (dc > 0 && dr > 0) {
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
    } else {
        return false;
    }

    if (empty) {
        if ((n->getParent() == W && t == BQUEEN) || n->getParent() == B && t == WQUEEN) {
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                captured = true;

                c->capture(n);
            }
            return true;
        } else if (n->getType() == NONE) {
            if (!check) {
                lastState[0] = new Piece(*c);
                lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
                captured = false;

                c->swap(n);
            }
            return true;
        }
    }

    return false;
}

bool Board::moveKing(Piece* c, Piece* n, bool check) {
    int dr = n->getRow() - c->getRow();
    int dc = n->getColumn() - c->getColumn();

    PieceType t = c->getType();
    
    if (std::abs(dr) > 1 || std::abs(dc) > 1) return false;
    
    if ((t == WKING && n->getParent() == B) || (t == BKING && n->getParent() == W)) {
        if (!check) {
            lastState[0] = new Piece(*c);
            lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
            captured = true;

            c->capture(n);
        }
        return true;
    } else if (n->getType() == NONE) {
        if (!check) {
            lastState[0] = new Piece(*c);
            lastState[1] = new Piece(*n);currentState[0] = c;currentState[1] = n;
            captured = false;

            c->swap(n);
        }
        return true;
    }

    return false;
}
