#include "HealthBox.h"
#include <random>

using namespace HealthSystem;

HealthBoxManager::HealthBoxManager() {
    // The constructor can be empty if you are dynamically adding health boxes
}


void HealthBoxManager::SetMaxHealthBoxes() {
        
        std::random_device rd;  // Seed with a real random value, if available
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(1, 5);  // Random number between 1 and 5
        MAX_HEALTH_BOXES = dis(gen);

        std::cout << "New MAX_HEALTH_BOXES set to: " << MAX_HEALTH_BOXES << std::endl;
    }


// Activate the health box at a specified position with a specified health amount
void HealthBox::Activate(Vector3 pos, int health) {
    position = pos;
    active = true;
    healthPoints = health;
    
    // Update the bounding box position when health box is activated
    Vector3 halfSize = Vector3Scale((Vector3){1.0f, 1.0f, 1.0f}, 0.5f);  
    body.min = Vector3Subtract(position, halfSize);
    body.max = Vector3Add(position, halfSize);
}

// Deactivate the health box
void HealthBox::Deactivate() {
    active = false;
    
    body.min = Vector3Zero();
    body.max = Vector3Zero();
}

// Draw the health box if it's active
void HealthBox::Draw() const {
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
            HealthBox newBox;
            newBox.Activate(availablePositions[randomIndex], RandomHealthValue());
            healthBoxes.push_back(newBox);
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
    for (const auto& healthbox : healthBoxes) {
        healthbox.Draw();
    }
}


void HealthBoxManager::Reset() {
    healthBoxes.clear(); // This will remove all health boxes
}

