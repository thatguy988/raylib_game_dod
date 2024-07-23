#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include "raylib.h"
#include "raymath.h"
#include "../bullet/bullet.h"
#include <vector>
#include <random>


namespace AmmoSystem {

    struct AmmoBoxData {
        std::vector<Vector3> positions;
        std::vector<BoundingBox> bodies;
        std::vector<bool> activeStates;
        std::vector<BulletSystem::WeaponType> ammoTypes;
        int MAX_AMMO_BOXES;

    };

    void SetMaxAmmoBoxes(AmmoBoxData& data);
    void InitializeAmmoBoxes(AmmoBoxData& data, std::vector<Vector3>& openPositions);
    void DrawAmmoBoxes(const AmmoBoxData& data);
    void ResetAmmoBoxes(AmmoBoxData& data);

} // namespace AmmoSystem

#endif // AMMO_BOX_H
