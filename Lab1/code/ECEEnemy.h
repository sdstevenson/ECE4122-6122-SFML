#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdio>


using namespace sf;

class LaserBlast;

class ECEEnemy : public Sprite {
public:
	ECEEnemy(int textureOption, const Vector2f setPos, const Texture& texture);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	bool shouldBeRemoved(const LaserBlast& laser) const;
	bool atLeftWall() const;
	bool atRightWall() const;
	Vector2f getPosition() const;
	float getScaledWidth() const;
	float getScaledHeight() const;

private:
	Vector2f position;
	Vector2f scale;
	float scaledWidth;
	float scaledHeight;
	Vector2f velocity;
};