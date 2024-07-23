#include <random>

#include "bullet.h"
#include "../../collision/collision.h"
#include "raymath.h"
#include "../enemy/enemy.h"

namespace BulletSystem {

    SparseSet::SparseSet(size_t maxCapacity) : capacity(maxCapacity), size(0){
        sparse.resize(maxCapacity, maxCapacity);
        dense.reserve(maxCapacity);
    }

    void SparseSet::Add(size_t index){
        if(size < capacity){
            dense.push_back(index);
            sparse[index] = size;
            size++;
        }
    }

    void SparseSet::Remove(size_t index){
        size_t denseIndex = sparse[index];
        size_t lastIndex = size - 1;

        if(denseIndex != lastIndex){
            dense[denseIndex] = dense[lastIndex];
            sparse[dense[denseIndex]] = denseIndex;
        }

        dense.pop_back();
        sparse[index] = capacity;
        size--;
    }

    bool SparseSet::Contains(size_t index) const{
        return sparse[index] < size;
    }
    
    void InitializeBulletData(BulletData& data, size_t maxBullets) {
        data.positions.resize(maxBullets);
        data.directions.resize(maxBullets);
        data.speeds.resize(maxBullets);
        data.activeStates.resize(maxBullets, false);
        data.playerBullets.resize(maxBullets);
        data.radii.resize(maxBullets, 0.1f);
        data.weaponTypes.resize(maxBullets, WeaponType::NONE);
        data.enemyTypes.resize(maxBullets, EnemySystem::EnemyType::NONE); // Initialize enemy types
    }

    void AddBullet(BulletData& data, SparseSet& set, const Vector3& position, const Vector3& direction, float speed, WeaponType weaponType, bool playerBullet, EnemySystem::EnemyType enemyType) {
        if (set.size < set.capacity) {
            size_t index = set.size;
            data.positions[index] = position;
            data.directions[index] = direction;
            data.speeds[index] = speed;
            data.activeStates[index] = true;
            data.playerBullets[index] = playerBullet;
            data.radii[index] = 0.1f; // Assuming a default radius, adjust as needed
            data.weaponTypes[index] = weaponType;
            data.enemyTypes[index] = enemyType; // Set the enemy type
            set.Add(index);
        }
    }

    void HandleInactiveBullets(BulletData& data, SparseSet& set) {
        for (size_t i = 0; i < set.size;) {
            size_t index = set.dense[i];
            if (!data.activeStates[index]) {
                set.Remove(index);
            } else {
                ++i;
            }
        }
    }

    void UpdateBullets(BulletData& data, SparseSet& set, const std::vector<BoundingBox>& wallBoundingBoxes, const BoundingBox& endpointBoundingBox) {
        for (size_t i = 0; i < set.size; ++i) {
            size_t index = set.dense[i];
            if (data.activeStates[index]) {
                data.positions[index] = Vector3Add(data.positions[index], Vector3Scale(data.directions[index], data.speeds[index]));
                if (CollisionHandling::CheckBulletCollision(data.positions[index], wallBoundingBoxes, endpointBoundingBox, data.radii[index])) {
                    data.activeStates[index] = false;
                }
            }
        }
        HandleInactiveBullets(data, set); // Remove inactive bullets after updating
    }

    void DrawBullets(const BulletData& data, const SparseSet& set) {
        for (size_t i = 0; i < set.size; ++i) {
            size_t index = set.dense[i];
            if (data.activeStates[index]) {
                DrawSphere(data.positions[index], data.radii[index], RED);
            }
        }
    }

    void ResetBulletData(BulletData& data, SparseSet& set) {
        data.positions.clear();
        data.directions.clear();
        data.speeds.clear();
        data.activeStates.clear();
        data.playerBullets.clear();
        data.radii.clear();
        data.weaponTypes.clear();
        data.enemyTypes.clear();
        set.sparse.clear();
        set.dense.clear();
        InitializeBulletData(data, set.capacity);
        set.size = 0;
    }

    int FindInactiveBullet(const BulletData& data, const SparseSet& set) {
        for (size_t i = 0; i < set.capacity; ++i) {
            if (!data.activeStates[i]) {
                return i;
            }
        }
        return -1;
    }

    void EnemyShootBullet(BulletData& data, SparseSet& set, const Vector3& enemyPosition, const Vector3& shootingDirection, float bulletSpeed, EnemySystem::EnemyType enemyType) {
        AddBullet(data, set, enemyPosition, shootingDirection, bulletSpeed, WeaponType::NONE, false, enemyType);
    }

    

    void Shoot(BulletData& data, SparseSet& set, WeaponManager& weaponManager, const Vector3& position, const Vector3& direction, float speed, WeaponType currentWeapon, bool playerBullet) {
        float currentTime = GetTime();
        switch (currentWeapon) {
            case WeaponType::PISTOL:
                if (currentTime - weaponManager.pistolGunLastShotTime >= weaponManager.pistolGunShotDelay && weaponManager.PistolAmmo > 0) {
                    AddBullet(data, set, position, direction, speed, currentWeapon, playerBullet, EnemySystem::EnemyType::NONE);
                    weaponManager.pistolGunLastShotTime = currentTime;
                    weaponManager.PistolAmmo--;
                }
                break;
            case WeaponType::SHOTGUN:
                if (currentTime - weaponManager.shotGunLastShotTime >= weaponManager.shotGunShotDelay && weaponManager.ShotgunAmmo > 0) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<float> dis(-weaponManager.spreadAngle, weaponManager.spreadAngle);

                    for (int i = 0; i < -weaponManager.shotgunPellets; ++i) {
                        // Randomly alter the direction within the spread angle
                        float angle = dis(gen); // Random angle in degrees
                        float rad = angle * DEG2RAD; // Convert to radians

                        // Create a rotation matrix based on the random angle
                        Matrix rotation = MatrixRotate(Vector3{0, 1, 0}, rad); // Assuming spread in horizontal plane
                        Vector3 spreadDir = Vector3Transform(direction, rotation);

                        AddBullet(data, set, position, direction, speed, currentWeapon, playerBullet, EnemySystem::EnemyType::NONE);

                       
                    }

                    weaponManager.shotGunLastShotTime = currentTime;
                    weaponManager.ShotgunAmmo--;
                }
                break;
            case WeaponType::MACHINE_GUN:
                if (currentTime - weaponManager.machineGunLastShotTime >= weaponManager.machineGunShotDelay && weaponManager.MachineGunAmmo > 0) {
                    AddBullet(data, set, position, direction, speed, currentWeapon, playerBullet, EnemySystem::EnemyType::NONE);
                    weaponManager.machineGunLastShotTime = currentTime;
                    weaponManager.MachineGunAmmo--;
                }
                break;
            default:
                break;
        }
    }
    
    void HandleWeaponInputAndShooting(BulletData& data, SparseSet& set, WeaponManager& weaponManager, const GameScreen::PlayerCamera& playerCamera, WeaponType& currentWeapon) {
        if (IsKeyPressed(KEY_ONE)) currentWeapon = WeaponType::PISTOL;
        if (IsKeyPressed(KEY_TWO)) currentWeapon = WeaponType::SHOTGUN;
        if (IsKeyPressed(KEY_THREE)) currentWeapon = WeaponType::MACHINE_GUN;

        Vector3 bulletDirection = Vector3Normalize(Vector3Subtract(playerCamera.camera.target, playerCamera.camera.position));

        float deltaTime = GetFrameTime(); // Get the time elapsed since the last frame

        // Update timers for all weapons
        weaponManager.machineGunLastShotTime += deltaTime;
        weaponManager.pistolGunLastShotTime += deltaTime;
        weaponManager.shotGunLastShotTime += deltaTime;

        // Handle machine gun shooting
        if (currentWeapon == WeaponType::MACHINE_GUN && IsKeyDown(KEY_SPACE)) {
            if (weaponManager.machineGunLastShotTime >= weaponManager.machineGunShotDelay) {
                Shoot(data, set, weaponManager, playerCamera.camera.position, bulletDirection, 1.0f, currentWeapon, true);
                weaponManager.machineGunLastShotTime = 0.0f; // Reset the timer
            }
        }

        // Handle pistol shooting
        if (currentWeapon == WeaponType::PISTOL && IsKeyPressed(KEY_SPACE)) {
            if (weaponManager.pistolGunLastShotTime >= weaponManager.pistolGunShotDelay) {
                Shoot(data, set, weaponManager, playerCamera.camera.position, bulletDirection, 0.1f, currentWeapon, true);
                weaponManager.pistolGunLastShotTime = 0.0f; // Reset the timer
            }
        }

        // Handle shotgun shooting
        if (currentWeapon == WeaponType::SHOTGUN && IsKeyPressed(KEY_SPACE)) {
            if (weaponManager.shotGunLastShotTime >= weaponManager.shotGunShotDelay) {
                Shoot(data, set, weaponManager, playerCamera.camera.position, bulletDirection, 1.0f, currentWeapon, true);
                weaponManager.shotGunLastShotTime = 0.0f; // Reset the timer
            }
        }
    }

}
