#include <cstdlib>
#pragma once

#include "Vector2.h"
#include "Footprint.h"

class Ship
{
	public:
		enum Orientation {UP, DOWN, LEFT, RIGHT};

		Ship(Vector2 pos, int length, Orientation orientation = Orientation::UP);


		Orientation facing;
		int length;
		Vector2 pos;
		int index = -1;

		Footprint footprint;
};

