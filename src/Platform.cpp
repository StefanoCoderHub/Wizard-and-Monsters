#include "stdafx.h"
#include "Platform.h"
#include "Global.h"

using namespace sf;
using namespace std;

Platform::Platform() {

}

Platform::Platform(FlyWeightPlatform* flyWeightPlatform, Vector2f position) {
	mFlyWeightPlatform = flyWeightPlatform;
	setPosition(position);
	setScale(Vector2f(0.8f, 0.8f));
	setActive(true);
}

Platform::~Platform() {
	
}

void Platform::render(sf::RenderWindow* SFMLView) {
	// create the sprite; the sprite has origin in its lower center
	Sprite sprite(*(mFlyWeightPlatform->getTexture(1)));
	sprite.setOrigin(Vector2f(sprite.getTextureRect().width / 2.0f, getYOrigin()));

	// set scale
	sprite.setScale(global::screenScale * getScale().x, global::screenScale * getScale().y);

	// draw the sprite relative to the main character offset from center
	Vector2f offset(global::mainCharacterX - getPosition().x, global::screenCenter.y - getPosition().y);
	offset = offset * global::screenScale;
	Vector2f pos(global::screenCenter.x - offset.x, global::screenCenter.y - offset.y);
	sprite.setPosition(pos);
	SFMLView->draw(sprite);

	sprite.setTexture(*(mFlyWeightPlatform->getTexture(0)));
	sprite.setPosition(pos - Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, 0));
	SFMLView->draw(sprite);

	sprite.setTexture(*(mFlyWeightPlatform->getTexture(2)));
	sprite.setPosition(pos + Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, 0));
	SFMLView->draw(sprite);

	// save platform size
	pos = getPosition();
	mStartY = pos.y;
	mEndY   = pos.y + sprite.getLocalBounds().height * sprite.getScale().y * 0.5f;
	mStartX = pos.x - sprite.getLocalBounds().width * sprite.getScale().x * 1.5f;
	mEndX   = pos.x + sprite.getLocalBounds().width * sprite.getScale().x * 1.5f;

}

void Platform::getSizeInfo(float& startY, float& endY, float& startX, float& endX) {
	startY = mStartY;
	endY   = mEndY;
	startX = mStartX;
	endX   = mEndX;
}
