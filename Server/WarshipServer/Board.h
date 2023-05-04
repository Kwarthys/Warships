#include <vector>
#include <sstream>
#include <string>
#include <iostream>

#include "Ship.h"
#include "MapNode.h"

#pragma once
class Board
{
	public:
		Board(int sizeX, int sizeY, int playerID);


		bool placeShip(Ship ship);
		bool placeNewShip(Ship::ShipType shipType, Ship::Orientation facing, int rootNode);
		bool isSpotFree(Footprint& spot);

		int shoot(Vector2 target);
		int shoot(int nodeIndex);

		int fromCoordToIndex(int x, int y) { return y * sizeX + x; };
		int fromCoordToIndex(Vector2 coords) { return coords.y * sizeX + coords.x; };
		Vector2 fromIndexToCoord(int index);

		int getPlayerID() { return playerID; }

		int getNumberOfShipsAfloat();
		bool isShipSunk(int shipIndex);

		void debugDisplayMap();

		std::string playerName;

		bool playerReady = false;

		std::vector<int> targetPairs;
		std::vector<int> targetResults;
		std::vector<int> targetedResultPairs;
		std::vector<int> newlySunkedShip;

	private:
		int playerID;
		int sizeX;
		int sizeY;
		std::vector<MapNode> map;
		std::vector<Ship> ships;


		void assignShipToMap(Ship& ship);
 };

