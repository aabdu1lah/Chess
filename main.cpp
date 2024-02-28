#include "raylib-cpp-5.0.1\include\raylib-cpp.hpp"

enum PIECES {
    KING,
    QUEEN,
    KNIGHT,
    BISHOP,
    ROOK,
    PAWN,
};

int main() {
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "Chess - A Project");

    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        BeginDrawing();
            window.ClearBackground();

        EndDrawing();
    }

    return 0;
}



