#include "Heuristics.h"

double Heuristic( GameState& state) {

    // Obtener la posición objetivo del jugador
    int Row = 0;
    int Col = 0;
    bitboard goal = finalPlayerState;
    while (goal >>= 1) {
        Col++;
    }
    while (goal >>= 6) {
        Row++;
    }

    // Contar los obstáculos entre el jugador y la meta en la misma fila
    int obstaclesCount = 0;
    for (const auto& car : state.cars) {
        int carRow = 0;
        int carCol = 0;
        bitboard carBitboard = car;
        while (carBitboard >>= 1) {
            carCol++;
        }
        while (carBitboard >>= 6) {
            carRow++;
        }
        if (carRow == Row && carCol > Col) {
            obstaclesCount++;
        }
    }
    for (const auto& truck : state.trucks) {
        int truckRow = 0;
        int truckCol = 0;
        bitboard truckBitboard = truck;
        while (truckBitboard >>= 1) {
            truckCol++;
        }
        while (truckBitboard >>= 6) {
            truckRow++;
        }
        if (truckRow == Row && truckCol > Col) {
            obstaclesCount++;
        }
    }

    // Calcular el costo estimado restante considerando solo los obstáculos en la misma fila
    double estimatedCost = obstaclesCount;
    //cout << "Valor de la heuristica: " << estimatedCost << endl;
    return estimatedCost;
}