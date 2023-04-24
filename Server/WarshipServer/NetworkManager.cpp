#include "NetworkManager.h"

void NetworkManager::startServer(CommandManager& cm)
{
    cout << "Hello Warship Server World!" << endl;

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

    bool received = false;


    SOCKADDR_IN csin;
    SOCKET csock;

    while (!received)
    {
        int sinsize = sizeof(csin);
        csock = accept(s, (SOCKADDR*)&csin, &sinsize);

        if (csock != INVALID_SOCKET)
        {
            char recvBuf[255];
            int rcvLen = recv(csock, recvBuf, 255,0);

            std::unique_ptr<Command> command = cm.deserialize(recvBuf, rcvLen);
            cm.displayCommand(*command.get());

            cout << "Sending command" << endl;

            StringCommand sc;
            sc.id = Command::NameSend;
            sc.parameter = 10;
            sc.data = "ServersZykyflex";

            char buf[255];

            int len = cm.serialize(sc, buf);

            //send(csock, "HELLO CLIENT\r\n", 14, 0);
            send(csock, buf, len, 0);

            received = true;
        }
        else
        {
            cout << "listening ..." << endl;
        }
    }

    closesocket(csock);
    WSACleanup();

    cout << "Properly closed" << endl;
}
