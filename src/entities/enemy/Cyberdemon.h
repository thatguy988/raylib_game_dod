#ifndef CYBERDEMON_H
#define CYBERDEMON_H

#include "enemy.h"
#include "../bullet/bullet.h"

namespace EnemySystem {

    struct EnemyData;

    void CalculateCyberdemonPathToRandomTarget(size_t index, EnemyData& data, MazeGenerator::MazeData& mazeData);
    void CalculateCyberdemonPathToPlayer(size_t index, EnemyData& data, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData);
    void MoveCyberdemonAlongPath(size_t index, EnemyData& data);
    void HandleCyberdemonAttackState(size_t index, EnemyData& data, const Vector3& playerPosition, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
    void UpdateCyberdemonEnemyState(size_t index, EnemyData& data, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData, bool rayHitsPlayer, bool playerEnemyCollision, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
    void DrawCyberdemon(size_t index, const EnemyData& data);

}

#endif // CYBERDEMON_H
