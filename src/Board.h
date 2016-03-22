#pragma once
#include <lib\Grid.h>
#include <utils\Color.h>
#include <sprites\Sprite.h>
#include "Constants.h"
#include "GameSettings.h"

// -------------------------------------------------------
// Color grid
// -------------------------------------------------------
enum TileState {
	TS_NORMAL,
	TS_WIGGLE,
	TS_SHRINKING
};

struct MyEntry {

	ds::Texture texture;
	int color;
	bool hidden;
	float scale;
	float timer;
	TileState state;
};

struct MovingCell {

	int x;
	int y;
	Vector2f start;
	Vector2f end;
	Vector2f current;
	ds::Texture texture;
};

class ColorGrid : public ds::Grid<MyEntry> {

public:
	ColorGrid() : ds::Grid<MyEntry>(MAX_X, MAX_Y) {}
	virtual ~ColorGrid() {}
	bool isMatch(const MyEntry& first, const MyEntry& right) {
		return first.color == right.color;
	}
};

//class MovingCells;
// -------------------------------------------------------
// Board
// -------------------------------------------------------
class Board {

enum BoardMode {
	BM_FILLING,
	BM_FLASHING,
	BM_MOVING,
	BM_READY
};

typedef ds::Array<ds::Sprite> Highlights;
typedef ds::Array<ds::Point> Points;
typedef ds::Array<ds::DroppedCell<MyEntry>> DroppedCells;
typedef ds::Array<MovingCell> MovingCells;

public:
	Board();
	virtual ~Board();
	void fill(int maxColors);
	int select(const Vector2f& mousePos);
	void move(const Vector2f& mousePos);
	int getMovesLeft() {
		return 100;
	}
	void init();
	void update(float elasped);
	void render();
	void debug();
	void debugContainer();
	void rebuild();
	void setGameSettings(GameSettings* settings) {
		m_Settings = settings;
	}
private:
	void drawGrid();
	void drawFillGrid();
	ID createTile(int layer,const Vector2f& pos, const ds::Rect& rect,const ds::Color& clr = ds::Color::WHITE);

	ColorGrid m_Grid;
	Points m_Points;
	DroppedCells m_DroppedCells;
	MovingCells m_MovingCells;
	BoardMode m_Mode;
	float m_Timer;
	int m_CellCounter;
	ds::Texture m_GridTex[3];
	int m_Counter;
	GameSettings* m_Settings;
	Vector2i _selectedPiece;
	int _flashCount;
	
};

