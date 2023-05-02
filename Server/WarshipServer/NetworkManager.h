#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>

#include "Command.h"
#include "ClientConnexion.h"

class GameManager;

using namespace std;

class NetworkManager
{
	public:
		void startServer();

		void sendCommandToEveryoneExcept(int exceptID, const Command& c);
		void sendCommandToPlayerID(int playerID, const Command& c);
		void sendCommandToEveryone(const Command& c);

		GameManager* gameManager;

	private:
		static void welcomeClients(NetworkManager* networkManager, SOCKET serverSocket);

		std::vector<ClientConnexion> connexions;
		std::vector<int> indeciesToSockets;
		CommandBuffer inwardComs;

		std::atomic<bool> acceptMoreConnexions = true;
};

