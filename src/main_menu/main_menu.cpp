#include "main_menu.h"
#include "raylib.h"


namespace MainMenu {

    int menuOption = 0;
    bool keyReleased = true;

    void InitMainMenu() {
        menuOption = 0;
        keyReleased = true;
    }

    GameState UpdateMainMenu() {
        if (IsKeyPressed(KEY_DOWN)) {
            menuOption++;
            if (menuOption > 1) menuOption = 0;
        }
        if (IsKeyPressed(KEY_UP)) {
            menuOption--;
            if (menuOption < 0) menuOption = 1;
        }

        if (keyReleased && IsKeyPressed(KEY_ENTER)) {
            if (menuOption == 0) {
                return GAME_SCREEN;
            } else if (menuOption == 1) {
                CloseWindow();
            }
            keyReleased = false;
        } else if (!IsKeyDown(KEY_ENTER)) {
            keyReleased = true;
        }
        return MAIN_MENU;
    }

    void DrawMainMenu() {
        const char* menuOptions[] = {"Start", "Quit"};
        int numberOfOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);

        // Draw main menu
        DrawText("Main Menu", 350, 100, 20, BLACK);

        // Draw menu options 
        for(int i = 0; i < numberOfOptions; ++i){
            Color optionColor = (menuOption == i) ? RED : BLACK;
            DrawText(menuOptions[i], 350, 150 + 30 * i, 20, optionColor);
        }
    }
}
