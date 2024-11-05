#ifndef READ_AND_PRINT_H
#define READ_AND_PRINT_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "Rush_Hour.h"
#include "Colors.h"

using namespace std;

void LoadLevelsInfo(GameState& gameState, const string& archivo, int level);
void ReadAndPrintFile(const string& filename);
void PrintBoard(GameState& gs);
void simplePrint(uint64_t board);
void clearScreen();
void PrintStats(int totalNodes, size_t pathSize, double totalTime, double timePerNode);

#endif // READ_AND_PRINT_H