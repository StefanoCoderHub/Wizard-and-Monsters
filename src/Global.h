#pragma once
#include "MainCharacter.h"
#include "Enemy.h"
#include "Platform.h"
#include "UserInterface.h"

namespace global {

	extern const int US_PER_FRAME;
	extern const int HEIGHT_PIXEL;
	extern const float CHARACTER_POS_X;
	extern const float CHARACTER_POS_Y;
	extern const int MAX_ENEMIES;
	extern const int MAX_PLATFORMS;

	// Variabili globali:
	extern HWND hWnd;                 // finestra principale
	extern HINSTANCE hInst;           // istanza corrente
	extern WCHAR szTitle[];           // Testo della barra del titolo
	extern WCHAR szWindowClass[];     // nome della classe della finestra principale

	// Button inputs:
	extern bool aButton, sButton, dButton;
	extern bool enterButton, enterButtonDown, lastEnterButton;
	extern bool spaceButtonDown, lastSpaceButton;
	extern bool upArrowDown, downArrowDown, lastUpArrow, lastDownArrow;
	extern bool mLButton, mRButton;

	// Screen info
	extern float screenScale, mainCharacterX;
	extern int screenWidth, screenHeight;
	extern sf::Vector2f screenCenter;

	// Game Data
	extern MainCharacter mainCharacter;
	extern Enemy enemies[];
	extern CharacterData* enemyData;
	extern Platform platforms[];
	extern userinterface::GamePhase gamePhase;
}