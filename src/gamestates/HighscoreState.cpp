#include "HighscoreState.h"
#include <core\log\Log.h>
#include <renderer\sprites.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

HighscoreState::HighscoreState(GameContext* context) : ds::GameState("HighscoreState"), _context(context) {
}


HighscoreState::~HighscoreState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::activate() {
	//_dialog
	/*
	_gui->activate("GameOver");	
	ds::GUIDialog* dlg = _gui->get("GameOver");
	std::string str;
	ds::string::formatInt(_context->levelIndex, str);
	dlg->updateText(10, str);
	if (_context->gameMode == GM_TIMER) {
		dlg->updateText(11, "Timer mode");
	}
	else {
		dlg->updateText(11, "Coverage mode");
	}
	ds::string::formatInt(_context->currentScore.fillrate,str);
	dlg->updateText(12, str + "%");
	ds::string::formatTime(_context->currentScore.minutes, _context->currentScore.seconds, str);
	dlg->updateText(13, str);
	ds::string::formatInt(_context->currentScore.score, str, 6);	
	dlg->updateText(14, str);
	*/
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::deactivate() {
	//_gui->deactivate("Highscores");
}

// --------------------------------------------
// update
// --------------------------------------------
int HighscoreState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
/*
int HighscoreState::onGUIButton(ds::DialogID dlgID, int button) {
	
	return button;
}
*/
// --------------------------------------------
// render
// --------------------------------------------
void HighscoreState::render() {
}


