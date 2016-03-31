#pragma once
#include <lib\Grid.h>
#include <utils\Color.h>
#include <sprites\Sprite.h>
#include "Constants.h"
#include "GameSettings.h"
#include <utils\StateManager.h>

const ds::Point INVALID_POINT(-1, -1);
// -------------------------------------------------------
// Color grid
// -------------------------------------------------------
enum TileState {
	TS_NORMAL,
	TS_WIGGLE,
	TS_SHRINKING
};

struct MyEntry {

	int color;
	bool hidden;
	float scale;
	float timer;
	TileState state;
};

struct MovingCell {

	int x;
	int y;
	v2 start;
	v2 end;
	v2 current;
	int color;
};

class ColorGrid : public ds::Grid<MyEntry> {

public:
	ColorGrid() : ds::Grid<MyEntry>(MAX_X, MAX_Y) {}
	virtual ~ColorGrid() {}
	bool isMatch(const MyEntry& first, const MyEntry& right) {
		return first.color == right.color;
	}
};

typedef ds::Array<ds::Sprite> Highlights;
typedef ds::Array<ds::Point> Points;
typedef ds::Array<ds::DroppedCell<MyEntry>> DroppedCells;
typedef ds::Array<MovingCell> MovingCells;

// -------------------------------------------------------
// Board context
// -------------------------------------------------------
struct BoardContext : public ds::StateContext {
	ColorGrid* grid;
	DroppedCells droppedCells;
	Points points;
	GameSettings* settings;
	MovingCells movingCells;
};

enum BoardMode {
	BM_UNKNOWN,
	BM_RUNNING,
	BM_SELECTION,
	BM_FILLING,
	BM_FLASHING,
	BM_DROPPING,
	BM_MOVING,
	BM_READY
};

// -------------------------------------------------------
// Mouse over state (wiggle)
// -------------------------------------------------------
class MouseOverState : public ds::State {

public:
	MouseOverState() : ds::State(), _selectedEntry(-1, -1), _lastUpdate(-1, -1) {}
	virtual ~MouseOverState() {}
	int activate();
	int update(float dt);
	int getMode() const {
		return BM_RUNNING;
	}
	const char* getName() const {
		return "MouseOverState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_PERMANENT;
	}
private:
	ds::Point _selectedEntry;
	ds::Point _lastUpdate;
};

// -------------------------------------------------------
// Moving cells state
// -------------------------------------------------------
class MovingCellsState : public ds::State {

public:
	MovingCellsState() : ds::State() {}
	virtual ~MovingCellsState() {}
	int deactivate();
	int update(float dt);
	int getMode() const {
		return BM_MOVING;
	}
	const char* getName() const {
		return "MovingCellsState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_TRANSIENT;
	}
};

// -------------------------------------------------------
// Prepare board state
// -------------------------------------------------------
class PrepareBoardState : public ds::State {

public:
	PrepareBoardState() : ds::State() {}
	virtual ~PrepareBoardState() {}
	int activate();
	int getMode() const {
		return BM_FILLING;
	}
	const char* getName() const {
		return "PrepareBoardState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_ONETIME;
	}
};

// -------------------------------------------------------
// Select cell state
// -------------------------------------------------------
class SelectCellState : public ds::State {

public:
	SelectCellState() : ds::State() {}
	virtual ~SelectCellState() {}
	int activate();
	int getMode() const {
		return BM_SELECTION;
	}
	const char* getName() const {
		return "SelectCellState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_ONETIME;
	}
};

// -------------------------------------------------------
// Select cell state
// -------------------------------------------------------
class ShrinkState : public ds::State {

public:
	ShrinkState() : ds::State() {}
	virtual ~ShrinkState() {}
	int update(float dt);
	int deactivate();
	int getMode() const {
		return BM_FLASHING;
	}
	const char* getName() const {
		return "ShrinkState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_TRANSIENT;
	}
};

// -------------------------------------------------------
// Dropping cells state
// -------------------------------------------------------
class DroppingCellsState : public ds::State {

public:
	DroppingCellsState() : ds::State() {}
	virtual ~DroppingCellsState() {}
	int activate();
	int update(float dt);
	int deactivate();
	int getMode() const {
		return BM_DROPPING;
	}
	const char* getName() const {
		return "DroppingCellsState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_TRANSIENT;
	}
};
// -------------------------------------------------------
// Board
// -------------------------------------------------------
class Board {

public:
	Board(GameSettings* settings);
	virtual ~Board();
	void select();
	int getMovesLeft() {
		return 100;
	}
	int update(float elasped);
	void render();
	void debug();
	void rebuild();
	void showDialog();
	void toggleShowBoard() {
		_showBoard = !_showBoard;
	}
	void toggleShowStates() {
		_showStates = !_showStates;
	}
private:
	void drawGrid();
	void drawFillGrid();

	ColorGrid m_Grid;
	
	ds::Texture _cellTextures[5];
	GameSettings* _settings;
	BoardContext _context;
	ds::StateManager* _states;
	int _dialogState;
	v2 _dialogPos;
	bool _showBoard;
	bool _showStates;
	
};

