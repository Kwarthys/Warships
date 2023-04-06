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
		Board(int sizeX, int sizeY);

		bool placeShip(Ship& ship);
		bool isSpotFree(Footprint& spot);

		bool shoot(Vector2 target);

		int fromCoordToIndex(int x, int y) { return y * sizeX + x; };
		int fromCoordToIndex(Vector2 coords) { return coords.y * sizeX + coords.x; };
		Vector2 fromIndexToCoord(int index);

		void debugDisplayMap();

	private:
		int sizeX;
		int sizeY;
		std::vector<MapNode> map;
		std::vector<Ship> ships;

		void assignShipToMap(Ship& ship);
 };

