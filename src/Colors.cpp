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
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu", gui));
	addGameState(new ds::BasicMenuGameState("Intro", "Intro", gui));
	addGameState(new ds::BasicMenuGameState("Credits", "Credits", gui));
	addGameState(new TestGUIState());
	addGameState(new MainGameState(_context));
	addGameState(new HighscoreState(gui, _context));
	addGameState(new GameOverState(gui, _context));
	connectGameStates("MainMenu", 1, "MainGameState");
	connectGameStates("MainGameState", 1, "GameOverState");
	_showSettings = false;
	_context->hud = gui->get("hud");
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
}

// -------------------------------------------------------
// On char 
// -------------------------------------------------------
void Colors::OnChar(char ascii,unsigned int keyState) {
	if (ascii == 's') {
		_showSettings = !_showSettings;
	}
	if (ascii == '1') {
		activate("TestGUIState");
	}
	if (ascii == '2') {
		deactivate("TestGUIState");
	}
	/*
	if ( ascii == 't' ) {
		showMessage(490);
	}
	if ( ascii == 'd' ) {
		m_Board.debug();
	}
	if (ascii == 'f') {
		m_Fading = !m_Fading;
		m_GrayScaleTimer = 0.0f;
	}
	if ( ascii == 'r' ) {
		m_Board.rebuild();
		resetScore();
	}
	*/
}



