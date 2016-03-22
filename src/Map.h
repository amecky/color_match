#pragma once
#include <math\Vector.h>

namespace grid {

	Vector2i convertMousePos(const Vector2f& mousePos);

	Vector2i convertMousePosToGridPos(const Vector2f& mousePos);

	Vector2f convertFromGrid(int gx,int gy);
}

