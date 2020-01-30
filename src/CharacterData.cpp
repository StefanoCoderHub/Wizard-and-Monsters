#include "stdafx.h"
#include "CharacterData.h"

using namespace sf;
using namespace std;
using namespace characterspace;

namespace characterspace {

	int ACTIONS_SIZE[N_CHARACTER][N_ACTIONS]{
		{  9, 11, 16, 24,  7, 16, 24 },
		{ 24, 12, 24, 12, 13, 24, 36 },
		{ 30, 12, 30, 12, 12, 24, 36 },
		{ 20,  8, 20,  8,  8, 20, 24 },
	};
	std::string CHARACTERS_PATHS[N_CHARACTER]{
		"Hero - Wizard/PNG/FW_Hero_1_",
		"Monster - Medusa/PNG/FW_Meduza_",
		"Monster - Minotaur/PNG/FW_Minotaur_",
		"Monster - Skeleton/PNG/FW_Skeleton_",
	};
	std::string ACTIONS_PATHS[N_ACTIONS]{
		"Attack__",
		"Crouching__",
		"Die__",
		"Hit__",
		"Jumping__",
		"Running__",
		"Walking__",
	};

	float health[N_CHARACTER] { 100, 100, 150, 100 };
	float damage[N_CHARACTER] {  60,   5,   8,   7 };
	float walkSpeed[N_CHARACTER] {  6, 4, 6, 6 };
	float runSpeed[N_CHARACTER]  { 12.7f, 7, 12.1f, 12.1f };
	float attackDistance[N_CHARACTER]  { 60, 85, 125, 75 };

	int attackFrame[N_CHARACTER] { 4, 13, 18, 10 };
	int jumpFrame[N_CHARACTER]   { 6, 12, 11,  7 };

}

CharacterData::CharacterData(CharacterType charType, float yOrigin) {
	// set character info
	mCharacterType = charType;
	mYOrigin = yOrigin;
	// load character textures
	int chType = (int)charType;
	for (int i = 0; i < N_ACTIONS; i++) {
		mActionsTextures[i] = new Texture[ACTIONS_SIZE[chType][i]];
		for (int f = 0; f < ACTIONS_SIZE[chType][i]; f++) {
			char fileName[8];
			sprintf_s(fileName, "%03d.png", f);
			(*(mActionsTextures[i] + f)).loadFromFile("Resources/Characters/" + CHARACTERS_PATHS[chType] + ACTIONS_PATHS[i] + string(fileName));
		}
	}
}

CharacterData::~CharacterData() {
	for (int i = 0; i < N_ACTIONS; i++) {
		delete[] mActionsTextures[i];
	}
}

sf::Texture* CharacterData::getTexture(CharacterState characterState, int nFrame) {
	if (characterState == CharacterState::STATE_WAITING) {
		return (mActionsTextures[(int)CharacterState::STATE_WALKING] + 0);
	}
	return (mActionsTextures[(int)characterState] + nFrame);
}

CharacterType CharacterData::getCharacterType() {
	return mCharacterType;
}

float CharacterData::getYOrigin() {
	return mYOrigin;
}

float CharacterData::getHealth() {
	return health[(int)mCharacterType];
}

float CharacterData::getDamage() {
	return damage[(int)mCharacterType];
}

float CharacterData::getWalkSpeed() {
	return walkSpeed[(int)mCharacterType];
}

float CharacterData::getRunSpeed() {
	return runSpeed[(int)mCharacterType];
}

float CharacterData::getAttackDistance() {
	return attackDistance[(int)mCharacterType];
}

int CharacterData::getAttackFrame() {
	return attackFrame[(int)mCharacterType];
}

int CharacterData::getJumpFrame() {
	return jumpFrame[(int)mCharacterType];
}
