
#ifndef ENEMY_H
#define ENEMY_H


#include <utility>
#include <vector>
#include <queue>



#include "raylib.h"
#include "common.h"
#include "bullet.h"




namespace EnemySystem {
    
    
        
    const int MAX_ENEMIES = 1;  // Maximum number of enemies in one maze
    
    
    enum class EnemyType {
        IMP,
        FAST,
        STRONG,
        NONE
    
    };
    
    enum class EnemyState {
        IDLE,
        ATTACKING,
        CHASING
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
        
        EnemyType type;
        
        int enemyHealth;
        
        
        
        Vector3 facingDirection; // The direction in which the enemy is facing
        
        
        Enemy() : position({0, 0, 0}), targetPosition({0, 0, 0}), movementSpeed(0.1f), active(false), 
          state(EnemyState::IDLE), path(), lastKnownPlayerPos({0,0,0}), shootingTimer(0.0f), 
          bulletSpeed(0.1f), shootingInterval(1.0f), shootingHeightOffset(0.75f), 
          type(EnemyType::IMP), enemyHealth(InitializeHealth(EnemyType::IMP)), 
          facingDirection({0, 0, 1}) {}




        
        
        static int InitializeHealth(EnemyType type) {
            switch (type) {
                case EnemyType::IMP: 
                    return 50;  // Health for IMP type
                case EnemyType::FAST: 
                    return 30;  // Health for FAST type
                case EnemyType::STRONG: 
                    return 100; // Health for STRONG type
                default: 
                    return 0;
            }
        }

    };

    class EnemyManager {
    public:
        Enemy enemies[MAX_ENEMIES];

        EnemyManager();
        //void InitializeEnemies(const int maze[MAX][MAX], int n, int m, float blockSize);
        void InitializeEnemies(const std::vector<Vector3>& openPositions);

        void UpdateEnemies(const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize, const std::vector<Vector3>& openPositions, BulletSystem::BulletManager& bulletManager); // Implement enemy logic
        void DrawEnemies();
        bool CheckPlayerSingleEnemyCollision(const Vector3& playerPosition, const Enemy& enemy, float collisionDistance);
        void Reset();
       

        
       

    private:
        void PlaceEnemy(int index, Vector3 position);
        // New method declarations
        void CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
        void CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);
        void MoveEnemyAlongPath(Enemy& enemy);
        void HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletManager& bulletManager);
        void UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
         

    };
}

#endif // ENEMY_H
