// collision.h

#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "common.h"
#include "enemy.h"
#include "bullet.h"
#include "ammobox.h"
#include "healthbox.h"


namespace CollisionHandling {
    int CheckCollision(const Vector3 &newPosition, const int maze[MAX][MAX], int n, int m, float blockSize);
    bool CheckBulletCollision(const Vector3 &bulletPosition, const int maze[MAX][MAX], int n, int m, float blockSize, float bulletRadius);
    
    bool CheckBulletEnemyCollision(BulletSystem::BulletManager& bulletManager, EnemySystem::EnemyManager& enemyManager);
    
    bool CheckBulletPlayerCollision(BulletSystem::BulletManager& bulletManager, const Vector3& playerPosition, int& playerHealth);
    bool CheckPlayerEnemyCollision(const Vector3& playerPosition, EnemySystem::EnemyManager& enemyManager, float collisionDistance);
    bool CheckPlayerAmmoBoxCollision(const Vector3& playerPosition, AmmoSystem::AmmoBoxManager& ammoBoxManager, BulletSystem::BulletManager& bulletManager, float collisionDistance);
    bool CheckPlayerHealthBoxCollision(const Vector3& playerPosition, HealthSystem::HealthBoxManager& healthBoxManager, int& playerHealth, int maxPlayerHealth, float collisionDistance);




}

#endif // COLLISION_H
