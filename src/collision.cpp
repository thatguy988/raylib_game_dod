
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
                             switch (bullet.weaponType){
                                 case BulletSystem::WeaponType::PISTOL:
                                    std::cout<<"HIT BY PISTOL"<<std::endl;
                                    break;
                                 case BulletSystem::WeaponType::SHOTGUN:
                                    std::cout<<"HIT BY SHOTGUN"<<std::endl;
                                    break;
                                 case BulletSystem::WeaponType::MACHINE_GUN:
                                    std::cout<<"HIT BY MACHINE_GUN"<<std::endl;
                                    break;
                                 default:
                                    break;
                             }
                             
                             
                            // std::cout<< "PLAYER BULLET HIT ENEMY" << std::endl;
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
    
    bool CheckBulletPlayerCollision(BulletSystem::BulletManager& bulletManager, const Vector3& playerPosition, int& playerHealth) {
        const float COLLISION_DISTANCE = 2.0f;  // Collision radius
        for (auto& bullet : bulletManager.bullets) {
            if (bullet.active && !bullet.playerbullet) {  // Check only enemy bullets
                // Check if bullet position is close to player position
                if (Vector3Distance(bullet.position, playerPosition) < COLLISION_DISTANCE) {
                    switch(bullet.enemyType){
                        case EnemySystem::EnemyType::IMP:
                            //std::cout<<"IMP attack"<<std::endl;
                            playerHealth -= 5;
                            break;
                        default:
                            break;
                    }
                    
                    bullet.active = false; // Deactivate the bullet
                    return true; // Collision with player detected
                }
            }
        }
        return false; // No collision detected
    }
    
    bool CheckPlayerEnemyCollision(const Vector3& playerPosition, EnemySystem::EnemyManager& enemyManager, float collisionDistance) {
        for (const auto& enemy : enemyManager.enemies) {
            if (enemy.active) {
                if (Vector3Distance(playerPosition, enemy.position) < collisionDistance) {
                    //std::cout << "Player collided with enemy" << std::endl;
                    return true; // Collision detected
                }
            }
        }
        return false; // No collision detected
    }
    
    bool CheckPlayerAmmoBoxCollision(const Vector3& playerPosition, AmmoSystem::AmmoBoxManager& ammoBoxManager,  BulletSystem::BulletManager& bulletManager, float collisionDistance) {
        for (auto& ammoBox : ammoBoxManager.ammoBoxes) {
            if (ammoBox.active) {
                if (Vector3Distance(playerPosition, ammoBox.position) < collisionDistance) {
                    bool maxAmmoReached = false;
                     // Handle ammo collection based on the ammo box type
                    switch (ammoBox.ammoType) {
                        case BulletSystem::WeaponType::PISTOL:
                            if(bulletManager.PistolAmmo < bulletManager.PistolAmmoCapacity){
                                bulletManager.PistolAmmo = std::min(bulletManager.PistolAmmo + 10, bulletManager.PistolAmmoCapacity);
                            }else{
                                maxAmmoReached = true;
                            }
                            break;
                        case BulletSystem::WeaponType::SHOTGUN:
                            if(bulletManager.ShotgunAmmo < bulletManager.ShotgunAmmoCapacity){
                                bulletManager.ShotgunAmmo = std::min(bulletManager.ShotgunAmmo + 4, bulletManager.ShotgunAmmoCapacity);
                            }else{
                                maxAmmoReached = true;
                            }
                            break;
                        case BulletSystem::WeaponType::MACHINE_GUN:
                            if(bulletManager.MachineGunAmmo < bulletManager.MachineGunAmmoCapacity){
                                bulletManager.MachineGunAmmo = std::min(bulletManager.MachineGunAmmo + 25, bulletManager.MachineGunAmmoCapacity);
                            }else{
                                maxAmmoReached = true;
                            }
                            break;
                        default:
                            break;  // Handle NONE or other cases if necessary
                    }
                    if (!maxAmmoReached){
                        //std::cout << "Player collected ammo box" << std::endl;
                        ammoBox.Deactivate(); //remove ammobox after collision
                    }else{
                        //std::cout<< "Player is at max ammo do not collect ammo box" << std::endl;   
                    }
                    return true;
                }
            }
        }
        return false; // No collision detected
    }
    // Method inside HealthBoxManager to check collision and update player health
    bool CheckPlayerHealthBoxCollision(const Vector3& playerPosition, HealthSystem::HealthBoxManager& healthBoxManager, int& playerHealth, int maxPlayerHealth, float collisionDistance) {
        if(playerHealth != maxPlayerHealth){
            for (auto& healthBox : healthBoxManager.healthBoxes) {
                if (healthBox.active && Vector3Distance(playerPosition, healthBox.position) < collisionDistance) {
                    playerHealth = std::min(playerHealth + healthBox.healthPoints, maxPlayerHealth);
                    healthBox.Deactivate(); // Deactivate the health box after collection
                    return true;
                }
            }
        }
        return false;
    }
    
    
    
    


    
    
    
}
