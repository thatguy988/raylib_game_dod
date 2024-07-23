#ifndef HEALTH_BOX_H
#define HEALTH_BOX_H

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <random>
#include <iostream>

namespace HealthSystem {

    // Define the HealthBox struct outside of the HealthBoxManager class
    struct HealthBoxData {
        std::vector<Vector3> positions;
        std::vector<BoundingBox> bodies;
        std::vector<bool> activeStates;
        std::vector<int> healthPoints;
        int MAX_HEALTH_BOXES;  
    };

    //function to modify healthboxes
    void SetMaxHealthBoxes(HealthBoxData& data);
    void InitializeHealthBoxes(HealthBoxData& data, std::vector<Vector3>& openPositions);
    void DrawHealthBoxes(const HealthBoxData& data);
    int RandomHealthValue();
    void ResetHealthBoxes(HealthBoxData& data);

} // namespace HealthSystem

#endif // HEALTH_BOX_H
