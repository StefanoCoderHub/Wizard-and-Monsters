#include "stdafx.h"
#include "Human.h"

float Human::getHealth() {
	return mHealth;
}

void Human::setHealth(float health) {
	mHealth = health;
	if (mHealth <= 0) {
		OnDie();
	}
}