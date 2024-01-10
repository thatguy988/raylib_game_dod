#include "Enemy.h"
#include "raymath.h"
#include <vector>
#include <random>
#include <iostream>
#include <utility>
#include <algorithm>  
#include <cmath>

#include <queue>
#include <unordered_set>
#include <unordered_map>



namespace EnemySystem {
    
    
    EnemyManager::EnemyManager() {
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            enemies[i] = Enemy();
        }
    }
    
    
     
    bool EnemyManager::CheckPlayerSingleEnemyCollision(const Vector3& playerPosition, const Enemy& enemy, float collisionDistance) {
        if (enemy.active) {
            if (Vector3Distance(playerPosition, enemy.position) < collisionDistance) {
               // std::cout << "Player collided with enemy" << std::endl;
                return true; // Collision detected
            }
        }
        return false; // No collision detected
    }
    
    
    void EnemySystem::EnemyManager::InitializeEnemies(const std::vector<Vector3>& openPositions) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

        const float enemyPlacementProbability = 1.0f; // 20% chance to place an enemy at each position

        std::vector<Vector3> availablePositions = openPositions; // Copy open positions

        for (int i = 0; i < MAX_ENEMIES && !availablePositions.empty(); ++i) {
            std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
            int randomIndex = positionDist(gen);

            // Check if we should place an enemy at this position
            if (probabilityDist(gen) <= enemyPlacementProbability) {
                PlaceEnemy(i, availablePositions[randomIndex]);
                availablePositions.erase(availablePositions.begin() + randomIndex); // Remove the chosen position
            }
        }
    }
    
   


    
    
    bool LineOfSight(const Vector3& enemyPos, const Vector3& playerPos, const int maze[MAX][MAX], int n, int m, float blockSize) {
        //bresenham algorithm
        int x0 = static_cast<int>(enemyPos.x / blockSize);
        int z0 = static_cast<int>(enemyPos.z / blockSize);
        int x1 = static_cast<int>(playerPos.x / blockSize);
        int z1 = static_cast<int>(playerPos.z / blockSize);

        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dz = -std::abs(z1 - z0), sz = z0 < z1 ? 1 : -1;
        int err = dx + dz, e2; // error value e_xy

        while (true) {
            // Check if current position hits a wall
            if (x0 >= 0 && x0 < m && z0 >= 0 && z0 < n) {
            
                if (maze[z0][x0] == 1) {
                    return false; // Collision detected, no clear line of sight
                }
            } else {
                return false; // Out of bounds, no clear line of sight
            }

            if (x0 == x1 && z0 == z1) break; // Check if reached player position

            e2 = 2 * err;
            if (e2 >= dz) { err += dz; x0 += sx; } // Move along x-axis
            if (e2 <= dx) { err += dx; z0 += sz; } // Move along z-axis
        }

        return true; // Clear line of sight
    }



  

    
    
   

    
    
    /*
    //bool LineOfSight(const Vector3& enemyPos, const Vector3& playerPos, const Vector3& facingDirection, float fovAngle, const int maze[MAX][MAX], int n, int m, float blockSize) {
    bool LineOfSight(const Vector3& enemyPos, const Vector3& playerPos, const int maze[MAX][MAX], int n, int m, float blockSize) {
        // Calculate the direction from the enemy to the player
        Vector3 delta = Vector3Subtract(playerPos, enemyPos);
        // Normalize the direction vector
       // Vector3 direction = Vector3Normalize(delta);
        // Calculate the total distance to check
        float maxSightRange = 100.0f; // Max sight range
        float distance = Vector3Length(delta);
        distance = fmin(distance, maxSightRange); // Clamp distance to max sight range

        // Calculate a step vector for the ray traversal
        Vector3 step = Vector3Scale(direction, blockSize / 0.5f); // Smaller step for higher accuracy

        // Initialize current position to the enemy's position
        Vector3 currentPos = enemyPos;
        // Traverse the ray
        for (float coveredDistance = 0; coveredDistance < distance; coveredDistance += Vector3Length(step)) {
            currentPos = Vector3Add(currentPos, step);

            // Calculate the grid coordinates
            int gridX = static_cast<int>(currentPos.x / blockSize + roundingOffset);
            int gridY = static_cast<int>(currentPos.z / blockSize + roundingOffset);

            // Check if current position is outside the maze bounds
            if (gridX < 0 || gridX >= m || gridY < 0 || gridY >= n) {
                return false; // Out of bounds, no clear line of sight
            }

            // Check if the current position hits a wall
            if (maze[gridY][gridX] == 1) {
                return false; // Collision detected, no clear line of sight
            }
        }
        
        
      

        return true; // Clear line of sight
    }
    */
    
    
    
    
    
    
    
    // Helper function to convert 2D grid position to a linear index
    int PosToIndex(int x, int y, int m) {
        return y * m + x;
    }

    // BFS function to find the shortest path
    std::vector<Vector3> BFS(const Vector3& start, const Vector3& end, const int maze[MAX][MAX], int n, int m, float blockSize) {
        std::queue<Vector3> queue;
        std::unordered_map<int, Vector3> cameFrom; // Map of positions and their predecessors
        std::vector<Vector3> path;

        // Convert start and end positions to grid indices
        int startX = static_cast<int>(start.x / blockSize);
        int startY = static_cast<int>(start.z / blockSize);
        int endX = static_cast<int>(end.x / blockSize);
        int endY = static_cast<int>(end.z / blockSize);

        queue.push(Vector3{static_cast<float>(startX), 0, static_cast<float>(startY)});
        cameFrom[PosToIndex(startX, startY, m)] = Vector3{-1, -1, -1}; // Use -1 to indicate the start

        while (!queue.empty()) {
            Vector3 current = queue.front();
            queue.pop();

            int x = static_cast<int>(current.x);
            int y = static_cast<int>(current.z);

            if (x == endX && y == endY) { // Reached the end
                // Backtrack to get the path
                while (!(current.x == -1 && current.z == -1)) {
                    path.push_back(Vector3{current.x * blockSize, 0, current.z * blockSize});
                    current = cameFrom[PosToIndex(static_cast<int>(current.x), static_cast<int>(current.z), m)];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            // Check adjacent cells (up, down, left, right)
            std::vector<Vector3> neighbors = {
                {current.x, 0, current.z - 1}, // Up
                {current.x, 0, current.z + 1}, // Down
                {current.x - 1, 0, current.z}, // Left
                {current.x + 1, 0, current.z}  // Right
            };

            for (auto& neighbor : neighbors) {
                int nx = static_cast<int>(neighbor.x);
                int ny = static_cast<int>(neighbor.z);
                if (nx >= 0 && nx < m && ny >= 0 && ny < n && maze[ny][nx] == 0 && cameFrom.find(PosToIndex(nx, ny, m)) == cameFrom.end()) {
                    queue.push(neighbor);
                    cameFrom[PosToIndex(nx, ny, m)] = current;
                }
            }
        }

        return path; // If no path found, return empty vector
    }
    
    
    // Function to get a random open position from the provided vector of open positions
    Vector3 GetRandomOpenPosition(const std::vector<Vector3>& openPositions) {
        if (openPositions.empty()) {
            return Vector3Zero(); // Return a default position if there are no open positions
        }

        std::random_device rd;  // Initialize random number generator
        std::mt19937 gen(rd()); // Seed the generator

        // Randomly select an index from the vector
        std::uniform_int_distribution<> dis(0, openPositions.size() - 1);
        int randomIndex = dis(gen);

        // Return the randomly selected open position
        return openPositions[randomIndex];
    }
    
    //new methods to refactor update enemy
    void EnemyManager::CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize) {
        if (enemy.path.empty()) {
            Vector3 randomTarget = GetRandomOpenPosition(openPositions);
            enemy.path = BFS(enemy.position, randomTarget, maze, n, m, blockSize);
        }
    }
    
    
    
    void EnemyManager::CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize) {
    
            enemy.lastKnownPlayerPos = playerPosition;
            enemy.path = BFS(enemy.position, playerPosition, maze, n, m, blockSize);
            if (!enemy.path.empty()) {
                enemy.path.pop_back(); // Remove the last step (player's position) from the path
            }
            
        
    }
    
    void EnemyManager::MoveEnemyAlongPath(Enemy& enemy) {
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
        }
    }
    
    void EnemyManager::HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletManager& bulletManager) {
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
    
    
    
    void EnemyManager::UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize) {
        bool canSeePlayer = LineOfSight(enemy.position, playerPosition, maze, n, m, blockSize);

        switch (enemy.state) {
            case EnemyState::IDLE:
                CalculatePathToRandomTarget(enemy, openPositions, maze, n, m, blockSize);
                if (canSeePlayer) {
                    enemy.path.clear();
                    CalculatePathToPlayer(enemy, playerPosition, maze, n, m, blockSize);
                    enemy.state = EnemyState::ATTACKING;
                }
                break;

            case EnemyState::ATTACKING:
                if (!canSeePlayer) {
                    enemy.path.clear();
                    CalculatePathToPlayer(enemy, enemy.lastKnownPlayerPos, maze, n, m, blockSize);
                    enemy.state = EnemyState::CHASING; 
                } else if (enemy.path.empty()) {
                    CalculatePathToPlayer(enemy, playerPosition, maze, n, m, blockSize);
                }
                break;

            case EnemyState::CHASING:
                    
                    if (enemy.path.empty()) {
                        enemy.state = EnemyState::IDLE; // Transition back to IDLE state
                    }
                
                break;
        }
    }


    


    void EnemyManager::UpdateEnemies(const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize, const std::vector<Vector3>& openPositions, BulletSystem::BulletManager& bulletManager) {
        for (auto& enemy : enemies) {
            if (!enemy.active) continue;
            bool playerEnemyCollision = CheckPlayerSingleEnemyCollision(playerPosition, enemy, 2.0f);
            

            UpdateEnemyState(enemy, playerPosition, openPositions, maze, n, m, blockSize);
            
            switch (enemy.state) {
                case EnemyState::IDLE:
                   std::cout<<"IN idle"<<std::endl;
                    CalculatePathToRandomTarget(enemy, openPositions, maze, n, m, blockSize);
                    break;

                case EnemyState::ATTACKING:
                    std::cout<<"IN attack"<<std::endl;

                    HandleAttackState(enemy, playerPosition, bulletManager);
                    break;
                case EnemyState::CHASING:
                    std::cout<<"IN chasing" <<std::endl; 
                    break;
            }
            
            
            if(playerEnemyCollision == false){
                
                MoveEnemyAlongPath(enemy);
            }else{
                std::cout<< "colliding with player" << std::endl;
            }
        }
    }
      

    void EnemyManager::DrawEnemies() {
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (enemies[i].active) {
                DrawCube(enemies[i].position, 1.0f, 2.5f, 1.0f, BLUE); // Simple blue cube for enemy
                
                
                // Visualization of the path (with safety check)
                if (!enemies[i].path.empty()) {
                    for (size_t j = 0; j < enemies[i].path.size() - 1; ++j) {
                        // Draw a line from the current point to the next point in the path
                        DrawLine3D(enemies[i].path[j], enemies[i].path[j + 1], BLACK);
                    }
                }   
            }
        }
    }

    void EnemyManager::PlaceEnemy(int index, Vector3 position) {
        if (index >= 0 && index < MAX_ENEMIES) {
            enemies[index].position = position;
            enemies[index].active = true;
        }
    }
    
    void EnemyManager::Reset() {
            for (auto& enemy : enemies) {
                enemy.active = false;
                enemy.state = EnemyState::IDLE;
                enemy.path.clear();
                enemy.lastKnownPlayerPos = Vector3Zero();
                enemy.shootingTimer = 0.0f;
                
            }
        }
}
