#ifndef DEMON_H
#define DEMON_H

#include "enemy.h"

namespace EnemySystem {

    class Demon : public Enemy {
    public:
        Demon();
        virtual ~Demon() = default;
        void Draw() const override;
            
            
        void CalculatePathToRandomTarget(Enemy& enemy, const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize);
        void CalculatePathToPlayer(Enemy& enemy, const Vector3& playerPosition, int maze[MAX][MAX], int n, int m, float blockSize);
        void MoveEnemyAlongPath(Enemy& enemy);
        void HandleAttackState(Enemy& enemy, bool playerEnemyCollision);
        void UpdateEnemyState(Enemy& enemy, const Vector3& playerPosition,  const std::vector<Vector3>& openPositions, int maze[MAX][MAX], int n, int m, float blockSize, bool rayHitsPlayer, bool playerEnemyCollision);
                

    private:
        
    };

}

#endif // DEMON_H
