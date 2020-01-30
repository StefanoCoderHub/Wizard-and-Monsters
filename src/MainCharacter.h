#pragma once
#include "Character.h"
#include "GameObject.h"
#include "Human.h"
#include "IObserver.h"
#include "FireBall.h"

class MainCharacter : public GameObject, public Human, public IObserver {
public:
	MainCharacter() { };
	MainCharacter(CharacterData* characterData, sf::Vector2f pos, sf::Texture* fireTexture);
	~MainCharacter();
	virtual void update() override;
	virtual void render(sf::RenderWindow* SFMLView) override;
	virtual void OnDie() override;
	virtual void OnEvent(eventsystem::EventType eventType) override;
	CharacterData* getCharacterData();

private:
	Character* mCharacter;
	CharacterData* mCharacterData;
	sf::Texture* mFireTexture;
	static const sf::Vector2f fireballVel;
	static const int MAX_FIREBALLS = 8;
	FireBall mFireBalls[MAX_FIREBALLS];
		
};
