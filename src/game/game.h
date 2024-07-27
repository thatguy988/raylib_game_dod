#ifndef GAME_H
#define GAME_H

#include <memory>
#include <utility>
#include <vector>

#include <iostream>
#include <random>
#include "raylib.h"

#include "../common.h"


#include "../maze/predefinedlevels.h"
#include "../maze/maze_generator.h"
#include "../collision/collision.h"
#include "../camera/player_camera.h"
#include "../entities/bullet/bullet.h"
#include "../entities/enemy/enemy.h"
#include "../entities/ammobox/ammobox.h"
#include "../entities/healthbox/healthbox.h"


 

namespace GameScreen {
    
    
void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();


}

#endif // GAME_H
