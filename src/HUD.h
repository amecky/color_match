#pragma once

class HUD {

public:
	HUD();
	~HUD();
	void render();
	void activate() {
		_active = true;
	}
	void deactivate() {
		_active = false;
	}
	void tick(float dt);
	void reset();
	void setNumber(int value);
private:
	float _timer;
	int _numbers[6];
	int _minutes[2];
	int _seconds[2];
	bool _active;
};

