// common.h
#pragma once

#include <string>
#include "raylib.h" 

struct Message {
    std::string text;
    float displayTimer;
    float duration;
    Color color;

    Message(const std::string& text = "", float duration = 3.0f, Color color = WHITE)
        : text(text), displayTimer(duration), duration(duration), color(color) {}
};


enum GameState {
    MAIN_MENU,
    GAME_SCREEN,
    PAUSE_MENU
};

extern GameState currentGameState; // Declaration only


const int MAX = 100;

const float roundingOffset = 0.5f;
