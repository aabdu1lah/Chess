#include <raylib-cpp-5.0.1\include\raylib-cpp.hpp>
#include <vector>
#include <iostream>
#include "board\board.h"
#include "scoreBoard\scoreBoard.h"

int main() {
    int screenWidth = 600;
    int screenHeight = 600;

    const char* boardPath = "assets\\chess.png";

    raylib::Window window(screenWidth, screenHeight, "Chess - A Project");
    Board board(boardPath);
    // ScoreBoard scoreBoard;
    
    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        // scoreBoard.setTurn(board.getTurn());
        
        BeginDrawing();
            window.ClearBackground();
            board.draw(0, 0);
            // scoreBoard.draw(0, 600);

        EndDrawing();
    }

    return 0;
}