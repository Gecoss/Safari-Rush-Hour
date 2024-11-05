#ifndef ASTAR_H
#define ASTAR_H

#include "Rush_Hour.h"
#include "ReadAndPrint.h"
#include "Heuristics.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <climits>
#include <vector>

using namespace std;

void AStar(GameState& game);

#endif // ASTAR_H