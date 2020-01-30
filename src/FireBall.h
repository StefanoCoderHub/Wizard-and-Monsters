#pragma once
#include "GameObject.h"

/*
FireBall is a GameObject but it's existance is know only by MainCharacter that uses it
*/
class FireBall : public GameObject {
public:
	FireBall() { };
	FireBall(sf::Vector2f position, sf::Texture* texture, bool isLeftOriented);
	~FireBall();

};

