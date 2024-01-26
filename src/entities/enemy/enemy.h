#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include <vector>
#include <queue>
#include "raylib.h"
#include "raymath.h"
#include "../../common.h"
#include "../bullet/bullet.h"

#include <random>
#include <iostream>
#include <utility>
#include <algorithm>  
#include <cmath>

namespace EnemySystem {

    enum class EnemyType {
        IMP,
        DEMON,
        BARON_OF_HELL,
        CYBER_DEMON,
        NONE
    };
    
    enum class EnemyState {
        IDLE,
        ATTACKING,
        CHASING
    };

    class Enemy {
    public:
        Vector3 position;
        BoundingBox body;
        Vector3 targetPosition;
        Vector3 rayPlayerPosition;
        float movementSpeed;
        bool active;
        EnemyState state;
        std::vector<Vector3> path;
        Vector3 lastKnownPlayerPos;
        float shootingTimer;
        float bulletSpeed;
        float shootingInterval;
        float shootingHeightOffset;
        EnemyType type;
        int enemyHealth;
        Ray ray;
        RayCollision rayHitInfo;
        Vector3 facingDirection;

        Enemy();
        virtual ~Enemy() = default;
        virtual void Draw() const;
        
        static int PosToIndex(int x, int y, int m);
        
        static std::vector<Vector3> BFS(const Vector3& start, const Vector3& end, const int maze[MAX][MAX], int n, int m, float blockSize);
        
        static Vector3 GetRandomOpenPosition(const std::vector<Vector3>& openPositions);

        

    protected:
        // Protected methods and members
    };

    
    
} // namespace EnemySystem

#endif // ENEMY_H


