#ifndef HEALTH_BOX_H
#define HEALTH_BOX_H

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>

namespace HealthSystem {

    // Define the HealthBox struct outside of the HealthBoxManager class
    struct HealthBox {
        Vector3 position;
        BoundingBox body; // Bounding box representing the health box's body
        bool active;
        int healthPoints;  // Amount of health provided by the box

        HealthBox() : position({0, 0, 0}), active(false), healthPoints(0) {
            // Initialize the bounding box
            Vector3 healthBoxSize = {1.0f, 1.0f, 1.0f}; // Example size, adjust as needed
            Vector3 halfSize = Vector3Scale(healthBoxSize, 0.5f);
            body.min = Vector3Subtract(position, halfSize);
            body.max = Vector3Add(position, halfSize);
        }

        void Activate(Vector3 pos, int health);
        void Deactivate();
        void Draw() const;
    };

    class HealthBoxManager {
    public:
        std::vector<HealthBox> healthBoxes;
        void SetMaxHealthBoxes();

        HealthBoxManager();
        void InitializeHealthBoxes(std::vector<Vector3>& openPositions);
        void DrawHealthBoxes();
        int RandomHealthValue();
        void Reset();
    private:
        int MAX_HEALTH_BOXES; 

    };

} // namespace HealthSystem

#endif // HEALTH_BOX_H
