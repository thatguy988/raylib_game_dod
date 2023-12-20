// collision.h

#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "common.h"


namespace CollisionHandling {
    int CheckCollision(const Vector3 &newPosition, const int maze[MAX][MAX], int n, int m, float blockSize);
    bool CheckBulletCollision(const Vector3 &bulletPosition, const int maze[MAX][MAX], int n, int m, float blockSize, float bulletRadius);

}

#endif // COLLISION_H
