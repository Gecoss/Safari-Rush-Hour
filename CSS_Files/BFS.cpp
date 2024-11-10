#include "BFS.h"

void BFS(GameState& initial) {
    queue<GameState> open;
    unordered_set<GameState, GameStateHash> visited;
    unordered_map<GameState, GameState, GameStateHash> parent;

    int totalNodes = 0;

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    open.push(initial);
    visited.insert(initial);

    while (!open.empty()) {
        GameState current = open.front();
        open.pop();
        totalNodes++;
        
        if (current.player == finalPlayerState) {
            
            chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
            auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
            double timePerNode = static_cast<double>(totalTime) / totalNodes;

            // Construir y mostrar el camino más corto
            vector<GameState> path;
            while (current != initial) {
                path.push_back(current);
                current = parent[current];
            }
            for(int i = path.size()-1; i >= 0 ; i--)
            {
                PrintBoard(path[i]);
            }
            
            cout << WHITE << "     ¡Juego finalizado!" << endl;
            PrintStats(totalNodes, path.size(), static_cast<double>(totalTime), timePerNode);
            
            return;
        }
        
        vector<GameState> neighbors = GetNeighbors(current, current.player);
        for (auto i = 0u; i < current.cars.size(); i++) {
            vector<GameState> carNeighbors = GetNeighbors(current, current.cars[i]);
            neighbors.insert(neighbors.end(), carNeighbors.begin(), carNeighbors.end());
        }
        for (auto i = 0u; i < current.trucks.size(); i++) {
            vector<GameState> truckNeighbors = GetNeighbors(current, current.trucks[i]);
            neighbors.insert(neighbors.end(), truckNeighbors.begin(), truckNeighbors.end());
        }

        for (GameState neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                open.push(neighbor);
                visited.insert(neighbor);
                parent[neighbor] = current;
            }
        }
    }

    cout << "No se encontró un camino al estado objetivo." << endl;
    return;
}