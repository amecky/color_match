#include "MainGameState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState"), _context(context) {
	_board = new Board(&context->settings);
	_gridTex[0] = ds::math::buildTexture(200, 420, 430, 486);
	_gridTex[1] = ds::math::buildTexture(200, 450, 320, 486);
	_gridTex[2] = ds::math::buildTexture(200, 860, 110, 486);
	_effect = new ds::GrayFadeEffect();
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
	_timer = 0.0f;
	_running = true;
	// FIXME: reset score
	_context->hud->activate();	
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainGameState::deactivate() {
	_context->hud->deactivate();
}

void MainGameState::stopGame() {
	_timer = 0.0f;
	_running = false;
	_context->hud->deactivate();
}
// --------------------------------------------
// update
// --------------------------------------------
int MainGameState::update(float dt) {
	_effect->tick(dt);
	if (_running) {
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
	}
	else {
		_timer += dt;
		if (_timer > _context->settings.gameOverTTL) {
			return 1;
		}
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
	ds::sprites::draw(v2(295, 362), _gridTex[0]);
	ds::sprites::draw(v2(670, 362), _gridTex[1]);
	ds::sprites::draw(v2(885, 362), _gridTex[2]);
	_effect->start();
	_board->render();
	_effect->render();
	if (!_running) {
		ds::sprites::draw(v2(512, 384), ds::math::buildTexture(840, 0, 640, 60));
	}
}

int MainGameState::onChar(int ascii) {
	if (ascii == 'e') {
		return 1;
	}
	if (ascii == 'r') {
		_board->rebuild();
	}
	if (ascii == 'f') {
		if (_effect->isActive()) {
			_effect->deactivate();
			_running = true;
		}
		else {
			_effect->activate();
			stopGame();
		}
	}
	return 0;
}
