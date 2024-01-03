
#include "collision.h"

#include "raymath.h"
#include <iostream>




//GameScreen
namespace CollisionHandling {
    int CheckCollision(const Vector3 &newPosition, const int maze[MAX][MAX], int n, int m, float blockSize) {
        int gridX = (int)(newPosition.x / blockSize + roundingOffset); // Center of the block
        int gridZ = (int)(newPosition.z / blockSize + roundingOffset); // Center of the block

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
                int gridX = (int)((bulletPosition.x + dx) / blockSize + roundingOffset);
                int gridZ = (int)((bulletPosition.z + dz) / blockSize + roundingOffset);

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
             if (bullet.active && bullet.playerbullet == true) {
                 for (auto& enemy : enemyManager.enemies) {
                     if (enemy.active) {
                         // Check if bullet position overlaps with enemy position
                         if (Vector3Distance(bullet.position, enemy.position) < COLLISION_DISTANCE) {
                             std::cout<< "PLAYER BULLET HIT ENEMY" << std::endl;
                             bullet.active = false; // Deactivate the bullet
                             enemy.active = false;  // Deactivate the enemy
                             return true; // Collision detected
                         }
                     }
                 }
             }
             
             //for bullet hitting enemy
             /*
             else if(bullet.active && !bullet.playerbullet){
                 for (auto& enemy : enemyManager.enemies) {
                     if (enemy.active) {
                         // Check if bullet position overlaps with enemy position
                         if (Vector3Distance(bullet.position, enemy.position) < COLLISION_DISTANCE) {
                             std::cout << "ENEMY BULLET HIT ENEMY" << std::endl;
                             bullet.active = false; // Deactivate the bullet
                             return true; // Collision detected
                         }
                     }
                 }
                 
             }
             */
         }
        return false; // No collision detected
    }
    
    bool CheckBulletPlayerCollision(BulletSystem::BulletManager& bulletManager, const Vector3& playerPosition) {
        const float COLLISION_DISTANCE = 2.0f;  // Collision radius
        for (auto& bullet : bulletManager.bullets) {
            if (bullet.active && !bullet.playerbullet) {  // Check only enemy bullets
                // Check if bullet position is close to player position
                if (Vector3Distance(bullet.position, playerPosition) < COLLISION_DISTANCE) {
                    bullet.active = false; // Deactivate the bullet
                    std::cout << "Bullet hit player" << std::endl;
                    return true; // Collision with player detected
                }
            }
        }
        return false; // No collision detected
    }
    
}
