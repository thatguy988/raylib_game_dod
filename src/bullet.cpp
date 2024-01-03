#include <random>

#include "bullet.h"
#include "collision.h"
#include "raymath.h"

namespace BulletSystem {

    BulletManager::BulletManager() {
        machineGunLastShotTime = 0.0f;
        for (int i = 0; i < MAX_BULLETS; ++i) {
            bullets[i] = Bullet();
        }
    }

    void BulletManager::UpdateBullets(int maze[MAX][MAX], int n, int m, float blockSize) {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                bullets[i].position = Vector3Add(bullets[i].position, Vector3Scale(bullets[i].direction, bullets[i].speed));
                
                if (CollisionHandling::CheckBulletCollision(bullets[i].position, maze, n, m, blockSize, 0.2f)) {
                    bullets[i].active = false; // Deactivate the bullet
                }
               
            }
        }
    }

    void BulletManager::DrawBullets() {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                DrawSphere(bullets[i].position, 0.2f, RED);
            }
        }
    }

    void BulletManager::ShootBullet(const Vector3& position, const Vector3& direction, float speed) {
        int bulletIndex = FindInactiveBullet();
        if (bulletIndex != -1) {
            bullets[bulletIndex].active = true;
            bullets[bulletIndex].position = position;
            bullets[bulletIndex].direction = direction;
            bullets[bulletIndex].speed = speed;
            bullets[bulletIndex].playerbullet = true;
        }
    }
    
    
    void BulletManager::EnemyShootBullet(const Vector3& enemyPosition, const Vector3& shootingDirection, float bulletSpeed) {
        int bulletIndex = FindInactiveBullet();
        if (bulletIndex != -1) {
            bullets[bulletIndex].active = true;
            bullets[bulletIndex].position = enemyPosition;
            bullets[bulletIndex].direction = shootingDirection;
            bullets[bulletIndex].speed = bulletSpeed;
            bullets[bulletIndex].playerbullet = false;
        }
    }
    
    
    
    void BulletManager::Shoot(const Vector3& position, const Vector3& direction, float speed, BulletSystem::WeaponType currentWeapon) {
        switch (currentWeapon) {
            case WeaponType::PISTOL:
                // Shoot a single bullet
                ShootBullet(position, direction, speed);
                break;
            case WeaponType::SHOTGUN:{
            
                int shotgunPellets = 10; // Number of pellets for the shotgun
                float spreadAngle = 10.0f; // Spread angle in degrees

                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float> dis(-spreadAngle, spreadAngle);

                for (int i = 0; i < shotgunPellets; ++i) {
                    // Randomly alter the direction within the spread angle
                    float angle = dis(gen); // Random angle in degrees
                    float rad = angle * DEG2RAD; // Convert to radians

                    // Create a rotation matrix based on the random angle
                    Matrix rotation = MatrixRotate(Vector3{0, 1, 0}, rad); // Assuming spread in horizontal plane
                    Vector3 spreadDir = Vector3Transform(direction, rotation);

                    ShootBullet(position, spreadDir, speed);
                }
            }
                break;
            case WeaponType::MACHINE_GUN:
                ShootBullet(position, direction, speed);
                break;
        }
    }


    int BulletManager::FindInactiveBullet() {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (!bullets[i].active) {
                return i;
            }
        }
        return -1;
    }

}
