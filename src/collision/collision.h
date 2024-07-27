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


namespace EnemySystem {
    struct EnemyData;
    enum class EnemyType;
}

namespace BulletSystem {
    struct BulletData;
    class SparseSet;
    struct WeaponManager;
}

namespace AmmoSystem {
    struct AmmoBoxData;
}

namespace CollisionHandling {
    
    int CheckCollision(const BoundingBox &playerBody, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox);
    
    bool CheckBulletCollision(const Vector3 &bulletPosition, const std::vector<BoundingBox>& wallBoundingBoxes,  const BoundingBox& endpointBoundingBox, float bulletRadius);

    void CheckBulletEnemyCollision(BulletSystem::BulletData& bulletdata, BulletSystem::SparseSet& set, EnemySystem::EnemyData& enemydata);

    bool CheckBulletPlayerCollision(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& playerBody, int& playerHealth);
    
    void CheckBulletOutOfBounds(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& boundary);



    bool CheckPlayerEnemyCollision(const BoundingBox &playerBody, EnemySystem::EnemyData& data);
    
    bool CheckPlayerAmmoBoxCollision(const BoundingBox &playerBody, AmmoSystem::AmmoBoxData& ammoBoxes, BulletSystem::WeaponManager& weapons);


    bool CheckPlayerHealthBoxCollision(const BoundingBox &playerBody,  HealthSystem::HealthBoxData& healthBoxes, int& playerHealth, int maxPlayerHealth);

}

#endif // COLLISION_H
