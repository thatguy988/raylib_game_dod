#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include "common.h"


namespace BulletSystem {
    
    enum class WeaponType {
        PISTOL,
        SHOTGUN,
        MACHINE_GUN
    };

    struct Bullet {
        Vector3 position;
        Vector3 direction;
        float speed;
        bool active;

        Bullet() : position({0, 0, 0}), direction({0, 0, 1}), speed(0.5f), active(false) {}
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

        
        BulletManager();
        void UpdateBullets(int maze[MAX][MAX], int n, int m, float blockSize);
        void DrawBullets();
        void Shoot(const Vector3& position, const Vector3& direction, float speed, WeaponType currentWeapon);
    private:
        int FindInactiveBullet();
        void ShootBullet(const Vector3& position, const Vector3& direction, float speed);

    };

}

#endif // BULLET_H
