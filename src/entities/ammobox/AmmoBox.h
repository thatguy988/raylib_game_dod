#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include "raylib.h"
#include "raymath.h"
#include "../bullet/bullet.h"
#include <vector>

namespace AmmoSystem {

    struct AmmoBox {
        Vector3 position;
        BoundingBox body; // Bounding box representing the ammo box's body
        bool active;
        BulletSystem::WeaponType ammoType;  // Type of ammo in the box

        AmmoBox() : position({0, 0, 0}), active(false), ammoType(BulletSystem::WeaponType::NONE) {
            // Initialize the bounding box
            Vector3 ammoBoxSize = {1.0f, 1.0f, 1.0f};
            Vector3 halfSize = Vector3Scale(ammoBoxSize, 0.5f);
            body.min = Vector3Subtract(position, halfSize);
            body.max = Vector3Add(position, halfSize);
        }

        void Activate(Vector3 pos);
        void Deactivate();
        void Draw() const;
    };

    class AmmoBoxManager {
    public:
    
        std::vector<AmmoBox> ammoBoxes;
        void SetMaxAmmoBoxes();

        AmmoBoxManager();
        void InitializeAmmoBoxes(std::vector<Vector3>& openPositions);
        void DrawAmmoBoxes();
        void Reset();
    private:
        int MAX_AMMO_BOXES;

    };

} // namespace AmmoSystem

#endif // AMMO_BOX_H
