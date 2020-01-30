#pragma once

class FlyWeightPlatform {
public:
	FlyWeightPlatform();
	~FlyWeightPlatform();
	sf::Texture* getTexture(int index);

private:
	sf::Texture mTextures[3];

};

