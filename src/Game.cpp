#include "stdafx.h"
#include "Game.h"
#include "Global.h"
#include "Enemy.h"
#include "Platform.h"
#include <stdlib.h>
#include <time.h>

using namespace sf;
using namespace global;
using namespace gamespace;

namespace gamespace {
	bool bossCreated = false;
	const int PIXEL_GAME_LENGTH = 1920 * 10;
	const int FRAME_PER_SECOND = 30;
	const int HEIGHT_ATTACK = 75;
	Vector2f gravity(0, 1.2f); // down direction
	Vector2f jumpAcceleration(0, -23); // up direction

}

bool applyAttack(CharacterData& characterData, Vector2f position, bool leftDirection, bool isAttackerPlayer, bool isBoss) {
	if (isAttackerPlayer) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemies[i].isActive()) {
				Vector2f dist = position - enemies[i].getPosition();
				dist.y += 42.0f;
				if ((dist.y < HEIGHT_ATTACK) && (dist.y > -HEIGHT_ATTACK)) {
					// same height
					if (((dist.x < characterData.getAttackDistance()) && (dist.x > 0) && (leftDirection)) || ((dist.x > -characterData.getAttackDistance()) && (dist.x < 0) && (!leftDirection))) {
						// right direction, right distance
						enemies[i].setHealth(enemies[i].getHealth() - characterData.getDamage());
						return true;
					}
				}
			}
		}
	}
	else {
		if (mainCharacter.isActive()) {
			Vector2f dist = position - mainCharacter.getPosition();
			if ((dist.y < HEIGHT_ATTACK) && (dist.y > -HEIGHT_ATTACK)) {
				// same height
				float attackDistance = characterData.getAttackDistance() * (isBoss ? 2.0f : 1.0f);
				if (((dist.x < attackDistance) && (dist.x > 0) && (leftDirection)) || ((dist.x > -attackDistance) && (dist.x < 0) && (!leftDirection))) {
					// right direction, right distance
					mainCharacter.setHealth(mainCharacter.getHealth() - characterData.getDamage() * (isBoss ? 2.0f : 1.0f));
					return true;
				}
			}
		}
	}
	return false;
}

void applyJump(GameObject& jumper) {
	jumper.setVelocity(jumper.getVelocity() + jumpAcceleration);
	jumper.setPosition(jumper.getPosition() + jumper.getVelocity());
}

void updateGame() {
	// create enemies
	int nSecondsForOneEnemy = 3;
	int nFramesForOneEnemy = FRAME_PER_SECOND * nSecondsForOneEnemy;
	if ((!bossCreated) && (mainCharacterX >= PIXEL_GAME_LENGTH)) {
		// boss
		addEnemy(Vector2f(mainCharacterX + screenWidth, screenCenter.y), true);
		bossCreated = true;
	}
	else if ((rand() % nFramesForOneEnemy) == 0) {
		// mini-boss
		addEnemy(Vector2f(mainCharacterX + screenWidth, screenCenter.y));
	}

	// give health to main character
	float health = mainCharacter.getHealth();
	if (health <= mainCharacter.getCharacterData()->getHealth()) {
		health += 1.5f / FRAME_PER_SECOND;
		if (health > mainCharacter.getCharacterData()->getHealth()) health = 100.0f;
		mainCharacter.setHealth(health);
	}

	// apply gravity and velocity
  	updateCharacterPosition(mainCharacter);
	for (int i = 0; i < MAX_ENEMIES; i++) {
		updateCharacterPosition(enemies[i]);
	}
}

void updateCharacterPosition(GameObject& character) {
	if (character.isActive()) {
		float height;
		if (isOverSomething(character, &height) && (character.getVelocity().y >= 0)) {
			character.setVelocity(Vector2f(0, 0));
			character.setPosition(Vector2f(character.getPosition().x, height));
		}
		else {
			character.setVelocity(character.getVelocity() + gravity);
			character.setPosition(character.getPosition() + character.getVelocity());
		}
	}
}

// return true if under the character there is something
bool isOverSomething(GameObject& character, float* height) {
	// over land
	if (character.getPosition().y >= screenCenter.y) {
		if (height) *height = screenCenter.y;
		return true;
	}

	Vector2f pos = character.getPosition();
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if (platforms[i].isActive()) {
			float startY, endY, startX, endX;
			platforms[i].getSizeInfo(startY, endY, startX, endX);
			if ((pos.y >= startY) && (pos.y <= endY) && (pos.x >= startX) && (pos.x <= endX)) {
				// is inside platform
				if (height) *height = startY;
				return true;
			}
		}
	}

	return false;
}	

void addEnemy(Vector2f pos, bool boss) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (!enemies[i].isActive()) {
			(&enemies[i])->~Enemy();
			new (&enemies[i]) Enemy(enemyData, pos, boss);
			break;
		}
	}
}