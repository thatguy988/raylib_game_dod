#ifndef CYBERDEMON_H
#define CYBERDEMON_H

#include "enemy.h"

namespace EnemySystem {

    class Cyberdemon : public Enemy {
    public:
        Cyberdemon();
        virtual ~Cyberdemon() = default;
        void Draw() const override;
        
        void CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
        void CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);
        void MoveEnemyAlongPath(Enemy& enemy);
        void HandleAttackState(Enemy& enemy, const Vector3& playerPosition, BulletSystem::BulletManager& bulletManager);
        void UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, BulletSystem::BulletManager& bulletManager);
        

    private:
        
    };

}

#endif // IMP_H
