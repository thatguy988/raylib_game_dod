#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

#include <utility>
#include <iostream>

#include "raylib.h"
#include "common.h"
//#include "enemy.h"
#include "collision.h"


namespace GameScreen {

    struct PlayerCamera {
        Camera3D camera;
        
   
        float cameraSpeed;
        float blockSize;
        
        
        
        PlayerCamera(float camSpeed, float blkSize);
        void InitializeCamera(std::pair<int, int> startCoords);
        int UpdateCamera(const int maze[MAX][MAX], int n, int m, EnemySystem::EnemyManager& enemyManager);
        float GetDistanceToClosestEnemy(const Vector3& position, EnemySystem::EnemyManager& enemyManager);
        

    };

    void HandleCameraRotation(Vector3 &forward, Camera3D &camera, float turnSpeed);
    void HandlePlayerMovement(Vector3 &newPosition, const Vector3 &forward, const Vector3 &right, Camera3D &camera, float cameraSpeed);


    
    

} // namespace GameScreen

#endif // PLAYER_CAMERA_H
