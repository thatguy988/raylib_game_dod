
#include "collision.h"

#include "raymath.h"
#include <iostream>




//GameScreen
namespace CollisionHandling {
    
    int CheckCollision(const BoundingBox &playerBody, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox) {
        // Check collision with walls
        for (const auto& wallBox : wallBoundingBoxes) {
            if (CheckCollisionBoxes(playerBody, wallBox)) {
                return 1; // Collision with a wall
            }
        }

        // Check collision with the endpoint
        if (CheckCollisionBoxes(playerBody, endpointBoundingBox)) {
            return 2; // Collision with endpoint
        }

        return 0; // No collision
    }

    
    
    bool CheckBulletCollision(const Vector3 &bulletPosition, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox, float bulletRadius) {
        for (const auto& wallBox : wallBoundingBoxes) {
            if (CheckCollisionBoxSphere(wallBox, bulletPosition, bulletRadius)) {
               // std::cout<<"BULLET AND WALL COLLISION" <<std::endl;
                return true; // Collision detected
            }
        }
        
        if (CheckCollisionBoxSphere(endpointBoundingBox, bulletPosition, bulletRadius)) {
               // std::cout<<"BULLET AND endpoint COLLISION" <<std::endl;
                return true; // Collision detected
        }
        return false; // No collision detected
    }

    
    
    
    bool CheckBulletEnemyCollision(BulletSystem::Bullet bullets[], int numBullets, EnemySystem::Enemy enemies[], int numEnemies) {
        for (int i = 0; i < numBullets; ++i) {
            if (bullets[i].active && bullets[i].playerbullet == true) {
                for (int j = 0; j < numEnemies; ++j) {
                    if (enemies[j].active) {
                        // Check if bullet position overlaps with enemy position
                        if (CheckCollisionBoxSphere(enemies[j].body, bullets[i].position, bullets[i].radius)) {
                            switch (bullets[i].weaponType) {
                                case BulletSystem::WeaponType::PISTOL:
                                    std::cout << "HIT BY PISTOL" << std::endl;
                                    break;
                                case BulletSystem::WeaponType::SHOTGUN:
                                    std::cout << "HIT BY SHOTGUN" << std::endl;
                                    break;
                                case BulletSystem::WeaponType::MACHINE_GUN:
                                    std::cout << "HIT BY MACHINE_GUN" << std::endl;
                                    break;
                                default:
                                    break;
                            }

                            bullets[i].active = false; // Deactivate the bullet
                            enemies[j].active = false;  // Deactivate the enemy
                            return true; // Collision detected
                        }
                    }
                }
            }
        }
        return false; // No collision detected
    }


    
    bool CheckBulletPlayerCollision(BulletSystem::Bullet bullets[], int numBullets, const BoundingBox &playerBody, int& playerHealth) {
        for (int i = 0; i < numBullets; ++i) {
            if (bullets[i].active && !bullets[i].playerbullet) {  // Check only enemy bullets
                // Check if bullet position is close to player position
                if (CheckCollisionBoxSphere(playerBody, bullets[i].position, bullets[i].radius)) {
                    switch(bullets[i].enemyType){
                        case EnemySystem::EnemyType::IMP:
                            //std::cout<<"IMP attack"<<std::endl;
                            playerHealth -= 5;
                            break;
                        default:
                            break;
                    }
                    
                    bullets[i].active = false; // Deactivate the bullet
                    return true; // Collision with player detected
                }
            }
        }
        return false; // No collision detected
    }

    
    bool CheckPlayerEnemyCollision(const BoundingBox &playerBody, EnemySystem::Enemy enemies[], int numEnemies) {
        for (int i = 0; i < numEnemies; ++i) {
            if (enemies[i].active) {
                if (CheckCollisionBoxes(playerBody, enemies[i].body)) {
                    // Collision logic
                    return true; // Collision detected
                }
            }
        }
        return false; // No collision detected
    }
    
    
    
    bool CheckPlayerAmmoBoxCollision(
        const BoundingBox &playerBody, 
        AmmoSystem::AmmoBox ammoBoxes[], int numAmmoBoxes, 
        int &pistolAmmo, int pistolAmmoCapacity, 
        int &shotgunAmmo, int shotgunAmmoCapacity, 
        int &machineGunAmmo, int machineGunAmmoCapacity
    ) {
        for (int i = 0; i < numAmmoBoxes; ++i) {
            if (ammoBoxes[i].active) {
                if (CheckCollisionBoxes(playerBody, ammoBoxes[i].body)) {
                    bool maxAmmoReached = false;

                    // Handle ammo collection based on the ammo box type
                    switch (ammoBoxes[i].ammoType) {
                        case BulletSystem::WeaponType::PISTOL:
                            if (pistolAmmo < pistolAmmoCapacity) {
                                pistolAmmo = std::min(pistolAmmo + 10, pistolAmmoCapacity);
                            } else {
                                maxAmmoReached = true;
                            }
                            break;
                        case BulletSystem::WeaponType::SHOTGUN:
                            if (shotgunAmmo < shotgunAmmoCapacity) {
                                shotgunAmmo = std::min(shotgunAmmo + 4, shotgunAmmoCapacity);
                            } else {
                                maxAmmoReached = true;
                            }
                            break;
                        case BulletSystem::WeaponType::MACHINE_GUN:
                            if (machineGunAmmo < machineGunAmmoCapacity) {
                                machineGunAmmo = std::min(machineGunAmmo + 25, machineGunAmmoCapacity);
                            } else {
                                maxAmmoReached = true;
                            }
                            break;
                        default:
                            break; // Handle NONE or other cases if necessary
                    }

                    if (!maxAmmoReached) {
                        ammoBoxes[i].Deactivate(); // Remove ammo box after collision
                        return true;
                    }
                }
            }
        }

        return false; // No collision detected
    }


   
    
    
    bool CheckPlayerHealthBoxCollision(const BoundingBox &playerBody, HealthSystem::HealthBox healthBoxes[], int numHealthBoxes, int& playerHealth, int maxPlayerHealth) {
        if(playerHealth != maxPlayerHealth){
            for (int i = 0; i < numHealthBoxes; ++i) {
                if (healthBoxes[i].active && CheckCollisionBoxes(playerBody, healthBoxes[i].body)) {
                    playerHealth = std::min(playerHealth + healthBoxes[i].healthPoints, maxPlayerHealth);
                    healthBoxes[i].Deactivate(); // Deactivate the health box after collection
                    return true;
                }
            }
        }
        return false;
    }

    
    


    
    
    
}
