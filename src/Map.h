#pragma once
#include <math\Vector.h>
#include <lib\Grid.h>

namespace grid {

	v2 convert(int gx,int gy);

	v2 convert(const ds::Point& p);

	ds::Point convert(const v2& pos);
}

