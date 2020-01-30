#pragma once
#include "CharacterData.h"
#include "Observable.h"

class Character : public Observable {
public:
	Character(CharacterData* characterData);
	~Character();
	void startAction(CharacterState characterState, bool resetPrevAction, bool keepDoing, bool stopDoing);
	sf::Texture* update();
	sf::Texture* getTexture();
	CharacterState getCharacterState();

private:
	CharacterData* mCharacterData;
	CharacterState mCharacterState;
	int mCurrentFrame;
	bool mKeepDoing; // repeat action after finish it
	bool mStopDoing; // stop doing the action after the next update; don't reset the frame counter

};

