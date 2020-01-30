#include "stdafx.h"
#include "GameObject.h"
#include "Global.h"

using namespace sf;

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

void GameObject::update() {

}

void GameObject::render(sf::RenderWindow* SFMLView) {
	// create the sprite; the sprite has origin in its lower center
	Sprite sprite(*mTexture);
	sprite.setOrigin(Vector2f(sprite.getTextureRect().width / 2.0f, sprite.getTextureRect().height - mYOrigin));
	sprite.setColor(mColor);

	// set scale
	sprite.setScale(Vector2f((mIsLeftOriented ? -1 : 1) * global::screenScale * mScale.x, global::screenScale * mScale.y));

	// draw the sprite relative to the main character offset from center
	Vector2f pos = getScreenPosition();
	sprite.setPosition(pos);
	SFMLView->draw(sprite);
}

void GameObject::setActive(bool enabled) {
	mEnabled = enabled;
}

bool GameObject::isActive() {
	return mEnabled;
}

void GameObject::setPosition(sf::Vector2f position) {
	mPosition = position;
}

sf::Vector2f GameObject::getPosition() {
	return mPosition;
}

sf::Vector2f GameObject::getScreenPosition() {
	Vector2f offset(global::mainCharacterX - mPosition.x, global::screenCenter.y - mPosition.y);
	offset = offset * global::screenScale;
	return Vector2f(global::screenCenter.x - offset.x, global::screenCenter.y - offset.y);
}

void GameObject::setScale(sf::Vector2f scale) {
	mScale = scale;
}

sf::Vector2f GameObject::getScale() {
	return mScale;
}

void GameObject::setLeftOriented(bool isLeftOriented) {
	mIsLeftOriented = isLeftOriented;
}

bool GameObject::isLeftOriented() {
	return mIsLeftOriented;
}

void GameObject::setYOrigin(float yOrigin) {
	mYOrigin = yOrigin;
}

float GameObject::getYOrigin() {
	return mYOrigin;
}

void GameObject::setTexture(sf::Texture* texture) {
	mTexture = texture;
}

sf::Texture* GameObject::getTexture() {
	return mTexture;
}

void GameObject::setColor(sf::Color color) {
	mColor = color;
}

sf::Color GameObject::getColor() {
	return mColor;
}

void GameObject::setVelocity(sf::Vector2f velocity) {
	mVelocity = velocity;
}

sf::Vector2f GameObject::getVelocity() {
	return mVelocity;
}