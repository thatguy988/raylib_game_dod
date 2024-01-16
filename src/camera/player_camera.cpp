#include "player_camera.h"
#include "raymath.h"


namespace GameScreen {
    
    
    GameScreen::PlayerCamera::PlayerCamera(float camSpeed, float blkSize, float height, float width) 
    : cameraSpeed(camSpeed), blockSize(blkSize), playerHeight(height), playerWidth(width) {
        
        camera = {0}; // Initialize your camera
        playerBody = {0}; // Initialize the bounding box representing the player's body
    }

 
    void PlayerCamera::InitializeCamera(std::pair<int, int> startCoords) {
        float scaledStartX = static_cast<float>(startCoords.second) * blockSize;
        float scaledStartZ = static_cast<float>(startCoords.first) * blockSize;
        float cameraHeight = 1.0f;
        
        camera.position = { scaledStartX, cameraHeight, scaledStartZ };
        camera.target = { scaledStartX, cameraHeight, scaledStartZ + blockSize };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        
        // Initialize the player body bounding box
        Vector3 playerBodySize = {playerWidth, playerHeight, playerWidth}; // Assuming player body is a cube
        Vector3 halfSize = Vector3Scale(playerBodySize, 0.5f);
        playerBody.min = Vector3Subtract(camera.position, halfSize);
        playerBody.max = Vector3Add(camera.position, halfSize);
    }
    
    int PlayerCamera::UpdateCamera(const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox, EnemySystem::Enemy enemies[], int numEnemies) {
        
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

        HandleCameraRotation(forward, camera, 0.1f);

        // Store current player body position as last known good position
        Vector3 lastBodyPosition = Vector3Add(playerBody.min, Vector3Scale((Vector3){playerWidth, playerHeight, playerWidth}, 0.5f));

        // Calculate the potential new position for the player body
        Vector3 potentialNewPosition = lastBodyPosition;
        HandlePlayerMovement(potentialNewPosition, forward, right, camera, cameraSpeed);

        // Update the player body bounding box for collision check
        Vector3 halfSize = Vector3Scale((Vector3){playerWidth, playerHeight, playerWidth}, 0.5f);
        BoundingBox potentialPlayerBody = { Vector3Subtract(potentialNewPosition, halfSize), Vector3Add(potentialNewPosition, halfSize) };
        

        // Check for collision at the new position
        int collisionType = CollisionHandling::CheckCollision(potentialPlayerBody, wallBoundingBoxes, endpointBoundingBox);
        bool playerEnemyCollision = CollisionHandling::CheckPlayerEnemyCollision(playerBody, enemies,numEnemies);
       
        // Calculate distance to the closest enemy from current and potential positions
        float distanceToEnemyFromCurrent = GetDistanceToClosestEnemy(camera.position, enemies, numEnemies);
        float distanceToEnemyFromPotential = GetDistanceToClosestEnemy(potentialNewPosition, enemies, numEnemies);
        
    
        // Collision resolution and position update
        if (collisionType == 0 && (!playerEnemyCollision || distanceToEnemyFromPotential > distanceToEnemyFromCurrent)) {
            playerBody = potentialPlayerBody; // Update playerBody with the new position
            camera.position = Vector3Subtract(potentialNewPosition, Vector3Scale(forward, 0.0f)); // Update camera position based on playerBody
        } else {
            // Push back to last known good position
            Vector3 newBodyPosition = lastBodyPosition;
            playerBody.min = Vector3Subtract(newBodyPosition, halfSize);
            playerBody.max = Vector3Add(newBodyPosition, halfSize);
            camera.position = Vector3Subtract(newBodyPosition, Vector3Scale(forward, 0.0f));
        }

        // Update the camera target
        camera.target = Vector3Add(camera.position, forward);
        
        return collisionType;
    }
    
    
    float PlayerCamera::GetDistanceToClosestEnemy(const Vector3& position, EnemySystem::Enemy enemies[], int numEnemies) {
        float minDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < numEnemies; ++i) {
            if (enemies[i].active) {
                float distance = Vector3Distance(position, enemies[i].position);
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
