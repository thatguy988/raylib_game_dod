#include "AmmoBox.h"
#include <random>

using namespace AmmoSystem;

// Constructor for AmmoBoxManager
AmmoBoxManager::AmmoBoxManager() {
    for (int i = 0; i < MAX_AMMO_BOXES; ++i) {
        ammoBoxes[i] = AmmoBox();
    }
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
void AmmoBox::Draw() {
    if (active) {
        DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN); // Adjust size and color as needed
    }
}

// Initialize ammo boxes randomly in the maze
void AmmoBoxManager::InitializeAmmoBoxes(std::vector<Vector3>& availablePositions) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

    const float ammoBoxPlacementProbability = 0.6f; // Adjust as needed


    for (int i = 0; i < MAX_AMMO_BOXES && !availablePositions.empty(); ++i) {
        std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
        int randomIndex = positionDist(gen);

        if (probabilityDist(gen) <= ammoBoxPlacementProbability) {
            ammoBoxes[i].Activate(availablePositions[randomIndex]);
            std::uniform_int_distribution<int> weaponTypeDist(0, 2); // 0 for Pistol, 1 for Shotgun, 2 for MachineGun
            ammoBoxes[i].ammoType = static_cast<BulletSystem::WeaponType>(weaponTypeDist(gen));
            availablePositions.erase(availablePositions.begin() + randomIndex);
        }
    }
}

// Draw all active ammo boxes
void AmmoBoxManager::DrawAmmoBoxes() {
    for (int i = 0; i < MAX_AMMO_BOXES; ++i) {
        ammoBoxes[i].Draw();
    }
}


        void AmmoBoxManager::Reset() {
            for (auto& ammoBox : ammoBoxes) {
                ammoBox.active = false;
                ammoBox.position = Vector3{0, 0, 0};
                ammoBox.ammoType = BulletSystem::WeaponType::NONE;
            }
        }
