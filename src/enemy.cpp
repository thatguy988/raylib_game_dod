#include "Enemy.h"
#include <vector>
#include <random>
#include <iostream>
#include <utility>
#include <algorithm>  // Add this include statement


namespace EnemySystem {

    EnemyManager::EnemyManager() {
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            enemies[i] = Enemy();
        }
    }
    void EnemyManager::InitializeEnemies(const int maze[MAX][MAX], int n, int m, float blockSize) {
        std::vector<Vector3> potentialEnemyPositions;

        // Find all potential enemy positions (where maze[][] == 0)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (maze[i][j] == 0) {
                    potentialEnemyPositions.push_back(Vector3{static_cast<float>(j) * blockSize, 0.0f, static_cast<float>(i) * blockSize});
                }
            }
        }
        
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

        const float enemyPlacementProbability = 0.2f; // 20% chance to place an enemy at each position

        for (int i = 0; i < MAX_ENEMIES && !potentialEnemyPositions.empty(); ++i) {
            std::uniform_int_distribution<> positionDist(0, potentialEnemyPositions.size() - 1);
            int randomIndex = positionDist(gen);
            
            
            // Check if we should place an enemy at this position
            if (probabilityDist(gen) <= enemyPlacementProbability) {
                PlaceEnemy(i, potentialEnemyPositions[randomIndex]);
                potentialEnemyPositions.erase(potentialEnemyPositions.begin() + randomIndex); // Remove the chosen position
            }
        }
        
    }
    
    
    

    
    void EnemyManager::UpdateEnemies() {
        // Implement enemy logic (movement, AI, etc.)
    }

    void EnemyManager::DrawEnemies() {
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (enemies[i].active) {
                DrawCube(enemies[i].position, 1.0f, 2.5f, 1.0f, BLUE); // Simple blue cube for enemy
            }
        }
    }

    void EnemyManager::PlaceEnemy(int index, Vector3 position) {
        if (index >= 0 && index < MAX_ENEMIES) {
            enemies[index].position = position;
            enemies[index].active = true;
        }
    }
}
