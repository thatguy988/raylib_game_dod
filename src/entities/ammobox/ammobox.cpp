#include "AmmoBox.h"

namespace AmmoSystem{




void SetMaxAmmoBoxes(AmmoBoxData& data) {
        
        std::random_device rd;  // Seed with a real random value, if available
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(1, 5);  // Random number between 1 and 5
        data.MAX_AMMO_BOXES = dis(gen);

        std::cout << "New MAX_AMMO_BOXES set to: " << data.MAX_AMMO_BOXES << std::endl;
    }




// Initialize ammo boxes randomly in the maze
void InitializeAmmoBoxes(AmmoBoxData& data, std::vector<Vector3>& availablePositions) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

    const float ammoBoxPlacementProbability = 1.0f; // Adjust as needed


    for (int i = 0; i < data.MAX_AMMO_BOXES && !availablePositions.empty(); ++i) {
        std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
        int randomIndex = positionDist(gen);
        
        
        
        if (probabilityDist(gen) <= ammoBoxPlacementProbability) {
            Vector3 pos = availablePositions[randomIndex];
            Vector3 halfSize = Vector3Scale((Vector3){1.0f, 1.0f, 1.0f}, 0.5f);
            BoundingBox box = {Vector3Subtract(pos, halfSize), Vector3Add(pos, halfSize)};
            std::uniform_int_distribution<int> weaponTypeDist(0,2);
            BulletSystem::WeaponType ammoType = static_cast<BulletSystem::WeaponType>(weaponTypeDist(gen));
            data.positions.push_back(pos);
            data.bodies.push_back(box);
            data.activeStates.push_back(true);
            data.ammoTypes.push_back(ammoType);

            availablePositions.erase(availablePositions.begin() + randomIndex);
        }

    }
}



void DrawAmmoBoxes(const AmmoBoxData& data) {
    for (size_t i = 0; i < data.positions.size(); ++i){
        if (data.activeStates[i]){
            DrawCube(data.positions[i], 1.0f, 1.0f, 1.0f, BROWN);
        }
    }
}



void ResetAmmoBoxes(AmmoBoxData& data) {
     data.positions.clear();
     data.bodies.clear();
     data.activeStates.clear();
     data.ammoTypes.clear();      
}
}// namespace AmmoSystem