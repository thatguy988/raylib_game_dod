#include "HealthBox.h"

namespace HealthSystem{




void SetMaxHealthBoxes(HealthBoxData& data) {
        
        std::random_device rd;  // Seed with a real random value, if available
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(1, 5);  // Random number between 1 and 5
        data.MAX_HEALTH_BOXES = dis(gen);

        std::cout << "New MAX_HEALTH_BOXES set to: " << data.MAX_HEALTH_BOXES << std::endl;
    }


// Initialize health boxes randomly in the maze
void InitializeHealthBoxes(HealthBoxData& data, std::vector<Vector3>& availablePositions) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

    const float healthBoxPlacementProbability = 0.6f; // Adjust as needed


    for (int i = 0; i < data.MAX_HEALTH_BOXES && !availablePositions.empty(); ++i) {
        std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
        int randomIndex = positionDist(gen);
        
        
        if (probabilityDist(gen) <= healthBoxPlacementProbability) {
            Vector3 pos = availablePositions[randomIndex];
            int health = HealthSystem::RandomHealthValue();
            Vector3 halfsize = Vector3Scale((Vector3){1.0f,1.0f,1.0f}, 0.5f);
            BoundingBox box ={Vector3Subtract(pos,halfsize), Vector3Add(pos,halfsize)};
            data.positions.push_back(pos);
            data.bodies.push_back(box);
            data.activeStates.push_back(true);
            data.healthPoints.push_back(health);

            
            availablePositions.erase(availablePositions.begin() + randomIndex);
        }

      
    }
}



// Randomly choose the health value for a health box (10, 25, or 50)
int RandomHealthValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> healthDist(0, 2); // 0 for 10HP, 1 for 25HP, 2 for 50HP

    switch (healthDist(gen)) {
        case 0:
            return 10;
        case 1:
            return 25;
        case 2:
            return 50;
        default:
            return 10; // Default value
    }
}



    
    
// Draw all active health boxes
void DrawHealthBoxes(const HealthBoxData& data) {
    for (size_t i = 0; i<data.positions.size(); ++i){
        if (data.activeStates[i]){
            DrawCube(data.positions[i], 1.0f, 1.0f, 1.0f, PINK);
        }
    }
    
}


void ResetHealthBoxes(HealthBoxData& data) {
    data.positions.clear();
    data.bodies.clear();
    data.activeStates.clear();
    data.healthPoints.clear();
}

} //namespace HealthSystem