#pragma comment(lib, "D11.lib")
#pragma comment(lib, "DieselCore.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "base\Settings.h"
#include <renderer\render_types.h>
#include "Constants.h"
#include <sprites\Sprite.h>
#include "Board.h"
#include "GameContext.h"
#include "HUD.h"

class Colors : public ds::BaseApp {

public:	
	Colors();
	virtual ~Colors() {
		delete _context;
	}	
	bool loadContent();
	const char* getTitle() {
		return "Colors";
	}
	bool initialize() {
		return true;
	}
	void init();
	void update(float dt);
	void render();
protected:
	void prepare(ds::Settings* settings);
private:
	GameContext* _context;
	bool _showSettings;
	HUD _hud;
	
};