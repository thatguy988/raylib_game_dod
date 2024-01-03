
#ifndef ENEMY_H
#define ENEMY_H


#include <utility>
#include <vector>
#include <queue>



#include "raylib.h"
#include "common.h"
#include "bullet.h"



namespace EnemySystem {
    
    
        
    const int MAX_ENEMIES = 5;  // Maximum number of enemies
    
    enum class EnemyState {
        IDLE,
        ATTACKING
    };

    struct Enemy {
        Vector3 position;
        Vector3 targetPosition; // New target position for movement
        float movementSpeed;    // Movement speed
        bool active;
        EnemyState state;
        std::vector<Vector3> path; // Path for the enemy to follow
        Vector3 lastKnownPlayerPos;
        
        float shootingTimer;
        float bulletSpeed;
        float shootingInterval;
        float shootingHeightOffset; // Height offset for shooting


        
        Enemy() : position({0, 0, 0}), targetPosition({0, 0, 0}), movementSpeed(0.1f), active(false), state(EnemyState::IDLE), path(), lastKnownPlayerPos({0,0,0}), shootingTimer(0.0f), bulletSpeed(0.1f), shootingInterval(1.0f), shootingHeightOffset(0.75f) {}

    };

    class EnemyManager {
    public:
        Enemy enemies[MAX_ENEMIES];

        EnemyManager();
        //void InitializeEnemies(const int maze[MAX][MAX], int n, int m, float blockSize);
        void InitializeEnemies(const std::vector<Vector3>& openPositions);

        void UpdateEnemies(const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize, const std::vector<Vector3>& openPositions, BulletSystem::BulletManager& bulletManager); // Implement enemy logic
        void DrawEnemies();
        
       

    private:
        void PlaceEnemy(int index, Vector3 position);

    };
}

#endif // ENEMY_H
