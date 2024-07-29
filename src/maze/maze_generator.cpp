#include "maze_generator.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <random>
#include <queue>
#include <utility>



namespace MazeGenerator {

    std::pair<std::pair<int,int>, std::pair<int, int>> GenerateMaze(MazeData& mazeData, int pruningProbability) {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 rng(seed);

        // Initialize all cells as walls
        for (int i = 0; i < mazeData.n; ++i) {
            for (int j = 0; j < mazeData.m; ++j) {
                mazeData.maze[i][j] = 1;
            }
        }

        // Directions: up, down, left, right
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Choose a starting point not on the outer edges
        int start_x = rng() % (mazeData.n - 2) + 1;
        int start_y = rng() % (mazeData.m - 2) + 1;
        mazeData.maze[start_x][start_y] = 0;
        

        // Add walls of the starting cell to the wall list
        std::vector<std::pair<int, int>> walls;
        for (int i = 0; i < 4; ++i) {
            int nx = start_x + dx[i];
            int ny = start_y + dy[i];
            if (nx > 0 && nx < mazeData.n - 1 && ny > 0 && ny < mazeData.m - 1) {
                walls.push_back({nx, ny});
            }
        }

        // Generate the maze
        while (!walls.empty()) {
            int randomIndex = rng() % walls.size();
            int wx = walls[randomIndex].first;
            int wy = walls[randomIndex].second;
            walls.erase(walls.begin() + randomIndex);

            int visitedCells = 0;
            for (int i = 0; i < 4; ++i) {
                int nx = wx + dx[i];
                int ny = wy + dy[i];
                if (nx >= 0 && nx < mazeData.n && ny >= 0 && ny < mazeData.m && mazeData.maze[nx][ny] == 0) {
                    visitedCells++;
                }
            }

            if (visitedCells == 1) {
                mazeData.maze[wx][wy] = 0;
                for (int i = 0; i < 4; ++i) {
                    int nx = wx + dx[i];
                    int ny = wy + dy[i];
                    if (nx > 0 && nx < mazeData.n - 1 && ny > 0 && ny < mazeData.m - 1 && mazeData.maze[nx][ny] == 1) {
                        walls.push_back({nx, ny});
                    }
                }
            }
        }

        // Prune dead ends
        for (int x = 0; x < mazeData.n; ++x) {
            for (int y = 0; y < mazeData.m; ++y) {
                if (mazeData.maze[x][y] == 0) {
                    int wallCount = 0;
                    std::vector<std::pair<int, int>> adjWalls;
                    for (int i = 0; i < 4; ++i) {
                        int nx = x + dx[i];
                        int ny = y + dy[i];
                        if (nx > 0 && nx < mazeData.n - 1 && ny > 0 && ny < mazeData.m - 1 && mazeData.maze[nx][ny] == 1) {
                            wallCount++;
                            adjWalls.push_back({nx, ny});
                        }
                    }
                    if (wallCount == 3 && static_cast<int>(rng() % 100) < pruningProbability){

                        int index = rng() % adjWalls.size();
                        int wx = adjWalls[index].first;
                        int wy = adjWalls[index].second;
                        mazeData.maze[wx][wy] = 0; // Turn one of the walls into a path
                    }
                }
            }
        }
        
        // Choose an ending point on one of the outer edges
          int end_x, end_y, end_edge;
          bool isEndPointValid;
          do{
          end_edge = rng() % 4; // 0: top, 1: right, 2: bottom, 3: left
          isEndPointValid = false;
          switch (end_edge) {
          case 0:
            end_x = 0;
            end_y = rng() % mazeData.m;
            break;
          case 1:
            end_x = mazeData.n - 1;
            end_y = rng() % mazeData.m;
            break;
          case 2:
            end_x = rng() % mazeData.n;
            end_y = 0;
            break;
          case 3:
            end_x = rng() % mazeData.n;
            end_y = mazeData.m - 1;
            break;
          }
          //check if the end point is adjacent to an existing path in the maze
          for(int i = 0; i<4; ++i){
              int nx = end_x + dx[i];
              int ny = end_y + dy[i];
              if (nx>=0 && nx < mazeData.n && ny >= 0 && ny < mazeData.m && mazeData.maze[nx][ny] == 0){
                  isEndPointValid = true;
                  break;
              }
          }
          }while(!isEndPointValid);
        
        
        mazeData.maze[end_x][end_y] = 2; // end point of maze
        mazeData.maze[start_x][start_y] = 3;
            
        return std::make_pair(std::make_pair(start_x, start_y), std::make_pair(end_x, end_y));
    }




    std::pair<int, int> InitializeMaze(MazeData& mazeData) {
            mazeData.blockSize = 4.0f;
            mazeData.wallHeight = 3.0f;
            mazeData.floorThickness = 0.1f;
            auto startEndCoords = GenerateMaze(mazeData, 70);
            mazeData.endCoords = startEndCoords.second;
            
            // Clear and update openPositions vector
            mazeData.openPositions.clear();
            for (int i = 0; i < mazeData.n; ++i) {
                for (int j = 0; j < mazeData.m; ++j) {
                    if (mazeData.maze[i][j] == 0) {
                        mazeData.openPositions.push_back(Vector3{static_cast<float>(j) * mazeData.blockSize, 0.0f, static_cast<float>(i) * mazeData.blockSize});
                    }
                }
            }

            mazeData.mazeWidth = mazeData.m * mazeData.blockSize;
            mazeData.mazeHeight = mazeData.n * mazeData.blockSize;
            return startEndCoords.first;
        }
    

    void UpdateMazeSize(MazeData& mazeData, int level) {
        const int initialBaseSize = 5; // Initial base size
        const int minSize = 5; // Minimum size for 'n' and 'm'
        const int maxSize = 25; // Maximum size for 'n' and 'm'
        const double growthFactor = 1.1; // Growth factor for the maze size per level
        const int levelThreshold = 10; // Level threshold to increase base size
        const int changeRange = 3; // Maximum change (increase or decrease) per level

        static std::mt19937 rng(std::random_device{}()); // Random number generator
        std::uniform_int_distribution<int> sizeChangeDist(-changeRange, changeRange); // Distribution for size change

        int baseSize = initialBaseSize + ((level - 1) / levelThreshold);

        // Independent size calculation for 'n' and 'm'
        int calculatedSizeN = static_cast<int>(baseSize * pow(growthFactor, (level - 1) % levelThreshold));
        int calculatedSizeM = static_cast<int>(baseSize * pow(growthFactor, (level - 1) % levelThreshold));

        // Random size change
        int sizeChangeN = sizeChangeDist(rng);
        int sizeChangeM = sizeChangeDist(rng);

        mazeData.n = std::clamp(calculatedSizeN + sizeChangeN, minSize, maxSize);
        mazeData.m = std::clamp(calculatedSizeM + sizeChangeM, minSize, maxSize);

        std::cout << "New maze size: " << mazeData.n << "x" << mazeData.m << std::endl;
        std::cout << "Level " << level << std::endl;
    }


    void DrawMaze(const MazeData& mazeData) {
        for (int i = 0; i < mazeData.n; ++i) {
            for (int j = 0; j < mazeData.m; ++j) {
                if (mazeData.maze[i][j] == 1) { 
                    // Draw walls
                    DrawCube(Vector3 { (float)j * mazeData.blockSize, mazeData.wallHeight / 2, (float)i * mazeData.blockSize }, 
                             mazeData.blockSize, mazeData.wallHeight, mazeData.blockSize, DARKGRAY);
                } else if(mazeData.maze[i][j] == 0 || mazeData.maze[i][j] == 3) {
                    // Draw floor
                    DrawCube(Vector3 { (float)j * mazeData.blockSize, -mazeData.floorThickness / 2, (float)i * mazeData.blockSize }, 
                             mazeData.blockSize, mazeData.floorThickness, mazeData.blockSize, LIGHTGRAY);
                } else if(mazeData.maze[i][j] == 2){
                    //Draw endpoint
                    DrawCube(Vector3 { (float)j * mazeData.blockSize, mazeData.wallHeight / 2, (float)i * mazeData.blockSize }, 
                             mazeData.blockSize, mazeData.wallHeight, mazeData.blockSize, GREEN);
                    // Draw floor
                    DrawCube(Vector3 { (float)j * mazeData.blockSize, -mazeData.floorThickness / 2, (float)i * mazeData.blockSize }, 
                             mazeData.blockSize, mazeData.floorThickness, mazeData.blockSize, GREEN);
                }
            }
        }
    }

    void InitializeOutOfBoundsBox(MazeData& mazeData, float buffer) {
        Vector3 center = {mazeData.mazeWidth / 2.0f, 0.0f, mazeData.mazeHeight / 2.0f};
        Vector3 size = {mazeData.mazeWidth + buffer, mazeData.wallHeight, mazeData.mazeHeight + buffer}; // Buffer on each side
        mazeData.outOfBoundsBox = {
            Vector3Subtract(center, Vector3Scale(size, 0.5f)), 
            Vector3Add(center, Vector3Scale(size, 0.5f))
        };
    }

    void GenerateWallBoundingBoxes(MazeData& mazeData) {
        mazeData.wallBoundingBoxes.clear();
        for (int i = 0; i < mazeData.n; ++i) {
            for (int j = 0; j < mazeData.m; ++j) {
                if (mazeData.maze[i][j] == 1 || mazeData.maze[i][j] == 2) {
                    Vector3 wallPosition = {(float)j * mazeData.blockSize, 
                                             mazeData.wallHeight / 2, 
                                             (float)i * mazeData.blockSize};
                    Vector3 halfSize = {mazeData.blockSize / 2, 
                                        mazeData.wallHeight / 2, 
                                        mazeData.blockSize / 2};
                    BoundingBox box = {Vector3Subtract(wallPosition, halfSize), 
                                       Vector3Add(wallPosition, halfSize)};

                    // Distinguish between regular walls and the endpoint
                    if (mazeData.maze[i][j] == 1) {
                        mazeData.wallBoundingBoxes.push_back(box);
                    } else if (mazeData.maze[i][j] == 2) {
                        mazeData.endpointBoundingBox = box;
                    }
                }
            }
        }
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> ConvertPredefinedLevelToMaze(const std::vector<std::vector<int>>& predefinedLevel, MazeData& mazeData) {
        mazeData.n = predefinedLevel.size();    // Number of rows
        mazeData.m = predefinedLevel[0].size(); // Number of columns

        for (int i = 0; i < mazeData.n; ++i) {
            for (int j = 0; j < mazeData.m; ++j) {
                mazeData.maze[i][j] = predefinedLevel[i][j];
                if (mazeData.maze[i][j] == 3) { // Assuming 3 represents the start point
                    mazeData.startCoords = {i, j};
                }
                if (mazeData.maze[i][j] == 2) { // Assuming 2 represents the end point
                    mazeData.endCoords = {i, j};
                }
            }
        }
        return {mazeData.startCoords, mazeData.endCoords};
    }

    
} // namespace MazeGenerator
