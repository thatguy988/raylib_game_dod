#include "raylib.h"
#include "game.h"

enum GameState {
    MAIN_MENU,
    GAME_SCREEN
};

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "raylib - Main Menu Example");
    SetTargetFPS(60);

    int menuOption = 0;
    GameState currentGameState = MAIN_MENU;
    bool keyReleased = true; // variable to track if key is released

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        if (currentGameState == MAIN_MENU) {
            if (IsKeyPressed(KEY_DOWN)) {
                menuOption++;
                if (menuOption > 2) menuOption = 0;
            }
            if (IsKeyPressed(KEY_UP)) {
                menuOption--;
                if (menuOption < 0) menuOption = 2;
            }

            if (keyReleased && IsKeyPressed(KEY_ENTER)) {
                if (menuOption == 0) {
                    currentGameState = GAME_SCREEN;
                    GameScreen::InitGame();
                    //Sleep(200);
                } else if (menuOption == 2) {
                    CloseWindow();
                    return 0;
                }
                keyReleased = false;
            }else if (!IsKeyDown(KEY_ENTER)){
                keyReleased = true;
            }
        } else if (currentGameState == GAME_SCREEN) {
            GameScreen::UpdateGame();
            if (IsKeyPressed(KEY_BACKSPACE)) {
                currentGameState = MAIN_MENU;
                //Sleep(200);
                // Optional: Add any code here to reset or unload the game screen
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (currentGameState == MAIN_MENU) {
                
                const char* menuOptions[] = {"Start", "Load", "Quit"};
                int numberOfOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);
                
                
                // Draw main menu
                DrawText("Main Menu", 350, 100, 20, BLACK);
                
                //Draw menu options 
                for(int i = 0; i < numberOfOptions; ++i){
                    Color optionColor = (menuOption == i) ? RED : BLACK;
                    DrawText(menuOptions[i], 350, 150 + 30 * i, 20, optionColor);
                }
                
            } else if (currentGameState == GAME_SCREEN) {
                // Draw game screen
                GameScreen::DrawGame();
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
