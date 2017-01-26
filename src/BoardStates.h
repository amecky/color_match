#pragma once
#include <core\utils\StateManager.h>

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
	p2i _selectedEntry;
	p2i _lastUpdate;
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
	int activate() {
		_timer = 0.0f;
		return 0;
	}
	int getMode() const {
		return BM_FLASHING;
	}
	const char* getName() const {
		return "ShrinkState";
	}
	ds::StateBehavior getBehavior() const {
		return ds::SB_PERMANENT;
	}
private:
	float _timer;
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
