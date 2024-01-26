#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "enemy.h"
#include "imp.h"
#include "demon.h"
#include "baronofhell.h"
#include "cyberdemon.h"
#include "raylib.h"
#include "raymath.h"
#include "../../common.h"
#include "../bullet/bullet.h"


#include <random>
#include <iostream>
#include <utility>
#include <algorithm>  
#include <cmath>

#include <vector>
#include <memory>

namespace EnemySystem {

    class EnemyManager {
    public:
        
        void SetRandomMaxEnemies(int currentLevel);

        
        std::vector<std::unique_ptr<Enemy>> enemies;
        
        EnemyManager();
        void InitializeEnemies(const std::vector<Vector3>& openPositions, int currentLevel);
        
        void UpdateEnemies(const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize, const std::vector<Vector3>& openPositions, BulletSystem::BulletManager& bulletManager, const BoundingBox &playerBody);
        void DrawEnemies();
     
        bool CheckPlayerSingleEnemyCollision(const BoundingBox &playerBody, const BoundingBox &enemyBody); 
        void Reset();
        
        void SetWallBoundingBoxes(const std::vector<BoundingBox>& boxes) {
            wallBoundingBoxes = boxes;
        }
        

    private:
        int MAX_ENEMIES;
        std::vector<BoundingBox> wallBoundingBoxes;
        void PlaceEnemy(Vector3 position);
    };

}

#endif // ENEMYMANAGER_H
