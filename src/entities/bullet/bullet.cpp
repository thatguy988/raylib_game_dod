#include <random>

#include "bullet.h"
#include "../../collision/collision.h"
#include "raymath.h"
#include "../enemy/enemy.h"

namespace BulletSystem {

    BulletManager::BulletManager() {
        machineGunLastShotTime = 0.0f;
        for (int i = 0; i < MAX_BULLETS; ++i) {
            bullets[i] = Bullet();
        }
    }
    
    
    
    // Add the CheckBulletOutOfBounds function in BulletManager
    void BulletManager::CheckBulletOutOfBounds(const BoundingBox& boundary) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active && !CheckCollisionBoxSphere(boundary, bullets[i].position, bullets[i].radius)) {
                std::cout << "bullet went out of bounds" << std::endl;
                // Bullet is out of bounds, deactivate it
                bullets[i].active = false;
            }
        }
    }
    
    
    
    void BulletManager::UpdateBullets(const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox) {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                bullets[i].position = Vector3Add(bullets[i].position, Vector3Scale(bullets[i].direction, bullets[i].speed));
                if (CollisionHandling::CheckBulletCollision(bullets[i].position, wallBoundingBoxes, endpointBoundingBox, bullets[i].radius)) {
                    bullets[i].active = false; // Deactivate the bullet
                    //std::cout << bullets[i].active << std::endl;
                }
               
            }
        }
    }

    void BulletManager::DrawBullets() {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                DrawSphere(bullets[i].position, bullets[i].radius, RED);
            }
        }
    }
    
    // Correct the EnemyTypeToString function
    std::string EnemyTypeToString(EnemySystem::EnemyType type) {
        switch (type) {
            case EnemySystem::EnemyType::IMP:
                return "IMP";
            case EnemySystem::EnemyType::DEMON:
                return "DEMON";
            case EnemySystem::EnemyType::BARON_OF_HELL:
                return "BARON_OF_HELL";
            case EnemySystem::EnemyType::NONE:
                return "NONE";
            default:
                return "UNKNOWN";
        }
    }
    
    // Convert WeaponType enum to string
    std::string WeaponTypeToString(WeaponType type) {
        switch (type) {
            case WeaponType::PISTOL:
                return "PISTOL";
            case WeaponType::SHOTGUN:
                return "SHOTGUN";
            case WeaponType::MACHINE_GUN:
                return "MACHINE_GUN";
            case WeaponType::NONE:
                return "NONE";
            default:
                return "UNKNOWN";
        }
    }

    void BulletManager::ShootBullet(const Vector3& position, const Vector3& direction, float speed, BulletSystem::WeaponType currentWeapon) {
        int bulletIndex = FindInactiveBullet();
        if (bulletIndex != -1) {
            bullets[bulletIndex].active = true;
            bullets[bulletIndex].position = position;
            bullets[bulletIndex].direction = direction;
            bullets[bulletIndex].speed = speed;
            bullets[bulletIndex].playerbullet = true;
            bullets[bulletIndex].weaponType = currentWeapon; // Set weapon type for player
            bullets[bulletIndex].enemyType = EnemySystem::EnemyType::NONE; // Enemy type is NONE for player bullets
            
           // std::cout << "Weapon Type: " << WeaponTypeToString(currentWeapon) << std::endl;
            //std::cout << "Enemy Type: " << EnemyTypeToString(bullets[bulletIndex].enemyType) << std::endl;



        }
    }
    
    
    void BulletManager::EnemyShootBullet(const Vector3& enemyPosition, const Vector3& shootingDirection, float bulletSpeed, EnemySystem::EnemyType enemyType) {
        int bulletIndex = FindInactiveBullet();
        if (bulletIndex != -1) {
            bullets[bulletIndex].active = true;
            bullets[bulletIndex].position = enemyPosition;
            bullets[bulletIndex].direction = shootingDirection;
            bullets[bulletIndex].speed = bulletSpeed;
            bullets[bulletIndex].playerbullet = false;
            bullets[bulletIndex].enemyType = enemyType; // Enemy type is NONE for player bullets
            bullets[bulletIndex].weaponType = BulletSystem::WeaponType::NONE; // Set weapon type for player
            
            //std::cout << "Weapon Type: " << WeaponTypeToString(bullets[bulletIndex].weaponType) << std::endl;
            //std::cout << "Enemy Type: " << EnemyTypeToString(enemyType) << std::endl;



            

        }
    }
    
    
    
    void BulletManager::Shoot(const Vector3& position, const Vector3& direction, float speed, BulletSystem::WeaponType currentWeapon) {
        switch (currentWeapon) {
            case WeaponType::PISTOL:
                // Shoot a single bullet
                if (PistolAmmo != 0){
                    ShootBullet(position, direction, speed, currentWeapon);
                    PistolAmmo -= 1;
                }else{
                    //std::cout << "Out of pistol ammo" << std::endl;
                }
                break;
            case WeaponType::SHOTGUN:{
                if (ShotgunAmmo != 0){
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

                        ShootBullet(position, spreadDir, speed, currentWeapon);
                       
                    }
                    ShotgunAmmo -= 1;
                }else{
                   // std::cout << "Out of shotgun ammo" << std::endl;
                }
            }
                break;
            case WeaponType::MACHINE_GUN:
                if (MachineGunAmmo != 0){
                    ShootBullet(position, direction, speed, currentWeapon);
                    MachineGunAmmo -= 1;
                }else{
                   // std::cout << "Out of machine gun ammo" << std::endl;
                }
                break;
           default://for none
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
    
    

        void BulletManager::Reset() {
            for (auto& bullet : bullets) {
                bullet.active = false;
                bullet.position = Vector3{0, 0, 0};
                bullet.direction = Vector3{0, 0, 1};
                bullet.speed = 0.5f;
                bullet.playerbullet = false;
                
            }

}
}
