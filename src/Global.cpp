// Global.cpp : Definisce il punto di ingresso dell'applicazione.
/*
How it works:
Every object has a position and the main character and all the enemies move in the space;
When it's time to render, the position of an object in the screen-space is calculated by the difference between its position and the main character position on X axis, and is
relative to a fixed point in the screen. This way the main character is fixed in that point and its movement results in moving all the world around him.
The center of the main character is specified by the constants CHARACTER_POS_X and CHARACTER_POS_Y
The game must be scaled based on the window size.
The scale factor is worked-out dividing the window height by 1080 which is the reference height;
This way the background image covers the entire height. Each object is scaled on X and Y based on the scale factor.
Each objects rendering position, that relative to the screen center, is scaled also on X axis.
The background images are printed in loop from left to right, till the entire screen in covered.
*/

#include "stdafx.h"
#include "Global.h"
#include "Game.h"
#include "resource.h"
#include "Background.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "GameObject.h"
#include "UserInterface.h"
#include "FlyWeightPlatform.h"
#include "Platform.h"
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds

using namespace sf;
using namespace std;
using namespace global;
using namespace userinterface;

namespace global {

	#define MAX_LOADSTRING 100
	
	const string musicPath = "Resources/Sounds/The-Hidden-Forest.ogg";
	const string fontPath = "Resources/digital_disco/DigitalDisco.ttf";
	const string tombstonePath = "Resources/Objects and Power Ups/Tombstone.png";
	const string firePath = "Resources/Characters/Hero - Wizard/Spriter File/Particles/Particle_1/Particles0003.png";

	const int US_PER_FRAME = 33300;
	const int HEIGHT_PIXEL = 1080; // height of virtual screen
	const float CHARACTER_POS_X = 0.30f; // main character X position relative to window width
	const float CHARACTER_POS_Y = 0.883f; // main character Y position relative to window height
	const int MAX_ENEMIES = 16;
	const int MAX_PLATFORMS = 16;

	// Global variables:
	HWND hWnd;                               // main window
	HINSTANCE hInst;                         // current instance
	WCHAR szTitle[MAX_LOADSTRING];           // title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];     // main window class name

	// Button inputs:
	bool aButton, sButton, dButton;
	bool enterButton, enterButtonDown, lastEnterButton;
	bool spaceButtonDown, lastSpaceButton;
	bool upArrowDown, downArrowDown, lastUpArrow, lastDownArrow;
	bool mLButton, mRButton;

	// Screen info
	float screenScale, mainCharacterX;
	int screenWidth, screenHeight;
	Vector2f screenCenter;

	// Game Data
	MainCharacter mainCharacter;
	Enemy enemies[MAX_ENEMIES];
	CharacterData* enemyData;
	Platform platforms[MAX_PLATFORMS];
	float yOrigins[4]{ 10, 10, 25, 18 }; // in pixels
	GamePhase gamePhase = GamePhase::WAITING_ENTER;

}

// File function declaration:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                createConsole();
float				getWindowScale(int* width, int* height);
void				getInput(bool clear = false);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Initialize application
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	// Create a new console, redirect input and output stream and print game instructions
	createConsole();

	// Load background textures
	if (!loadBackgroundTextures()) {
		printf("Failed loading background textures\n");
		return FALSE;
	}

	// Get window scale and screen x offset
	try {
		screenScale = getWindowScale(&screenWidth, &screenHeight);
		screenCenter = Vector2f(screenWidth * CHARACTER_POS_X, screenHeight * CHARACTER_POS_Y);
	}
	catch (int e) {
		printf("Press any key to continue...");
		getchar();
		return FALSE;
	}

	// open music
	Music music;
	if (music.openFromFile(musicPath)) {
		music.play();
		music.setLoop(true);
	}

	// load fonts
	userinterface::font.loadFromFile(fontPath);

	// create a new SFMLView for the rendering stuff	
	RenderWindow SFMLView(hWnd);
	SFMLView.setVerticalSyncEnabled(true);

	CharacterType lastEnemyType = CharacterType::WIZARD; // WIZARD for null
	CharacterData* wizardData = new CharacterData(CharacterType::WIZARD, yOrigins[0]);
	FlyWeightPlatform* flyWeightPlatform = new FlyWeightPlatform();
	Texture tombstoneTexture, fireTexture;
	tombstoneTexture.loadFromFile(tombstonePath);
	fireTexture.loadFromFile(firePath);
	Clock vClock;
	MSG msg;
	msg.message = static_cast<UINT>(~WM_QUIT);
	while (msg.message != WM_QUIT) {
		// reset clock
		vClock.restart();

		// Ciclo di messaggi principale:
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) break;

		// input system
		getInput();

		// process ui
		switch (gamePhase) {
			case GamePhase::WAITING_ENTER:
				if (!mainCharacter.isActive()) {
					(&mainCharacter)->~MainCharacter();
					new (&mainCharacter) MainCharacter(wizardData, screenCenter, &fireTexture);
				}
				if (enterButtonDown) {
					gamePhase = GamePhase::LEVEL_SELECTION;
				}
				getInput(true);
			break;
			case GamePhase::LEVEL_SELECTION:
				if (enterButtonDown) {
					gamePhase = GamePhase::LEVEL_START;
				}
				getInput(true);
			break;
			case GamePhase::LEVEL_START: {
				gamespace::bossCreated = false;
				// destroy all old enemies
				for (int i = 0; i < MAX_ENEMIES; i++) {
					if (enemies[i].isActive()) {
						enemies[i].setActive(false);
					}
				}
				// load textures for next enemy type
				CharacterType newEnemyType = (CharacterType)(userinterface::levelSelection + 1);
				if (newEnemyType != lastEnemyType) {
					delete enemyData;
					enemyData = new CharacterData(newEnemyType, yOrigins[userinterface::levelSelection + 1]);
					lastEnemyType = newEnemyType;
				}
				// create platforms
				int y = screenCenter.y;

				// this piece of code should be written better but...
				platforms[0] = Platform(flyWeightPlatform, Vector2f(1500, y - 150));
				platforms[1] = Platform(flyWeightPlatform, Vector2f(1900, y - 250));
				platforms[2] = Platform(flyWeightPlatform, Vector2f(2300, y - 350));
				platforms[3] = Platform(flyWeightPlatform, Vector2f(2700, y - 200));
				platforms[4] = Platform(flyWeightPlatform, Vector2f(3800, y - 150));
				platforms[5] = Platform(flyWeightPlatform, Vector2f(4250, y - 350));
				platforms[6] = Platform(flyWeightPlatform, Vector2f(3850, y - 450));
				platforms[7] = Platform(flyWeightPlatform, Vector2f(4300, y - 600));
				platforms[8] = Platform(flyWeightPlatform, Vector2f(5400, y - 200));
				platforms[9] = Platform(flyWeightPlatform, Vector2f(7000, y - 250));
				platforms[10] = Platform(flyWeightPlatform, Vector2f(7450, y - 400));
				platforms[11] = Platform(flyWeightPlatform, Vector2f(7900, y - 650));
				platforms[12] = Platform(flyWeightPlatform, Vector2f(8400, y - 200));
				platforms[13] = Platform(flyWeightPlatform, Vector2f(9000, y - 400));
				platforms[14] = Platform(flyWeightPlatform, Vector2f(8600, y - 600));
				platforms[15] = Platform(flyWeightPlatform, Vector2f(9800, y - 150));

				gamePhase = GamePhase::GAME_MODE;
				getInput(true);
			}
			break;
			case GamePhase::GAME_MODE:
				updateGame();
				if (!mainCharacter.isActive()) {
					gamePhase = GamePhase::GAME_END;
					gameWin = false;
				}
			break;
			case GamePhase::GAME_END:
				if ((alphaFading >= 255) && enterButtonDown) {
					int sel = userinterface::endSelection;
					// destroy all old enemies
					for (int i = 0; i < MAX_ENEMIES; i++) {
						if (enemies[i].isActive()) {
							enemies[i].setActive(false);
						}
					}
					// reset
					(&mainCharacter)->~MainCharacter();
					new (&mainCharacter) MainCharacter(wizardData, screenCenter, &fireTexture);
					if (sel == 0) {
						gamePhase = GamePhase::LEVEL_START;
					}
					else if (sel == 1) {
						gamePhase = GamePhase::WAITING_ENTER;
					}
					else if (sel == 2) {
						PostQuitMessage(0);
					}
				}
			break;
		}
		
		// update cycle
		if (mainCharacter.isActive())
			mainCharacter.update();
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemies[i].isActive())
				enemies[i].update();
		}

		// start rendering clearing view	
		SFMLView.clear(sf::Color::White);
		mainCharacterX = mainCharacter.getPosition().x;

		// draw background
		updateBackground(&SFMLView);
		
		// rendering cycle (platforms, main character, enemies)
		for (int i = 0; i < MAX_PLATFORMS; i++) {
			if (platforms[i].isActive())
				platforms[i].render(&SFMLView);
		}
		if (mainCharacter.isActive()) {
			mainCharacter.render(&SFMLView);
		}
		else {
			// tombstone
			Sprite sprite(tombstoneTexture);
			sprite.setOrigin(Vector2f(sprite.getTextureRect().width / 2.0f, sprite.getTextureRect().height - 13));

			sprite.setPosition(screenCenter);
			SFMLView.draw(sprite);
		}
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemies[i].isActive()) 
				enemies[i].render(&SFMLView);
		}

		// draw first layer
		updateBackground(&SFMLView, true);
	
		// draw UI
		renderUI(&SFMLView, gamePhase);

		// display the SFMLView
		SFMLView.display();

		this_thread::sleep_for(chrono::microseconds(US_PER_FRAME - vClock.getElapsedTime().asMicroseconds()));
	}	

	return 0;
}

//
//  FUNZIONE: MyRegisterClass()
//
//  SCOPO: Registra la classe di finestre.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNZIONE: InitInstance(HINSTANCE, int)
//
//   SCOPO: Salva l'handle di istanza e crea la finestra principale
//
//   COMMENTI:
//
//        In questa funzione l'handle di istanza viene salvato in una variabile globale e
//        viene creata e visualizzata la finestra principale del programma.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Archivia l'handle di istanza nella variabile globale

	int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP, 0, 0, width, height, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNZIONE: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  SCOPO: Elabora i messaggi per la finestra principale.
//
//  WM_COMMAND  - elabora il menu dell'applicazione
//  WM_PAINT    - Disegna la finestra principale
//  WM_DESTROY  - inserisce un messaggio di uscita e restituisce un risultato
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CHAR:
			switch (wParam) {
				case 0x1B: 
					// Esc
					PostQuitMessage(0);
				break;
			}
		break;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Aggiungere qui il codice di disegno che usa HDC...
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

void createConsole() {
	// create a console
	AllocConsole();
	std::wstring strW = L"Game instructions";
	SetConsoleTitle(strW.c_str());

	// connect output stream
	FILE *fpstdout = stdout;
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);

	// print game instructions
	printf("Game instructions:\n");
	printf("A -> run left\n");
	printf("D -> run right\n");
	printf("A + S -> walk left\n");
	printf("D + S -> walk right\n");
	printf("Enter -> attack\n");
	printf("Spacebar -> jump\n");
	printf("Esc -> quit game\n");
	printf("\n");

	system("pause");
	printf("Game started\n");

	FreeConsole();
}

float getWindowScale(int* width, int* height) {
	// Let's get screen size
	int heightRet = 0;
	RECT rect;
	if (GetWindowRect(global::hWnd, &rect)) {
		if (width)  *width  = rect.right - rect.left;
		if (height) *height = rect.bottom - rect.top;
		heightRet = rect.bottom - rect.top;
	}
	else {
		printf("Failed getting main window size\n");
		throw EXCEPTION_NONCONTINUABLE;
	}
	return (float)heightRet / HEIGHT_PIXEL;
}

void getInput(bool clear) {
	if (clear) {
		aButton = sButton = dButton = spaceButtonDown = enterButtonDown = enterButton = mLButton = mRButton = false;
		return;
	}

	// only if focused
	if (GetFocus() == hWnd) {
		// A-S-D
		aButton = Keyboard::isKeyPressed(Keyboard::A);
		sButton = Keyboard::isKeyPressed(Keyboard::S);
		dButton = Keyboard::isKeyPressed(Keyboard::D);
		// space
		spaceButtonDown = Keyboard::isKeyPressed(Keyboard::Space) && (!lastSpaceButton);
		lastSpaceButton = Keyboard::isKeyPressed(Keyboard::Space);
		// enter
		enterButtonDown = Keyboard::isKeyPressed(Keyboard::Enter) && (!lastEnterButton);
		lastEnterButton = enterButton = Keyboard::isKeyPressed(Keyboard::Enter);
		// arrows
		upArrowDown = Keyboard::isKeyPressed(Keyboard::Up) && (!lastUpArrow);
		downArrowDown = Keyboard::isKeyPressed(Keyboard::Down) && (!lastDownArrow);
		lastUpArrow = Keyboard::isKeyPressed(Keyboard::Up);
		lastDownArrow = Keyboard::isKeyPressed(Keyboard::Down);
		// get mouse input
		mLButton = Mouse::isButtonPressed(Mouse::Left);
		mRButton = Mouse::isButtonPressed(Mouse::Right);
	}
}