#include "Ship.h"

Ship::Ship(Vector2 pos, int length, Orientation orientation) : pos(pos), length(length), facing(orientation)
{
	/*** Compute Footprint ***/
	int w = 1;
	int h = 1;
	int x = pos.x;
	int y = pos.y;
	switch (facing)
	{
	case UP:
		h = length;
		break;

	case DOWN:
		h = length;
		y = y - (length - 1);
		break;

	case RIGHT:
		w = length;
		break;

	case LEFT:
		w = length;
		x = x - (length - 1);
		break;
	}

	footprint = Footprint(x, y, w, h); //pos still 0,0 for now but will be moved elsewhere
}
