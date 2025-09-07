#pragma once
#include <SFML/Graphics.hpp>
#include <cstdio>

using namespace sf;

class ECEBuzzy;
class ECEEnemy;

class LaserBlast : public Sprite {
public:
	LaserBlast(const ECEBuzzy& buzz);
	LaserBlast(const ECEEnemy& buzz);
	const Vector2f getPosition() const;
	void updatePosition();
	bool shouldBeRemoved() const;

private:
	Vector2f position;
	float yVelocity;
	float topBound;
	float bottomBound;

	//Overwride draw from sf::Drawable
	virtual void draw(RenderTarget& target, RenderStates states) const override;
};