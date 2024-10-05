#include "../lib/setup.h"

const std::vector<std::pair<int, int>> setup::moves{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
const unsigned int setup::max_time = 1000;
const unsigned int setup::maxTasks = 4;
const unsigned int setup::maxCapacity = 10;
const unsigned int setup::maxStopTime = 0;