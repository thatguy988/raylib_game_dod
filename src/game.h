#ifndef GAME_H
#define GAME_H

#include <utility>
#include "common.h"

 

namespace GameScreen {
    

    
    
void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();

void updateMazeSize();
  
// Method declarations
std::pair<int, int> InitializeMaze();
void InitializeCamera();

}

#endif // GAME_H
