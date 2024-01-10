#include "HealthBox.h"
#include <random>

using namespace HealthSystem;

// Constructor for HealthBoxManager
HealthBoxManager::HealthBoxManager() {
    for (int i = 0; i < MAX_HEALTH_BOXES; ++i) {
        healthBoxes[i] = HealthBox();
    }
}

// Activate the health box at a specified position with a specified health amount
void HealthBoxManager::HealthBox::Activate(Vector3 pos, int health) {
    position = pos;
    active = true;
    healthPoints = health;
}

// Deactivate the health box
void HealthBoxManager::HealthBox::Deactivate() {
    active = false;
}

// Draw the health box if it's active
void HealthBoxManager::HealthBox::Draw() {
    if (active) {
        DrawCube(position, 1.0f, 1.0f, 1.0f, PINK); // Adjust size and color as needed
    }
}

// Initialize health boxes randomly in the maze
void HealthBoxManager::InitializeHealthBoxes(std::vector<Vector3>& availablePositions) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> probabilityDist(0.0f, 1.0f);

    const float healthBoxPlacementProbability = 0.6f; // Adjust as needed


    for (int i = 0; i < MAX_HEALTH_BOXES && !availablePositions.empty(); ++i) {
        std::uniform_int_distribution<> positionDist(0, availablePositions.size() - 1);
        int randomIndex = positionDist(gen);

        if (probabilityDist(gen) <= healthBoxPlacementProbability) {
            healthBoxes[i].Activate(availablePositions[randomIndex], RandomHealthValue());
            availablePositions.erase(availablePositions.begin() + randomIndex);
        }
    }
}

// Randomly choose the health value for a health box (10, 25, or 50)
int HealthBoxManager::RandomHealthValue() {
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
void HealthBoxManager::DrawHealthBoxes() {
    for (int i = 0; i < MAX_HEALTH_BOXES; ++i) {
        healthBoxes[i].Draw();
    }
}

void HealthBoxManager::Reset() {
            for (auto& healthBox : healthBoxes) {
                healthBox.active = false;
                healthBox.position = Vector3{0, 0, 0};
                healthBox.healthPoints = 0;
            }
        }
