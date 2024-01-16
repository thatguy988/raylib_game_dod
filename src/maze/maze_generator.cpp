#include "maze_generator.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <random>
#include <queue>
#include <utility>



namespace MazeGenerator {
    

    bool isValid(int x, int y, int n, int m, int maze[MAX][MAX]) {
        return (x >= 0 && x < n && y >= 0 && y < m && maze[x][y] == 0);
    }

    std::pair<std::pair<int,int>, std::pair<int, int>> GenerateMaze(int n, int m, int pruningProbability, int maze[MAX][MAX]) {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 rng(seed);

        // Initialize all cells as walls
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                maze[i][j] = 1;
            }
        }

        // Directions: up, down, left, right
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Choose a starting point not on the outer edges
        int start_x = rng() % (n - 2) + 1;
        int start_y = rng() % (m - 2) + 1;
        maze[start_x][start_y] = 0;
        

        // Add walls of the starting cell to the wall list
        std::vector<std::pair<int, int>> walls;
        for (int i = 0; i < 4; ++i) {
            int nx = start_x + dx[i];
            int ny = start_y + dy[i];
            if (nx > 0 && nx < n - 1 && ny > 0 && ny < m - 1) {
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
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && maze[nx][ny] == 0) {
                    visitedCells++;
                }
            }

            if (visitedCells == 1) {
                maze[wx][wy] = 0;
                for (int i = 0; i < 4; ++i) {
                    int nx = wx + dx[i];
                    int ny = wy + dy[i];
                    if (nx > 0 && nx < n - 1 && ny > 0 && ny < m - 1 && maze[nx][ny] == 1) {
                        walls.push_back({nx, ny});
                    }
                }
            }
        }

        // Prune dead ends
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < m; ++y) {
                if (maze[x][y] == 0) {
                    int wallCount = 0;
                    std::vector<std::pair<int, int>> adjWalls;
                    for (int i = 0; i < 4; ++i) {
                        int nx = x + dx[i];
                        int ny = y + dy[i];
                        if (nx > 0 && nx < n - 1 && ny > 0 && ny < m - 1 && maze[nx][ny] == 1) {
                            wallCount++;
                            adjWalls.push_back({nx, ny});
                        }
                    }
                    if (wallCount == 3 && static_cast<int>(rng() % 100) < pruningProbability){

                        int index = rng() % adjWalls.size();
                        int wx = adjWalls[index].first;
                        int wy = adjWalls[index].second;
                        maze[wx][wy] = 0; // Turn one of the walls into a path
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
            end_y = rng() % m;
            break;
          case 1:
            end_x = n - 1;
            end_y = rng() % m;
            break;
          case 2:
            end_x = rng() % n;
            end_y = 0;
            break;
          case 3:
            end_x = rng() % n;
            end_y = m - 1;
            break;
          }
          //check if the end point is adjacent to an existing path in the maze
          for(int i = 0; i<4; ++i){
              int nx = end_x + dx[i];
              int ny = end_y + dy[i];
              if (nx>=0 && nx < n && ny >= 0 && ny < m && maze[nx][ny] == 0){
                  isEndPointValid = true;
                  break;
              }
          }
          }while(!isEndPointValid);
        
        
        maze[end_x][end_y] = 2; // end point of maze
        maze[start_x][start_y] = 3;
            
        return std::make_pair(std::make_pair(start_x, start_y), std::make_pair(end_x, end_y));
    }
    
    int FindShortestPath(int startX, int startY, int endX, int endY, int n, int m, int maze[MAX][MAX], int dist[MAX][MAX]) {
        std::queue<Point> q;
        q.push(Point(startX, startY));
        dist[startX][startY] = 0;

        int dx[] = {-1, 1, 0, 0}; // Directions: up, down, left, right
        int dy[] = {0, 0, -1, 1};

        while (!q.empty()) {
            Point p = q.front();
            q.pop();

            for (int i = 0; i < 4; ++i) {
                int newX = p.x + dx[i];
                int newY = p.y + dy[i];

                if (isValid(newX, newY, n, m, maze) && dist[newX][newY] > dist[p.x][p.y] + 1) {
                    dist[newX][newY] = dist[p.x][p.y] + 1;
                    q.push(Point(newX, newY));
                }
            }
        }

        return dist[endX][endY];
    }
    
    
    
    

} // namespace MazeGenerator
