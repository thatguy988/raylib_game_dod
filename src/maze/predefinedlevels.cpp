#include "predefinedlevels.h"

namespace PredefinedLevels {

    const std::vector<std::vector<MazeCells>> level10 = {
        {MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::STARTING_POINT, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::FLOOR, MazeCells::WALL},
        {MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::ENDING_POINT, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL, MazeCells::WALL}
    };
    

    const std::vector<std::vector<MazeCells>>& GetLevel(int levelNumber) {
        static const std::vector<std::vector<MazeCells>> emptyLevel; // Static empty level

        switch (levelNumber) {
            case 10:
                return level10;
            // Add more cases for other predefined levels
            default:
                return emptyLevel;
        }
    }

} // namespace PredefinedLevels
