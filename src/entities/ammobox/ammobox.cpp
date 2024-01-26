#include "AmmoBox.h"
#include <random>

using namespace AmmoSystem;

// Constructor for AmmoBoxManager
AmmoBoxManager::AmmoBoxManager() {
    
}



void AmmoBoxManager::SetMaxAmmoBoxes() {
        
        std::random_device rd;  // Seed with a real random value, if available
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(1, 5);  // Random number between 1 and 5
        MAX_AMMO_BOXES = dis(gen);

        std::cout << "New MAX_AMMO_BOXES set to: " << MAX_AMMO_BOXES << std::endl;
    }


// Activate the ammo box at a specified position
void AmmoBox::Activate(Vector3 pos) {
    position = pos;
    active = true;
    
    // Update the bounding box position when ammo box is activated
    Vector3 halfSize = Vector3Scale((Vector3){1.0f, 1.0f, 1.0f}, 0.5f);  // Adjust size as needed
    body.min = Vector3Subtract(position, halfSize);
    body.max = Vector3Add(position, halfSize);
}

// Deactivate the ammo box
void AmmoBox::Deactivate() {
    active = false;
    
    // Reset the bounding box 
    body.min = Vector3Zero();
    body.max = Vector3Zero();
}

// Draw the ammo box if it's active
void AmmoBox::Draw() const {
    if (active) {
        DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN); // Adjust size and color as needed
    }
}

// Initialize ammo boxes randomly in the maze
void AmmoBoxManager::InitializeAmmoBoxes(std::vector<Vector3>& availablePositions) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

    const float ammoBoxPlacementProbability = 1.0f; // Adjust as needed


    for (int i = 0; i < MAX_AMMO_BOXES && !availablePositions.empty(); ++i) {
        std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
        int randomIndex = positionDist(gen);
        
        
        
        if (probabilityDist(gen) <= ammoBoxPlacementProbability) {
            AmmoBox newBox;
            std::uniform_int_distribution<int> weaponTypeDist(0, 2); // 0 for Pistol, 1 for Shotgun, 2 for MachineGun
            newBox.ammoType = static_cast<BulletSystem::WeaponType>(weaponTypeDist(gen));
            newBox.Activate(availablePositions[randomIndex]);
            ammoBoxes.push_back(newBox);
            availablePositions.erase(availablePositions.begin() + randomIndex);
        }

    }
}



void AmmoBoxManager::DrawAmmoBoxes() {
    for (const auto& ammobox : ammoBoxes) {
        ammobox.Draw();
    }
}



void AmmoBoxManager::Reset() {
     ammoBoxes.clear();       
}