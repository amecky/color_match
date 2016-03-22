#pragma once
#include "..\Constants.h"
#include "..\GameContext.h"
#include <base\GameState.h>
#include "..\Board.h"

class MainGameState : public ds::GameState {

public:
	MainGameState(GameContext* context);
	~MainGameState();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	void activate();
	void deactivate();
	int onChar(int ascii);
private:
	GameContext* _context;
	Board* _board;
};

