#pragma once

#define N_CHARACTER 4
#define N_ACTIONS 7

namespace characterspace {
	extern int ACTIONS_SIZE[N_CHARACTER][N_ACTIONS];
	extern std::string CHARACTERS_PATHS[N_CHARACTER];
	extern std::string ACTIONS_PATHS[N_ACTIONS];

	extern float health[N_CHARACTER];
	extern float damage[N_CHARACTER];
	extern float walkSpeed[N_CHARACTER];
	extern float runSpeed[N_CHARACTER];
	extern float attackDistance[N_CHARACTER];
}

enum class CharacterType {
	WIZARD,
	MEDUSA,
	MINOTAUR,
	SKELETON,
};

enum class CharacterState {
	STATE_ATTACKING,
	STATE_CROUCHING,
	STATE_DYING,
	STATE_HITTED,
	STATE_JUMPING,
	STATE_RUNNING,
	STATE_WALKING,
	STATE_WAITING,
};

class CharacterData {
public:
	CharacterData(CharacterType characterType, float yOrigin);
	~CharacterData();
	sf::Texture* getTexture(CharacterState characterState, int nFrame);
	CharacterType getCharacterType();
	float getYOrigin();
	float getHealth();
	float getDamage();
	float getWalkSpeed();
	float getRunSpeed();
	float getAttackDistance();
	int getAttackFrame();
	int getJumpFrame();

private:
	sf::Texture* mActionsTextures[N_ACTIONS];
	CharacterType mCharacterType;
	float mYOrigin; // Y axis origin from bottom in pixels
};

