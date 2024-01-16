// common.h
#pragma once



enum GameState {
    MAIN_MENU,
    GAME_SCREEN,
    PAUSE_MENU
};

extern GameState currentGameState; // Declaration only


const int MAX = 100;

const float roundingOffset = 0.5f;
