#include "stdafx.h"
#include "FireBall.h"

using namespace sf;

FireBall::FireBall(sf::Vector2f position, Texture* texture, bool isLeftOriented) {
	setPosition(position);
	setTexture(texture);
	setLeftOriented(isLeftOriented);
	setActive(true);
}

FireBall::~FireBall() {

}