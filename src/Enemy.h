#pragma once
#include "Character.h"
#include "GameObject.h"
#include "Human.h"
#include "IObserver.h"

class Enemy : public GameObject, public Human, public IObserver {
public:
	Enemy() { };
	Enemy(CharacterData* characterData, sf::Vector2f pos, bool boss);
	~Enemy();
	virtual void update() override;
	virtual void OnDie() override;
	virtual void OnEvent(eventsystem::EventType eventType) override;

private:
	Character* mCharacter;
	CharacterData* mCharacterData;

	static const float bossSize;
	bool mIsBoss;
	float mInitialHealth; // initial health for red color

	static const int maxWalkingFrame = 130, maxRunningFrame = 180;
	static const int minWalkingFrame = 80, minRunningFrame = 80;
	bool mIsCurrentlyWalking;
	int mFrameActionCounter; // frame left to do walking or running
	
};

