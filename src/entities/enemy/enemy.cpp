#include "Enemy.h"
#include "raymath.h"
#include <vector>


#include <queue>
#include <unordered_set>
#include <unordered_map>



namespace EnemySystem {
    
    
    // Default constructor for Enemy
    Enemy::Enemy() {
        position = {0, 0, 0}; // Default position
        targetPosition = {0, 0, 0}; // Default target position
        rayPlayerPosition = {0, 0, 0}; // Default raycast target position
        movementSpeed = 0.1f; // Default movement speed
        active = false; // Initially not active
        state = EnemyState::IDLE; // Initial state
        path.clear(); // Clear any predefined path
        lastKnownPlayerPos = {0, 0, 0}; // Reset last known player position
        shootingTimer = 0.0f; // Reset shooting timer
        bulletSpeed = 0.1f; // Default bullet speed
        shootingInterval = 1.0f; // Default shooting interval
        shootingHeightOffset = 1.0f; // Default shooting height offset
        type = EnemyType::NONE; // Default enemy type
        enemyHealth = 0; // Default health
        facingDirection = {0, 0, 1}; // Default facing direction

        // Initialize the bounding box
        Vector3 enemySize = {1.0f, 2.5f, 1.0f}; // Default size
        Vector3 halfSize = Vector3Scale(enemySize, 0.5f);
        body.min = Vector3Subtract(position, halfSize);
        body.max = Vector3Add(position, halfSize);

        // Initialize the ray and ray hit information
        ray.position = position; // Set ray origin to enemy's position
        ray.direction = {0, 0, 0}; // Default direction
        rayHitInfo.hit = false;
        rayHitInfo.distance = 0;
        rayHitInfo.point = {0, 0, 0};
        rayHitInfo.normal = {0, 0, 0};
    }
    
    
    // Helper function to convert 2D grid position to a linear index
    int Enemy::PosToIndex(int x, int y, int m) {
        return y * m + x;
    }

    // BFS function to find the shortest path
    std::vector<Vector3> Enemy::BFS(const Vector3& start, const Vector3& end, const int maze[MAX][MAX], int n, int m, float blockSize) {
        std::queue<Vector3> queue;
        std::unordered_map<int, Vector3> cameFrom; // Map of positions and their predecessors
        std::vector<Vector3> path;

        // Convert start and end positions to grid indices
        int startX = static_cast<int>(start.x / blockSize + roundingOffset);
        int startY = static_cast<int>(start.z / blockSize + roundingOffset);
        int endX = static_cast<int>(end.x / blockSize + roundingOffset);
        int endY = static_cast<int>(end.z / blockSize + roundingOffset);

        queue.push(Vector3{static_cast<float>(startX), 0, static_cast<float>(startY)});
        cameFrom[PosToIndex(startX, startY, m)] = Vector3{-1, -1, -1}; // Use -1 to indicate the start

        while (!queue.empty()) {
            Vector3 current = queue.front();
            queue.pop();

            int x = static_cast<int>(current.x);
            int y = static_cast<int>(current.z);

            if (x == endX && y == endY) { // Reached the end
                // Backtrack to get the path
                while (!(current.x == -1 && current.z == -1)) {
                    path.push_back(Vector3{current.x * blockSize, 0, current.z * blockSize});
                    current = cameFrom[PosToIndex(static_cast<int>(current.x), static_cast<int>(current.z), m)];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            // Check adjacent cells (up, down, left, right)
            std::vector<Vector3> neighbors = {
                {current.x, 0, current.z - 1}, // Up
                {current.x, 0, current.z + 1}, // Down
                {current.x - 1, 0, current.z}, // Left
                {current.x + 1, 0, current.z}  // Right
            };

            for (auto& neighbor : neighbors) {
                int nx = static_cast<int>(neighbor.x);
                int ny = static_cast<int>(neighbor.z);
                if (nx >= 0 && nx < m && ny >= 0 && ny < n && maze[ny][nx] == 0 && cameFrom.find(PosToIndex(nx, ny, m)) == cameFrom.end()) {
                    queue.push(neighbor);
                    cameFrom[PosToIndex(nx, ny, m)] = current;
                }
            }
        }

        return path; // If no path found, return empty vector
    }
    
    
    // Function to get a random open position from the provided vector of open positions
    Vector3 Enemy::GetRandomOpenPosition(const std::vector<Vector3>& openPositions) {
        if (openPositions.empty()) {
            return Vector3Zero(); // Return a default position if there are no open positions
        }

        std::random_device rd;  // Initialize random number generator
        std::mt19937 gen(rd()); // Seed the generator

        // Randomly select an index from the vector
        std::uniform_int_distribution<> dis(0, openPositions.size() - 1);
        int randomIndex = dis(gen);

        // Return the randomly selected open position
        return openPositions[randomIndex];
    }
    
    
    void Enemy::Draw() const {
        //default render
        DrawCube(position, 1.0f, 2.5f, 1.0f, GRAY);
    }
    
}
