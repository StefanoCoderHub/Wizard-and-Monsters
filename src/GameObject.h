#pragma once

class GameObject {
public:
	GameObject();
	~GameObject();
	virtual void update();
	virtual void render(sf::RenderWindow* SFMLView);

	void setActive(bool enabled);
	bool isActive();

	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition();
	sf::Vector2f getScreenPosition();

	void setScale(sf::Vector2f scale);
	sf::Vector2f getScale();

	void setLeftOriented(bool isLeftOriented);
	bool isLeftOriented();

	void setYOrigin(float yOrigin);
	float getYOrigin();

	void setTexture(sf::Texture* texture);
	sf::Texture* getTexture();

	void setColor(sf::Color color);
	sf::Color getColor();
	
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getVelocity();

private:
	// status property
	bool mEnabled = false;

	// transform property
	sf::Vector2f mPosition; // the position of a GameObject is relative to a 1920x1080 screen
	sf::Vector2f mScale = sf::Vector2f(1.0f, 1.0f);
	bool mIsLeftOriented = false;
	float mYOrigin = 0.0f; // Y axis origin from bottom in pixels
		
	// renderer property
	sf::Texture* mTexture; // texture of the GameObject
	sf::Color mColor = sf::Color::White; // color of the GameObject

	// rigid-body property
	sf::Vector2f mVelocity;

};

