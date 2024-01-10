#ifndef BULLET_H
#define BULLET_H

#include <iostream>

#include "raylib.h"
#include "common.h"

// Forward declaration of EnemySystem namespace and Enemy struct
namespace EnemySystem {
    struct Enemy;
    enum class EnemyType; // Forward declaration of the EnemyType enum

}


namespace BulletSystem {
    
    
  
    
    enum class WeaponType {
        PISTOL,
        SHOTGUN,
        MACHINE_GUN,
        NONE
    };

    struct Bullet {
        Vector3 position;
        Vector3 direction;
        float speed;
        bool active;
        bool playerbullet;
        
        EnemySystem::EnemyType enemyType; // Type of enemy that shot the bullet

        WeaponType weaponType;

        


        Bullet() : position({0, 0, 0}), direction({0, 0, 1}), speed(0.5f), active(false), playerbullet(false) {}
    };

    class BulletManager {
    public:
        static const int MAX_BULLETS = 100;
        Bullet bullets[MAX_BULLETS];
        
        float machineGunLastShotTime;
        const float machineGunShotDelay = 0.25f; // Delay in seconds between machine gun shots
        float shotGunLastShotTime;
        const float shotGunShotDelay = 1.0f;
        float pistolGunLastShotTime;
        const float pistolGunShotDelay = 0.5f;
        
        //current number of bullets player is holding
        int ShotgunAmmo = 10;
        int PistolAmmo = 100;
        int MachineGunAmmo = 50;
        
        
        
        //max number of bullets player can hold
        int ShotgunAmmoCapacity = 50; 
        int PistolAmmoCapacity = 100;
        int MachineGunAmmoCapacity = 200;

        
        BulletManager();
        void UpdateBullets(int maze[MAX][MAX], int n, int m, float blockSize);
        void DrawBullets();
        void Shoot(const Vector3& position, const Vector3& direction, float speed, WeaponType currentWeapon);
        void EnemyShootBullet(const Vector3& enemyPosition, const Vector3& shootingDirection, float bulletSpeed, EnemySystem::EnemyType enemyType);
        void Reset();

    private:
        int FindInactiveBullet();
        void ShootBullet(const Vector3& position, const Vector3& direction, float speed, WeaponType currentWeapon);


    };

}

#endif // BULLET_H
