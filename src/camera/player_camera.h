#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

#include <utility>
#include <iostream>

#include "raylib.h"

#include "../common.h"



#include "../collision/collision.h"


namespace GameScreen {

    struct PlayerCamera {
        Camera3D camera;
        BoundingBox playerBody; // Bounding box representing the player's body

   
        float cameraSpeed;
        float blockSize;
        float playerHeight;
        float playerWidth;
        
        
        PlayerCamera(float camSpeed, float blkSize, float height, float width);
        void InitializeCamera(std::pair<int, int> startCoords);
        
        

        int UpdateCamera(const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox, EnemySystem::Enemy enemies[], int numEnemies);
        float GetDistanceToClosestEnemy(const Vector3& position, EnemySystem::Enemy enemies[], int numEnemies);
        
    };

    void HandleCameraRotation(Vector3 &forward, Camera3D &camera, float turnSpeed);
    void HandlePlayerMovement(Vector3 &newPosition, const Vector3 &forward, const Vector3 &right, Camera3D &camera, float cameraSpeed);


    
    

} // namespace GameScreen

#endif // PLAYER_CAMERA_H



