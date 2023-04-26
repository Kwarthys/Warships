#include "ClientConnexion.h"

//blocks till client shuts down connection
void ClientConnexion::manageClientCommunication(ClientConnexion& clientConnexion)
{
    while (true)
    {
        char recvBuf[255];
        std::cout << "ready to listen to " << clientConnexion.clientSocket << std::endl;
        int rcvLen = recv(clientConnexion.clientSocket, recvBuf, 255, 0);

        std::cout << "Received " << rcvLen << std::endl;

        if (rcvLen != 0)
        {
            std::unique_ptr<Command> command = CommandManager::deserialize(recvBuf, rcvLen);
            command->socketID = clientConnexion.clientSocket;
            std::cout << "Placing command in buffer. Command:";
            CommandManager::displayCommand(*command);
            clientConnexion.inwardCommandBuffer->waitToAdd(std::move(command));
        }
        else
        {
            std::cout << "Client" << clientConnexion.clientSocket << " terminated connexion" << std::endl;
            closesocket(clientConnexion.clientSocket);
            clientConnexion.running = false;
            return;
        }
    }
}

void ClientConnexion::sendToClient(const Command& c) const
{
    std::cout << "sending Command: ";
    CommandManager::displayCommand(c);
    std::cout << std::endl;

    char buf[255];
    int len = CommandManager::serialize(c, buf);

    send(clientSocket, buf, len, 0);
}

void ClientConnexion::startClientListening()
{
    listenerThread = std::thread(manageClientCommunication, std::ref(*this));
}
