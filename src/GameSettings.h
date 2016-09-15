#pragma once
#include <core\data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	float rebuildTime;
	float flashTTL;
	float droppingTTL;
	float grayScaleFactor;
	float gameOverTTL;

	GameSettings() : ds::DynamicGameSettings("settings.json") {
		add("flash_ttl", &flashTTL , 0.6f);
		add("rebuild_time", &rebuildTime, 1.0f);
		add("dropping_ttl", &droppingTTL, 0.3f);
		add("gray_scale_factor", &grayScaleFactor, 2.0f);
		add("game_over_ttl", &gameOverTTL, 2.0f);
	}

};
