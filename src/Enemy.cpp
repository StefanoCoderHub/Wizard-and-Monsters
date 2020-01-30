#include "stdafx.h"
#include "Enemy.h"
#include "Global.h"
#include "Game.h"
#include "UserInterface.h"

using namespace sf;
using namespace global;

const float Enemy::bossSize = 1.5f;

Enemy::Enemy(CharacterData* characterData, Vector2f pos, bool boss) {
	mCharacterData = characterData;
	mCharacter = new Character(characterData);
	mCharacter->addObserver(this);
	mInitialHealth = characterData->getHealth() * (boss ? 8.0f : 1.0f);
	setHealth(mInitialHealth);
	if (boss) setScale(Vector2f(bossSize, bossSize));
	setPosition(pos);
	setYOrigin(characterData->getYOrigin());
	mIsBoss = boss;
	mIsCurrentlyWalking = false;
	mFrameActionCounter = (rand() % (maxRunningFrame - minRunningFrame)) + minRunningFrame;
	setActive(true);
}

Enemy::~Enemy() {
	delete mCharacter;
}

void Enemy::update() {
	float attackDistance = mCharacterData->getAttackDistance() * (mIsBoss ? 1.5f : 1.0f);
	Vector2f dist = getPosition() - mainCharacter.getPosition();
	if ((-attackDistance < dist.x) && (dist.x < attackDistance) && (dist.y < gamespace::HEIGHT_ATTACK) && (dist.y > -gamespace::HEIGHT_ATTACK)) {
		// attack player
		setLeftOriented(dist.x > 0);
		mCharacter->startAction(CharacterState::STATE_ATTACKING, false, false, true);
	}
	else {
		if (mIsCurrentlyWalking) {
			// walk toward player
			if ((dist.x > 30) || (dist.x < -30)) setLeftOriented(dist.x > 0);
			Vector2f direction = isLeftOriented() ? Vector2f(-mCharacterData->getWalkSpeed(), 0) : Vector2f(mCharacterData->getWalkSpeed(), 0);
			setPosition(getPosition() + direction);
			mCharacter->startAction(CharacterState::STATE_WALKING, false, false, true);
			// decrement counter
			if (--mFrameActionCounter <= 0) {
				mIsCurrentlyWalking = false;
				mFrameActionCounter = (rand() % (maxRunningFrame - minRunningFrame)) + minRunningFrame;
			}
		}
		else {
			// run toward player
			if ((dist.x > 30) || (dist.x < -30)) setLeftOriented(dist.x > 0);
			Vector2f direction = isLeftOriented() ? Vector2f(-mCharacterData->getRunSpeed(), 0) : Vector2f(mCharacterData->getRunSpeed(), 0);
			setPosition(getPosition() + direction);
			mCharacter->startAction(CharacterState::STATE_RUNNING, false, false, true);
			// decrement counter
			if (--mFrameActionCounter <= 0) {
				mIsCurrentlyWalking = true;
				mFrameActionCounter = (rand() % (maxWalkingFrame - minWalkingFrame)) + minWalkingFrame;
			}
		}
	}

	setTexture(mCharacter->update());
	float value = getHealth() * (255.0f / mInitialHealth);
	setColor(Color(255, value, value, 255));
}

void Enemy::OnDie() {
	setActive(false);
	if (mIsBoss) {
		gamePhase = userinterface::GamePhase::GAME_END;
		userinterface::gameWin = true;
	}
}

void Enemy::OnEvent(eventsystem::EventType eventType) {
	if (eventType == eventsystem::EventType::ATTACK) {
		applyAttack(*mCharacterData, getPosition(), isLeftOriented(), false, mIsBoss);
	}
	else if (eventType == eventsystem::EventType::JUMP) {
		applyJump(*this);
	}
}