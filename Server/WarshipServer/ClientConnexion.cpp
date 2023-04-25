#include "ClientConnexion.h"

//blocks till client shuts down connection
void ClientConnexion::manageClientCommunication(CommandManager& cm)
{
    while (true)
    {
        char recvBuf[255];
        int rcvLen = recv(clientSocket, recvBuf, 255, 0);

        if (rcvLen != 0)
        {
            std::unique_ptr<Command> command = cm.deserialize(recvBuf, rcvLen);
            command.get()->socketID = clientSocket;
            cm.displayCommand(*command.get());

            std::cout << "Sending command" << std::endl;

            StringCommand sc;
            sc.id = Command::NameSend;
            sc.parameter = clientSocket;
            sc.data = "ServersZykyflex";

            char buf[255];

            int len = cm.serialize(sc, buf);

            //send(csock, "HELLO CLIENT\r\n", 14, 0);
            send(clientSocket, buf, len, 0);
        }
        else
        {
            std::cout << "Client" << clientSocket << " terminated connexion" << std::endl;
            closesocket(clientSocket);
            return;
        }
    }
}
