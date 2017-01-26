#include "BoardStates.h"
#include "Board.h"
#include "Map.h"
#include <base\InputSystem.h>
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
			m.scale = v2(1, 1);
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
	int shrinking = 0;
	_timer += dt;
	ctx->script->set(SID("TIMER"), v4(_timer));
	const ds::vm::Method& m = ctx->script->getMethod(SID("shrink"));
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				if (e.state == TS_SHRINKING) {
					ctx->script->execute(m);
					float norm = ctx->script->getRegister(1).x;
					e.scale = ctx->script->getRegister(3).x;
					if (norm < 1.0f) {
						++shrinking;
					}
				}
			}
		}
	}
	if (shrinking == 0) {
		return 1;
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
