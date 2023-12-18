#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <utility>
#include <vector>

const int MAX = 100;

namespace MazeGenerator {

    struct Point {
        int x, y;
        Point(int _x, int _y) : x(_x), y(_y) {}
    };

    std::pair<std::pair<int, int>, std::pair<int, int>> GenerateMaze(int n, int m, int pruningProbability, int maze[MAX][MAX]);
    int FindShortestPath(int startX, int startY, int endX, int endY, int n, int m, int maze[MAX][MAX], int dist[MAX][MAX]);
    


} // namespace MazeGenerator

#endif // MAZE_GENERATOR_H
