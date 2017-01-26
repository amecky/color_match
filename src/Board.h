#pragma once
#include <core\lib\Grid.h>
#include <core\graphics\Color.h>
#include <sprites\Sprite.h>
#include "Constants.h"
#include "GameSettings.h"
#include <core\utils\StateManager.h>
#include "GameContext.h"
#include <core\script\vm.h>

const p2i INVALID_POINT(-1, -1);
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
	v2 scale;
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
typedef ds::Array<p2i> Points;
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
	ds::vm::Script* script;
};


// -------------------------------------------------------
// Board
// -------------------------------------------------------
class Board {

	struct Selection {
		p2i grid;
		float timer;
		bool active;
	};

public:
	Board(GameContext* context);
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
	Selection _selection;
	ds::Texture _cellTexture;
	GameContext* _ctx;
	BoardContext _context;
	ds::StateManager* _states;
	int _dialogState;
	v2 _dialogPos;
	bool _showBoard;
	bool _showStates;
	ds::vm::Script* _wiggleScript;
	
};

