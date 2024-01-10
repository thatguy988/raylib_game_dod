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
    #include "ammobox.h"
    #include "healthbox.h"
    
    #define LIGHTRED        (Color){ 230, 41, 55, 128 }     




   

    namespace GameScreen {
        
        BulletSystem::WeaponType currentWeapon = BulletSystem::WeaponType::PISTOL;
        
        bool isRedFlashActive = false;
        float redFlashDuration = 0.2f; // Duration of the flash in seconds
        float redFlashTimer = 0.0f;    // Timer to track the flash duration
        
        int playerHealth = 100; //health of player
        int maxPlayerHealth = 100; //max health of player

        int level = 1;
        int n;
        int m;
        int maze[MAX][MAX];
        int dist[MAX][MAX];
        
        std::vector<Vector3> openPositions; // Vector to store open positions

        std::vector<Vector3> openPositionsForItems;
     
        
        
        
        PlayerCamera playerCamera(0.1f, 4.0f);
        std::pair<int, int> endCoords;
        
        BulletSystem::BulletManager bulletManager;
        EnemySystem::EnemyManager enemyManager;
        AmmoSystem::AmmoBoxManager ammoBoxManager;
        HealthSystem::HealthBoxManager healthBoxManager;
        
        
        


        std::pair<int, int> InitializeMaze() {
            auto startEndCoords = MazeGenerator::GenerateMaze(n, m, 70, maze);
            endCoords = startEndCoords.second;
            
            
            // Clear and update openPositions vector
            openPositions.clear();
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (maze[i][j] == 0) {
                        openPositions.push_back(Vector3{static_cast<float>(j) * playerCamera.blockSize, 0.0f, static_cast<float>(i) * playerCamera.blockSize});
                    }
                }
            }
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
            enemyManager.Reset();
            bulletManager.Reset();
            ammoBoxManager.Reset();
            healthBoxManager.Reset();
            
            
            updateMazeSize();
            auto startCoords = InitializeMaze();
            playerCamera.InitializeCamera(startCoords);
            openPositionsForItems = openPositions;
            enemyManager.InitializeEnemies(openPositions);
            
            
            
            
            
            
            ammoBoxManager.InitializeAmmoBoxes(openPositionsForItems);
            healthBoxManager.InitializeHealthBoxes(openPositionsForItems);
            

            //enemyManager.InitializeEnemies(maze, n, m, GameScreen::playerCamera.blockSize);
           
            
            

                        
          

                    

        }

        void UpdateGame() {
            
            
            
            
            auto exitpoint = playerCamera.UpdateCamera(maze, n, m, enemyManager);
            if(exitpoint == 2 && IsKeyPressed(KEY_R)){
                level += 1;
                InitGame();
                //updateMazeSize();
                //auto newStartCoords = InitializeMaze();
                //playerCamera.InitializeCamera(newStartCoords);   
                //enemyManager.InitializeEnemies(openPositions);
                //ammoBoxManager.InitializeAmmoBoxes(openPositions);
                //healthBoxManager.InitializeHealthBoxes(openPositions);


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
            //CollisionHandling::CheckBulletPlayerCollision(bulletManager, playerCamera.camera.position);
            
            // Check for bullet-player collision
            if (CollisionHandling::CheckBulletPlayerCollision(bulletManager, playerCamera.camera.position, playerHealth)) {
                isRedFlashActive = true; // Activate the red flash
                redFlashTimer = redFlashDuration; // Reset the timer
                
            }
            
            // check collision between player and ammo box
            CollisionHandling::CheckPlayerAmmoBoxCollision(playerCamera.camera.position, ammoBoxManager, bulletManager, 2.0f);
            //check collision between player and health box
            CollisionHandling:: CheckPlayerHealthBoxCollision(playerCamera.camera.position, healthBoxManager, playerHealth, maxPlayerHealth, 2.0f);
            
                


            // Update the red flash timer
            if (isRedFlashActive) {
                redFlashTimer -= GetFrameTime();
                if (redFlashTimer <= 0) {
                    isRedFlashActive = false; // Deactivate the red flash
                }
            }
            
            enemyManager.UpdateEnemies(playerCamera.camera.position, maze, n, m, GameScreen::playerCamera.blockSize, openPositions, bulletManager);
            
            
            
            
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
        ammoBoxManager.DrawAmmoBoxes();
        healthBoxManager.DrawHealthBoxes();
        

        
        
        
        EndMode3D();
        
        // Draw the red flash if active
        if (isRedFlashActive) {
            DrawRectangle(0, 0, 1000, 600, LIGHTRED);
        }

        DrawText("Game Screen", 20, 20, 40, WHITE);
        
        // Display ammo information
        char ammoText[50]; // Buffer to hold the formatted ammo text
        switch (currentWeapon) {
            case BulletSystem::WeaponType::PISTOL:
                sprintf(ammoText, "Pistol Ammo: %d/%d", bulletManager.PistolAmmo, bulletManager.PistolAmmoCapacity);
                break;
            case BulletSystem::WeaponType::SHOTGUN:
                sprintf(ammoText, "Shotgun Ammo: %d/%d", bulletManager.ShotgunAmmo, bulletManager.ShotgunAmmoCapacity);
                break;
            case BulletSystem::WeaponType::MACHINE_GUN:
                sprintf(ammoText, "Machine Gun Ammo: %d/%d", bulletManager.MachineGunAmmo, bulletManager.MachineGunAmmoCapacity);
                break;
            default://for none
                break;
        }
        DrawText(ammoText, 20, 70, 20, WHITE); // Position the text below "Game Screen"
        char healthText[50];
        sprintf(healthText, "Health: %d/%d", playerHealth, maxPlayerHealth);
        DrawText(healthText, 20, 100, 20, WHITE);
  

        
        
        



    }


    void UnloadGame() {
        enemyManager.Reset();
        bulletManager.Reset();
        ammoBoxManager.Reset();
        healthBoxManager.Reset();
       

    }
    

} // namespace GameScreen