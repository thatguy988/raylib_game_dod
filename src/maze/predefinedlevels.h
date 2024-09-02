#ifndef PREDEFINEDLEVELS_H
#define PREDEFINEDLEVELS_H

#include <vector>
#include "maze_cells.h"

namespace PredefinedLevels {

    const std::vector<std::vector<MazeCells>>& GetLevel(int levelNumber);

} // namespace PredefinedLevels

#endif // PREDEFINEDLEVELS_H
