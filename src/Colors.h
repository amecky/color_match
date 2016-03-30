#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include "base\Settings.h"
#include <renderer\render_types.h>
#include "Constants.h"
#include <sprites\Sprite.h>
#include "Board.h"
#include "GameContext.h"

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
	void init();
	void OnChar(char ascii,unsigned int keyState);
	void update(float dt);
	void draw();
	void onGUIButton(ds::DialogID dlgID,int button);
	
private:
	GameContext* _context;
	bool _showSettings;
	
};