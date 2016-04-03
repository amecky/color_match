#include "Colors.h"
#include "utils\Log.h"
#include <utils\font.h>
#include <renderer\shader.h>
#include <utils\StringUtils.h>
#include "Map.h"
#include <renderer\shader.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include <sprites\SpriteBatch.h>
#include "gamestates\HighscoreState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\TestGUI.h"

ds::BaseApp *app = new Colors(); 

Colors::Colors() : ds::BaseApp() {	
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Colors::loadContent() {	
	_context = new GameContext;
	_context->settings.load();
	_context->headColor = ds::Color::WHITE;
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu", gui));
	addGameState(new ds::BasicMenuGameState("Intro", "Intro", gui));
	addGameState(new ds::BasicMenuGameState("Credits", "Credits", gui));
	addGameState(new TestGUIState());
	addGameState(new MainGameState(_context));
	addGameState(new HighscoreState(gui, _context));
	addGameState(new GameOverState(gui, _context));
	connectGameStates("MainMenu", 1, "MainGameState");
	connectGameStates("MainGameState", 1, "GameOverState");
	connectGameStates("GameOverState", 1, "MainGameState");
	_showSettings = false;
	_context->hud = gui->get("hud");

	addShortcut("Show TestGUI", '1', GE_SHOW_TEST_GUI);
	addShortcut("Hide TestGUI", '2', GE_HIDE_TEST_GUI);
	addShortcut("Rebuild Board", 'r', GE_REBUILD_BOARD);
	addShortcut("End Game", 'e', GE_END_GAME);
	addShortcut("Fade to gray", 'f', GE_FADE_TO_GRAY);
	addShortcut("Toggle Board", 'b', GE_TOGGLE_SHOW_BOARD);
	addShortcut("Toggle States", 's', GE_TOGGLE_SHOW_STATES);
	addShortcut("Toggle Settings", 't', GE_TOGGLE_SETTINGS);
	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void Colors::init() {
	activate("TestGUIState");
}
// -------------------------------------------------------
// On GUI button
// -------------------------------------------------------
void Colors::onGUIButton(ds::DialogID dlgID,int button) {
	LOG << "dialog " << dlgID << " button " << button;
	if (dlgID == 1 && button == 2) {
		shutdown();
	}
}

// -------------------------------------------------------
// process events
// -------------------------------------------------------
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

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Colors::update(float dt) {
	/*
	if (m_Mode == GM_WAITING) {
		m_Timer += dt;
		if (m_Timer >= m_Settings->tweeningTTL) {
			m_Mode = GM_RUNNING;
		}
	}
	if (m_Mode == GM_RUNNING) {
		m_HUD.tick(dt);
	}
	*/
	/*
	if ( m_Message.isActive()) {
		m_PointMessage.timer += gameTime.elapsed;
		ds::Color clr = ds::Color::WHITE;
		float sn = sin(m_PointMessage.timer / MESSAGE_TTL * ds::PI);
		clr.a = sn;
		clr.r = sn;
		m_Message.setColor(m_PointMessage.id,clr);
		float scale = 1.0f + sn * MESSAGE_SCALE;
		m_Message.setScale(m_PointMessage.id,scale);
		Vector2f p = m_PointMessage.position;

		p.x = p.x + m_PointMessage.deltaX * (1.0f - scale) /  MESSAGE_SCALE;
		p.y = p.y + m_PointMessage.deltaY * (1.0f - scale) /  MESSAGE_SCALE;

		m_Message.setPosition(m_PointMessage.id,p);
		if ( m_PointMessage.timer > MESSAGE_TTL ) {
			m_Message.setActive(false);
		}
	}
	*/
	//m_Board.update(dt);
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Colors::draw() {	
	ds::sprites::draw(v2(512, 734), ds::math::buildTexture(720,   0, 1024, 68));
	ds::sprites::draw(v2(235, 734), ds::math::buildTexture(  0, 340,  316, 20), 0.0f, 1.0f, 1.0f, _context->headColor);
	ds::sprites::draw(v2(512,  34), ds::math::buildTexture(800,   0, 1024, 68));
}
