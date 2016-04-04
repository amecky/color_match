#include "HUD.h"
#include <sprites\SpriteBatch.h>
#include <math\GameMath.h>

HUD::HUD() {
	_active = false;
	for (int i = 0; i < 6; ++i) {
		_numbers[i] = i;
	}
	_minutes[0] = 0;
	_minutes[1] = 1;
	_seconds[0] = 0;
	_seconds[1] = 4;
	_timer = 0.0f;
}

HUD::~HUD() {
}

void HUD::reset() {
	_active = false;
	for (int i = 0; i < 6; ++i) {
		_numbers[i] = 0;
	}
	_minutes[0] = 0;
	_minutes[1] = 0;
	_seconds[0] = 0;
	_seconds[1] = 0;
	_timer = 0.0f;
	setNumber(0);
}

void HUD::tick(float dt) {
	if (_active) {
		_timer += dt;
		if (_timer > 1.0f) {
			_timer -= 1.0f;
			_seconds[1] += 1;
			if (_seconds[1] >= 10) {
				_seconds[1] = 0;
				_seconds[0] += 1;
				if (_seconds[0] >= 6) {
					_seconds[0] = 0;
					_minutes[0] += 1;
					if (_minutes[0] >= 10) {
						_minutes[0] = 0;
						_minutes[1] += 1;
					}
				}
			}
		}
	}
}

void HUD::setNumber(int value) {
	int tmp = value;
	int div = 1;
	for (int i = 0; i < 6; ++i) {
		if (i > 0) {
			div *= 10;
		}
	}
	for (int i = 0; i < 6; ++i) {
		int r = tmp / div;
		_numbers[i] = r;
		tmp = tmp - r * div;
		div /= 10;
	}
}

void HUD::render() {
	if (_active) {
		v2 p(103, 30);
		for (int i = 0; i < 6; ++i) {
			ds::sprites::draw(p, ds::math::buildTexture(0, 340 + _numbers[i] * 48, 46, 42));
			p.x += 48.0f;
		}
		p.x = 725.0f;
		for (int i = 0; i < 2; ++i) {
			ds::sprites::draw(p, ds::math::buildTexture(0, 340 + _minutes[i] * 48, 46, 42));
			p.x += 48.0f;
		}
		ds::sprites::draw(p, ds::math::buildTexture(0, 820, 46, 42));
		p.x += 48.0f;
		for (int i = 0; i < 2; ++i) {
			ds::sprites::draw(p, ds::math::buildTexture(0, 340 + _seconds[i] * 48, 46, 42));
			p.x += 48.0f;
		}
	}
}
