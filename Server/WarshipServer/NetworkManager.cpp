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

    std::thread welcomingThread(welcomeClients, std::ref(*this), s);

    bool gameOn = true;

    while (gameOn)
    {
        if (acceptMoreConnexions)
        {
            if (connexions.size() >= 2)
            {
                acceptMoreConnexions = false;
                shutdown(s, SD_BOTH);
                std::cout << "Stopping further connexions" << endl;
            }
        }

        std::unique_ptr<Command> c;
        //if (inwardComs.tryToGet(c))
        //{
        //  welp i need some time to think this through
        //}
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

void NetworkManager::welcomeClients(NetworkManager& networkManager, SOCKET serverSocket)
{
    SOCKADDR_IN csin;
    int sinsize = sizeof(csin);

    while (networkManager.acceptMoreConnexions)
    {
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&csin, &sinsize);

        if (clientSocket != INVALID_SOCKET)
        {
            networkManager.connexions.emplace_back(clientSocket, &networkManager.inwardComs);
            networkManager.indeciesToSockets.push_back(clientSocket);

            networkManager.connexions.back().startClientListening(); //Start coms in a new thread, will be one per client

            IntArrayCommand c;
            c.id = Command::IDAttrib;
            c.parameter = networkManager.indeciesToSockets.back();
            c.data.push_back(0);

            networkManager.connexions.back().sendToClient(c);
        }
    }
}
