#ifndef BARONOFHELL_H
#define BARONOFHELL_H

#include "enemy.h"

namespace EnemySystem {

    class BaronOfHell : public Enemy {
    public:
        BaronOfHell();
        virtual ~BaronOfHell() = default;

        void Draw() const override;
        void CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
        void CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);
        void MoveEnemyAlongPath(Enemy& enemy);
        void HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
        void UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set);
        

    private:
        
        
    };

}

#endif // BARONOFHELL_H
