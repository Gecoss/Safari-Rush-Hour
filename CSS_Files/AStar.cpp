#include "AStar.h"

void AStar(GameState& game) {
    unordered_map<GameState, GameState, GameStateHash> parent;
    unordered_map<GameState, double, GameStateHash> gScore; // Almacena el costo g de cada estado
    unordered_set<GameState, GameStateHash> visited;
    priority_queue<pair<GameState, double>, vector<pair<GameState, double>>, CompareCost> open;

    double initialCost = 0.0;
    double initialF = initialCost + Heuristic(game);
    int totalNodes = 0;

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    open.push(make_pair(game, initialF));
    gScore[game] = initialCost;
    visited.insert(game);

    while (!open.empty()) {
        GameState current = open.top().first;
        open.pop();
        totalNodes++;
        
        if ((current.player == finalPlayerState)) {
            chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
            auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
            double timePerNode = static_cast<double>(totalTime) / totalNodes;

            // Construir y mostrar el camino más corto
            vector<GameState> path;
            while (current != game) {
                path.push_back(current);
                current = parent[current];
            }
            for(int i = path.size()-1; i >= 0 ; i--)
            {
                //PrintBoard(path[i]);
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
            // Verifica si el vecino del jugador es el estado objetivo
            // Verifica si el vecino del jugador ya ha sido visitado
            if (visited.find(neighbor) == visited.end()) {
                double g = gScore[current] + 1.0; // Peso del movimiento
                double f = g + Heuristic(neighbor);

                // Buscar en la cola prioritaria si el vecino ya está presente
                bool found = false;
                priority_queue<pair<GameState, double>, vector<pair<GameState, double>>, CompareCost> tempQueue;

                while (!open.empty()) {
                    GameState state = open.top().first;
                    double cost = open.top().second;
                    open.pop();

                    if (state == neighbor) {
                        found = true;
                        if (cost <= f) {
                            break; // Ya se encontró una solución mejor para este vecino
                        }
                    }

                    tempQueue.push(make_pair(state, cost));
                }

                if (!found) {
                    open.push(make_pair(neighbor, f));
                }

                // Volver a llenar la cola prioritaria con los elementos temporales
                while (!tempQueue.empty()) {
                    GameState state = tempQueue.top().first;
                    double cost = tempQueue.top().second;
                    tempQueue.pop();
                    open.push(make_pair(state, cost));
                }

                visited.insert(neighbor);
                parent[neighbor] = current;
                gScore[neighbor] = g;
            }
        }
    }
    cout << "No se encontró un camino al estado objetivo." << endl;
    return;
}