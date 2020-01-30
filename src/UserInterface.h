#pragma once

namespace userinterface {
	extern sf::Font font;
	extern int levelSelection;
	
	enum class GamePhase {
		WAITING_ENTER,
		LEVEL_SELECTION,
		LEVEL_START,
		GAME_MODE,
		GAME_END,
	};

	extern bool gameWin;
	extern int endSelection;

	extern int alphaFading;
}

void renderUI(sf::RenderWindow* SFMLView, userinterface::GamePhase gamePhase);

