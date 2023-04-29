#include <cstdlib>
#include <vector>
#include <iostream>
#pragma once

#include "Vector2.h"
#include "Footprint.h"

class Ship
{
	public:
		enum Orientation {UP, DOWN, LEFT, RIGHT};
		enum class ShipType { None, Destroyer, Submarine, Cruiser, Battleship, Carrier};

		Ship(Vector2 pos, int length, Orientation orientation = Orientation::UP);

		bool takeHit(Vector2 hit);
		bool isSunk();

		static int typeToLength(ShipType type);


		Orientation facing;
		int length;
		Vector2 pos;
		int index = -1;

		std::vector<bool> hitPoints;

		Footprint footprint;

	protected:
		int fromPointToLength(Vector2 hit);
};

