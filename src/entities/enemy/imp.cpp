#include "imp.h"

namespace EnemySystem {

    void CalculateImpPathToRandomTarget(size_t index, EnemyData& data, MazeGenerator::MazeData& mazeData) {
        if (data.paths[index].empty()) {
            Vector3 randomTarget = GetRandomOpenPosition(mazeData.openPositions);
            data.paths[index] = BFS(data.positions[index], randomTarget, mazeData);
        }
    }
    
    
    
    void CalculateImpPathToPlayer(size_t index, EnemyData& data, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData) {
            data.lastKnownPlayerPos[index] = playerPosition;
            data.paths[index] = BFS(data.positions[index], playerPosition, mazeData);
            if (!data.paths[index].empty()) {
                data.paths[index].pop_back(); // Remove the last step (player's position) from the path
            }
    }
    
    void MoveImpAlongPath(size_t index, EnemyData& data) {
        if (!data.paths[index].empty()) {
            Vector3 nextStep = data.paths[index].front();
            if (Vector3Distance(data.positions[index], nextStep) <= data.movementSpeeds[index]) {
                data.positions[index] = nextStep;
                data.paths[index].erase(data.paths[index].begin()); // Remove the reached point
            } else {
                Vector3 direction = Vector3Normalize(Vector3Subtract(nextStep, data.positions[index]));
                data.positions[index] = Vector3Add(data.positions[index], Vector3Scale(direction, data.movementSpeeds[index]));
                data.facingDirections[index] = direction;
            }
            // Update the enemy's bounding box
            Vector3 halfSize = Vector3Scale((Vector3){1.0f, 2.5f, 1.0f}, 0.5f);  // Adjust size as needed
            data.bodies[index].min = Vector3Subtract(data.positions[index], halfSize);
            data.bodies[index].max = Vector3Add(data.positions[index], halfSize);
        }
    }
    
    void HandleImpAttackState(size_t index, EnemyData& data, const Vector3& playerPosition, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set) {
        if (data.shootingTimers[index] >= data.shootingIntervals[index]) {
            Vector3 shootingPosition = data.positions[index];
            shootingPosition.y += data.shootingHeightOffsets[index]; // Adjust bullet shooting height
            Vector3 shootingDirection = Vector3Normalize(Vector3Subtract(playerPosition, shootingPosition));
            BulletSystem::EnemyShootBullet(bulletManager, set, shootingPosition, shootingDirection, data.bulletSpeeds[index], data.types[index]);
            data.shootingTimers[index] = 0.0f; // Reset the timer
        } else {
            data.shootingTimers[index] += GetFrameTime(); // Update the timer
        }
    }
    
    
    void UpdateImpEnemyState(size_t index, EnemyData& data, const Vector3& playerPosition, MazeGenerator::MazeData& mazeData, bool rayHitsPlayer, bool playerEnemyCollision, BulletSystem::BulletData& bulletManager, BulletSystem::SparseSet& set) {

        switch (data.states[index]) {
            case EnemyState::IDLE:
                //std::cout<<"IN idle"<<std::endl;
                CalculateImpPathToRandomTarget(index, data, mazeData);
                if (rayHitsPlayer) {
                    data.paths[index].clear();
                    CalculateImpPathToPlayer(index, data, playerPosition, mazeData);
                    data.states[index] = EnemyState::ATTACKING;
                }
                break;

            case EnemyState::ATTACKING:
                //std::cout<<"IN attack"<<std::endl;
                if (!rayHitsPlayer) {
                    data.paths[index].clear();
                    CalculateImpPathToPlayer(index, data, data.lastKnownPlayerPos[index], mazeData);
                    data.states[index] = EnemyState::CHASING; 
                } else if (data.paths[index].empty()) {
                    CalculateImpPathToPlayer(index, data, playerPosition, mazeData);
                }
                HandleImpAttackState(index, data, playerPosition, bulletManager, set);

                break;

            case EnemyState::CHASING:
                    //std::cout<<"IN chasing" <<std::endl; 
                    if (data.paths[index].empty()) {
                        data.states[index] = EnemyState::IDLE; // Transition back to IDLE state
                    }
                break;
        }
        
    }
    
    
    void DrawImp(size_t index, const EnemyData& data){
        DrawCube(data.positions[index], 1.0f, 2.5f, 1.0f, BLUE);
    }

}
