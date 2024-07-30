   #include "game.h"
    


    namespace GameScreen {
                
        
        // Method to update and draw the UI elements
        void DrawUI(const GameData& gameData) {
            // Draw the red flash if active
            if (gameData.isRedFlashActive) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), LIGHTRED);
            }

            // Draw game text
            DrawText("Game Screen", 20, 20, 40, WHITE);
            
            // Display ammo information
            char ammoText[50]; // Buffer to hold the formatted ammo text
            switch (gameData.currentWeapon) {
                case BulletSystem::WeaponType::PISTOL:
                    sprintf(ammoText, "Pistol Ammo: %d/%d", gameData.weaponsManager.PistolAmmo, gameData.weaponsManager.PistolAmmoCapacity);
                    break;
                case BulletSystem::WeaponType::SHOTGUN:
                    sprintf(ammoText, "Shotgun Ammo: %d/%d", gameData.weaponsManager.ShotgunAmmo, gameData.weaponsManager.ShotgunAmmoCapacity);
                    break;
                case BulletSystem::WeaponType::MACHINE_GUN:
                    sprintf(ammoText, "Machine Gun Ammo: %d/%d", gameData.weaponsManager.MachineGunAmmo, gameData.weaponsManager.MachineGunAmmoCapacity);
                    break;
                default:
                    // Handling for NONE or other cases, if necessary
                    break;
            }
            DrawText(ammoText, 20, 70, 20, WHITE); // Position the text below "Game Screen"

            // Display health information
            char healthText[50];
            sprintf(healthText, "Health: %d/%d", gameData.playerHealth, gameData.maxPlayerHealth);
            DrawText(healthText, 20, 100, 20, WHITE);

            // Display the pickup message if the timer is active
            if (gameData.pickupMessage.displayTimer > 0) {
                DrawText(gameData.pickupMessage.text.c_str(), 20, 130, 20, gameData.pickupMessage.color);
            }
        }
        
        

        void InitGame(GameData& gameData) {
            EnemySystem::ResetEnemies(gameData.enemyManager);
            BulletSystem::ResetBulletData(gameData.bulletManager, gameData.sparseSet);
            AmmoSystem::ResetAmmoBoxes(gameData.ammoBoxManager);
            HealthSystem::ResetHealthBoxes(gameData.healthBoxManager);
            EnemySystem::SetRandomMaxEnemies(gameData.level, gameData.enemyManager);
            AmmoSystem::SetMaxAmmoBoxes(gameData.ammoBoxManager);
            HealthSystem::SetMaxHealthBoxes(gameData.healthBoxManager);

            gameData.pickupMessage = Message(); // reset message
            
            
            
            if (gameData.level == 10) {
                auto predefinedLevel = PredefinedLevels::GetLevel(gameData.level);
                auto coords = MazeGenerator::ConvertPredefinedLevelToMaze(predefinedLevel, gameData.MazeData);
                gameData.MazeData.startCoords = coords.first;
                
                
                // Clear and update openPositions vector
                gameData.MazeData.openPositions.clear();
                for (int i = 0; i < gameData.MazeData.n; ++i) {
                    for (int j = 0; j < gameData.MazeData.m; ++j) {
                        if (gameData.MazeData.maze[i][j] == 0) {
                            gameData.MazeData.openPositions.push_back(Vector3{static_cast<float>(j) * gameData.MazeData.blockSize, 0.0f, static_cast<float>(i) * gameData.MazeData.blockSize});
                        }
                    }
                }
                
                
                gameData.playerCamera.InitializeCamera(gameData.MazeData.startCoords, gameData.MazeData.blockSize);
                gameData.MazeData.openPositionsForItems = gameData.MazeData.openPositions;
                EnemySystem::InitializeEnemyData(gameData.enemyManager);
                EnemySystem::InitializeEnemies(gameData.enemyManager, gameData.MazeData.openPositions,gameData.level);
                AmmoSystem::InitializeAmmoBoxes(gameData.ammoBoxManager, gameData.MazeData.openPositionsForItems);
                HealthSystem::InitializeHealthBoxes(gameData.healthBoxManager, gameData.MazeData.openPositionsForItems); 
                BulletSystem::InitializeBulletData(gameData.bulletManager, gameData.MAX_BULLETS);

                MazeGenerator::GenerateWallBoundingBoxes(gameData.MazeData);
                                
                
                MazeGenerator::InitializeOutOfBoundsBox(gameData.MazeData, 10.0f); // 10 units buffer around the maze
                // Additional setup after converting the level
            } else {
                MazeGenerator::UpdateMazeSize(gameData.MazeData, gameData.level);
                auto startCoords = MazeGenerator::InitializeMaze(gameData.MazeData);
                gameData.playerCamera.InitializeCamera(startCoords, gameData.MazeData.blockSize);
                gameData.MazeData.openPositionsForItems = gameData.MazeData.openPositions;
                EnemySystem::InitializeEnemyData(gameData.enemyManager);
                EnemySystem::InitializeEnemies(gameData.enemyManager, gameData.MazeData.openPositions, gameData.level);
                AmmoSystem::InitializeAmmoBoxes(gameData.ammoBoxManager,gameData.MazeData.openPositionsForItems);
                HealthSystem::InitializeHealthBoxes(gameData.healthBoxManager, gameData.MazeData.openPositionsForItems); 
                
                MazeGenerator::GenerateWallBoundingBoxes(gameData.MazeData);
                                
                MazeGenerator::InitializeOutOfBoundsBox(gameData.MazeData, 10.0f); // 10 units buffer around the maze
            }


        }

        void UpdateGame(GameData& gameData) {
            
            
            auto exitpoint = gameData.playerCamera.UpdateCamera(gameData.MazeData.wallBoundingBoxes, gameData.MazeData.endpointBoundingBox, gameData.enemyManager);
            // Check if level is 50 and if the enemy (Cyberdemon) is defeated
            if (gameData.level == 10) {
                // Assuming there's only one enemy and it's a Cyberdemon
                if (gameData.enemyManager.activeStates[0]) {
                    gameData.canExitLevel = false; // Can't exit if the Cyberdemon is still active
                }else{
                    gameData.canExitLevel = true;
                }
            }

            if (exitpoint == 2 && IsKeyPressed(KEY_R) && gameData.canExitLevel) {
                gameData.level += 1;
                InitGame(gameData);
            }
            
            
            
            BulletSystem::HandleWeaponInputAndShooting(gameData.bulletManager, gameData.sparseSet, gameData.weaponsManager, gameData.playerCamera, gameData.currentWeapon);

            BulletSystem::UpdateBullets(gameData.bulletManager, gameData.sparseSet, gameData.MazeData.wallBoundingBoxes, gameData.MazeData.endpointBoundingBox);
            
            CollisionHandling::CheckBulletOutOfBounds(gameData.bulletManager, gameData.sparseSet, gameData.MazeData.outOfBoundsBox);


            
            CollisionHandling::CheckBulletEnemyCollision(gameData.bulletManager, gameData.sparseSet, gameData.enemyManager);

            EnemySystem::CleanUpInactiveEnemies(gameData.enemyManager);


            // Check for bullet-player collision
            auto bulletplayerinfo = CollisionHandling::CheckBulletPlayerCollision(gameData.bulletManager, gameData.sparseSet, gameData.playerCamera.playerBody);
            if(bulletplayerinfo.first){
                gameData.playerHealth -= bulletplayerinfo.second; //update player health
                gameData.isRedFlashActive = true; // Activate the red flash
                gameData.redFlashTimer = gameData.redFlashDuration; // Reset the timer
            }
            

            
            
                

            
            // check collision between player and ammo box
            auto ammoboxcollisioninfo = CollisionHandling::CheckPlayerAmmoBoxCollision(gameData.playerCamera.playerBody, gameData.ammoBoxManager, gameData.weaponsManager);
            if (ammoboxcollisioninfo.collided){
                gameData.pickupMessage.text = "Picked up " + std::to_string(ammoboxcollisioninfo.amount) + " " +
                                 (ammoboxcollisioninfo.ammoType == BulletSystem::WeaponType::PISTOL ? "pistol" :
                                  ammoboxcollisioninfo.ammoType == BulletSystem::WeaponType::SHOTGUN ? "shotgun" : "machine gun") + " ammo";
                gameData.pickupMessage.displayTimer = gameData.pickupMessage.duration;
            }
            
            

            //check collision between player and health box
            auto healthboxcollisioninfo = CollisionHandling:: CheckPlayerHealthBoxCollision(gameData.playerCamera.playerBody, gameData.healthBoxManager, gameData.playerHealth, gameData.maxPlayerHealth);
            if (healthboxcollisioninfo.collided){
                gameData.pickupMessage.text = "Picked up " + std::to_string(healthboxcollisioninfo.amount) + " HP";
                gameData.pickupMessage.displayTimer = gameData.pickupMessage.duration;
            }
                
            
                


            // Update the red flash timer
            if (gameData.isRedFlashActive) {
                gameData.redFlashTimer -= GetFrameTime();
                if (gameData.redFlashTimer <= 0) {
                    gameData.isRedFlashActive = false;
                }
            }

            // Update the pickup message timer
            if (gameData.pickupMessage.displayTimer > 0) {
                gameData.pickupMessage.displayTimer -= GetFrameTime();
            }

            EnemySystem::UpdateEnemies(gameData.enemyManager, gameData.playerCamera.camera.position, gameData.MazeData, gameData.bulletManager, gameData.sparseSet, gameData.playerCamera.playerBody);
            
            
            
            
        }
        
        

    void DrawGame(const GameData& gameData) {
        ClearBackground(BLACK);
        
        BeginMode3D(gameData.playerCamera.camera);
        
        
        
        MazeGenerator::DrawMaze(gameData.MazeData);
        EnemySystem::DrawEnemies(gameData.enemyManager);
        BulletSystem::DrawBullets(gameData.bulletManager, gameData.sparseSet);
        AmmoSystem::DrawAmmoBoxes(gameData.ammoBoxManager);
        HealthSystem::DrawHealthBoxes(gameData.healthBoxManager);
        EndMode3D();
        
        DrawUI(gameData);
 
    }
    

    void UnloadGame(GameData& gameData) {
        EnemySystem::ResetEnemies(gameData.enemyManager);
        BulletSystem::ResetBulletData(gameData.bulletManager, gameData.sparseSet);
        AmmoSystem::ResetAmmoBoxes(gameData.ammoBoxManager);
        HealthSystem::ResetHealthBoxes(gameData.healthBoxManager);
        gameData.level = 1;
       

    }
    

} // namespace GameScreen