#pragma once
#include "GameObject.h"
#include "FlyWeightPlatform.h"

class Platform : public GameObject {
public:
	Platform();
	Platform(FlyWeightPlatform* flyWeightPlatform, sf::Vector2f position);
	~Platform();
	virtual void render(sf::RenderWindow* SFMLView) override;
	void getSizeInfo(float& startY, float& endY, float& startX, float& endX);

private:
	FlyWeightPlatform* mFlyWeightPlatform;
	float mStartY, mEndY, mStartX, mEndX;

};

