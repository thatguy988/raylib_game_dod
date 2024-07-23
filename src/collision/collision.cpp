
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

    void CheckBulletOutOfBounds(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& boundary) {
        for (size_t i = 0; i < set.size; ++i) {
            size_t index = set.dense[i];
            if (data.activeStates[index] && !CheckCollisionBoxSphere(boundary, data.positions[index], data.radii[index])) {
                std::cout << "bullet went out of bounds" << std::endl;
                // Bullet is out of bounds, deactivate it
                data.activeStates[index] = false;
            }
        }
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

    
    bool CheckBulletEnemyCollision(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, std::vector<std::unique_ptr<EnemySystem::Enemy>>& enemies) {
        for (size_t i = 0; i < set.size; ++i) {
            size_t index = set.dense[i];
            if (data.activeStates[index] && data.playerBullets[index]) {
                for (auto& enemy : enemies) {
                    if (enemy->active) {
                        // Check if bullet position overlaps with enemy position
                        if (CheckCollisionBoxSphere(enemy->body, data.positions[index], data.radii[index])) {
                            // Collision logic
                            switch (data.weaponTypes[index]) {
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

                            data.activeStates[index] = false; // Deactivate the bullet
                            enemy->active = false;    // Deactivate the enemy
                            return true; // Collision detected
                        }
                    }
                }
            }
        }
        return false; // No collision detected
    }


    
    bool CheckBulletPlayerCollision(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& playerBody, int& playerHealth) {
        for (size_t i = 0; i < set.size; ++i) {
            size_t index = set.dense[i];
            if (data.activeStates[index] && !data.playerBullets[index]) {  // Check only enemy bullets
                // Check if bullet position is close to player position
                if (CheckCollisionBoxSphere(playerBody, data.positions[index], data.radii[index])) {
                    switch (data.enemyTypes[index]) {
                        case EnemySystem::EnemyType::IMP:
                            //std::cout << "IMP attack" << std::endl;
                            playerHealth -= 5;
                            break;
                        default:
                            break;
                    }
                    
                    data.activeStates[index] = false; // Deactivate the bullet
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
    
    
     bool CheckPlayerAmmoBoxCollision(const BoundingBox &playerBody, AmmoSystem::AmmoBoxData& ammoBoxes, BulletSystem::WeaponManager& weapons) {
        for (size_t i = 0; i < ammoBoxes.positions.size(); ++i) {
            if (ammoBoxes.activeStates[i]) {
                if (CheckCollisionBoxes(playerBody, ammoBoxes.bodies[i])) {
                    bool maxAmmoReached = false;

                    // Handle ammo collection based on the ammo box type
                    switch (ammoBoxes.ammoTypes[i]) {
                        case BulletSystem::WeaponType::PISTOL:
                            if (weapons.PistolAmmo < weapons.PistolAmmoCapacity) {
                                weapons.PistolAmmo = std::min(weapons.PistolAmmo + 10, weapons.PistolAmmoCapacity);
                            } else {
                                maxAmmoReached = true;
                            }
                            break;
                        case BulletSystem::WeaponType::SHOTGUN:
                            if (weapons.ShotgunAmmo < weapons.ShotgunAmmoCapacity) {
                                weapons.ShotgunAmmo = std::min(weapons.ShotgunAmmo + 4, weapons.ShotgunAmmoCapacity);
                            } else {
                                maxAmmoReached = true;
                            }
                            break;
                        case BulletSystem::WeaponType::MACHINE_GUN:
                            if (weapons.MachineGunAmmo < weapons.MachineGunAmmoCapacity) {
                                weapons.MachineGunAmmo = std::min(weapons.MachineGunAmmo + 25, weapons.MachineGunAmmoCapacity);
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
