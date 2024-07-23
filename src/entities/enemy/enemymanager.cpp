#include "enemymanager.h"

namespace EnemySystem {


    
    // Constructor doesn't need to initialize enemies anymore
    EnemyManager::EnemyManager() {
        // No initialization needed here, enemies will be dynamically added
    }
    
    
    void EnemyManager::SetRandomMaxEnemies(int currentLevel) {
        if(currentLevel == 50){
            MAX_ENEMIES = 1;
            std::cout << "New MAX_ENEMIES set to: " << MAX_ENEMIES << std::endl;
            return;
            
        }
        std::random_device rd;  // Seed with a real random value, if available
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(1, 5);  // Random number between 1 and 5
        MAX_ENEMIES = dis(gen);

        std::cout << "New MAX_ENEMIES set to: " << MAX_ENEMIES << std::endl;
    }
    
    bool EnemyManager::CheckPlayerSingleEnemyCollision(const BoundingBox &playerBody, const BoundingBox &enemyBody) {
            if (CheckCollisionBoxes(playerBody, enemyBody)){
                // Collision logic
                return true;   
            }
        return false; // No collision detected
    }
    
    
    
    void EnemyManager::InitializeEnemies(const std::vector<Vector3>& openPositions, int currentLevel) {
        if (currentLevel == 50) {
            // For level 50, spawn a single Cyberdemon
            if (!openPositions.empty()) {
                auto cyberdemonPosition = openPositions.front(); // Use the first open position for simplicity
                auto cyberdemon = std::make_unique<Cyberdemon>();
                cyberdemon->position = cyberdemonPosition;
                enemies.push_back(std::move(cyberdemon));
            }
            return;
        }
        
        std::cout << "Initializing enemies. Current level: " << currentLevel << ", MAX_ENEMIES: " << MAX_ENEMIES << std::endl;


        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);
        std::uniform_int_distribution<int> enemyTypeDist(0, 2); // For 3 types of enemies

        const float enemyPlacementProbability = 1.0f; // Probability to place an enemy at each position

        std::vector<Vector3> availablePositions = openPositions;

        for (int i = 0; i < MAX_ENEMIES && !availablePositions.empty(); ++i) {
            std::cout << "Creating enemy " << i << " of " << MAX_ENEMIES << std::endl;

            std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
            int randomIndex = positionDist(gen);

            if (probabilityDist(gen) <= enemyPlacementProbability) {
                Vector3 chosenPosition = availablePositions[randomIndex];
                availablePositions.erase(availablePositions.begin() + randomIndex);

                // Randomly choose an enemy type
                int enemyType = enemyTypeDist(gen);
                std::unique_ptr<Enemy> newEnemy;

                switch (enemyType) {
                    case 0:
                        newEnemy = std::make_unique<Imp>();
                        break;
                    case 1:
                        newEnemy = std::make_unique<Demon>();
                        break;
                    case 2:
                        newEnemy = std::make_unique<BaronOfHell>();
                        break;
                }
                std::cout << "Enemy created at position: " << chosenPosition.x << ", " << chosenPosition.y << ", " << chosenPosition.z << std::endl;


                newEnemy->position = chosenPosition;
                enemies.push_back(std::move(newEnemy));
            }
        }
    }

    
    void EnemyManager::UpdateEnemies(const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize, const std::vector<Vector3>& openPositions, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set, const BoundingBox &playerBody) {
        for (auto& enemy : enemies) {
            if (!enemy->active) continue;
            
            // Update the enemy ray's origin and direction
            enemy->ray.position = enemy->position;
            enemy->rayPlayerPosition = playerPosition;
            enemy->ray.direction = Vector3Normalize(Vector3Subtract(playerPosition, enemy->position));

            bool rayHitsPlayer = false;
            bool isObstructed = false;
            
            for (const auto& box : wallBoundingBoxes) {
                RayCollision collision = GetRayCollisionBox(enemy->ray, box);
                if (collision.hit) {
                    if (collision.distance < Vector3Distance(enemy->ray.position, playerPosition)) {
                        isObstructed = true;
                        break;
                    }
                }
            }

            if (!isObstructed) {
                RayCollision playerCollision = GetRayCollisionBox(enemy->ray, playerBody);
                rayHitsPlayer = playerCollision.hit;
            }

            bool playerEnemyCollision = CheckPlayerSingleEnemyCollision(playerBody, enemy->body);
            
            
            // Call specific methods for each enemy type using dynamic_cast
            if (auto imp = dynamic_cast<Imp*>(enemy.get())) {
                 imp->UpdateEnemyState(*enemy, playerPosition, openPositions, maze, n, m, blockSize, rayHitsPlayer, bulletManager, set);
                 if (!playerEnemyCollision) {
                    imp->MoveEnemyAlongPath(*enemy);
                } else {
                    std::cout << "Colliding with player" << std::endl;
                }
            } else if (auto demon = dynamic_cast<Demon*>(enemy.get())){
                demon->UpdateEnemyState(*enemy,playerPosition,openPositions,maze,n,m,blockSize,rayHitsPlayer,playerEnemyCollision);
                if(!playerEnemyCollision){
                    demon->MoveEnemyAlongPath(*enemy);
                }else{
                    std::cout<<"Demon colliding with player" << std::endl;
                }
            }else if (auto baronofhell = dynamic_cast<BaronOfHell*>(enemy.get())){
                baronofhell->UpdateEnemyState(*enemy,playerPosition,openPositions,maze,n,m,blockSize,rayHitsPlayer,bulletManager, set);
                if (!playerEnemyCollision) {
                    baronofhell->MoveEnemyAlongPath(*enemy);
                } else {
                    std::cout << "Baron Colliding with player" << std::endl;
                }
                
            }else if (auto cyberdemon = dynamic_cast<Cyberdemon*>(enemy.get())){
                cyberdemon->UpdateEnemyState(*enemy,playerPosition,openPositions,maze,n,m,blockSize,rayHitsPlayer,bulletManager, set);
                if (!playerEnemyCollision) {
                    cyberdemon->MoveEnemyAlongPath(*enemy);
                } else {
                    std::cout << "Cyberdemon Colliding with player" << std::endl;
                }
        }
    }
    }
    
    void EnemyManager::DrawEnemies() {
        for (const auto& enemy : enemies) {
            if (enemy->active) {
                enemy->Draw();
            }
        }
    }
    
            
    void EnemyManager::PlaceEnemy(Vector3 position) {
        // Create a new enemy and set its properties
        auto newEnemy = std::make_unique<Enemy>();
        newEnemy->position = position;
        newEnemy->active = true;

        // Update the enemy's bounding box
        Vector3 halfSize = Vector3Scale((Vector3){1.0f, 2.5f, 1.0f}, 0.5f);
        newEnemy->body.min = Vector3Subtract(position, halfSize);
        newEnemy->body.max = Vector3Add(position, halfSize);

        // Add the new enemy to the vector of enemies
        enemies.push_back(std::move(newEnemy));
    }
    
    void EnemyManager::Reset() {
        enemies.clear();
    }
}
