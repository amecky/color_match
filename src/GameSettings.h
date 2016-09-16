#pragma once
#include <core\data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	float rebuildTime;
	float flashTTL;
	float droppingTTL;
	float grayScaleFactor;
	float gameOverTTL;

	struct Wiggle {
		float ttl;
		float frequency;
		float amplitude;
	} wiggle;

	GameSettings() : ds::DynamicGameSettings("content\\settings.json") {
		add("settings.flash_ttl", &flashTTL , 0.6f);
		add("rebuild_time", &rebuildTime, 1.0f);
		add("settings.dropping_ttl", &droppingTTL, 0.3f);
		add("gray_scale_factor", &grayScaleFactor, 2.0f);
		add("game_over_ttl", &gameOverTTL, 2.0f);
		add("wiggle.ttl", &wiggle.ttl, 1.0f);
		add("wiggle.frequency", &wiggle.frequency, 1.0f);
		add("wiggle.amplitude", &wiggle.amplitude, 1.0f);
	}

};
