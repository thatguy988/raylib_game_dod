#ifndef HEALTH_BOX_H
#define HEALTH_BOX_H

#include "raylib.h"
#include <vector>

namespace HealthSystem {

    const int MAX_HEALTH_BOXES = 5; // Adjust how many boxes we want at one time

    class HealthBoxManager {
    public:
        struct HealthBox {
            Vector3 position;
            bool active;
            int healthPoints;  // Amount of health provided by the box

            HealthBox() : position({0, 0, 0}), active(false), healthPoints(0) {}

            void Activate(Vector3 pos, int health);
            void Deactivate();
            void Draw();
        };

        HealthBox healthBoxes[MAX_HEALTH_BOXES];

        HealthBoxManager();
        void InitializeHealthBoxes(std::vector<Vector3>& openPositions);
        void DrawHealthBoxes();
        int RandomHealthValue();
        void Reset();
    };

} // namespace HealthSystem

#endif // HEALTH_BOX_H
