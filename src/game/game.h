#ifndef GAME_H
#define GAME_H

#include <utility>
#include <vector>

#include "../common.h"


 

namespace GameScreen {
    

    
    
void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();

void updateMazeSize();

std::pair<int, int> InitializeMaze();




void InitializeCamera();

}

#endif // GAME_H
