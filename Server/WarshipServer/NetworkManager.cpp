#include "NetworkManager.h"

void NetworkManager::startServer()
{
    std::cout << "Hello Warship Server World!" << endl;

    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(4242);

    bind(s, (SOCKADDR*)&sin, sizeof(sin));

    listen(s, 0);

    SOCKADDR_IN csin;
    int sinsize = sizeof(csin);

    std::thread welcomingThread(welcomeClients, this, s);

    int receivedCommands = 0;

    while (receivedCommands < 5)
    {
        /*
        if (acceptMoreConnexions)
        {
            if (connexions.size() >= 3)
            {
                std::cout << "Stopping further connexions" << endl;
                acceptMoreConnexions = false;
                //shutdown(s, SD_BOTH);
            }
        }
        */
       
        std::unique_ptr<Command> c = inwardComs.tryToGet();
        if (c != nullptr)
        {
            CommandManager::displayCommand(*c);
            receivedCommands++;
        }
    }

    
    //waiting for all threads to complete before cleaning the network utilities
    //ie waiting for all clients to disconnect at the end before shutting the server
    for (size_t i = 0; i < connexions.size(); i++)
    {
        connexions[i].listenerThread.join();
    }

    WSACleanup();

    std::cout << "Properly closed" << endl;
}

void NetworkManager::welcomeClients(NetworkManager* networkManager, SOCKET serverSocket)
{
    SOCKADDR_IN csin;
    int sinsize = sizeof(csin);

    while (networkManager->acceptMoreConnexions)
    {
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&csin, &sinsize);

        std::cout << "Accepted " << clientSocket << endl;

        if (clientSocket != INVALID_SOCKET)
        {
            networkManager->connexions.emplace_back(clientSocket, &networkManager->inwardComs);//TODO Debug "abort() has been called" on second connexion
            networkManager->indeciesToSockets.emplace_back(clientSocket);

            std::cout << "added connexion to "  << clientSocket << endl;

            networkManager->connexions.back().startClientListening(); //Start coms in a new thread, will be one per client

            IntArrayCommand c;
            c.id = Command::IDAttrib;
            c.parameter = networkManager->indeciesToSockets.back();
            c.data.push_back(0);

            networkManager->connexions.back().sendToClient(c);
        }
    }
}
