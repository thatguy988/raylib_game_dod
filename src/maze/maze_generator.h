#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <iostream>
#include <utility>
#include <vector>
#include "raylib.h"
#include "raymath.h"



#include "../common.h"
#include "maze_cells.h"



namespace MazeGenerator {

    struct Point {
        int x, y;
        Point(int _x, int _y) : x(_x), y(_y) {}
    };

    struct MazeData
        {
            int n;//number of rows
            int m;//number of columns
            MazeCells maze[MAX][MAX];
            float blockSize;
            std::vector<Vector3> openPositions; // Vector to store open positions
            std::vector<Vector3> openPositionsForItems;
            std::pair<int, int> endCoords;
            std::pair<int, int> startCoords;
            float wallHeight;
            float floorThickness;
            std::vector<BoundingBox> wallBoundingBoxes;
            BoundingBox endpointBoundingBox;
            BoundingBox outOfBoundsBox;

            float mazeWidth;
            float mazeHeight;
        };

    std::pair<std::pair<int, int>, std::pair<int, int>> GenerateMaze(MazeData& mazeData, int pruningProbability);
    std::pair<int, int> InitializeMaze(MazeData& mazeData);
    void UpdateMazeSize(MazeData& mazeData, int level);
    void DrawMaze(const MazeData& mazeData);

    void InitializeOutOfBoundsBox(MazeData& mazeData, float buffer);
    void GenerateWallBoundingBoxes(MazeData& mazeData);
    std::pair<std::pair<int, int>, std::pair<int, int>> ConvertPredefinedLevelToMaze(const std::vector<std::vector<MazeCells>>& predefinedLevel, MazeData& mazeData);





} // namespace MazeGenerator

#endif // MAZE_GENERATOR_H
