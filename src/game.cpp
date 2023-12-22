    #include <iostream>
    #include <utility>
    #include <random>
    #include "raylib.h"
    #include "game.h"
    #include "maze_generator.h"
    #include "raymath.h"
    #include "collision.h"
    #include "player_camera.h"
    #include "bullet.h"
    #include "enemy.h"


   

    namespace GameScreen {
        
        BulletSystem::WeaponType currentWeapon = BulletSystem::WeaponType::PISTOL;


        int level = 1;
        int n;
        int m;
        int maze[MAX][MAX];
        int dist[MAX][MAX];
     
        
        PlayerCamera playerCamera(0.1f, 4.0f);
        std::pair<int, int> endCoords;
        
        BulletSystem::BulletManager bulletManager;
        EnemySystem::EnemyManager enemyManager;
        


        std::pair<int, int> InitializeMaze() {
            auto startEndCoords = MazeGenerator::GenerateMaze(n, m, 70, maze);
            endCoords = startEndCoords.second;
            return startEndCoords.first;
            
        }
     
        
      
        void updateMazeSize() {
            int baseSize = 5; // The base size for 'n' and 'm' when level = 1
            int maxSize = 25; // Maximum size for 'n' and 'm'
            int minSize = 5;  // Minimum size for 'n' and 'm'
            int incrementPerLevel = 2; // Base increment for the maze size per level
            int randomRange = 3; // Range of random adjustment (+/-)

            static std::mt19937 rng(std::random_device{}()); // Initialize a random number generator
            std::uniform_int_distribution<int> dist(-randomRange, randomRange); // Distribution in range [-randomRange, randomRange]

            int randomAdjustmentN = dist(rng); // Random adjustment between -randomRange and +randomRange
            int randomAdjustmentM = dist(rng); // Random adjustment between -randomRange and +randomRange

            int newN = baseSize + ((level - 1) * incrementPerLevel) + randomAdjustmentN;
            int newM = baseSize + ((level - 1) * incrementPerLevel) + randomAdjustmentM;

            // Ensure 'n' and 'm' stay within the min and max bounds
            newN = std::max(minSize, std::min(newN, maxSize));
            newM = std::max(minSize, std::min(newM, maxSize));
            
            n = newN;
            m = newM;

            std::cout << "New maze size: " << n << "x" << m << std::endl;
            std::cout << "Level " << level << std::endl;
        }
        

        void InitGame() {
            updateMazeSize();
            auto startCoords = InitializeMaze();
            playerCamera.InitializeCamera(startCoords);
            std::cout<< "initialize enemies"<< std::endl;
            enemyManager.InitializeEnemies(maze, n, m, GameScreen::playerCamera.blockSize);
            std::cout<< "done"<< std::endl;

                    

        }

        void UpdateGame() {
            
            auto exitpoint = playerCamera.UpdateCamera(maze, n, m);
            if(exitpoint == 2 && IsKeyPressed(KEY_R)){
                level += 1;
                updateMazeSize();
                auto newStartCoords = InitializeMaze();
                playerCamera.InitializeCamera(newStartCoords);   
                enemyManager.InitializeEnemies(maze, n, m, GameScreen::playerCamera.blockSize);
            }    
            
            if (IsKeyPressed(KEY_ONE)) currentWeapon = BulletSystem::WeaponType::PISTOL;
            if (IsKeyPressed(KEY_TWO)) currentWeapon = BulletSystem::WeaponType::SHOTGUN;
            if (IsKeyPressed(KEY_THREE)) currentWeapon = BulletSystem::WeaponType::MACHINE_GUN;
            
            
            Vector3 bulletDirection = Vector3Normalize(Vector3Subtract(playerCamera.camera.target, playerCamera.camera.position));
            
            float deltaTime = GetFrameTime(); // Get the time elapsed since the last frame

            // Update timers for all weapons
            bulletManager.machineGunLastShotTime += deltaTime;
            bulletManager.pistolGunLastShotTime += deltaTime;
            bulletManager.shotGunLastShotTime += deltaTime;

            // Handle machine gun shooting
            if (currentWeapon == BulletSystem::WeaponType::MACHINE_GUN && IsKeyDown(KEY_SPACE)) {
                if (bulletManager.machineGunLastShotTime >= bulletManager.machineGunShotDelay) {
                    bulletManager.Shoot(playerCamera.camera.position, bulletDirection, 1.0f, currentWeapon);
                    bulletManager.machineGunLastShotTime = 0.0f; // Reset the timer
                }
            }

            // Handle pistol shooting
            if (currentWeapon == BulletSystem::WeaponType::PISTOL && IsKeyPressed(KEY_SPACE)) {
                if(bulletManager.pistolGunLastShotTime >= bulletManager.pistolGunShotDelay){
                    bulletManager.Shoot(playerCamera.camera.position, bulletDirection, 0.1f, currentWeapon);
                    bulletManager.pistolGunLastShotTime = 0.0f; // Reset the timer
                }
            }

            // Handle shotgun shooting
            if (currentWeapon == BulletSystem::WeaponType::SHOTGUN && IsKeyPressed(KEY_SPACE)) {
                if(bulletManager.shotGunLastShotTime >= bulletManager.shotGunShotDelay){
                    bulletManager.Shoot(playerCamera.camera.position, bulletDirection, 1.0f, currentWeapon);
                    bulletManager.shotGunLastShotTime = 0.0f; // Reset the timer
                }
            }
            
            bulletManager.UpdateBullets(maze, n, m, GameScreen::playerCamera.blockSize);
            
            CollisionHandling::CheckBulletEnemyCollision(bulletManager, enemyManager);
            
            
        }
        
        

    void DrawGame() {
        ClearBackground(BLACK);
        
        float wallHeight = 3.0f; // Height of the walls
        float floorThickness = 0.1f; // Thickness of the floor
        
        
    

        BeginMode3D(GameScreen::playerCamera.camera);
        
        for (int i = 0; i < GameScreen::n; ++i) {
            for (int j = 0; j < GameScreen::m; ++j) {
                if (GameScreen::maze[i][j] == 1) { 
                    // Draw walls
                    DrawCube(Vector3 { (float)j * GameScreen::playerCamera.blockSize, wallHeight / 2, (float)i * GameScreen::playerCamera.blockSize }, 
                             GameScreen::playerCamera.blockSize, wallHeight, GameScreen::playerCamera.blockSize, DARKGRAY);
                } else if(GameScreen::maze[i][j] == 0 || GameScreen::maze[i][j] == 3) {
                    // Draw floor
                    DrawCube(Vector3 { (float)j * GameScreen::playerCamera.blockSize, -floorThickness / 2, (float)i * GameScreen::playerCamera.blockSize }, 
                             GameScreen::playerCamera.blockSize, floorThickness, GameScreen::playerCamera.blockSize, LIGHTGRAY);
                } else if(GameScreen::maze[i][j] == 2){
                    //Draw Wall
                    DrawCube(Vector3 { (float)j * GameScreen::playerCamera.blockSize, wallHeight / 2, (float)i * GameScreen::playerCamera.blockSize }, 
                             GameScreen::playerCamera.blockSize, wallHeight, GameScreen::playerCamera.blockSize, GREEN);
                    // Draw floor
                    DrawCube(Vector3 { (float)j * GameScreen::playerCamera.blockSize, -floorThickness / 2, (float)i * GameScreen::playerCamera.blockSize }, 
                             GameScreen::playerCamera.blockSize, floorThickness, GameScreen::playerCamera.blockSize, GREEN);
                    
                }
            }
        }
        enemyManager.DrawEnemies();
        bulletManager.DrawBullets();
        
        EndMode3D();
       

        DrawText("Game Screen", 20, 20, 40, WHITE);
    }


    void UnloadGame() {
       

    }
    

} // namespace GameScreen