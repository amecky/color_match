#pragma once
#include <data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	float rebuildTime;
	float flashTTL;
	float droppingTTL;
	float grayScaleFactor;

	GameSettings() {
		addFloat("flash_ttl", &flashTTL , 0.6f);
		addFloat("rebuild_time", &rebuildTime, 1.0f);
		addFloat("dropping_ttl", &droppingTTL, 0.3f);
		addFloat("gray_scale_factor", &grayScaleFactor, 2.0f);
	}

	const char* getFileName() const {
		return "settings.json";
	}

};
