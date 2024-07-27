#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include <variant>
#include "raylib.h"
#include "raymath.h"
#include <random>
#include <iostream>
#include <utility>
#include <algorithm>  
#include <cmath>



#include "../../common.h"
#include "../bullet/bullet.h"
#include "../../maze/maze_generator.h"
#include "imp.h"
#include "Cyberdemon.h"

namespace EnemySystem {

    enum class EnemyType {
        IMP,
        CYBER_DEMON,
        NONE
    };
    
    enum class EnemyState {
        IDLE,
        ATTACKING,
        CHASING
    };


    struct EnemyData {
        std::vector<Vector3> positions;
        std::vector<BoundingBox> bodies;
        std::vector<EnemyType> types;
        std::vector<EnemyState> states;
        std::vector<std::vector<Vector3>> paths;
        std::vector<Vector3> lastKnownPlayerPos;
        std::vector<float> shootingTimers;
        std::vector<float> bulletSpeeds;
        std::vector<float> shootingIntervals;
        std::vector<float> shootingHeightOffsets;
        std::vector<float> movementSpeeds;
        std::vector<int> health;
        std::vector<bool> activeStates;

        int MAX_ENEMIES;

        // Raycasting and direction attributes
        std::vector<Ray> rays;
        std::vector<RayCollision> rayHitInfos;
        std::vector<Vector3> facingDirections;
        std::vector<Vector3> targetPositions;
        std::vector<Vector3> rayPlayerPositions;

        

        std::vector<std::function<void(size_t, const Vector3&, MazeGenerator::MazeData&, bool, bool, BulletSystem::BulletData&, BulletSystem::SparseSet&)>> updateFunctions;
        std::vector<std::function<void(size_t)>> drawFunctions;
        std::vector<std::function<void(size_t)>> moveFunctions;
    };

    

    int PosToIndex(int x, int y, int m);   
    std::vector<Vector3> BFS(const Vector3& start, const Vector3& end, MazeGenerator::MazeData& mazeData); 
    Vector3 GetRandomOpenPosition(const std::vector<Vector3>& openPositions);

    void InitializeEnemies(EnemyData& data, const std::vector<Vector3>& openPositions, int currentLevel);
    void InitializeEnemyData(EnemyData& data);
    void AddEnemy(EnemyData& data, const Vector3& position, EnemyType type);
    void UpdateEnemies(EnemyData& data, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set, const BoundingBox &playerBody);
    void DrawEnemies(const EnemyData& data);
    void ResetEnemies(EnemyData& data);

    bool CheckPlayerSingleEnemyCollision(const BoundingBox &playerBody, const BoundingBox &enemyBody);
    void SetRandomMaxEnemies(int currentLevel, EnemyData& data);

    void CleanUpInactiveEnemies(EnemyData& data);



    
    
} // namespace EnemySystem

#endif // ENEMY_H


