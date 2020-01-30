#pragma once
#include "CharacterData.h"
#include "GameObject.h"

namespace gamespace {
	extern bool bossCreated;
	extern const int HEIGHT_ATTACK;
}

bool applyAttack(CharacterData& characterData, sf::Vector2f position, bool leftDirection, bool isAttackerPlayer, bool isBoss);
void applyJump(GameObject& jumper);
void updateCharacterPosition(GameObject& character);
bool isOverSomething(GameObject& character, float* height = nullptr);
void updateGame();
void addEnemy(sf::Vector2f pos, bool boss = false);