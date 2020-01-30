#include "stdafx.h"
#include "UserInterface.h"
#include "Global.h"

using namespace sf;
using namespace std;
using namespace global;
using namespace userinterface;

namespace userinterface {
	Font font;

	const int fadingInc = 5;
	int alphaFading = 255;
	int direction = -1;

	Color highlightColor = Color(0, 150, 0, 255);
	int levelSelection = 0;

	const string heartPath = "Resources/Objects and Power Ups/Heart.png";
	Texture heartTexture;
	bool lifeTextureLoaded = false;

	bool gameWin;
	int endSelection = 0;

	const int fadingIncEnd = 5;
}

void renderUI(sf::RenderWindow* SFMLView, GamePhase gamePhase) {
	sf::Text text;
	text.setFont(font);
	switch (gamePhase) {
		case GamePhase::WAITING_ENTER:
			text.setString("Wizard & Monsters");
			text.setCharacterSize(102); // in pixels, not points!
			text.setFillColor(Color::Blue);
			text.setOutlineColor(Color::Green);
			text.setOutlineThickness(4);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.22f));
			SFMLView->draw(text);

			text.setString("Press enter to start...");
			text.setCharacterSize(32); // in pixels, not points!
			text.setFillColor(Color(0, 0, 0, alphaFading));
			text.setOutlineThickness(0);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.92f));
			SFMLView->draw(text);

			alphaFading += direction * fadingInc;
			if (alphaFading >= 255) {
				alphaFading = 255;
				direction = -1;
			}
			if (alphaFading <= 30) {
				alphaFading = 30;
				direction = 1;
			}
		break;
		case GamePhase::LEVEL_SELECTION:
			if (downArrowDown && (levelSelection < 2)) {
				levelSelection++;
			}
			if (upArrowDown && (levelSelection > 0)) {
				levelSelection--;
			}

			text.setString(" Level 1 - Medusa ");
			text.setCharacterSize(36); // in pixels, not points!
			text.setFillColor(Color::Blue);
			text.setOutlineColor(levelSelection == 0 ? highlightColor : Color::Green);
			text.setOutlineThickness(2);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.42f));
			SFMLView->draw(text);

			text.setString("Level 2 - Minotaur");
			text.setFillColor(Color::Blue);
			text.setOutlineColor(levelSelection == 1 ? highlightColor : Color::Green);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.5f));
			SFMLView->draw(text);

			text.setString("Level 3 - Skeleton");
			text.setFillColor(Color::Blue);
			text.setOutlineColor(levelSelection == 2 ? highlightColor : Color::Green);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.58f));
			SFMLView->draw(text);
		break;
		case GamePhase::LEVEL_START:
			text.setString("Loading textures, please wait...");
			text.setCharacterSize(32); // in pixels, not points!
			text.setFillColor(Color(0, 0, 0, 255));
			text.setOutlineThickness(0);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.92f));
			SFMLView->draw(text);
		break;
		case GamePhase::GAME_MODE:
			if (!lifeTextureLoaded) {
				heartTexture.loadFromFile(heartPath);
				lifeTextureLoaded = true;
			}
			else {
				// print one heart for each 20 points of life
				float health = mainCharacter.getHealth();
				if (health > 0) {
					Sprite sprite(heartTexture);
					sprite.setOrigin(Vector2f(0, 0));
					sprite.setScale(Vector2f(0.4f, 0.4f));

					Vector2f pos(25, 25);
					for (; health >= 20; health -= 20) {
						sprite.setPosition(pos);
						SFMLView->draw(sprite);
						pos.x += sprite.getTextureRect().width * 0.46f;
					}

					// print what remain
					sprite.setPosition(pos);
					sprite.setColor(Color(255, 255, 255, 255 * (health / 20.0f)));
					SFMLView->draw(sprite);
				}	
			}
			alphaFading = 0;
		break;
		case GamePhase::GAME_END:
			if (gameWin) {
				text.setString("You Win!");
			}
			else {
				text.setString("You Lose");
			}
			
			text.setCharacterSize(86); // in pixels, not points!
			text.setFillColor(Color(0, 0, 255, alphaFading));
			text.setOutlineColor(Color(0, 255, 0, alphaFading));
			text.setOutlineThickness(4);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);

			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.22f));
			SFMLView->draw(text);

			if (downArrowDown && (endSelection < 2)) {
				endSelection++;
			}
			if (upArrowDown && (endSelection > 0)) {
				endSelection--;
			}
			highlightColor = Color(0, 150, 0, alphaFading);

			text.setString("Repeat game");
			text.setCharacterSize(36); // in pixels, not points!
			text.setFillColor(Color(0, 0, 255, alphaFading));
			text.setOutlineColor(endSelection == 0 ? highlightColor : Color(0, 255, 0, alphaFading));
			text.setOutlineThickness(2);
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.42f));
			SFMLView->draw(text);

			text.setString("Back to menu");
			text.setOutlineColor(endSelection == 1 ? highlightColor : Color(0, 255, 0, alphaFading));
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.5f));
			SFMLView->draw(text);

			text.setString(" Quit game ");
			text.setOutlineColor(endSelection == 2 ? highlightColor : Color(0, 255, 0, alphaFading));
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(Vector2f(screenWidth * 0.5f, screenHeight * 0.58f));
			SFMLView->draw(text);

			alphaFading += fadingIncEnd;
			if (alphaFading >= 255) {
				alphaFading = 255;
			}
		break;
	}
}