#pragma once

#include <iostream>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include "Command.h"
#include "CommandManager.h"

class CommandBuffer
{
	public:
		void waitToAdd(std::unique_ptr<Command> commandPtr);
		std::unique_ptr<Command> tryToGet();

		static void testThreading();
		static void testThreadingTask(std::unique_ptr<Command> toAdd, CommandBuffer& commandBuffer);

	private:
		std::queue<std::unique_ptr<Command>> commands;
		//std::atomic<bool> locked = false;
		std::mutex mutex;
};

