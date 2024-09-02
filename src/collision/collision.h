#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include <memory>
#include "../common.h"
#include "../entities/bullet/bullet.h"
#include "../entities/enemy/enemy.h"
#include "../entities/ammobox/ammobox.h"
#include "../entities/healthbox/HealthBox.h"
#include "../maze/maze_cells.h"

namespace EnemySystem {
    struct EnemyData;
    enum class EnemyType;
}

namespace BulletSystem {
    struct BulletData;
    class SparseSet;
    struct WeaponManager;
    enum class WeaponType; 

}

namespace AmmoSystem {
    struct AmmoBoxData;
}


namespace CollisionHandling {

    struct CollisionResults {
        bool collided;
        int amount; // amount of ammo or health
        BulletSystem::WeaponType ammoType; // if ammobox what weapon type
    };
    
    MazeCells CheckCollision(const BoundingBox &playerBody, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox);
    
    bool CheckBulletCollision(const Vector3 &bulletPosition, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox, float bulletRadius);

    void CheckBulletEnemyCollision(BulletSystem::BulletData& bulletdata, BulletSystem::SparseSet& set, EnemySystem::EnemyData& enemydata);

    std::pair<bool, int> CheckBulletPlayerCollision(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& playerBody);
    
    void CheckBulletOutOfBounds(BulletSystem::BulletData& data, BulletSystem::SparseSet& set, const BoundingBox& boundary);

    bool CheckPlayerEnemyCollision(const BoundingBox &playerBody, EnemySystem::EnemyData& data);
    
    CollisionResults CheckPlayerAmmoBoxCollision(const BoundingBox &playerBody, AmmoSystem::AmmoBoxData& ammoBoxes, BulletSystem::WeaponManager& weapons);

    CollisionResults CheckPlayerHealthBoxCollision(const BoundingBox &playerBody, HealthSystem::HealthBoxData& healthBoxes, int& playerHealth, int maxPlayerHealth);

}

#endif // COLLISION_H
