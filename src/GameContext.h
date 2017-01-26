#pragma once
#include "Constants.h"
#include "GameSettings.h"
#include <dialogs\GUIDialog.h>
#include "HUD.h"

struct Score {
	int piecesLeft;
	int highestCombo;
	int points;
	int highscore;
	int totalPieces;
	int minutes;
	int seconds;
};

struct GameContext {

	Score score;
	GameSettings settings;
	ds::Color headColor;
	ds::Color colors[8];

	void resetScore() {
		score.highestCombo = 0;
		score.highscore = 0;
		score.piecesLeft = MAX_X * MAX_Y;
		score.points = 0;
		score.totalPieces = MAX_X * MAX_Y;
		score.minutes = 0;
		score.seconds = 0;
	}

	void pick_colors() {
		// prepare colors
		float goldenRatioConjugate = 0.618033988749895f;
		float currentHue = math::random(0.0f, 1.0f);
		for (int i = 0; i < 8; i++) {
			ds::HSL hslColor = ds::HSL(currentHue * 360.0f, 50.0f, 50.0f);
			colors[i] = ds::color::hsl2rgb(hslColor);
			currentHue += goldenRatioConjugate;
			if (currentHue > 1.0f) {
				currentHue -= 1.0f;
			}
		}
	}

};

