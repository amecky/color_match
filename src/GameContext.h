#pragma once
#include "Constants.h"
#include "GameSettings.h"
#include <dialogs\GUIDialog.h>

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
	ds::GUIDialog* hud;

	void resetScore() {
		score.highestCombo = 0;
		score.highscore = 0;
		score.piecesLeft = MAX_X * MAX_Y;
		score.points = 0;
		score.totalPieces = MAX_X * MAX_Y;
		score.minutes = 0;
		score.seconds = 0;
		//m_PercentageNumbers.create(Vector2f(512, 50), 0);
		//m_HUD.reset();
	}

};