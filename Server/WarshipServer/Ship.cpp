#include "Ship.h"

Ship::Ship()
{
	facing = (Orientation)(std::rand() % 4);
	length = std::rand() % 4 + 1;

	pos = Vector2();


	/*** Compute Footprint ***/
	int w = 0;
	int h = 0;
	switch (facing)
	{
	case UP:
		h = length;
		break;

	case DOWN:
		h = -length;
		break;

	case RIGHT:
		w = length;
		break;

	case LEFT:
		w = -length;
		break;
	}

	footprint = Footprint(pos.x, pos.y, w, h); //pos still 0,0 for now but will be moved elsewhere
}
