#pragma once
#include <data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	float flashTTL;
	float flashScale;
	float flashScaleStart;
	float tweeningTTL;
	ds::Rect flashRect;
	float droppingTTL;
	float grayScaleFactor;
	float tweeningYOffset;
	float tweeningYAdd;
	float shakePeriod;
	float shakeAmount;

	GameSettings() {
		addFloat("flash_ttl", &flashTTL , 0.6f);
		addFloat("flash_scale", &flashScale, 0.2f);
		addRect("flash_rect", &flashRect , ds::Rect(680,320,52,52));
		addFloat("flash_scale_start", &flashScaleStart, 0.9f);		
		addFloat("dropping_ttl", &droppingTTL, 0.3f);
		addFloat("gray_scale_factor", &grayScaleFactor, 2.0f);
		addFloat("tweening_ttl", &tweeningTTL, 2.0f);
		addFloat("tweening_y_offset", &tweeningYOffset, 60);
		addFloat("tweening_y_add", &tweeningYAdd, 1000);
		addFloat("shake_period", &shakePeriod, 15.0f);
		addFloat("shake_amount", &shakeAmount, 3.0f);
	}

	const char* getFileName() const {
		return "settings.json";
	}

};
