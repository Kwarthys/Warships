#include "NetworkManager.h"
#include "GameManager.h"

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

    connexions.reserve(12); //ClientConnexion are not movable (i don't know how to make them), so we reserve and prevent needing to move them

    std::thread welcomingThread(welcomeClients, this, s);

    bool keepRunning = true;

    while (keepRunning)
    {
        if (acceptMoreConnexions)
        {
            if (connexions.size() >= 9)
            {
                std::cout << "Stopping further connexions" << endl;
                acceptMoreConnexions = false;
                shutdown(s, SD_BOTH);
            }
        }
       
        //taking the most of what we can pull from the buffer, then sleeping 1s before trying again
        bool readCommands = true;
        while (readCommands)
        {
            std::unique_ptr<Command> c = inwardComs.tryToGet();//async coms from clients waiting here
            if (c != nullptr)
            {
                gameManager->treatCommand(c.get());//sync coms to clients will be sent here
            }
            else
            {
                readCommands = false;
            }
        }
        //sleep 1s
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (connexions.size() > 0)
        {
            bool allDed = true;
            for (size_t i = 0; i < connexions.size() && allDed; i++)
            {
                if (connexions.at(i).isRunning())
                {
                    allDed = false;
                }
            }
            keepRunning = !allDed; //stop server if all clients are disconnected
        }
    }

    acceptMoreConnexions = false;
    shutdown(s, SD_BOTH);
    
    //waiting for all threads to complete before cleaning the network utilities
    //ie waiting for all clients to disconnect at the end before shutting the server
    //for (size_t i = 0; i < connexions.size(); i++)
    //{
    //    connexions[i].listenerThread.join();
    //}
    // no longer necessary, keeping at hand just in case

    //TODO Welcoming thread never leaves ACCEPT, need to adress that
    //waiting him to stop
    std::cout << "Waiting welcomer thread" << endl;
    welcomingThread.join();

    WSACleanup();

    std::cout << "Properly closed" << endl;

    //weird abort() called here ?  Maybe cleanup ?
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
            networkManager->connexions.emplace_back(clientSocket, &(networkManager->inwardComs));//client connexions cannont be moved, so sized connexions to avoid any need for a move
            networkManager->indeciesToSockets.push_back(clientSocket%255);//can only send 8bit, so cuting what's above

            std::cout << "added connexion to "  << clientSocket << endl;

            networkManager->connexions.back().startClientListening(); //Start coms in a new thread, will be one per client

            IntArrayCommand c;
            c.id = Command::IDAttrib;
            c.parameter = networkManager->indeciesToSockets.back();
            c.data.push_back(networkManager->connexions.size()-1);

            networkManager->connexions.back().sendToClient(c);
        }
    }

    cout << "Welcomer Thread closed" << endl;
}

void NetworkManager::sendCommandToEveryoneExcept(int exceptID, const Command& c)
{
    for (size_t i = 0; i < connexions.size(); i++)
    {
        if (indeciesToSockets.at(i) != exceptID)
        {
            connexions.at(i).sendToClient(c);
        }
    }
}

void NetworkManager::sendCommandToPlayerID(int playerID, const Command& c)
{
    for (size_t i = 0; i < connexions.size(); i++)
    {
        if (indeciesToSockets.at(i) == playerID)
        {
            connexions.at(i).sendToClient(c);
            return;
        }
    }
}

void NetworkManager::sendCommandToEveryone(const Command& c)
{
    for (size_t i = 0; i < connexions.size(); i++)
    {
        connexions.at(i).sendToClient(c);
    }
}
