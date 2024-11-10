#include "ReadAndPrint.h"

void LoadLevelsInfo(GameState& gameState, const string& archivo, int level) {
    
    ifstream archivoEntrada(archivo);
    string linea;

    if(archivoEntrada.is_open()) {
        int ActualLine = 1;
        int nivel = 0;

        while (getline(archivoEntrada, linea)) {
            istringstream iss(linea);
            string token;
            if(ActualLine%5 == 1) {
                // Obtén el nivel
                getline(iss, token, ':');
                nivel = stoi(token);
            }

            // Verifica si el nivel coincide con el valor deseado
            if (nivel == level) {
                // Obtén la información para el miembro player
                if(ActualLine%5 == 2) {
                    getline(iss, token);
                    gameState.player = stoull(token, nullptr, 16);
                }

                // Obtén la información para el miembro cars
                if(ActualLine%5 == 3) {
                    getline(iss, token);
                    istringstream carsIss(token);
                    bitboard car;
                    gameState.cars.clear();
                    while (carsIss >> hex >> car) gameState.cars.push_back(car);
                }

                // Obtén la información para el miembro trucks
                if(ActualLine%5 == 4) {
                    getline(iss, token);
                    istringstream trucksIss(token);
                    bitboard truck;
                    gameState.trucks.clear();
                    while (trucksIss >> hex >> truck) gameState.trucks.push_back(truck);
                }
                ActualLine++;
            }
        }
        archivoEntrada.close();
    } else cout << "No se pudo abrir el archivo." << endl; 
}

void ReadAndPrintFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "No se pudo abrir el archivo: " << filename << endl;
    }
}

void PrintBoard(GameState& gs) {
    bitboard one = 0x8000000000000000;
    stringstream board;

    board << LIGHT_BROWN <<"   ╔════════════════════╗" << endl;

    for (int i = 0; i < 36; i++) {

        if(i % 6 == 0) board <<"   "<< LIGHT_BROWN << "║ ";
       
        if(one&gs.player) board << RED << "███";                                                    //Auto jugador color rojo
        for(auto car : gs.cars)                                                                     //Auto Azul de dos casillas
            if(one&car) {
                int pos = find(gs.cars.begin(), gs.cars.end(), car) - gs.cars.begin() + 1;
                board << BLUE_BACKGROUND << BLUE << "█"  << WHITE << pos << BLUE << "█" << RESET; 
            }                                               
        for(auto truck : gs.trucks)                                                                 //Camion Verde de tres casillas
            if(one&truck) { 
                int pos = find(gs.trucks.begin(), gs.trucks.end(), truck) - gs.trucks.begin() + 1;
                board << GREEN_BACKGROUND << GREEN << "█"  << WHITE << pos << GREEN << "█" << RESET; 
            }                         
        if(one&~(gs.player | GetCarsAsBitboard(gs) | GetTrucksAsBitboard(gs))) board << GREY <<"░░░"; //Casilla vacia
        
        if(i % 6 == 5) board << LIGHT_BROWN << " ║" << endl;

        one >>= 1;
    }

    board << LIGHT_BROWN <<"   ╚════════════════════╝" << endl;
    cout << board.str();
}

void clearScreen() {
    if (std::system("clear") != 0) 
        std::cerr << "Error al limpiar la pantalla." << std::endl;
        // Manejar el error de limpieza de pantalla si es necesario
}

void simplePrint(uint64_t board){
    uint64_t one=0x8000000000000000;
    for(int i=0;i<36;i++){
        if(one&board){
            std::cout<<" o ";
        }else{
            std::cout<<" - ";
        }
        if(i%6==5){
            std::cout<<std::endl;
        }
        one >>= 1;
    }
    std::cout<<std::endl;
}

void PrintStats(int totalNodes, size_t pathSize, double totalTime, double timePerNode){

    cout << WHITE << "        Estadísticas" << endl;
    cout << "   Nodos recorridos: " << totalNodes << endl;
    cout << "   Movimientos: " << pathSize << endl;
    cout << "    Tiempo total: " << totalTime << " ms" << endl;
    cout << " Tiempo/nodo: " << timePerNode << " ms" << endl;
}