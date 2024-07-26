#ifndef IMP_H
#define IMP_H

#include "enemy.h"
#include "../bullet/bullet.h"
namespace BulletSystem {
    struct BulletData;
    struct SparseSet;
}

namespace EnemySystem {

    struct EnemyData;


        
    void CalculateImpPathToRandomTarget(size_t index, EnemyData& data, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
    void CalculateImpPathToPlayer(size_t index, EnemyData& data, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);

    void MoveImpAlongPath(size_t index, EnemyData& data);
    void HandleImpAttackState(size_t index, EnemyData& data, const Vector3& playerPosition, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
    // update enemystate will be update imp
    void UpdateImpEnemyState(size_t index, EnemyData& data, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, bool playerEnemyCollision, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
    void DrawImp(size_t index, const EnemyData& data);
    


}

#endif // IMP_H
