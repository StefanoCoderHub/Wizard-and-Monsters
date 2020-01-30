#include "stdafx.h"
#include "MainCharacter.h"
#include "Global.h"
#include "GameObject.h"
#include "Character.h"
#include "Game.h"
#include <windows.h>

using namespace sf;
using namespace std;
using namespace global;

const sf::Vector2f MainCharacter::fireballVel = Vector2f(28.0f, 0);

MainCharacter::MainCharacter(CharacterData* characterData, Vector2f pos, Texture* fireTexture) {
	mCharacterData = characterData;
	mCharacter = new Character(characterData);
	mCharacter->addObserver(this);
	setHealth(characterData->getHealth());
	setPosition(pos);
	setYOrigin(characterData->getYOrigin());
	mFireTexture = fireTexture;
	setActive(true);
}

MainCharacter::~MainCharacter() {
	delete mCharacter;
}

void MainCharacter::update() {
	// move character
	Vector2f newPos = getPosition();
	if ((!sButton) && aButton && (!dButton)) {
		// run left
		newPos.x -= mCharacterData->getRunSpeed();
		setLeftOriented(true);
		if (mCharacter->getCharacterState() == CharacterState::STATE_WAITING)
			mCharacter->startAction(CharacterState::STATE_RUNNING, false, false, true);
	}
	if ((!sButton) && (!aButton) && dButton) {
		// run right
		newPos.x += mCharacterData->getRunSpeed();
		setLeftOriented(false);
		if (mCharacter->getCharacterState() == CharacterState::STATE_WAITING)
			mCharacter->startAction(CharacterState::STATE_RUNNING, false, false, true);
	}
	if (sButton && aButton && (!dButton)) {
		// walk left
		newPos.x -= mCharacterData->getWalkSpeed();
		setLeftOriented(true);
		if (mCharacter->getCharacterState() == CharacterState::STATE_WAITING)
			mCharacter->startAction(CharacterState::STATE_WALKING, false, false, true);
	}
	if (sButton && (!aButton) && dButton) {
		// walk right
		newPos.x += mCharacterData->getWalkSpeed();
		setLeftOriented(false);
		if (mCharacter->getCharacterState() == CharacterState::STATE_WAITING)
			mCharacter->startAction(CharacterState::STATE_WALKING, false, false, true);
	}
	setPosition(newPos);
	if (enterButtonDown) {
		// attack
		mCharacter->startAction(CharacterState::STATE_ATTACKING, true, false, false);
	}
	if (spaceButtonDown) {
		// jump
		if (isOverSomething(*this)) {
			mCharacter->startAction(CharacterState::STATE_JUMPING, true, false, false);
		}
	}

	// update frame image
	setTexture(mCharacter->update());

	// update fireballs
	for (int i = 0; i < MAX_FIREBALLS; i++) {
		if (mFireBalls[i].isActive()) {
			mFireBalls[i].setPosition(mFireBalls[i].getPosition() + (mFireBalls[i].isLeftOriented() ? -fireballVel : fireballVel));
			if (applyAttack(*mCharacterData, mFireBalls[i].getPosition(), mFireBalls[i].isLeftOriented(), true, false)) {
				mFireBalls[i].setActive(false);
			}
			Vector2f screenPosition = mFireBalls[i].getScreenPosition();
			if ((screenPosition.x > screenWidth + 70) || (screenPosition.x < -70) || (screenPosition.y > screenHeight + 70) || (screenPosition.y < -70)) {
				mFireBalls[i].setActive(false);
			}
		}
	}
}

void MainCharacter::render(sf::RenderWindow* SFMLView) {
	GameObject::render(SFMLView);
	// render fireballs
	for (int i = 0; i < MAX_FIREBALLS; i++) {
		if (mFireBalls[i].isActive()) {
			mFireBalls[i].render(SFMLView);
		}
	}
}

void MainCharacter::OnDie() {
	setActive(false);
}

void MainCharacter::OnEvent(eventsystem::EventType eventType) {
	if (eventType == eventsystem::EventType::ATTACK) {
		for (int i = 0; i < MAX_FIREBALLS; i++) {
			if (!mFireBalls[i].isActive()) {
				Vector2f startingPos = getPosition() + Vector2f(isLeftOriented() ? -20.0f : 20.0f, -42.0f);
				(&mFireBalls[i])->~FireBall();
				new (&mFireBalls[i]) FireBall(startingPos, mFireTexture, isLeftOriented());
				break;
			}
		}
	}
	else if (eventType == eventsystem::EventType::JUMP) {
		applyJump(*this);
	}
}

CharacterData* MainCharacter::getCharacterData() {
	return mCharacterData;
}