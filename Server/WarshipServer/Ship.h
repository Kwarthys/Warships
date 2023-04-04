#include <cstdlib>
#pragma once

#include "Vector2.h"
#include "Footprint.h"

class Ship
{
	public:
		Ship();

		enum Orientation {UP, DOWN, LEFT, RIGHT};

		Orientation facing;
		int length;
		Vector2 pos;

		Footprint footprint;
};

