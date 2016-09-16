#include "MainGameState.h"
#include <core\log\Log.h>
#include <renderer\sprites.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState"), _context(context) {
	_board = new Board(&context->settings);
	_gridTex[0] = math::buildTexture(200, 420, 430, 486);
	_gridTex[1] = math::buildTexture(200, 450, 320, 486);
	_gridTex[2] = math::buildTexture(200, 860, 110, 486);
	//_effect = new ds::GrayFadeEffect();
}


MainGameState::~MainGameState() {
	//delete _effect;
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
	//_effect->deactivate();
	// FIXME: reset score
	_context->hud.reset();
	_context->hud.activate();	
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainGameState::deactivate() {
	_context->hud.deactivate();
}

// --------------------------------------------
// stop game
// --------------------------------------------
void MainGameState::stopGame() {
	_timer = 0.0f;
	_running = false;
	_context->hud.deactivate();
}
// --------------------------------------------
// update
// --------------------------------------------
int MainGameState::update(float dt) {
	//_effect->tick(dt);
	if (_running) {
		int points = _board->update(dt);
		if (points > 0) {
			_context->score.points += points * points * 10;
			_context->hud.setNumber(_context->score.points);
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
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->draw(v2(295, 362), _gridTex[0]);
	sprites->draw(v2(670, 362), _gridTex[1]);
	sprites->draw(v2(885, 362), _gridTex[2]);
	//_effect->start();
	_board->render();
	//_effect->render();
	//if (!_running) {
		//ds::sprites::draw(v2(512, 384), ds::math::buildTexture(880, 0, 640, 60));
	//}
}

// --------------------------------------------
// process events
// --------------------------------------------
/*
int MainGameState::processEvents(const ds::EventStream& events) {
	for (uint32_t i = 0; i < events.num(); ++i) {
		uint32_t type = events.getType(i);
		if (type == GE_REBUILD_BOARD) {
			_board->rebuild(); 
		}
		else if ( type == GE_END_GAME) {
			return 1;
		}
		else if (type == GE_FADE_TO_GRAY) {
			if (_effect->isActive()) {
				_effect->deactivate();
				_running = true;
			}
			else {
				_effect->activate();
				stopGame();
			}
		}
		else if (type == GE_TOGGLE_SHOW_BOARD) {
			_board->toggleShowBoard();
		}
		else if (type == GE_TOGGLE_SHOW_STATES) {
			_board->toggleShowStates();
		}
	}
	return 0;
}
*/
