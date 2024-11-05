#ifndef RUSH_HOUR_H
#define RUSH_HOUR_H

#include "Colors.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

typedef uint64_t bitboard;

//Bordes tablero
static const bitboard firstRow = 0xfc00000000000000, lastRow = 0x00000003f0000000;
static const bitboard firstCol = 0x8208208200000000, lastCol = 0x0410410410000000;
static const bitboard finalPlayerState = 0x0000c00000000000;

struct GameState {   
    //Tablero
    bitboard player = 0x0;
    bitboard obstacles = 0x0;
    vector<bitboard> cars ={};
    vector<bitboard> trucks ={};

    GameState() = default;

    GameState(const GameState& other) {
        // Copiar los miembros de datos de other a *this
        player = other.player;
        obstacles = other.obstacles;
        cars = other.cars;
        trucks = other.trucks;
        // ...
    }

    GameState& operator=(const GameState& other) {
        // Comparar todos los miembros para determinar la igualdad
        
            // Copiar los miembros de datos de other a *this
            player = other.player;
            obstacles = other.obstacles;
            cars = other.cars;
            trucks = other.trucks;
            // ...
        
        return *this;
    }

    bool operator==(const GameState& other) const {
        // Comparar todos los miembros para determinar la igualdad
        return player == other.player &&
               obstacles == other.obstacles &&
               cars == other.cars &&
               trucks == other.trucks;
    }

    bool operator!=(const GameState& other) const {
        return !(*this == other);
    }

};

struct CompareCost{
    bool operator()(const pair<GameState, double>& a, const pair<GameState, double>& b) {
        return a.second > b.second;
    }
};

bitboard GetCarsAsBitboard(GameState& gs);
bitboard GetTrucksAsBitboard(GameState& gs);
string DetectOrientation(bitboard& board);
vector<bitboard> GetDirections(GameState& gs, bitboard board);
vector<vector<bitboard>> ExpandDirections(GameState& gs, bitboard board);
vector<GameState> GetNeighbors(GameState& gs, bitboard board);
vector<bitboard> FindMoveableTokens(GameState& gs);
GameState SelectTokenToMove(GameState& gs, const vector<bitboard>& tokens);

struct GameStateHash {
    std::size_t operator()(const GameState& gsHash) const {
        // Calcula un valor hash basado en los miembros relevantes de GameState
        std::size_t hash = 0;
        // Hash del miembro player
        hash_combine(hash, std::hash<bitboard>{}(gsHash.player));
        // Hash del miembro obstacles
        hash_combine(hash, std::hash<bitboard>{}(gsHash.obstacles));
        // Hash de los vectores cars y trucks
        for (const auto& car : gsHash.cars)
            hash_combine(hash, std::hash<bitboard>{}(car));
        for (const auto& truck : gsHash.trucks)
            hash_combine(hash, std::hash<bitboard>{}(truck));

        return hash;
    }

private:
    // Función auxiliar para combinar hashes
    template <typename T>
    void hash_combine(std::size_t& seed, const T& value) const {
        seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

#endif // RUSH_HOUR_H