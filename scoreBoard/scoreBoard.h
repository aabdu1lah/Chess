#pragma once
#include <raylib-cpp-5.0.1\include\raylib-cpp.hpp>

class ScoreBoard {
    const int boardWidth = 600;
    const int boardHeight = 100;

    int turn = 1;

    raylib::Color cyan = raylib::Color(0, 255, 255);
    raylib::Text turnText = raylib::Text(TextFormat("Current Player = %01i", turn), 20, BLACK);

public:
    void draw(int x, int y);
    void setTurn(int x);
};