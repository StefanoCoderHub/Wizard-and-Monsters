#pragma once
class Human {
public:
	float getHealth();
	void setHealth(float health);
	virtual void OnDie() = 0;

private:
	float mHealth;

};

