
#include <iostream>
#include <utility>

#include "raylib.h"
#include "game.h"
#include "maze_generator.h"
#include "raymath.h"

namespace GameScreen {
    
    const int n = 12; // Horizontal size of the maze
    const int m = 16; // Vertical size of the maze

        

    Camera3D camera;
    const float cameraSpeed = 0.1f; // Adjust speed as needed
        
    std::pair<int, int> endCoords;
    float blockSize = 4.0f;  // This should match the block size used in DrawGame
       

    int maze[MAX][MAX]; // The maze array
    int dist[MAX][MAX]; // Distance matrix for SPF
    
    
    std::pair<int, int> InitializeMaze() {
        auto startEndCoords = MazeGenerator::GenerateMaze(n, m, 70, maze);
        endCoords = startEndCoords.second; // Store the end coordinates globally
        return startEndCoords.first; // Return the start coordinates
    }
    
    void InitializeCamera(std::pair<int, int> startCoords) {
        float scaledStartX = (float)startCoords.second * blockSize;
        float scaledStartZ = (float)startCoords.first * blockSize;
        float cameraHeight = 1.0f;

        camera.position = Vector3 { scaledStartX, cameraHeight, scaledStartZ };
        camera.target = Vector3 { scaledStartX, cameraHeight, scaledStartZ + blockSize };
        camera.up = Vector3 { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
    }

    void InitGame() {
        auto startCoords = InitializeMaze();
        InitializeCamera(startCoords);
    }
    

    void HandleCameraRotation(Vector3 &forward) {
        float turnSpeed = 0.1f; // Adjust as needed

        if (IsKeyDown(KEY_RIGHT)) {
            Matrix rotation = MatrixRotateY(-turnSpeed);
            forward = Vector3Transform(forward, rotation);
        }

        if (IsKeyDown(KEY_LEFT)) {
            Matrix rotation = MatrixRotateY(turnSpeed);
            forward = Vector3Transform(forward, rotation);
        }
    }

    void HandlePlayerMovement(Vector3 &newPosition, const Vector3 &forward, const Vector3 &right) {
        if (IsKeyDown(KEY_W)) newPosition = Vector3Add(camera.position, Vector3Scale(forward, cameraSpeed));
        if (IsKeyDown(KEY_S)) newPosition = Vector3Add(camera.position, Vector3Scale(forward, -cameraSpeed));
        if (IsKeyDown(KEY_A)) newPosition = Vector3Add(camera.position, Vector3Scale(right, -cameraSpeed));
        if (IsKeyDown(KEY_D)) newPosition = Vector3Add(camera.position, Vector3Scale(right, cameraSpeed));
    }
    
    int CheckCollision(const Vector3 &newPosition) {
        int gridX = (int)(newPosition.x / blockSize + 0.5f); // Center of the block
        int gridZ = (int)(newPosition.z / blockSize + 0.5f); // Center of the block

        if (gridX >= 0 && gridX < m && gridZ >= 0 && gridZ < n) {
            if (maze[gridZ][gridX] == 1) {
                return 1; // Collision with a wall
            } else if (maze[gridZ][gridX] == 2) {
                return 2; // collision with endpoint
            }
        }
        return 0; // no collision
    }
    

    void UpdateGame() {
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
        
        HandleCameraRotation(forward);
        Vector3 newPosition = camera.position;
        HandlePlayerMovement(newPosition, forward, right);
        
        int collisionType = CheckCollision(newPosition);
        
        if (collisionType == 1) {
        // Collision with a wall
        newPosition = camera.position; // Prevent movement
        } else if (collisionType == 2 && IsKeyPressed(KEY_R)) {
            // Collision with the endpoint and 'R' key pressed
            auto newStartCoords = InitializeMaze();
            // Reposition the camera at the new starting position
            camera.position.x = (float)newStartCoords.second * blockSize;
            camera.position.z = (float)newStartCoords.first * blockSize;
        }

        if (collisionType == 0) {
            // No collision, update camera position
            camera.position = newPosition;
        }
   
        // Update the camera target
        camera.target = Vector3Add(camera.position, forward);
    }
    

    void DrawGame() {
        ClearBackground(BLACK);
        
        float wallHeight = 3.0f; // Height of the walls
        float floorThickness = 0.1f; // Thickness of the floor

        BeginMode3D(camera);
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (maze[i][j] == 1) { 
                    // Draw walls
                    DrawCube(Vector3 { (float)j * blockSize, wallHeight / 2, (float)i * blockSize }, 
                             blockSize, wallHeight, blockSize, DARKGRAY);
                } else if(maze[i][j] == 0) {
                    // Draw floor
                    DrawCube(Vector3 { (float)j * blockSize, -floorThickness / 2, (float)i * blockSize }, 
                             blockSize, floorThickness, blockSize, LIGHTGRAY);
                } else if(maze[i][j] == 2){
                    //Draw Wall
                    DrawCube(Vector3 { (float)j * blockSize, wallHeight / 2, (float)i * blockSize }, 
                             blockSize, wallHeight, blockSize, GREEN);
                    // Draw floor
                    DrawCube(Vector3 { (float)j * blockSize, -floorThickness / 2, (float)i * blockSize }, 
                             blockSize, floorThickness, blockSize, GREEN);
                    
                }
            }
        }

        EndMode3D();

        DrawText("Game Screen", 20, 20, 40, WHITE);
    }


    void UnloadGame() {
        // Unload game resources 
    }
    
  
    
    


} // namespace GameScreen
