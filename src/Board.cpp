#include "Board.h"
#include "Constants.h"
#include <core\math\GameMath.h>
#include <core\log\Log.h>
#include "Map.h"
#include <core\math\tweening.h>
#include <renderer\sprites.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>
#include <core\script\vm.h>
#include <resources\ResourceContainer.h>
// -------------------------------------------------------
// Mouse over state
// -------------------------------------------------------
int MouseOverState::activate() {
	_selectedEntry = INVALID_POINT;
	return 0;
}

int MouseOverState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	v2 mousePos = ds::input::getMousePosition();
	p2i gp = grid::convert(mousePos);
	if (gp.x >= 0 && gp.y >= 0) {
		if (gp != _selectedEntry) {
			_selectedEntry = gp;
			MyEntry& me = ctx->grid->get(gp.x, gp.y);
			if (me.state == TS_NORMAL) {
				me.timer = 0.0f;
				me.state = TS_WIGGLE;
				sendEvent(BE_SELECTED, &gp, sizeof(p2i));
			}
		}
	}
	return 0;
}

// -------------------------------------------------------
// Moving cells state
// -------------------------------------------------------
int MovingCellsState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		const MovingCell& m = ctx->movingCells[i];
		MyEntry& e = ctx->grid->get(m.x, m.y);
		e.hidden = false;
	}
	ctx->movingCells.clear();
	return 0;
}

int MovingCellsState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		m.current = tweening::interpolate(&tweening::easeInOutCubic, m.start, m.end, getTimer(), ctx->settings->droppingTTL);
	}
	return 0;
}

// -------------------------------------------------------
// PrepareBoardState
// -------------------------------------------------------
int PrepareBoardState::activate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	ctx->grid->clear();
	ctx->movingCells.clear();
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			int cid = math::random(0.0f, 4.99f);
			int offset = offset = cid * CELL_SIZE;
			v2 p = grid::convert(x, y);
			MyEntry& e = ctx->grid->get(x, y);
			e.color = cid;
			e.hidden = true;
			e.scale = 1.0f;
			e.state = TS_NORMAL;
			e.timer = 0.0f;

			MovingCell m;
			m.x = x;
			m.y = y;
			m.color = e.color;
			m.start = grid::convert(x, y + 20);
			m.end = grid::convert(x, y);
			m.current = m.start;
			ctx->movingCells.push_back(m);

			ctx->grid->set(x, y, e);
		}
	}
	return 0;
}

// -------------------------------------------------------
// SelectCellState
// -------------------------------------------------------
int SelectCellState::activate() {
	int ret = 0;
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	v2 mousePos = ds::input::getMousePosition();
	p2i converted = grid::convert(mousePos);
	ctx->points.clear();
	if (converted.x >= 0 && converted.y >= 0) {
		MyEntry& me = ctx->grid->get(converted.x, converted.y);
		ctx->points.clear();
		ctx->grid->findMatchingNeighbours(converted.x, converted.y, ctx->points);
		if (ctx->points.size() > 1) {
			for (size_t i = 0; i < ctx->points.size(); ++i) {
				const p2i& gp = ctx->points[i];
				MyEntry& c = ctx->grid->get(gp.x, gp.y);
				c.state = TS_SHRINKING;
				c.timer = 0.0f;
			}
			LOG << "pieces: " << ctx->points.size();
			int score = ctx->points.size();
			sendEvent(BE_SCORE, &score, sizeof(int));
			return 1;
		}
		ctx->points.clear();
	}
	sendEvent(BE_INVALID_SELECTION);
	return 0;
}

// -------------------------------------------------------
// ShrinkState
// -------------------------------------------------------
int ShrinkState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				if (e.state == TS_SHRINKING) {
					float norm = getTimer() / ctx->settings->flashTTL;
					e.scale = 1.0f - norm * 0.9f;
				}
			}
		}
	}
	return 0;
}

int ShrinkState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	ctx->points.clear();
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				if (e.state == TS_SHRINKING) {
					ctx->points.push_back(p2i(x, y));
				}
			}
		}
	}
	return 0;
}

// -------------------------------------------------------
// DroppingCellsState
// -------------------------------------------------------
int DroppingCellsState::activate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);	
	ctx->grid->remove(ctx->points, true);
	ctx->droppedCells.clear();
	ctx->movingCells.clear();
	ctx->grid->dropCells(ctx->droppedCells);
	for (size_t i = 0; i < ctx->droppedCells.size(); ++i) {
		const ds::DroppedCell<MyEntry>& dc = ctx->droppedCells[i];
		MyEntry& e = ctx->grid->get(dc.to);
		e.hidden = true;
		MovingCell m;
		m.x = dc.to.x;
		m.y = dc.to.y;
		m.color = e.color;
		m.start = grid::convert(dc.from);
		m.end = grid::convert(dc.to);
		m.current = m.start;
		ctx->movingCells.push_back(m);
	}
	return 0;
}

int DroppingCellsState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		m.current = tweening::interpolate(&tweening::easeInOutCubic, m.start, m.end, getTimer(), ctx->settings->droppingTTL);
	}
	return 0;
}

int DroppingCellsState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		MyEntry& e = ctx->grid->get(m.x, m.y);
		e.hidden = false;
	}
	ctx->movingCells.clear();
	return 0;
}


// -------------------------------------------------------
// Board
// -------------------------------------------------------
Board::Board(GameContext* context) : _ctx(context) {	
	_cellTexture = math::buildTexture(0,0,36,36);
	_context.grid = &m_Grid;
	_context.settings = &_ctx->settings;
	_states = new ds::StateManager(&_context);
	_states->add<MouseOverState>();
	_states->add<MovingCellsState>();
	_states->add<PrepareBoardState>();
	_states->add<SelectCellState>();
	_states->add<ShrinkState>();
	_states->add<DroppingCellsState>();
	_states->addTransition(BM_FILLING,   0, BM_MOVING);
	_states->addTransition(BM_MOVING, 0, BM_RUNNING, _ctx->settings.droppingTTL);
	_states->addTransition(BM_SELECTION, 1, BM_FLASHING);
	_states->addTransition(BM_SELECTION, 0, BM_RUNNING);
	_states->addTransition(BM_FLASHING, 0, BM_DROPPING, _ctx->settings.flashTTL);
	_states->addTransition(BM_DROPPING, 0, BM_RUNNING, _ctx->settings.droppingTTL);
	_dialogState = 1;
	_dialogPos = v2(10, 760);
	_showStates = false;
	_showBoard = false;
	_wiggleScript = ds::res::getScript("Wiggle");
}

Board::~Board(void) {
	delete _states;
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Board::render() {	
	// pieces
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (!e.hidden) {
					sprites->draw(grid::convert(x, y),_cellTexture,0.0f,v2(e.scale,e.scale),_ctx->colors[e.color]);
				}
			}
		}
	}
	// moving cells
	for (size_t i = 0; i < _context.movingCells.size(); ++i) {
		const MovingCell& cell = _context.movingCells[i];
		sprites->draw(cell.current, _cellTexture,0.0f,v2(1,1),_ctx->colors[cell.color]);
	}
	if (_showStates) {
		//_states->showDialog();
	}
	if (_showBoard) {
		showDialog();
	}
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
int Board::update(float elapsed) {
	int ret = 0;
	_states->tick(elapsed);
	uint32_t n = ds::events::num();
	if (ds::events::num() > 0 ) {
		for (int i = 0; i < n; ++i) {
			if (ds::events::getType(i) == BE_INVALID_SELECTION) {
				// play sound
				LOG << "INVALID SELECTION!!!";
			}
			else if (ds::events::getType(i) == BE_SCORE) {
				ds::events::get(i, &ret);
				LOG << "ret: " << ret;
			}
			else if (ds::events::getType(i) == BE_SELECTED) {
				p2i gp;
				ds::events::get(i, &gp);
				LOG << "gp: " << gp.x << " " << gp.y;
				if (gp != _selection.grid) {
					_selection.grid = gp;
					_selection.timer = 1.0f;
					_selection.active = true;
				}
			}
		}
	}

	if (_selection.active) {
		_selection.timer -= elapsed;
		if (_selection.timer <= 0.0f) {
			ds::Array<p2i> points;
			m_Grid.findMatchingNeighbours(_selection.grid.x, _selection.grid.y, points);
			if (points.size() > 1) {
				for (size_t i = 0; i <points.size(); ++i) {
					const p2i& gp = points[i];
					MyEntry& c = m_Grid.get(gp.x, gp.y);
					c.timer = 0.0f;
					c.state = TS_WIGGLE;
				}
				LOG << "pieces: " << points.size();
			}
			_selection.timer = 0.0f;
			_selection.active = false;
		}
	}
	
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (e.state == TS_WIGGLE) {
					e.timer += elapsed;
					_wiggleScript->set(0, v4(e.timer));
					_wiggleScript->execute();
					e.scale = _wiggleScript->getRegister(4).x;
					if (_wiggleScript->getRegister(1).x >= 1.0f) {
						e.state = TS_NORMAL;
						e.scale = 1.0f;
					}
				}
			}
		}
	}
	return ret;
}

// -------------------------------------------------------
// rebuild grid
// -------------------------------------------------------
void Board::rebuild() {
	_states->activate(BM_FILLING);
	_selection.grid = INVALID_POINT;
	_selection.timer = 0.0f;
	_selection.active = false;
}

// -------------------------------------------------------
// Select
// -------------------------------------------------------
void Board::select() {
	if (_states->getCurrentMode() == BM_RUNNING) {
		_states->activate(BM_SELECTION);
	}
}

// -------------------------------------------------------
// debug
// -------------------------------------------------------
void Board::debug() {
	char buffer[32];
	LOG << "---------------------- GRID ------------------------";
	for (int y = MAX_Y - 1; y >= 0; --y) {
		std::string line;
		for (int x = 0; x < MAX_X; ++x) {
			if (m_Grid.isFree(x, y)) {
				line += " -- ";
			}
			else {
				MyEntry& c = m_Grid.get(x, y);
				sprintf(buffer, " %2d ", c.color);
				line += buffer;
			}
		}
		LOG << line;
	}
}

void Board::showDialog() {
	/*
	char buffer[32];
	gui::start(1, &_dialogPos);
	gui::begin("State", &_dialogState);

	for (int y = MAX_Y - 1; y >= 0; --y) {
		std::string line;
		for (int x = 0; x < MAX_X; ++x) {
			if (m_Grid.isFree(x, y)) {
				line += " -- ";
			}
			else {
				MyEntry& c = m_Grid.get(x, y);
				sprintf(buffer, " %2d ", c.color);
				line += buffer;
			}
		}
		gui::Label(line.c_str());
	}
	gui::end();
	*/
}
