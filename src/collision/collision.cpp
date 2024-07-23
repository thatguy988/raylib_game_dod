
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

    
    bool CheckBulletEnemyCollision(BulletSystem::Bullet bullets[], int numBullets, std::vector<std::unique_ptr<EnemySystem::Enemy>>& enemies) {
        for (int i = 0; i < numBullets; ++i) {
            if (bullets[i].active && bullets[i].playerbullet == true) {
                for (auto& enemy : enemies) {
                    if (enemy->active) {
                        // Check if bullet position overlaps with enemy position
                        if (CheckCollisionBoxSphere(enemy->body, bullets[i].position, bullets[i].radius)) {
                            // Collision logic
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
                            enemy->active = false;    // Deactivate the enemy
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

    
    bool CheckPlayerEnemyCollision(const BoundingBox &playerBody, std::vector<std::unique_ptr<EnemySystem::Enemy>>& enemies) {
        for (auto& enemy : enemies) {
            if (enemy->active) {
                if (CheckCollisionBoxes(playerBody, enemy->body)) {
                    // Collision logic
                    return true; // Collision detected
                }
            }
        }
        return false; // No collision detected
    }
    
    
     bool CheckPlayerAmmoBoxCollision(
        const BoundingBox &playerBody, 
        AmmoSystem::AmmoBoxData& ammoBoxes, 
        int &pistolAmmo, int pistolAmmoCapacity, 
        int &shotgunAmmo, int shotgunAmmoCapacity, 
        int &machineGunAmmo, int machineGunAmmoCapacity
    ) {
        for (size_t i = 0; i < ammoBoxes.positions.size(); ++i) {
            if (ammoBoxes.activeStates[i]) {
                if (CheckCollisionBoxes(playerBody, ammoBoxes.bodies[i])) {
                    bool maxAmmoReached = false;

                    // Handle ammo collection based on the ammo box type
                    switch (ammoBoxes.ammoTypes[i]) {
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
                        ammoBoxes.activeStates[i] = false; // Remove ammo box after collision
                        return true;
                    }
                }
            }
        }

        return false; // No collision detected
    }

    bool CheckPlayerHealthBoxCollision(const BoundingBox &playerBody, HealthSystem::HealthBoxData& healthBoxes, int& playerHealth, int maxPlayerHealth) {
        if (playerHealth != maxPlayerHealth) {
            for (size_t i = 0; i < healthBoxes.positions.size(); ++i) {
                if (healthBoxes.activeStates[i] && CheckCollisionBoxes(playerBody, healthBoxes.bodies[i])) {
                    playerHealth = std::min(playerHealth + healthBoxes.healthPoints[i], maxPlayerHealth);
                    healthBoxes.activeStates[i] = false; // Deactivate the health box after collection
                    return true;
                }
            }
        }
        return false;
    }
    
     
    
}
