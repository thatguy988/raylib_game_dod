#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include "raylib.h"
#include "bullet.h"
#include <vector>

namespace AmmoSystem {

    const int MAX_AMMO_BOXES = 5; // Adjust how many boxes we want at one time

    class AmmoBoxManager {
    public:
        struct AmmoBox {
            Vector3 position;
            bool active;
            BulletSystem::WeaponType ammoType;  // Type of ammo in the box


            AmmoBox() : position({0, 0, 0}), active(false), ammoType(BulletSystem::WeaponType::NONE) {}

            void Activate(Vector3 pos);
            void Deactivate();
            void Draw();
        };

        AmmoBox ammoBoxes[MAX_AMMO_BOXES];

        AmmoBoxManager();
        void InitializeAmmoBoxes(std::vector<Vector3>& openPositions);
        void DrawAmmoBoxes();
        void Reset();
    };

} // namespace AmmoSystem

#endif // AMMO_BOX_H
