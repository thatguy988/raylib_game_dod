// collision.h

#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"

#include <memory>

#include "../common.h"



#include "../entities/enemy/enemy.h"
#include "../entities/bullet/bullet.h"
#include "../entities/ammobox/ammobox.h"
#include "../entities/healthbox/HealthBox.h"


namespace CollisionHandling {
    
    int CheckCollision(const BoundingBox &playerBody, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox);
    
    bool CheckBulletCollision(const Vector3 &bulletPosition, const std::vector<BoundingBox>& wallBoundingBoxes,  const BoundingBox& endpointBoundingBox, float bulletRadius);
    
    bool CheckBulletEnemyCollision(BulletSystem::Bullet bullets[], int numBullets, std::vector<std::unique_ptr<EnemySystem::Enemy>>& enemies);
    
    
    bool CheckBulletPlayerCollision(BulletSystem::Bullet bullets[], int numBullets, const BoundingBox &playerBody, int& playerHealth);


    bool CheckPlayerEnemyCollision(const BoundingBox &playerBody, std::vector<std::unique_ptr<EnemySystem::Enemy>>& enemies);
    
    bool CheckPlayerAmmoBoxCollision(const BoundingBox &playerBody, AmmoSystem::AmmoBoxData& ammoBoxes, int &pistolAmmo, int pistolAmmoCapacity, int &shotgunAmmo, int shotgunAmmoCapacity, int &machineGunAmmo, int machineGunAmmoCapacity);


    bool CheckPlayerHealthBoxCollision(const BoundingBox &playerBody,  HealthSystem::HealthBoxData& healthBoxes, int& playerHealth, int maxPlayerHealth);

}

#endif // COLLISION_H
