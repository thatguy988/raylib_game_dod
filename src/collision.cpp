
#include "collision.h"

#include "raymath.h"
#include <iostream>




//GameScreen
namespace CollisionHandling {
    int CheckCollision(const Vector3 &newPosition, const int maze[MAX][MAX], int n, int m, float blockSize) {
        int gridX = (int)(newPosition.x / blockSize + 0.5f); // Center of the block
        int gridZ = (int)(newPosition.z / blockSize + 0.5f); // Center of the block

        if (gridX >= 0 && gridX < m && gridZ >= 0 && gridZ < n) {
            if (maze[gridZ][gridX] == 1) {
                return 1; // Collision with a wall
            } else if (maze[gridZ][gridX] == 2) {
                return 2; // collision with endpoint
            }
        }
        return 0; // no collision
    }
    
    bool CheckBulletCollision(const Vector3 &bulletPosition, const int maze[MAX][MAX], int n, int m, float blockSize, float bulletRadius) {
        // Check collision in a small area around the bullet, not just its center
        for (float dx = -bulletRadius; dx <= bulletRadius; dx += bulletRadius) {
            for (float dz = -bulletRadius; dz <= bulletRadius; dz += bulletRadius) {
                // Calculate the grid coordinates for the current point
                int gridX = (int)((bulletPosition.x + dx) / blockSize + 0.5f);
                int gridZ = (int)((bulletPosition.z + dz) / blockSize + 0.5f);

                // Check if the point is within the maze boundaries
                if (gridX >= 0 && gridX < m && gridZ >= 0 && gridZ < n) {
                    // Check if the point collides with a wall
                    if (maze[gridZ][gridX] == 1) {
                        return true; // Collision detected
                    }
                }
            }
        }
        return false; // No collision detected
    }
    bool CheckBulletEnemyCollision(BulletSystem::BulletManager& bulletManager, EnemySystem::EnemyManager& enemyManager) {
         const float COLLISION_DISTANCE = 2.0f;
         for (auto& bullet : bulletManager.bullets) {
             if (bullet.active) {
                 for (auto& enemy : enemyManager.enemies) {
                     if (enemy.active) {
                         // Check if bullet position overlaps with enemy position
                         if (Vector3Distance(bullet.position, enemy.position) < COLLISION_DISTANCE) {
                             bullet.active = false; // Deactivate the bullet
                             enemy.active = false;  // Deactivate the enemy
                             return true; // Collision detected
                         }
                     }
                 }
             }
         }
        return false; // No collision detected
    }
    
   

    
    
  
    
    
    

}
/*
namespace CollisionHandling {
    int CheckCollision(const Vector3 &newPosition, const int maze[MAX][MAX], int n, int m, float blockSize) {
        // If blockSize is constant and 1/blockSize can be pre-calculated, replace blockSize with its inverse and use multiplication here.
        int gridX = (int)(newPosition.x / blockSize + 0.5f); // Center of the block
        int gridZ = (int)(newPosition.z / blockSize + 0.5f); // Center of the block

        // Check maze boundaries first
        if (gridX < 0 || gridX >= m || gridZ < 0 || gridZ >= n) {
            return 0; // Out of bounds, no collision
        }

        // Return collision type with a single statement
        return (maze[gridZ][gridX] == 1) ? 1 : ((maze[gridZ][gridX] == 2) ? 2 : 0);
    }
}
*/