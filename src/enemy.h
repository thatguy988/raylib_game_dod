#ifndef ENEMY_H
#define ENEMY_H


#include <utility>
#include <vector>

#include "raylib.h"
#include "common.h"



namespace EnemySystem {

    const int MAX_ENEMIES = 20;  // Maximum number of enemies
    
    enum class EnemyState {
        IDLE,
        ATTACKING
    };

    struct Enemy {
        Vector3 position;
        bool active;
        EnemyState state;
        std::vector<Vector3> path;

        Enemy() : position({0, 0, 0}), active(false), state(EnemyState::IDLE) {}
    };

    class EnemyManager {
    public:
        Enemy enemies[MAX_ENEMIES];

        EnemyManager();
        void InitializeEnemies(const int maze[MAX][MAX], int n, int m, float blockSize);
        void UpdateEnemies(); // Implement enemy logic
        void DrawEnemies();
    private:
        void PlaceEnemy(int index, Vector3 position);
    };
}

#endif // ENEMY_H
