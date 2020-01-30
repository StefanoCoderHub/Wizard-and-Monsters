#include "stdafx.h"
#include "Character.h"

using namespace sf;

Character::Character(CharacterData* characterData) {
	mCharacterData = characterData;
	mCharacterState = CharacterState::STATE_WAITING;
	mCurrentFrame = 0;
	mKeepDoing = false;
}

Character::~Character() {

}

// start a new action
void Character::startAction(CharacterState characterState, bool resetPrevAction, bool keepDoing, bool stopDoing) {
	mCharacterState = characterState;
	if (resetPrevAction) mCurrentFrame = 0;
	mKeepDoing = keepDoing;
	mStopDoing = stopDoing;
}

// move on to the next frame
Texture* Character::update() {
	Texture* tex = getTexture();
	// notify if attack made
	if (mCharacterState == CharacterState::STATE_ATTACKING) {
		if (mCurrentFrame == mCharacterData->getAttackFrame()) {
			OnEvent(eventsystem::EventType::ATTACK);
		}
	}
	// notify if jump made
	if (mCharacterState == CharacterState::STATE_JUMPING) {
		if (mCurrentFrame == mCharacterData->getJumpFrame()) {
			OnEvent(eventsystem::EventType::JUMP);
		}
	}
	if (mCharacterState != CharacterState::STATE_WAITING) {
		// update action image
		mCurrentFrame++;
		if (mCurrentFrame >= characterspace::ACTIONS_SIZE[(int)mCharacterData->getCharacterType()][(int)mCharacterState]) {
			// action ended
			mCurrentFrame = 0;
		}
		if (mStopDoing || ((mCurrentFrame == 0) && (!mKeepDoing))) {
			mCharacterState = CharacterState::STATE_WAITING;
		}
	}
	return tex;
}

Texture* Character::getTexture() {
	return mCharacterData->getTexture(mCharacterState, mCurrentFrame);
}

CharacterState Character::getCharacterState() {
	return mCharacterState;
}
