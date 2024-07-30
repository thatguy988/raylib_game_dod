#ifndef GAME_H
#define GAME_H

#include <memory>
#include <utility>
#include <vector>

#include <iostream>
#include <random>
#include "raylib.h"

#include "../common.h"

#include "../maze/predefinedlevels.h"
#include "../maze/maze_generator.h"
#include "../collision/collision.h"
#include "../camera/player_camera.h"
#include "../entities/bullet/bullet.h"
#include "../entities/enemy/enemy.h"
#include "../entities/ammobox/ammobox.h"
#include "../entities/healthbox/healthbox.h"


#define LIGHTRED        (Color){ 230, 41, 55, 128 }     


namespace GameScreen {



    struct GameData{
        BulletSystem::WeaponType currentWeapon = BulletSystem::WeaponType::PISTOL;
        
        bool isRedFlashActive = false;
        float redFlashDuration = 0.2f; // Duration of the flash in seconds
        float redFlashTimer = 0.0f;    // Timer to track the flash duration
        
        int playerHealth = 100; //health of player
        int maxPlayerHealth = 100; //max health of player
        int level = 1;
        bool canExitLevel = true;
        PlayerCamera playerCamera{0.1f, 2.0f, 1.0f};
       

        size_t MAX_BULLETS = 200;
        BulletSystem::BulletData bulletManager;
        BulletSystem::SparseSet sparseSet{MAX_BULLETS};

        BulletSystem::WeaponManager weaponsManager;
        EnemySystem::EnemyData enemyManager;
        AmmoSystem::AmmoBoxData ammoBoxManager;
        HealthSystem::HealthBoxData healthBoxManager;

        MazeGenerator::MazeData MazeData;

        Message pickupMessage;

    };
    
    
    void InitGame(GameData& gameData);
    void UpdateGame(GameData& gameData);
    void DrawGame(const GameData& gameData);
    void UnloadGame(GameData& gameData);


}

#endif // GAME_H
