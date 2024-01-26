#include "baronofhell.h"

namespace EnemySystem {

    BaronOfHell::BaronOfHell() : Enemy() {
        // Initialize common Enemy properties
        position = {0, 0, 0}; // Default position
        targetPosition = {0, 0, 0}; // Default target position
        rayPlayerPosition = {0, 0, 0}; // Default raycast target position
        movementSpeed = 0.1f; // Movement speed specific to Imp
        active = true; // Active status
        state = EnemyState::IDLE; // Initial state
        path.clear(); // Clear any predefined path
        lastKnownPlayerPos = {0, 0, 0}; // Reset last known player position
        shootingTimer = 0.0f; // Reset shooting timer
        bulletSpeed = 0.1f; // Bullet speed for Imp
        shootingInterval = 0.5f; // Shooting interval specific to Baron
        shootingHeightOffset = 1.0f; // Height offset for shooting
        type = EnemyType::BARON_OF_HELL; // Enemy type
        enemyHealth = 200; // Health specific to Baron
        facingDirection = {0, 0, 1}; // Default facing direction

        // Initialize the bounding box
        Vector3 enemySize = {1.0f, 2.5f, 1.0f}; // Example size for Imp
        Vector3 halfSize = Vector3Scale(enemySize, 0.5f);
        body.min = Vector3Subtract(position, halfSize);
        body.max = Vector3Add(position, halfSize);

        // Initialize the ray and ray hit information
        ray.position = position; // Set ray origin to Imp's position
        ray.direction = Vector3Normalize(Vector3Subtract(rayPlayerPosition, position)); // Direction towards target position

        rayHitInfo.hit = false;
        rayHitInfo.distance = 0;
        rayHitInfo.point = {0, 0, 0};
        rayHitInfo.normal = {0, 0, 0};

    }

    void BaronOfHell::CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize) {
        if (enemy.path.empty()) {
            Vector3 randomTarget = Enemy::GetRandomOpenPosition(openPositions);
            enemy.path = Enemy::BFS(enemy.position, randomTarget, maze, n, m, blockSize);
        }
    }
    
    
    
    void BaronOfHell::CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize) {
            enemy.lastKnownPlayerPos = playerPosition;
            enemy.path = Enemy::BFS(enemy.position, playerPosition, maze, n, m, blockSize);
            if (!enemy.path.empty()) {
                enemy.path.pop_back(); // Remove the last step (player's position) from the path
            }
    }
    
    void BaronOfHell::MoveEnemyAlongPath(Enemy& enemy) {
        if (!enemy.path.empty()) {
            Vector3 nextStep = enemy.path.front();
            if (Vector3Distance(enemy.position, nextStep) <= enemy.movementSpeed) {
                enemy.position = nextStep;
                enemy.path.erase(enemy.path.begin()); // Remove the reached point
            } else {
                Vector3 direction = Vector3Normalize(Vector3Subtract(nextStep, enemy.position));
                enemy.position = Vector3Add(enemy.position, Vector3Scale(direction, enemy.movementSpeed));
                enemy.facingDirection = direction;
            }
            // Update the enemy's bounding box
            Vector3 halfSize = Vector3Scale((Vector3){1.0f, 2.5f, 1.0f}, 0.5f);  // Adjust size as needed
            enemy.body.min = Vector3Subtract(enemy.position, halfSize);
            enemy.body.max = Vector3Add(enemy.position, halfSize);
        }
    }
    
    void BaronOfHell::HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletManager& bulletManager) {
        if (enemy.shootingTimer >= enemy.shootingInterval) {
            Vector3 shootingPosition = enemy.position;
            shootingPosition.y += enemy.shootingHeightOffset; // Adjust bullet shooting height
            Vector3 shootingDirection = Vector3Normalize(Vector3Subtract(playerPosition, shootingPosition));
            bulletManager.EnemyShootBullet(shootingPosition, shootingDirection, enemy.bulletSpeed, enemy.type);
            enemy.shootingTimer = 0.0f; // Reset the timer
        } else {
            enemy.shootingTimer += GetFrameTime(); // Update the timer
        }
    }
    
    
    
    void BaronOfHell::UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, BulletSystem::BulletManager& bulletManager) {
        switch (enemy.state) {
            case EnemyState::IDLE:
                //std::cout<<"IN idle"<<std::endl;
                CalculatePathToRandomTarget(enemy, openPositions, maze, n, m, blockSize);
                if (rayHitsPlayer) {
                    enemy.path.clear();
                    CalculatePathToPlayer(enemy, playerPosition, maze, n, m, blockSize);
                    enemy.state = EnemyState::ATTACKING;
                }
                break;

            case EnemyState::ATTACKING:
                //std::cout<<"IN attack"<<std::endl;
                if (!rayHitsPlayer) {
                    enemy.path.clear();
                    CalculatePathToPlayer(enemy, enemy.lastKnownPlayerPos, maze, n, m, blockSize);
                    enemy.state = EnemyState::CHASING; 
                } else if (enemy.path.empty()) {
                    CalculatePathToPlayer(enemy, playerPosition, maze, n, m, blockSize);
                }
                HandleAttackState(enemy, playerPosition, bulletManager);

                break;

            case EnemyState::CHASING:
                    //std::cout<<"IN chasing" <<std::endl; 
                    if (enemy.path.empty()) {
                        enemy.state = EnemyState::IDLE; // Transition back to IDLE state
                    }
                break;
        }
        
    }
    
    
    void BaronOfHell::Draw() const {
        DrawCube(position, 1.0f, 2.5f, 1.0f, PURPLE);
    }

}
