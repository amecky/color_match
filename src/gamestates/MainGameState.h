#pragma once
#include "..\Constants.h"
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include "..\Board.h"
#include <postprocess\GrayFadePostProcess.h>

class MainGameState : public ds::GameState {

	enum GameMode {
		GM_RUNNING,
		GM_OVER
	};

public:
	MainGameState(GameContext* context);
	~MainGameState();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	void activate();
	void deactivate();
	//int onChar(int ascii);
	//int processEvents(const ds::EventStream& events);
private:
	void stopGame();
	GameContext* _context;
	Board* _board;
	ds::Texture _gridTex[3];
	HUD* _hud;
	ds::GUIDialog* _gameOver;
	float _timer;
	GameMode _mode;
	ds::GrayFadePostProcess* _process;
};

