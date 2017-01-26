#pragma once
#include <core\base\GameObject.h>

class HUD : public ds::GameObject {

public:
	HUD();
	~HUD();
	void render();
	void tick(float dt);
	void reset();
	void setNumber(int value);
private:
	float _timer;
	int _numbers[6];
	int _minutes[2];
	int _seconds[2];
};

