#pragma once
#include <Point.h>

namespace grid {

	v2 convert(int gx,int gy);

	v2 convert(const p2i& p);

	p2i convert(const v2& pos);
}

