#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include "CommandManager.h"
#include "Command.h"
#include "CommandBuffer.h"

class ClientConnexion
{
	public:
		ClientConnexion(SOCKET associatedSocket, CommandBuffer* inwardCommandBuffer) :
			clientSocket(associatedSocket), inwardCommandBuffer(inwardCommandBuffer) {}

		ClientConnexion(const ClientConnexion& tocopy)
		{
			clientSocket = tocopy.clientSocket;
			inwardCommandBuffer = tocopy.inwardCommandBuffer;
		}

		void startClientListening();
		void sendToClient(const Command& c) const;
		bool isRunning() { return running; }

		std::thread listenerThread;

	private:
		static void manageClientCommunication(ClientConnexion& clientConnexion);

		SOCKET clientSocket;
		CommandBuffer* inwardCommandBuffer;
		std::atomic<bool> running = true;

};

