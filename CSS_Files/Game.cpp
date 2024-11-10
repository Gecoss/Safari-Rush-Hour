#include "Rush_Hour.h"
#include "ReadAndPrint.h"
#include "BFS.h"
#include "AStar.h"

int main() {
    int level;
    GameState GS;
    srand(time(0));
    
    clearScreen();
    ReadAndPrintFile("Interfaces/Title.txt");
    ReadAndPrintFile("Interfaces/Options.txt");
    cout << R"(
                                                 [Ingrese una opcion]: )";
    int mode;
    cin >> mode;

    while((mode < 1) || (mode > 2) || cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        clearScreen();
        ReadAndPrintFile("Interfaces/Title.txt");
        ReadAndPrintFile("Interfaces/Options.txt");
        cout << R"(
                                         [Error, ingrese una opcion valida]: )";
        cin >> mode;
    }
    bool endGame = false;
    switch(mode){
        //Modo de juego manual
        case 1:
            //Seleccion de nivel
            clearScreen();
            ReadAndPrintFile("Interfaces/Title.txt");
            ReadAndPrintFile("Interfaces/LevelsList.txt");
            cout << R"(
                                         [Ingrese un nivel]: )";
            cin >> level;

            while((level < 1) || (level > 6) || cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clearScreen();
                ReadAndPrintFile("Interfaces/Title.txt");
                ReadAndPrintFile("Interfaces/LevelsList.txt");
                cout << R"(
                                     [Error, ingrese una opcion valida]: )";
                cin >> level;
            }

            LoadLevelsInfo(GS,"Data/Levels.txt", level);
            //Juego
            while(!endGame){
                clearScreen();
                PrintBoard(GS);
                GS = SelectTokenToMove(GS, FindMoveableTokens(GS));
                //Condicion de termino
                if((GS.player & lastCol) != 0) { endGame = true; }
            }

            //Nivel Finalizado
            clearScreen();
            PrintBoard(GS);
            cout << "   Nivel Finalizado" << endl;
            
            break;
        case 2:
            for(int i = 0; i < 6; i++) {
                level = i + 1;
                cout << endl << WHITE << "─────────┤Nivel N" << level << "├─────────" << endl;
                LoadLevelsInfo(GS,"Data/Levels.txt", level);
                PrintBoard(GS);
                cout<<"---------------------------------"<<endl;
                cout<<"BFS..."<<endl;
                BFS(GS);
                cout<<"---------------------------------"<<endl;
                cout<<"Astar..."<<endl;
                AStar(GS);
                cout<<"---------------------------------"<<endl;
            }
            break;
        default:
            break;
    }

    return 0;
}