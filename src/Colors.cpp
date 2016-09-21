#include "Colors.h"
#include <core\log\Log.h>
#include <utils\font.h>
#include <core\string\StringUtils.h>
#include "Map.h"
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include <renderer\sprites.h>
#include "gamestates\HighscoreState.h"
#include <core\script\vm.h>

ds::BaseApp *app = new Colors(); 

Colors::Colors() : ds::BaseApp() {	
}

// -------------------------------------------------------
// prepare
// -------------------------------------------------------
void Colors::prepare(ds::Settings* settings) {
	settings->screenWidth = 1024;
	settings->screenHeight = 768;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
	settings->logTypes = ds::LogTypes::LT_CONSOLE | ds::LogTypes::LT_FILE;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Colors::loadContent() {	
	_context = new GameContext;
	_context->settings.load();
	_context->headColor = ds::Color::WHITE;
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu"));
	//addGameState(new ds::BasicMenuGameState("Intro", "Intro"));
	//addGameState(new ds::BasicMenuGameState("Credits", "Credits"));
	addGameState(new MainGameState(_context));
	//addGameState(new HighscoreState(gui, _context));
	connectGameStates("MainMenu", 1, "MainGameState");
	connectGameStates("MainGameState", 1, "MainGameState");
	//connectGameStates("MainGameState", 1, "GameOverState");
	//connectGameStates("GameOverState", 1, "MainGameState");
	_showSettings = false;
	/*
	addShortcut("Show TestGUI", '1', GE_SHOW_TEST_GUI);
	addShortcut("Hide TestGUI", '2', GE_HIDE_TEST_GUI);
	addShortcut("Rebuild Board", 'r', GE_REBUILD_BOARD);
	addShortcut("End Game", 'e', GE_END_GAME);
	addShortcut("Fade to gray", 'f', GE_FADE_TO_GRAY);
	addShortcut("Toggle Board", 'b', GE_TOGGLE_SHOW_BOARD);
	addShortcut("Toggle States", 's', GE_TOGGLE_SHOW_STATES);
	addShortcut("Toggle Settings", 't', GE_TOGGLE_SETTINGS);
	*/
	RID material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);

	addShortcut("Stop game", 'e', BE_STOP_GAME);
	addShortcut("Test script", 't', BE_TEST_SCRIPT);

	ds::vm::Script* ctx = ds::res::getScript("MoveSomething");
	ctx->execute();
	LOG << "--------------------> " << DBG_V4(ctx->getRegister(4));

	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void Colors::init() {
	activate("MainMenu");
}
// -------------------------------------------------------
// On GUI button
// -------------------------------------------------------
/*
void Colors::onGUIButton(ds::DialogID dlgID,int button) {
	LOG << "dialog " << dlgID << " button " << button;
	if (dlgID == 1 && button == 2) {
		shutdown();
	}
}
*/
// -------------------------------------------------------
// process events
// -------------------------------------------------------
/*
void Colors::processEvents(const ds::EventStream& events) {
	for (uint32_t i = 0; i < events.num(); ++i) {
		uint32_t type = events.getType(i);
		switch (type) {
			case GE_SHOW_TEST_GUI: activate("TestGUIState"); break;
			case GE_HIDE_TEST_GUI: deactivate("TestGUIState"); break;
			case GE_TOGGLE_SETTINGS: _showSettings = !_showSettings; break;
		}
	}
}
*/
// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Colors::update(float dt) {
	_context->hud.tick(dt);
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Colors::render() {	
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->draw(v2(512, 734), math::buildTexture(720,   0, 1024, 68));
	sprites->draw(v2(512, 34), math::buildTexture(800, 0, 1024, 68));
	_context->hud.render();
}
