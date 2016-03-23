#include "Board.h"
#include "Constants.h"
#include <math\GameMath.h>
#include <utils\Log.h>
#include "Map.h"
#include <math\tweening.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>

ds::Point convert(const v2& pos) {
	int my = ((int)pos.y - STARTY + HALF_CELL_SIZE) / CELL_SIZE;
	int mx = ((int)pos.x - STARTX + HALF_CELL_SIZE) / CELL_SIZE;
	return ds::Point(mx, my);
}
// -------------------------------------------------------
// Mouse over state
// -------------------------------------------------------
int MouseOverState::activate() {
	_selectedEntry = INVALID_POINT;
	return 0;
}

int MouseOverState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	v2 mousePos = ds::renderer::getMousePosition();
	ds::Point gp = convert(mousePos);
	if (gp.x >= 0 && gp.y >= 0) {
		if (gp != _selectedEntry) {
			_selectedEntry = gp;
			MyEntry& me = ctx->grid->get(gp.x, gp.y);
			if (me.state == TS_NORMAL) {
				me.timer = 0.0f;
				me.state = TS_WIGGLE;
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
			int cid = ds::math::random(0.0f, 4.99f);
			int offset = offset = cid * CELL_SIZE;
			Vector2f p = grid::convertFromGrid(x, y);
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
			m.start = grid::convertFromGrid(x, y + 20);
			m.end = grid::convertFromGrid(x, y);
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
	v2 mousePos = ds::renderer::getMousePosition();
	ds::Point converted = convert(mousePos);
	ctx->points.clear();
	if (converted.x >= 0 && converted.y >= 0) {
		MyEntry& me = ctx->grid->get(converted.x, converted.y);
		ctx->points.clear();
		ctx->grid->findMatchingNeighbours(converted.x, converted.y, ctx->points);
		if (ctx->points.size() > 1) {
			//ret = m_Points.size();
			for (size_t i = 0; i < ctx->points.size(); ++i) {
				const ds::Point& gp = ctx->points[i];
				MyEntry& c = ctx->grid->get(gp.x, gp.y);
				c.state = TS_SHRINKING;
				c.timer = 0.0f;
			}
			LOG << "flash count: " << ctx->points.size();
			return 1;
		}
		ctx->points.clear();
		return 0;
	}
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
					float norm = e.timer / ctx->settings->flashTTL;
					e.scale = 1.0f - norm * 0.9f;
				}
			}
		}
	}
	return 0;
}

int ShrinkState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				if (e.state == TS_SHRINKING) {
					ctx->grid->remove(x, y);
				}
			}
		}
	}
	return 0;
}
// -------------------------------------------------------
// Board
// -------------------------------------------------------
Board::Board(GameSettings* settings) : _settings(settings) {
	m_GridTex[0] = ds::math::buildTexture(200, 420, 430, 486);
	m_GridTex[1] = ds::math::buildTexture(200, 450, 320, 486);
	m_GridTex[2] = ds::math::buildTexture(200, 860, 110, 486);
	for (int i = 0; i < 5; ++i) {
		_cellTextures[i] = ds::math::buildTexture(680.0f, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
	}
	_context.grid = &m_Grid;
	_context.settings = settings;
	_states = new ds::StateManager(&_context);
	_states->add<MouseOverState>();
	_states->add<MovingCellsState>();
	_states->add<PrepareBoardState>();
	_states->add<SelectCellState>();
	_states->add<ShrinkState>();
	_states->addTransition(BM_FILLING,   0, BM_MOVING);
	_states->addTransition(BM_MOVING,    0, BM_RUNNING, _settings->droppingTTL);
	_states->addTransition(BM_SELECTION, 1, BM_FLASHING);
	_states->addTransition(BM_SELECTION, 0, BM_RUNNING);
	_states->addTransition(BM_FLASHING,  0, BM_RUNNING, _settings->flashTTL);
}

Board::~Board(void) {
	delete _states;
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Board::render() {

	ds::sprites::draw(Vector2f(295, 362), m_GridTex[0]);
	ds::sprites::draw(Vector2f(670, 362), m_GridTex[1]);
	ds::sprites::draw(Vector2f(885, 362), m_GridTex[2]);

	// pieces
	ds::Sprite sp;
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (!e.hidden) {
					ds::sprites::draw(grid::convertFromGrid(x, y),_cellTextures[e.color],0.0f,e.scale,e.scale);
				}
			}
		}
	}

	// moving cells
	for (size_t i = 0; i < _context.movingCells.size(); ++i) {
		const MovingCell& cell = _context.movingCells[i];
		ds::sprites::draw(cell.current, _cellTextures[cell.color]);
	}

}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Board::update(float elapsed) {

	_states->tick(elapsed);
	/*
	if (m_Mode == BM_FILLING) {
		m_Timer += elapsed;
		if (m_Timer > _settings->tweeningTTL) {
			m_Mode = BM_READY;
			m_Timer = 0.0f;
		}
	}
	else if (m_Mode == BM_FLASHING) {
		m_Timer += elapsed;
		if (m_Timer > _settings->flashTTL) {
			m_Mode = BM_READY;
			m_Timer = 0.0f;
			m_Grid.remove(m_Points,true);
			m_DroppedCells.clear();
			m_Grid.dropCells(m_DroppedCells);
			for (size_t i = 0; i < m_DroppedCells.size(); ++i) {
				ds::DroppedCell<MyEntry>* dc = &m_DroppedCells[i];
				MyEntry& e = m_Grid.get(dc->to.x, dc->to.y);
				e.hidden = true;
				MovingCell m;
				m.x = dc->to.x;
				m.y = dc->to.y;
				m.color = e.color;
				m.start = grid::convertFromGrid(dc->from.x, dc->from.y);
				m.end = grid::convertFromGrid(dc->to.x, dc->to.y);
				m_MovingCells.push_back(m);
			}
			if (!m_DroppedCells.empty()) {
				m_Mode = BM_MOVING;
				m_Timer = 0.0f;
			}			
		}
		
	}
	*/
	/*
	else if (m_Mode == BM_MOVING) {
		m_Timer += elapsed;
		if (m_Timer > _settings->droppingTTL) {
			m_Mode = BM_READY;
			m_Timer = 0.0f;
			for (size_t i = 0; i < m_MovingCells.size(); ++i) {
				MovingCell& m = m_MovingCells[i];
				MyEntry& e = m_Grid.get(m.x, m.y);
				e.hidden = false;
			}
			m_MovingCells.clear();
		}
		else {
			float norm = m_Timer / _settings->droppingTTL;
			for (size_t i = 0; i < m_MovingCells.size(); ++i) {
				MovingCell& m = m_MovingCells[i];
				m.current = tweening::interpolate(&tweening::easeOutQuad, m.start, m.end, m_Timer, _settings->droppingTTL);
			}
		}
	}
	*/
	/*
	else if (m_Mode == BM_READY) {
		v2 mousePos = ds::renderer::getMousePosition();
		int my = ((int)mousePos.y - STARTY + HALF_CELL_SIZE)/CELL_SIZE;
		int mx = ((int)mousePos.x - STARTX + HALF_CELL_SIZE)/CELL_SIZE;

		if (mx >= 0 && my >= 0) {
			if (mx != _selectedPiece.x || my != _selectedPiece.y) {
				_selectedPiece = Vector2i(mx,my);
				MyEntry& me = m_Grid(mx, my);
				if (me.state == TS_NORMAL) {
					me.timer = 0.0f;
					me.state = TS_WIGGLE;
				}
			}
		}
	}
	*/
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (e.state == TS_WIGGLE) {
					e.timer += elapsed;
					// FIXME: wiggleTTL
					if (e.timer >= 0.4f) {
						e.state = TS_NORMAL;
						e.scale = 1.0f;
					}
					else {
						float norm = e.timer / 0.4f;
						e.scale = 1.0f + sin(norm * TWO_PI * 2.0f) * 0.1f;
					}
				}
			}
		}
	}
}

// -------------------------------------------------------
// move
// -------------------------------------------------------
//void Board::move(const Vector2f& mousePos) {
	//Vector2i converted = grid::convertMousePosToGridPos(mousePos);
	//if ( converted.x > 0 && converted.y > 0) {
		//LOG << "move grid pos " << converted.x << " " << converted.y;
		//m_Grid.shiftColumns(converted.x);
	//}
//}

void Board::rebuild() {
	m_MovingCells.clear();
	_states->activate(BM_FILLING);
}
// -------------------------------------------------------
// Select
// -------------------------------------------------------
int Board::select(const Vector2f& mousePos) {
	if (_states->getCurrentMode() == BM_RUNNING) {
		_states->activate(BM_SELECTION);
	}
	int ret = -1;
	return ret;
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