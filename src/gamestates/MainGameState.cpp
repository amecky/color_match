#include "MainGameState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState"), _context(context) {
	_board = new Board(&context->settings);
}


MainGameState::~MainGameState() {
	delete _board;
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainGameState::activate() {	
	_context->resetScore();
	_board->rebuild();
	// FIXME: reset score
	_context->hud->activate();	
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainGameState::deactivate() {
	_context->hud->deactivate();
}

// --------------------------------------------
// update
// --------------------------------------------
int MainGameState::update(float dt) {
	int points = _board->update(dt);
	if (points > 0) {
		_context->score.points += points * 10;
		_context->hud->setNumber(1, _context->score.points);
		if (points > _context->score.highestCombo) {
			_context->score.highestCombo = points;
			LOG << "new highest combo: " << _context->score.highestCombo;
		}
		_context->score.piecesLeft -= points;
		float percentage = (1.0f - static_cast<float>(_context->score.piecesLeft) / static_cast<float>(_context->score.totalPieces)) * 100.0f;
		//m_HUD.setPercentage(static_cast<int>(percentage));
	}
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	_board->select();
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void MainGameState::render() {
	_board->render();
}

int MainGameState::onChar(int ascii) {
	if (ascii == 'e') {
		return 1;
	}
	if (ascii == 'r') {
		_board->rebuild();
	}
	return 0;
}
