#include "pause_menu.h"
#include "raylib.h"

namespace PauseMenu {

    static int pauseMenuOption = 0;
    static bool keyReleased = true; // Track if key is released

    void InitPauseMenu() {
        pauseMenuOption = 0;
        keyReleased = true;
    }

    GameState UpdatePauseMenu() {
        if (IsKeyPressed(KEY_DOWN)) {
            pauseMenuOption++;
            if (pauseMenuOption > 1) pauseMenuOption = 0;
        }
        if (IsKeyPressed(KEY_UP)) {
            pauseMenuOption--;
            if (pauseMenuOption < 0) pauseMenuOption = 1;
        }

        if (keyReleased && IsKeyPressed(KEY_ENTER)) {
            if (pauseMenuOption == 0) { // Resume
                return GAME_SCREEN;
            } else if (pauseMenuOption == 1) { // Quit
                return MAIN_MENU;
            }
            keyReleased = false;
        } else if (!IsKeyDown(KEY_ENTER)) {
            keyReleased = true;
        }
      return PAUSE_MENU;
    }

    void DrawPauseMenu() {
        const char* pauseOptions[] = {"Resume", "Quit"};
        int numberOfOptions = sizeof(pauseOptions) / sizeof(pauseOptions[0]);

        DrawText("Pause Menu", 350, 100, 20, BLACK);
        for (int i = 0; i < numberOfOptions; ++i) {
            Color optionColor = (pauseMenuOption == i) ? RED : BLACK;
            DrawText(pauseOptions[i], 350, 150 + 30 * i, 20, optionColor);
        }
    }

} // namespace PauseMenu
