#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include <vector>
#include "raylib.h"

#include "../../camera/player_camera.h"
#include "../../common.h"


namespace GameScreen {
    struct PlayerCamera; // Forward declaration
}

namespace EnemySystem {
    enum class EnemyType; // Forward declaration
}


namespace BulletSystem {
    
    enum class WeaponType {
        PISTOL,
        SHOTGUN,
        MACHINE_GUN,
        NONE
    };

    struct BulletData {
        std::vector<Vector3> positions;
        std::vector<Vector3> directions;
        std::vector<float> speeds;
        std::vector<bool> activeStates;
        std::vector<bool> playerBullets;
        std::vector<float> radii;
        std::vector<WeaponType> weaponTypes;
        std::vector<EnemySystem::EnemyType> enemyTypes;
    };

    struct WeaponManager {
        float machineGunLastShotTime;
        const float machineGunShotDelay = 0.25f;
        float shotGunLastShotTime;
        const float shotGunShotDelay = 1.0f;
        float pistolGunLastShotTime;
        const float pistolGunShotDelay = 0.5f;
        
        int ShotgunAmmo = 10;
        int PistolAmmo = 100;
        int MachineGunAmmo = 50;
        
        const int ShotgunAmmoCapacity = 50; 
        const int PistolAmmoCapacity = 100;
        const int MachineGunAmmoCapacity = 200;

        int shotgunPellets = 10; // Number of pellets for the shotgun
        float spreadAngle = 10.0f; // Spread angle in degrees
    };

    class SparseSet{
    public:
        std::vector<size_t> sparse;
        std::vector<size_t> dense;
        size_t capacity;
        size_t size;

        SparseSet(size_t maxCapacity);
        void Add(size_t index);
        void Remove(size_t index);
        bool Contains(size_t index) const;
    };

    void InitializeBulletData(BulletData& data, size_t maxBullets);
    void AddBullet(BulletData& data, SparseSet& set, const Vector3& position, const Vector3& direction, float speed, WeaponType weaponType, bool playerBullet, EnemySystem::EnemyType enemyType);
    void UpdateBullets(BulletData& data, SparseSet& set, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox);
    void DrawBullets(const BulletData& data, const SparseSet& set);
    void ResetBulletData(BulletData& data, SparseSet& set);
    void HandleInactiveBullets(BulletData& data, SparseSet& set);
    int FindInactiveBullet(const BulletData& data, const SparseSet& set);
    void Shoot(BulletData& data, SparseSet& set, WeaponManager& weaponManager, const Vector3& position, const Vector3& direction, float speed, WeaponType currentWeapon, bool playerBullet);
    void EnemyShootBullet(BulletData& data, SparseSet& set, const Vector3& enemyPosition, const Vector3& shootingDirection, float bulletSpeed, EnemySystem::EnemyType enemyType);
    void HandleWeaponInputAndShooting(BulletData& data, SparseSet& set, WeaponManager& weaponManager, const GameScreen::PlayerCamera& playerCamera, WeaponType& currentWeapon);


}

#endif // BULLET_H
