#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

#include <memory>
#include <utility>
#include <iostream>

#include "raylib.h"

#include "../common.h"



#include "../collision/collision.h"
#include "../maze/maze_cells.h"

namespace EnemySystem {
    struct EnemyData;
}

namespace GameScreen {

    struct PlayerCamera {
        Camera3D camera;
        BoundingBox playerBody; // Bounding box representing the player's body

   
        float cameraSpeed;
        float playerHeight;
        float playerWidth;
        
        
        PlayerCamera(float camSpeed, float height, float width);
        void InitializeCamera(std::pair<int, int> startCoords, float blockSize);
        
        MazeCells UpdateCamera(const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox, EnemySystem::EnemyData& data);


        float GetDistanceToClosestEnemy(const Vector3& position, EnemySystem::EnemyData& data);
        
    };

    void HandleCameraRotation(Vector3 &forward, Camera3D &camera, float turnSpeed);
    void HandlePlayerMovement(Vector3 &newPosition, const Vector3 &forward, const Vector3 &right, Camera3D &camera, float cameraSpeed);


    
    

} // namespace GameScreen

#endif // PLAYER_CAMERA_H



