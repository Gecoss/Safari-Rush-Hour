#include "Rush_Hour.h"

bitboard GetCarsAsBitboard(GameState& gs) { 

    bitboard aux = 0x0;
    for(auto car : gs.cars) aux |= car;
    return aux;
}

bitboard GetTrucksAsBitboard(GameState& gs) { 

    bitboard aux = 0x0;
    for(auto truck : gs.trucks) aux |= truck;
    return aux;
}

//Detecta la orientacion vertical u horizontal de una
string DetectOrientation(bitboard& board){

    bitboard aux = board & -board;

    if(((aux >> 1) & board) || ((aux << 1) & board)) { return "Horizontal"; }
    else if(((aux >> 6) & board) || ((aux << 6) & board)) { return "Vertical"; }

    return "Error al detectar orientación";
}

//Obtiene las direcciones a las que se puede mover el bitboard/ficha del tablero
vector<bitboard> GetDirections(GameState& gs, bitboard board) {
    string orientation = DetectOrientation(board);
    bitboard vecinoDerecha = 0x0, vecinoIzquierda = 0x0, vecinoArriba = 0x0, vecinoAbajo = 0x0;

    if (orientation == "Horizontal") {
        vecinoDerecha = (board & ~lastCol) >> 1;
        vecinoIzquierda = (board & ~firstCol) << 1;
    } else if (orientation == "Vertical") {
        vecinoArriba = (board & ~firstRow) << 6;
        vecinoAbajo = (board & ~lastRow) >> 6;
    }

    bitboard blockedCells = GetCarsAsBitboard(gs) | GetTrucksAsBitboard(gs) | gs.obstacles | gs.player;
    bitboard directions = (vecinoDerecha | vecinoIzquierda | vecinoArriba | vecinoAbajo) & ~(blockedCells);
    vector<bitboard> bb_neighbors;

    if (directions != 0x0) {
        
        bb_neighbors.push_back(directions & -directions);
        if((directions & ~(bb_neighbors[0] & -bb_neighbors[0])) != 0x0)
            bb_neighbors.push_back(directions & ~(bb_neighbors[0] & -bb_neighbors[0]));
    } else {
        bb_neighbors.push_back(0x0);
    }

    return bb_neighbors;
}

//Expande las direcciones de la fichae para saber los vecinos de toda la fila/columna
vector<vector<bitboard>> ExpandDirections(GameState& gs, bitboard board) {
    
    bitboard blockedCells = GetCarsAsBitboard(gs) | GetTrucksAsBitboard(gs) | gs.obstacles | gs.player;
    vector<bitboard> auxVector = GetDirections(gs, board);
    vector<bitboard> bb_neighbors1;
    vector<bitboard> bb_neighbors2;
    vector<vector<bitboard>> bb;
    
    bb_neighbors1.push_back(auxVector[0]);
    bb.push_back(bb_neighbors1); 
    
    if(auxVector.size() == 2){
        bb_neighbors2.push_back(auxVector[1]);
        bb.push_back(bb_neighbors2);
    }

    for(auto j = 0u; j < bb.size(); j++){
        if(bb[j][0] != 0x0){
            bitboard aux_neighbor, borde = 0x0;
            int shift = 0;

            if ((board << 1) & bb[j][0])     {shift = 1; borde = firstCol;}   //Si el vecino esta hacia la izquerda
            else if((board >> 1) & bb[j][0]) {shift = 1; borde = lastCol;}   //Si el vecino esta hacia la derecha
            else if((board << 6) & bb[j][0]) {shift = 6; borde = firstRow;}   //Si el vecino esta hacia la arriba
            else if((board >> 6) & bb[j][0]) {shift = 6; borde = lastRow;}   //Si el vecino esta hacia la abajo

            //Expandir hacia la direccion
            for(auto i = 0u; i < bb[j].size(); i++){

                if(borde == firstCol || borde == firstRow)
                    aux_neighbor = bb[j][i] << shift;
                else if(borde == lastCol || borde == lastRow)
                    aux_neighbor = bb[j][i] >> shift;
            
                //Si la expancion no se encuentra sobre una casilla bloqueada o el vecino actual no esta en el borde del tablero 
                if(((aux_neighbor & ~(blockedCells)) != 0x0) && !(bb[j][i] & borde)) {
                    bb[j].push_back(aux_neighbor);
                }
                else 
                    break;
            }          
        }
    }
    
    return bb;
}

//Transforma los vecinos de bitboards a un GameState 
vector<GameState> GetNeighbors(GameState& gs, bitboard board) {
    string ficha;
    GameState gs_copy = gs;
    vector<GameState> neighbors;
    vector<vector<bitboard>> bb_neighbors_vector = ExpandDirections(gs, board);

    for(auto bb_neighbors : bb_neighbors_vector){
        for(auto i = 0u;  i < bb_neighbors.size(); i++){
            if(bb_neighbors[i] != 0x0){
                bitboard aux = board;
                if ((aux << (6*(i+1))) & bb_neighbors[i])       aux <<= (6*(i+1));
                else if ((aux >> (6*(i+1))) & bb_neighbors[i])  aux >>= (6*(i+1));
                else if ((aux << (1*(i+1))) & bb_neighbors[i])  aux <<= (1*(i+1));
                else if ((aux >> (1*(i+1))) & bb_neighbors[i])  aux >>= (1*(i+1));

                // Actualizar player si coincide con board
                if (gs_copy.player == board)
                    gs_copy.player = aux;
                
                
                // Actualizar cars si coincide con board
                auto carIt = find(gs_copy.cars.begin(), gs_copy.cars.end(), board);
                if (carIt != gs_copy.cars.end()) 
                    *carIt = aux;
                

                // Actualizar truck si coincide con board
                auto truckIt = find(gs_copy.trucks.begin(), gs_copy.trucks.end(), board);
                if (truckIt != gs_copy.trucks.end()) 
                    *truckIt = aux;
    
                neighbors.push_back(gs_copy); 
                gs_copy = gs;          
                
            }
        }
    }
    
    return neighbors;
}

vector<bitboard> FindMoveableTokens(GameState& gs){

    vector<bitboard> movableBitboards;
    int indice = 0;

    cout << endl;
    if(!GetNeighbors(gs, gs.player).empty()){
        indice++;
        cout << CYAN << "   " << indice << ".- " << RED << "██████" << RESET << endl << endl;
        movableBitboards.push_back(gs.player);
    }

    for(auto car : gs.cars){
        if(!GetNeighbors(gs, car).empty()){
            indice++;
            int pos = find(gs.cars.begin(), gs.cars.end(), car) - gs.cars.begin() + 1;
            cout << CYAN << "   " << indice << ".- " << BLUE_BACKGROUND << BLUE << "███" << WHITE << pos << BLUE << "███" << RESET << "  ";
            movableBitboards.push_back(car);
        }
    }
    
    cout << endl << endl;

    for(auto truck : gs.trucks){
        if(!GetNeighbors(gs, truck).empty()){
            indice++;
            int pos = find(gs.trucks.begin(), gs.trucks.end(), truck) - gs.trucks.begin() + 1;
            cout << CYAN << "   " << indice << ".- " << GREEN_BACKGROUND << GREEN << "████" << WHITE << pos << GREEN << "████" << RESET << "  ";
            movableBitboards.push_back(truck);
        }
    }

    cout << endl << endl;

    return movableBitboards;
}

GameState SelectTokenToMove(GameState& gs, const vector<bitboard>& tokens) {

    int selectedTokenIndex;
    cout << "   Ingrese el" << CYAN << " índice" << RESET << " de la ficha a mover ["<< CYAN << "1 - " << tokens.size() << RESET << "]: ";
    cin >> selectedTokenIndex;
    selectedTokenIndex--;

    while(selectedTokenIndex < 0 || selectedTokenIndex > (static_cast<int>(tokens.size()) - 1) || cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "   [Error, ingrese una opcion valida ["<< CYAN << "1 - " << tokens.size() << RESET << "]]: ";
        cin >> selectedTokenIndex;
        selectedTokenIndex--;
    }

    bitboard selectedToken = tokens[selectedTokenIndex];
    GameState gsSelectedToken;
    vector<GameState> neighbors = GetNeighbors(gs, selectedToken);
    cout << endl << "   Movimientos disponibles: " << endl << endl;
    auto directions = ExpandDirections(gs, selectedToken);
    int moveIndex = 1;
    stringstream ss;

    for(auto direction : directions){
        for(auto i = 0u;  i < direction.size(); i++){
            if(direction[i] != 0x0){
                bitboard aux = selectedToken;
                ss << CYAN << "   " << moveIndex << ".- " << RESET;

                if ((aux << (6*(i+1))) & direction[i])       ss <<"Arriba    ";
                else if ((aux >> (6*(i+1))) & direction[i])  ss << "Abajo    ";
                else if ((aux << (1*(i+1))) & direction[i])  ss <<"Izquierda ";
                else if ((aux >> (1*(i+1))) & direction[i])  ss <<"Derecha   "; 

                ss << "[ " << (i+1) << "Posicion/es ]" << endl;
            }
            moveIndex++;
        }
    }
    cout << ss.str();

    cout << endl << "   Ingrese el " << CYAN << "índice " << RESET << "del movimiento a seleccionar: ";
    int selectedMovement;
    cin >> selectedMovement;

    while(selectedMovement < 1 || selectedMovement > (moveIndex-1) || cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "   [Error, ingrese una opcion valida]: ";
        cin >> selectedMovement;
    }

    return neighbors[selectedMovement - 1];
     
    return gs;
}