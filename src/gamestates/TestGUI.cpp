#include "TestGUI.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

const char* ITEMS[] = { "item1", "item2", "item3", "item4", "item5", "item6", "item7" };

TestGUIState::TestGUIState() : ds::GameState("TestGUIState") {
	_dropDownItems.push_back("Entry 1");
	_dropDownItems.push_back("Entry 2");
	_dropDownItems.push_back("Entry 3");
	_dropDownItems.push_back("Entry 4");
	_dropDownItems.push_back("Entry 5");
	_dropDownItems.push_back("Entry 6");
	_dropDownItems.push_back("Entry 7");
	_dropDownItems.push_back("Entry 8");
	_dropdownState = 0;
	_dropdownSelection = -1;
}


TestGUIState::~TestGUIState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void TestGUIState::activate() {
	
}

// --------------------------------------------
// activate
// --------------------------------------------
void TestGUIState::deactivate() {
}

// --------------------------------------------
// update
// --------------------------------------------
int TestGUIState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int TestGUIState::onGUIButton(ds::DialogID dlgID, int button) {	
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void TestGUIState::render() {
	v2 p = v2(300, 500);
	int state = 1;
	float f = 100.0f;
	int i = 123;
	v2 v2(500, 400);
	ds::Color clr(128, 32, 200, 192);
	gui::start(1, &p);
	gui::begin("Test",&state);
	gui::Button("Button");
	gui::Text("This is %d and %3.2f", i, f);
	gui::InputFloat("Float", &f);
	gui::InputInt("Int", &i);
	gui::InputVec2("Vec2", &v2);	
	gui::Separator();
	gui::Label("Label", "%g %g", v2.x, v2.y);
	gui::Value("Value", v2);
	gui::InputColor("Color", &clr);
	gui::DropDownBox(ITEMS, 7, &_dropdownSelection, &_dropdownState);
	gui::end();
}


