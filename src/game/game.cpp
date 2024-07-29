   #include "game.h"
    
    
 
    
    
    
    #define LIGHTRED        (Color){ 230, 41, 55, 128 }     

    namespace GameScreen {
        
        BulletSystem::WeaponType currentWeapon = BulletSystem::WeaponType::PISTOL;
        
        bool isRedFlashActive = false;
        float redFlashDuration = 0.2f; // Duration of the flash in seconds
        float redFlashTimer = 0.0f;    // Timer to track the flash duration
        
        int playerHealth = 100; //health of player
        int maxPlayerHealth = 100; //max health of player

        int level = 1;


        



        bool canExitLevel = true;

        
        PlayerCamera playerCamera(0.1f, 2.0f, 1.0f);
       

        int MAX_BULLETS = 200;
        BulletSystem::BulletData bulletManager;
        BulletSystem::SparseSet sparseSet(MAX_BULLETS);

        BulletSystem::WeaponManager weaponsManager;
        EnemySystem::EnemyData enemyManager;
        AmmoSystem::AmmoBoxData ammoBoxManager;
        HealthSystem::HealthBoxData healthBoxManager;

        MazeGenerator::MazeData MazeData;

        Message pickupMessage;
        
        
        
        // Method to update and draw the UI elements
        void DrawUI() {
            // Draw the red flash if active
            if (isRedFlashActive) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), LIGHTRED);
            }

            // Draw game text
            DrawText("Game Screen", 20, 20, 40, WHITE);
            
            // Display ammo information
            char ammoText[50]; // Buffer to hold the formatted ammo text
            switch (currentWeapon) {
                case BulletSystem::WeaponType::PISTOL:
                    sprintf(ammoText, "Pistol Ammo: %d/%d", weaponsManager.PistolAmmo, weaponsManager.PistolAmmoCapacity);
                    break;
                case BulletSystem::WeaponType::SHOTGUN:
                    sprintf(ammoText, "Shotgun Ammo: %d/%d", weaponsManager.ShotgunAmmo, weaponsManager.ShotgunAmmoCapacity);
                    break;
                case BulletSystem::WeaponType::MACHINE_GUN:
                    sprintf(ammoText, "Machine Gun Ammo: %d/%d", weaponsManager.MachineGunAmmo, weaponsManager.MachineGunAmmoCapacity);
                    break;
                default:
                    // Handling for NONE or other cases, if necessary
                    break;
            }
            DrawText(ammoText, 20, 70, 20, WHITE); // Position the text below "Game Screen"

            // Display health information
            char healthText[50];
            sprintf(healthText, "Health: %d/%d", playerHealth, maxPlayerHealth);
            DrawText(healthText, 20, 100, 20, WHITE);

            // Display the pickup message if the timer is active
            if (pickupMessage.displayTimer > 0) {
                DrawText(pickupMessage.text.c_str(), 20, 130, 20, pickupMessage.color);
            }
        }
        
        

        void InitGame() {
            EnemySystem::ResetEnemies(enemyManager);
            BulletSystem::ResetBulletData(bulletManager, sparseSet);
            AmmoSystem::ResetAmmoBoxes(ammoBoxManager);
            HealthSystem::ResetHealthBoxes(healthBoxManager);
            EnemySystem::SetRandomMaxEnemies(level, enemyManager);
            AmmoSystem::SetMaxAmmoBoxes(ammoBoxManager);
            HealthSystem::SetMaxHealthBoxes(healthBoxManager);

            pickupMessage = Message(); // reset message
            
            
            
            if (level == 10) {
                auto predefinedLevel = PredefinedLevels::GetLevel(level);
                auto coords = MazeGenerator::ConvertPredefinedLevelToMaze(predefinedLevel, MazeData);
                MazeData.startCoords = coords.first;
                
                
                // Clear and update openPositions vector
                MazeData.openPositions.clear();
                for (int i = 0; i < MazeData.n; ++i) {
                    for (int j = 0; j < MazeData.m; ++j) {
                        if (MazeData.maze[i][j] == 0) {
                            MazeData.openPositions.push_back(Vector3{static_cast<float>(j) * MazeData.blockSize, 0.0f, static_cast<float>(i) * MazeData.blockSize});
                        }
                    }
                }
                
                
                playerCamera.InitializeCamera(MazeData.startCoords, MazeData.blockSize);
                MazeData.openPositionsForItems = MazeData.openPositions;
                EnemySystem::InitializeEnemyData(enemyManager);
                EnemySystem::InitializeEnemies(enemyManager, MazeData.openPositions,level);
                AmmoSystem::InitializeAmmoBoxes(ammoBoxManager, MazeData.openPositionsForItems);
                HealthSystem::InitializeHealthBoxes(healthBoxManager, MazeData.openPositionsForItems); 
                BulletSystem::InitializeBulletData(bulletManager, MAX_BULLETS);

                MazeGenerator::GenerateWallBoundingBoxes(MazeData);
                                
                
                MazeGenerator::InitializeOutOfBoundsBox(MazeData, 10.0f); // 10 units buffer around the maze
                // Additional setup after converting the level
            } else {
                MazeGenerator::UpdateMazeSize(MazeData, level);
                auto startCoords = MazeGenerator::InitializeMaze(MazeData);
                playerCamera.InitializeCamera(startCoords, MazeData.blockSize);
                MazeData.openPositionsForItems = MazeData.openPositions;
                EnemySystem::InitializeEnemyData(enemyManager);
                EnemySystem::InitializeEnemies(enemyManager, MazeData.openPositions, level);
                AmmoSystem::InitializeAmmoBoxes(ammoBoxManager,MazeData.openPositionsForItems);
                HealthSystem::InitializeHealthBoxes(healthBoxManager, MazeData.openPositionsForItems); 
                
                MazeGenerator::GenerateWallBoundingBoxes(MazeData);
                                
                MazeGenerator::InitializeOutOfBoundsBox(MazeData, 10.0f); // 10 units buffer around the maze
            }


        }

        void UpdateGame() {
            
            
            auto exitpoint = playerCamera.UpdateCamera(MazeData.wallBoundingBoxes, MazeData.endpointBoundingBox, enemyManager);
            // Check if level is 50 and if the enemy (Cyberdemon) is defeated
            if (level == 10) {
                // Assuming there's only one enemy and it's a Cyberdemon
                if (enemyManager.activeStates[0]) {
                    canExitLevel = false; // Can't exit if the Cyberdemon is still active
                }else{
                    canExitLevel = true;
                }
            }

            if (exitpoint == 2 && IsKeyPressed(KEY_R) && canExitLevel) {
                level += 1;
                InitGame();
            }
            
            
            
            BulletSystem::HandleWeaponInputAndShooting(bulletManager, sparseSet, weaponsManager, playerCamera, currentWeapon);

            BulletSystem::UpdateBullets(bulletManager, sparseSet, MazeData.wallBoundingBoxes, MazeData.endpointBoundingBox);
            
            CollisionHandling::CheckBulletOutOfBounds(bulletManager, sparseSet, MazeData.outOfBoundsBox);


            
            CollisionHandling::CheckBulletEnemyCollision(bulletManager, sparseSet, enemyManager);

            EnemySystem::CleanUpInactiveEnemies(enemyManager);


            // Check for bullet-player collision
            auto bulletplayerinfo = CollisionHandling::CheckBulletPlayerCollision(bulletManager, sparseSet, playerCamera.playerBody, playerHealth);
            if(bulletplayerinfo.first){
                playerHealth -= bulletplayerinfo.second; //update player health
                isRedFlashActive = true; // Activate the red flash
                redFlashTimer = redFlashDuration; // Reset the timer
            }
            

            
            
                

            
            // check collision between player and ammo box
            auto ammoboxcollisioninfo = CollisionHandling::CheckPlayerAmmoBoxCollision(playerCamera.playerBody, ammoBoxManager, weaponsManager);
            if (ammoboxcollisioninfo.collided){
                pickupMessage.text = "Picked up " + std::to_string(ammoboxcollisioninfo.amount) + " " +
                                 (ammoboxcollisioninfo.ammoType == BulletSystem::WeaponType::PISTOL ? "pistol" :
                                  ammoboxcollisioninfo.ammoType == BulletSystem::WeaponType::SHOTGUN ? "shotgun" : "machine gun") + " ammo";
                pickupMessage.displayTimer = pickupMessage.duration;
            }
            
            

            //check collision between player and health box
            auto healthboxcollisioninfo = CollisionHandling:: CheckPlayerHealthBoxCollision(playerCamera.playerBody, healthBoxManager, playerHealth, maxPlayerHealth);
            if (healthboxcollisioninfo.collided){
                pickupMessage.text = "Picked up " + std::to_string(healthboxcollisioninfo.amount) + " HP";
                pickupMessage.displayTimer = pickupMessage.duration;
            }
                
            
                


            // Update the red flash timer
            if (isRedFlashActive) {
                redFlashTimer -= GetFrameTime();
                if (redFlashTimer <= 0) {
                    isRedFlashActive = false;
                }
            }

            // Update the pickup message timer
            if (pickupMessage.displayTimer > 0) {
                pickupMessage.displayTimer -= GetFrameTime();
            }

            EnemySystem::UpdateEnemies(enemyManager, playerCamera.camera.position, MazeData, bulletManager, sparseSet, playerCamera.playerBody);
            
            
            
            
        }
        
        

    void DrawGame() {
        ClearBackground(BLACK);
        
        BeginMode3D(GameScreen::playerCamera.camera);
        
        
        
        MazeGenerator::DrawMaze(MazeData);
        EnemySystem::DrawEnemies(enemyManager);
        BulletSystem::DrawBullets(bulletManager, sparseSet);
        AmmoSystem::DrawAmmoBoxes(ammoBoxManager);
        HealthSystem::DrawHealthBoxes(healthBoxManager);
        EndMode3D();
        
        DrawUI();
 
    }
    

    void UnloadGame() {
        EnemySystem::ResetEnemies(enemyManager);
        BulletSystem::ResetBulletData(bulletManager, sparseSet);
        AmmoSystem::ResetAmmoBoxes(ammoBoxManager);
        HealthSystem::ResetHealthBoxes(healthBoxManager);
        level = 1;
       

    }
    

} // namespace GameScreen