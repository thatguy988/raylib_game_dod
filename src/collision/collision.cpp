
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
        const std::vector<size_t>& denseSet = set.GetDense();
        for (size_t i = 0; i < set.GetSize(); ++i) {
            size_t index = denseSet[i];
            if (data.activeStates[index] && !CheckCollisionBoxSphere(boundary, data.positions[index], data.radii[index])) {
                std::cout << "bullet went out of bounds" << std::endl;
                // Bullet is out of bounds, deactivate it
                data.activeStates[index] = false;
            }
        }
    }

    
    //check one bullet
    bool CheckBulletCollision(const Vector3 &bulletPosition, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox, float bulletRadius) {
        for (const auto& wallBox : wallBoundingBoxes) {
            if (CheckCollisionBoxSphere(wallBox, bulletPosition, bulletRadius)) {
                //std::cout<<"BULLET AND WALL COLLISION" <<std::endl;
                return true; // Collision detected
            }
        }
        
        if (CheckCollisionBoxSphere(endpointBoundingBox, bulletPosition, bulletRadius)) {
               // std::cout<<"BULLET AND endpoint COLLISION" <<std::endl;
                return true; // Collision detected
        }
        return false; // No collision detected
    }

    void CheckBulletEnemyCollision(BulletSystem::BulletData& bulletdata, BulletSystem::SparseSet& set, EnemySystem::EnemyData& enemydata) {
        const std::vector<size_t>& denseSet = set.GetDense();
        for (size_t i = 0; i < set.GetSize(); ++i) {
            size_t bulletIndex = denseSet[i];
            if (bulletdata.activeStates[bulletIndex] && bulletdata.playerBullets[bulletIndex]) {
                for (size_t j = 0; j < enemydata.positions.size(); ++j) {
                    if (enemydata.activeStates[j]) {
                        // Check if bullet position overlaps with enemy position
                        if (CheckCollisionBoxSphere(enemydata.bodies[j], bulletdata.positions[bulletIndex], bulletdata.radii[bulletIndex])) {
                            // Collision logic
                            switch (bulletdata.weaponTypes[bulletIndex]) {
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
                            bulletdata.activeStates[bulletIndex] = false; // Deactivate the bullet
                            enemydata.activeStates[j] = false;    // Deactivate the enemy
                            // Continue to the next bullet after handling the collision
                            break;
                        }
                    }
                }
            }
        }
    }


    
    bool CheckBulletPlayerCollision(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& playerBody, int& playerHealth) {
        const std::vector<size_t>& denseSet = set.GetDense();
        bool collisionDetected = false;

        for (size_t i = 0; i < set.GetSize(); ++i) {
            size_t index = denseSet[i];
            if (data.activeStates[index] && !data.playerBullets[index]) {  // Check only enemy bullets
                // Check if bullet position is close to player position
                if (CheckCollisionBoxSphere(playerBody, data.positions[index], data.radii[index])) {
                    switch (data.enemyTypes[index]) {
                        case EnemySystem::EnemyType::IMP:
                            playerHealth -= 5;
                            break;
                        default:
                            break;
                    }
                    data.activeStates[index] = false; // Deactivate the bullet
                    collisionDetected = true; // Set the flag to true
                }
            }
        }

        return collisionDetected; // Return the flag
    }




    
    bool CheckPlayerEnemyCollision(const BoundingBox &playerBody, EnemySystem::EnemyData& data) {
        for (size_t i; i < data.positions.size(); ++i) {
            if (data.activeStates[i]) {
                if (CheckCollisionBoxes(playerBody, data.bodies[i])) {
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
