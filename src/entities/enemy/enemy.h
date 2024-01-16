
#ifndef ENEMY_H
#define ENEMY_H


#include <utility>
#include <vector>
#include <queue>



#include "raylib.h"
#include "raymath.h"

#include "../../common.h"


#include "../bullet/bullet.h"




namespace EnemySystem {
    
    
        
    
    
    enum class EnemyType {
        IMP,
        DEMON,
        BARON_OF_HELL,
        NONE
    
    };
    
    enum class EnemyState {
        IDLE,
        ATTACKING,
        CHASING
    };

    struct Enemy {
        Vector3 position;
        BoundingBox body; // Bounding box representing the enemy's body

        Vector3 targetPosition; // New target position for movement
        
        Vector3 rayPlayerPosition;// target position for raycasting
        
        
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
        
        
        Ray ray;                // Ray for raycasting
        RayCollision rayHitInfo; // Ray hit information
        
        
        
        Vector3 facingDirection; // The direction in which the enemy is facing
        
        
        Enemy() : position({0, 0, 0}), targetPosition({0,0,0}), rayPlayerPosition({0,0,0}), movementSpeed(0.1f), active(false), 
          state(EnemyState::IDLE), path(), lastKnownPlayerPos({0,0,0}), shootingTimer(0.0f), 
          bulletSpeed(0.1f), shootingInterval(1.0f), shootingHeightOffset(0.75f), 
          type(EnemyType::IMP), enemyHealth(InitializeHealth(EnemyType::IMP)),
          facingDirection({0, 0, 1}) {
            // Initialize the bounding box
            Vector3 enemySize = {1.0f, 2.5f, 1.0f}; // Example size, adjust as needed
            Vector3 halfSize = Vector3Scale(enemySize, 0.5f);
            body.min = Vector3Subtract(position, halfSize);
            body.max = Vector3Add(position, halfSize);
            
            
            // Initialize the ray and ray hit information
            ray.position = position; // Set ray origin to enemy's position
            ray.direction = Vector3Normalize(Vector3Subtract(rayPlayerPosition, position)); // Direction towards target position

            rayHitInfo.hit = false;
            rayHitInfo.distance = 0;
            rayHitInfo.point = {0, 0, 0};
            rayHitInfo.normal = {0, 0, 0};

        }

       
        
        static int InitializeHealth(EnemyType type) {
            switch (type) {
                case EnemyType::IMP: 
                    return 50;  // Health for IMP type
                case EnemyType::DEMON: 
                    return 30;  // Health for FAST type
                case EnemyType::BARON_OF_HELL: 
                    return 100; // Health for STRONG type
                default: 
                    return 0;
            }
        }

    };

    class EnemyManager {
    public:
        static constexpr int MAX_ENEMIES = 1; // Maximum number of enemies in one maze


        Enemy enemies[MAX_ENEMIES];

        EnemyManager();
        void InitializeEnemies(const std::vector<Vector3>& openPositions);

        void UpdateEnemies(const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize, const std::vector<Vector3>& openPositions, BulletSystem::BulletManager& bulletManager, const BoundingBox &playerBody);
        void DrawEnemies();
     
        bool CheckPlayerSingleEnemyCollision(const BoundingBox &playerBody, const BoundingBox &enemyBody); 
        void Reset();
        
        void SetWallBoundingBoxes(const std::vector<BoundingBox>& boxes) {
            wallBoundingBoxes = boxes;
        }
       
       

    private:
    
    
        std::vector<BoundingBox> wallBoundingBoxes;

        void PlaceEnemy(int index, Vector3 position);
        
        void CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
        void CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);
        void MoveEnemyAlongPath(Enemy& enemy);
        void HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletManager& bulletManager);
        void UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, BulletSystem::BulletManager& bulletManager);
         

    };
}

#endif // ENEMY_H
