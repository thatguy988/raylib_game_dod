#include "player_camera.h"
#include "raymath.h"


namespace GameScreen {

    PlayerCamera::PlayerCamera(float camSpeed, float blkSize) : cameraSpeed(camSpeed), blockSize(blkSize) {
        camera = { 0 };
    }
    




    void PlayerCamera::InitializeCamera(std::pair<int, int> startCoords) {
        float scaledStartX = static_cast<float>(startCoords.second) * blockSize;
        float scaledStartZ = static_cast<float>(startCoords.first) * blockSize;
        float cameraHeight = 1.0f;
        
        

        camera.position = { scaledStartX, cameraHeight, scaledStartZ };
        camera.target = { scaledStartX, cameraHeight, scaledStartZ + blockSize };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
    }
    
    
    
    int PlayerCamera::UpdateCamera(const int maze[MAX][MAX], int n, int m, EnemySystem::EnemyManager& enemyManager) {
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

        HandleCameraRotation(forward, camera, 0.1f);
        
        
        //store current position as last known good position
        Vector3 lastPosition = camera.position;


        // Calculate the potential new position
        Vector3 potentialNewPosition = camera.position;
        HandlePlayerMovement(potentialNewPosition, forward, right, camera, cameraSpeed);

        // Check for collision at the new position
        int collisionType = CollisionHandling::CheckCollision(potentialNewPosition, maze, n, m, blockSize);
        
        // Check for collisions between the player and enemies
        bool playerEnemyCollision = CollisionHandling::CheckPlayerEnemyCollision(potentialNewPosition, enemyManager, 2.0f);
        
        // Calculate distance to the closest enemy from current and potential positions
        float distanceToEnemyFromCurrent = GetDistanceToClosestEnemy(camera.position, enemyManager);
        float distanceToEnemyFromPotential = GetDistanceToClosestEnemy(potentialNewPosition, enemyManager);

        // Update the camera position if there's no wall collision and either no enemy collision or moving further from enemy
        if (collisionType == 0 && (!playerEnemyCollision || distanceToEnemyFromPotential > distanceToEnemyFromCurrent)) {
            camera.position = potentialNewPosition;
        }else{
            //push back
            camera.position = lastPosition;
        }

        // Update the camera target
        camera.target = Vector3Add(camera.position, forward);
        
        return collisionType;
    }

    float PlayerCamera::GetDistanceToClosestEnemy(const Vector3& position, EnemySystem::EnemyManager& enemyManager) {
        float minDistance = std::numeric_limits<float>::max();
        for (const auto& enemy : enemyManager.enemies) {
            if (enemy.active) {
                float distance = Vector3Distance(position, enemy.position);
                if (distance < minDistance) {
                    minDistance = distance;
                }
            }
        }
        return minDistance;
    }

   
    
    void HandleCameraRotation(Vector3 &forward, Camera3D &camera, float turnSpeed) {
        if (IsKeyDown(KEY_RIGHT)) {
            Matrix rotation = MatrixRotateY(-turnSpeed);
            forward = Vector3Transform(forward, rotation);
        }

        if (IsKeyDown(KEY_LEFT)) {
            Matrix rotation = MatrixRotateY(turnSpeed);
            forward = Vector3Transform(forward, rotation);
        }
    }
    
    void HandlePlayerMovement(Vector3 &newPosition, const Vector3 &forward, const Vector3 &right, Camera3D &camera, float cameraSpeed) {
        if (IsKeyDown(KEY_W)) newPosition = Vector3Add(camera.position, Vector3Scale(forward, cameraSpeed));
        if (IsKeyDown(KEY_S)) newPosition = Vector3Add(camera.position, Vector3Scale(forward, -cameraSpeed));
        if (IsKeyDown(KEY_A)) newPosition = Vector3Add(camera.position, Vector3Scale(right, -cameraSpeed));
        if (IsKeyDown(KEY_D)) newPosition = Vector3Add(camera.position, Vector3Scale(right, cameraSpeed));
    }

} //namespace GameScreen
