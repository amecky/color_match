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
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Colors::loadContent() {	
	_context = new GameContext;
	_context->settings.load();
	_context->headColor = ds::Color::WHITE;

	ds::game::add_game_object(new HUD());

	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu"));
	addGameState(new ds::BasicMenuGameState("Credits", "Credits"));
	addGameState(new MainGameState(_context));
	//addGameState(new HighscoreState(gui, _context));
	connectGameStates("MainMenu", 1, "MainGameState");
	connectGameStates("MainGameState", 1, "MainGameState");
	connectGameStates("MainGameState", 8, "MainMenu");
	connectGameStates("MainMenu", 3, "Credits");
	connectGameStates("MainMenu", 4, "Credits");
	connectGameStates("Credits", 1, "MainMenu");
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
	addShortcut("Fade to gray", 'f', BE_FADING);

	ds::vm::Script* ctx = ds::res::getScript("MoveSomething");
	ctx->execute();
	LOG << "--------------------> " << ctx->getRegister(4);

	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void Colors::init() {
	pushState("MainGameState");
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Colors::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Colors::render() {	
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->draw(v2(512, 734), math::buildTexture(720,   0, 1024, 68));
	sprites->draw(v2(512, 34), math::buildTexture(800, 0, 1024, 68));
}
