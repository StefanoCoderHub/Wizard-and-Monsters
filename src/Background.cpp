#include "stdafx.h"
#include "Background.h"
#include "Global.h"
#include <cmath>

using namespace sf;
using namespace std;
using namespace global;
using namespace background;

namespace background {

	#define BACKGROUND_INERTIA 0.14f
	const string paths[8]{
		"Resources/Background and Tilesets/Forest/BG_1.png",
		"Resources/Background and Tilesets/Forest/BG_2_Clouds.png",
		"Resources/Background and Tilesets/Forest/BG_3_Mountains.png",
		"Resources/Background and Tilesets/Forest/BG_4_Forest_3.png",
		"Resources/Background and Tilesets/Forest/BG_5_Forest_2.png",
		"Resources/Background and Tilesets/Forest/BG_6_Forest_1.png",
		"Resources/Background and Tilesets/Forest/BG_7_Walking_Platform.png",
		"Resources/Background and Tilesets/Forest/BG_8_Foreground.png",
	};

	Texture backgroundTextures[8];
	bool texturesLoaded = false;

}

bool loadBackgroundTextures() {
	// Load some textures to display
	for (int i = 0; i < 8; i++) {
		if (!backgroundTextures[i].loadFromFile(paths[i])) return false;
		backgroundTextures[i].setRepeated(true);
	}
	texturesLoaded = true;
	return true;
}

void updateBackground(RenderWindow* SFMLView, bool firstLayer) {
	if (!texturesLoaded) return;

	// from furthest to nearest
	for (int i = (firstLayer ? 7 : 0); i < (firstLayer ? 8 : 7); i++) {
		// create the sprite
		Sprite sprite(backgroundTextures[i]);

		// slide the background appropriately
		float speed = 1.0f - (6 - i) * BACKGROUND_INERTIA; // the sixth layer moves along with character
		int xPos = -mainCharacterX * speed;

		int xTex = sprite.getTextureRect().width * screenScale;
		// get the negative remainder
		xPos = xPos % xTex;
		if (xPos > 0) xPos -= xTex;

		// set origin and scale
		sprite.setOrigin(Vector2f(0, 0));
		sprite.setScale(Vector2f(screenScale, screenScale));

		// draw the background repeatedly from left to right
		for (int x = xPos; x < screenWidth; x += xTex) {
			sprite.setPosition(Vector2f(x, 0));
			SFMLView->draw(sprite);
		}
	}
	
}