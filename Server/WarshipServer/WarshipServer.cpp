// WarshipServer.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>

#include "Board.h"
#include "Ship.h"
#include "CommandManager.h"
#include "NetworkManager.h"

using namespace std;

int main()
{
    /**** SERIALIZATION TESTING *****/
    //CommandManager cm;
    //cm.testSerialization();

    /**** BOARD TESTING *****/
    /*
    Board board(10,10);

    board.debugDisplayMap();

    Vector2 pos = Vector2(0, 0);
    Ship s(pos, 4, Ship::UP);
    board.placeShip(s);
    board.debugDisplayMap();

    pos = Vector2(2, 2);
    Ship s2(pos, 3, Ship::RIGHT);
    board.placeShip(s2);
    board.debugDisplayMap();

    pos = Vector2(9,9);
    Ship s3(pos, 4, Ship::DOWN);
    board.placeShip(s3);
    board.debugDisplayMap();

    board.shoot(Vector2(9, 9));
    board.shoot(Vector2(9, 6));
    board.shoot(Vector2(9, 7));
    board.shoot(Vector2(9, 8));
    board.debugDisplayMap();
    */

    /**** NETWORK & SERIALIZATION TESTING *****/
    NetworkManager networkManager;
    CommandManager commandManager;

    networkManager.startServer(commandManager);


    return 0;
}

//keeping that at hand just in case

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
