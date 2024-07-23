#ifndef IMP_H
#define IMP_H

#include "enemy.h"

namespace EnemySystem {

    class Imp : public Enemy {
    public:
        Imp();
        virtual ~Imp() = default;
        void Draw() const override;
        
        void CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
        void CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);
        void MoveEnemyAlongPath(Enemy& enemy);
        void HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
        void UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
        

    private:
        
    };

}

#endif // IMP_H
