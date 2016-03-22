#include "Map.h"
#include "Constants.h"
#include "Map.h"

namespace grid {

	Vector2i convertMousePos(const Vector2f& mousePos) {
		int my = 768 - (int)mousePos.y;	
		my = my - STARTY + HALF_CELL_SIZE;
		int mx = (int)mousePos.x - STARTX + HALF_CELL_SIZE;
		return Vector2i(mx,my);
	}


	Vector2i convertMousePosToGridPos(const Vector2f& mousePos) {
		Vector2i sp = convertMousePos(mousePos);
		int x = sp.x / CELL_SIZE;
		int y = sp.y / CELL_SIZE;
		return Vector2i(x,y);
	}

	Vector2f convertFromGrid(int gx,int gy) {
		return Vector2f(STARTX + gx * CELL_SIZE,STARTY + gy * CELL_SIZE);	
	}
}