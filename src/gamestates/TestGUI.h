#pragma once
#include "..\Constants.h"
#include "..\GameContext.h"
#include <base\GameState.h>

class TestGUIState : public ds::GameState {

public:
	TestGUIState();
	~TestGUIState();
	int update(float dt);
	void render();
	int onGUIButton(ds::DialogID dlgID, int button);
	void activate();
	void deactivate();
private:
	ds::Array<const char*> _dropDownItems;
	int _dropdownState;
	int _dropdownSelection;
	bool _check;
	float _stepValue;
	ds::Color _color;
	int _colorState;
};

