#pragma once
#include "..\Constants.h"
#include "..\GameContext.h"
#include <base\GameState.h>
#include "..\Board.h"
#include <effects\GrayFadeRenderEffect.h>

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
	void stopGame();
	GameContext* _context;
	Board* _board;
	ds::Texture _gridTex[3];
	ds::GrayFadeEffect* _effect;
	bool _running;
	float _timer;
};

