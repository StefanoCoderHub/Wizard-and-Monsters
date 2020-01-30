#include "stdafx.h"
#include "FlyWeightPlatform.h"

using namespace sf;
using namespace std;

FlyWeightPlatform::FlyWeightPlatform() {
	string paths = "Resources/Background and Tilesets/Forest_Platform_1/Ground_Platforms_";
	for (int i = 24; i < 27; i++) {
		char fileName[7];
		sprintf_s(fileName, "%02d.png", i);
		if (!mTextures[i - 24].loadFromFile(paths + string(fileName))) {
			printf("Failed loading platform textures");
		}
	}
}

FlyWeightPlatform::~FlyWeightPlatform() {

}

sf::Texture* FlyWeightPlatform::getTexture(int index) {
	return &(mTextures[index]);
}
