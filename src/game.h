#ifndef GAME_H
#define GAME_H

#include <utility>


 

namespace GameScreen {
    

    
    
void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();
  
// Method declarations
std::pair<int, int> InitializeMaze();
void InitializeCamera();

}

#endif // GAME_H
