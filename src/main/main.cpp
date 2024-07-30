#include "raylib.h"
#include "../game/game.h"
#include "../pause_menu/pause_menu.h"
#include "../main_menu/main_menu.h"
#include "../common.h"



int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "raylib - Game");
    SetTargetFPS(60);

    /*
    int menuOption = 0;
    bool keyReleased = true; // variable to track if key is released
    */
    GameState currentGameState = MAIN_MENU;

    GameScreen::GameData gameData;



    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        if (currentGameState == MAIN_MENU) {
            GameState newGameState = MainMenu::UpdateMainMenu();
            if (newGameState == GAME_SCREEN){
                GameScreen::InitGame(gameData);
                currentGameState = newGameState;
            }
        } else if (currentGameState == GAME_SCREEN) {
            GameScreen::UpdateGame(gameData);
            if (IsKeyPressed(KEY_BACKSPACE)) {
                currentGameState = PAUSE_MENU;
                PauseMenu::InitPauseMenu();
            }
        } else if (currentGameState == PAUSE_MENU){
            GameState newGameState = PauseMenu::UpdatePauseMenu();
            if (newGameState == MAIN_MENU){
                GameScreen::UnloadGame(gameData);
                currentGameState = newGameState;   
            } else if (newGameState == GAME_SCREEN){
                currentGameState = newGameState;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (currentGameState == MAIN_MENU) {
                MainMenu::DrawMainMenu();
            } else if (currentGameState == GAME_SCREEN) {
                GameScreen::DrawGame(gameData);
            } else if (currentGameState == PAUSE_MENU){
                PauseMenu::DrawPauseMenu();
            }
                
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
