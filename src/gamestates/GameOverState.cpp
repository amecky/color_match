#include "GameOverState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

GameOverState::GameOverState(ds::DialogManager* gui, GameContext* context) : ds::GameState("GameOverState"), _context(context), _gui(gui) {
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	_gui->activate("GameOver");
	/*
	ds::GUIDialog* dlg = _gui->get("GameOver");
	std::string str;
	ds::string::formatInt(_context->score.points, str);
	dlg->updateText(3, str);
	int delta = _context->score.totalPieces - _context->score.piecesLeft;
	float d = (float)delta / (float)_context->score.totalPieces * 100.0f;
	int p = static_cast<int>(d);
	ds::string::formatInt(p,str);
	dlg->updateText(7, str + " %");
	ds::string::formatTime(_context->score.minutes, _context->score.seconds, str);
	dlg->updateText(5, str);
	ds::string::formatInt(_context->score.highestCombo, str);	
	dlg->updateText(9, str);
	*/
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::deactivate() {
	_gui->deactivate("GameOver");
}

// --------------------------------------------
// update
// --------------------------------------------
int GameOverState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int GameOverState::onGUIButton(ds::DialogID dlgID, int button) {
	
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void GameOverState::render() {
}


