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

    std::vector<ClientConnexion> connexions;
    std::vector<int> indeciesToSockets;

    CommandBuffer inwardComs;

    bool received = false;
    while (!received)
    {
        SOCKADDR_IN csin;
        int sinsize = sizeof(csin);
        SOCKET clientSocket = accept(s, (SOCKADDR*)&csin, &sinsize);

        if (clientSocket != INVALID_SOCKET)
        {
            connexions.emplace_back(clientSocket, inwardComs);
            indeciesToSockets.push_back(clientSocket);

            connexions.back().startClientListening(); //Start coms in a new thread, will be one per client

            IntArrayCommand c;
            c.id = Command::IDAttrib;
            c.parameter = indeciesToSockets.back();
            c.data.push_back(0);

            connexions.back().sendToClient(c);

            received = true;
        }
        else
        {
            std::cout << "listening ..." << endl;
        }
    }

    //waiting for all threads to complete before cleaning the network utilities
    for (size_t i = 0; i < connexions.size(); i++)
    {
        connexions[i].listenerThread.join();
    }

    WSACleanup();

    std::cout << "Properly closed" << endl;
}
