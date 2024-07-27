#include "Enemy.h"
#include "raymath.h"
#include <vector>


#include <queue>
#include <unordered_set>
#include <unordered_map>



namespace EnemySystem {


    void SetRandomMaxEnemies(int currentLevel, EnemyData& data) {
        if(currentLevel == 10){
            data.MAX_ENEMIES = 1;
            std::cout << "New MAX_ENEMIES set to: " << data.MAX_ENEMIES << std::endl;
            return;
            
        }
        std::random_device rd;  // Seed with a real random value, if available
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(1, 5);  // Random number between 1 and 5
        data.MAX_ENEMIES = dis(gen);

        std::cout << "New MAX_ENEMIES set to: " << data.MAX_ENEMIES << std::endl;
    }
    
    // Helper function to convert 2D grid position to a linear index
    int PosToIndex(int x, int y, int m) {
        return y * m + x;
    }

    // BFS function to find the shortest path
    std::vector<Vector3> BFS(const Vector3& start, const Vector3& end, MazeGenerator::MazeData& mazeData) {
        std::queue<Vector3> queue;
        std::unordered_map<int, Vector3> cameFrom; // Map of positions and their predecessors
        std::vector<Vector3> path;

        // Convert start and end positions to grid indices
        int startX = static_cast<int>(start.x / mazeData.blockSize + roundingOffset);
        int startY = static_cast<int>(start.z / mazeData.blockSize + roundingOffset);
        int endX = static_cast<int>(end.x / mazeData.blockSize + roundingOffset);
        int endY = static_cast<int>(end.z / mazeData.blockSize + roundingOffset);

        queue.push(Vector3{static_cast<float>(startX), 0, static_cast<float>(startY)});
        cameFrom[PosToIndex(startX, startY, mazeData.m)] = Vector3{-1, -1, -1}; // Use -1 to indicate the start

        while (!queue.empty()) {
            Vector3 current = queue.front();
            queue.pop();

            int x = static_cast<int>(current.x);
            int y = static_cast<int>(current.z);

            if (x == endX && y == endY) { // Reached the end
                // Backtrack to get the path
                while (!(current.x == -1 && current.z == -1)) {
                    path.push_back(Vector3{current.x * mazeData.blockSize, 0, current.z * mazeData.blockSize});
                    current = cameFrom[PosToIndex(static_cast<int>(current.x), static_cast<int>(current.z), mazeData.m)];
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
                if (nx >= 0 && nx < mazeData.m && ny >= 0 && ny < mazeData.n && mazeData.maze[ny][nx] == 0 && cameFrom.find(PosToIndex(nx, ny, mazeData.m)) == cameFrom.end()) {
                    queue.push(neighbor);
                    cameFrom[PosToIndex(nx, ny, mazeData.m)] = current;
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


    void InitializeEnemies(EnemyData& data, const std::vector<Vector3>& openPositions, int currentLevel) {
        if (currentLevel == 10) {
            // For level 50, spawn a single Cyberdemon
            if (!openPositions.empty()) {
                auto cyberdemonPosition = openPositions.front(); // Use the first open position for simplicity
                AddEnemy(data, cyberdemonPosition, EnemyType::CYBER_DEMON);
            }
            return;
        }

        std::cout << "Initializing enemies. Current level: " << currentLevel << ", MAX_ENEMIES: " << data.MAX_ENEMIES << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);
        //std::uniform_int_distribution<int> enemyTypeDist(0, 2); // For 3 types of enemies

        const float enemyPlacementProbability = 1.0f; // Probability to place an enemy at each position

        std::vector<Vector3> availablePositions = openPositions;

        for (int i = 0; i < data.MAX_ENEMIES && !availablePositions.empty(); ++i) {
            std::cout << "Creating enemy " << i << " of " << data.MAX_ENEMIES << std::endl;

            std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
            int randomIndex = positionDist(gen);

            if (probabilityDist(gen) <= enemyPlacementProbability) {
                Vector3 chosenPosition = availablePositions[randomIndex];
                availablePositions.erase(availablePositions.begin() + randomIndex);

                // Randomly choose an enemy type
                EnemyType enemyType = EnemyType::IMP;
                AddEnemy(data, chosenPosition, enemyType);
            }
        }
    }

    
    void InitializeEnemyData(EnemyData& data) {
        data.positions.resize(data.MAX_ENEMIES);
        data.bodies.resize(data.MAX_ENEMIES);
        data.types.resize(data.MAX_ENEMIES, EnemyType::NONE);
        data.states.resize(data.MAX_ENEMIES, EnemyState::IDLE);
        data.paths.resize(data.MAX_ENEMIES);
        data.lastKnownPlayerPos.resize(data.MAX_ENEMIES);
        data.shootingTimers.resize(data.MAX_ENEMIES, 0.0f);
        data.bulletSpeeds.resize(data.MAX_ENEMIES);
        data.shootingIntervals.resize(data.MAX_ENEMIES);
        data.shootingHeightOffsets.resize(data.MAX_ENEMIES);
        data.movementSpeeds.resize(data.MAX_ENEMIES);
        data.health.resize(data.MAX_ENEMIES);
        data.activeStates.resize(data.MAX_ENEMIES, false);

        data.rays.resize(data.MAX_ENEMIES);
        data.rayHitInfos.resize(data.MAX_ENEMIES);
        data.facingDirections.resize(data.MAX_ENEMIES);
        data.targetPositions.resize(data.MAX_ENEMIES);
        data.rayPlayerPositions.resize(data.MAX_ENEMIES);

        data.updateFunctions.resize(data.MAX_ENEMIES);
        data.drawFunctions.resize(data.MAX_ENEMIES);
        data.moveFunctions.resize(data.MAX_ENEMIES);

        
    }

    void AddEnemy(EnemyData& data, const Vector3& position, EnemyType type) {
        for (size_t i = 0; i < data.positions.size(); ++i) {
            if (!data.activeStates[i]) {
                data.positions[i] = position;
                data.types[i] = type;
                data.states[i] = EnemyState::IDLE;
                data.paths[i].clear();
                data.lastKnownPlayerPos[i] = {0, 0, 0};
                data.shootingTimers[i] = 0.0f;
                data.shootingHeightOffsets[i] = 1.0f;
                data.activeStates[i] = true;

                Vector3 enemySize = {1.0f, 2.5f, 1.0f};
                Vector3 halfSize = Vector3Scale(enemySize, 0.5f);
                data.bodies[i].min = Vector3Subtract(position, halfSize);
                data.bodies[i].max = Vector3Add(position, halfSize);

                data.rays[i].position = position;
                data.rays[i].direction = Vector3Zero();
                data.rayHitInfos[i] = {0};
                data.facingDirections[i] = {0, 0, 1};
                data.targetPositions[i] = {0, 0, 0};
                data.rayPlayerPositions[i] = {0, 0, 0};

                switch (type) {
                    case EnemyType::IMP:
                        data.updateFunctions[i] = [i, &data](size_t index, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData, bool rayHitsPlayer, bool playerEnemyCollision, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set) {
                            UpdateImpEnemyState(index, data, playerPosition, mazeData, rayHitsPlayer, playerEnemyCollision, bulletManager, set);
                        };
                            
                        data.drawFunctions[i] = [i, &data](size_t index) { 
                            DrawImp(index, data); 
                        };
                        data.moveFunctions[i] = [i, &data](size_t index){
                            MoveImpAlongPath(index, data);
                        };
                        
                        data.movementSpeeds[i] = 0.1f;
                        data.health[i] = 100;
                        data.bulletSpeeds[i] = 0.1f;
                        data.shootingIntervals[i] = 1.0f;
                        break;
                    case EnemyType::CYBER_DEMON:
                        data.updateFunctions[i] = [i, &data](size_t index, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData, bool rayHitsPlayer, bool playerEnemyCollision, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set) {
                            UpdateCyberdemonEnemyState(index, data, playerPosition, mazeData, rayHitsPlayer, playerEnemyCollision, bulletManager, set);
                        };

                        data.drawFunctions[i] = [i, &data](size_t index) { 
                            DrawCyberdemon(index, data); 
                        };

                        data.moveFunctions[i] = [i, &data](size_t index){
                            MoveCyberdemonAlongPath(index, data);
                        };
                        data.movementSpeeds[i] = 0.1f;
                        data.health[i] = 500;
                        data.bulletSpeeds[i] = 0.1f;
                        data.shootingIntervals[i] = 4.0f;
                        break;
                    default:
                        data.updateFunctions[i] = nullptr;
                        data.drawFunctions[i] = nullptr;
                        break;
                }
                break;
            }
        }
    }

    bool CheckPlayerSingleEnemyCollision(const BoundingBox &playerBody, const BoundingBox &enemyBody) {
        if (CheckCollisionBoxes(playerBody, enemyBody)){
                    // Collision logic
                    return true;   
                }
            return false; // No collision detected
        }

    void UpdateEnemies(EnemyData& data, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set, const BoundingBox &playerBody) {
        for (size_t i = 0; i < data.positions.size(); ++i) {
            if (!data.activeStates[i]) continue;

            // Update the enemy ray's origin and direction
            data.rays[i].position = data.positions[i];
            data.rayPlayerPositions[i] = playerPosition;
            data.rays[i].direction = Vector3Normalize(Vector3Subtract(playerPosition, data.positions[i]));

            bool rayHitsPlayer = false;
            bool isObstructed = false;

            for (const auto& box : mazeData.wallBoundingBoxes) {
                RayCollision collision = GetRayCollisionBox(data.rays[i], box);
                if (collision.hit) {
                    if (collision.distance < Vector3Distance(data.rays[i].position, playerPosition)) {
                        isObstructed = true;
                        break;
                    }
                }
            }
            if (!isObstructed) {
                RayCollision playerCollision = GetRayCollisionBox(data.rays[i], playerBody);
                rayHitsPlayer = playerCollision.hit;
            }
            bool playerEnemyCollision = CheckPlayerSingleEnemyCollision(playerBody, data.bodies[i]);

            if (data.updateFunctions[i]) {
                data.updateFunctions[i](i, playerPosition, mazeData, rayHitsPlayer, playerEnemyCollision, bulletManager, set);
            }

            // Additional logic for moving the enemy along the path or handling player collision can be added here
            if (!playerEnemyCollision) {
                // Example move function call
                data.moveFunctions[i](i);
            } else if (playerEnemyCollision) {
                std::cout << "Colliding with player" << std::endl;
            }
        }
    }

    void DrawEnemies(const EnemyData& data) {
        for (size_t i = 0; i < data.positions.size(); ++i) {
            if (data.activeStates[i] && data.drawFunctions[i]) {
                data.drawFunctions[i](i);
            }
        }
    }

    void ResetEnemies(EnemyData& data) {
        std::fill(data.activeStates.begin(), data.activeStates.end(), false);
    }


    void CleanUpInactiveEnemies(EnemyData& data) {
        for (size_t i = 0; i < data.positions.size(); ++i) {
            if (!data.activeStates[i]) {
                // Remove or reset enemy data
                data.positions[i] = {0, 0, 0};
                data.bodies[i] = {{0, 0, 0}, {0, 0, 0}};
                data.types[i] = EnemyType::NONE;
                data.states[i] = EnemyState::IDLE;
                data.paths[i].clear();
                data.lastKnownPlayerPos[i] = {0, 0, 0};
                data.shootingTimers[i] = 0.0f;
                data.bulletSpeeds[i] = 0.0f;
                data.shootingIntervals[i] = 0.0f;
                data.shootingHeightOffsets[i] = 0.0f;
                data.movementSpeeds[i] = 0.0f;
                data.health[i] = 0;
                data.rays[i] = {0};
                data.rayHitInfos[i] = {0};
                data.facingDirections[i] = {0, 0, 0};
                data.targetPositions[i] = {0, 0, 0};
                data.rayPlayerPositions[i] = {0, 0, 0};
                data.updateFunctions[i] = nullptr;
                data.drawFunctions[i] = nullptr;
                data.moveFunctions[i] = nullptr;
            }
        }
    }



    
}
