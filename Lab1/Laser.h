#pragma once
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace sf;

class ECEBuzzy;
class ECEEnemy;

class LaserBlast : public Sprite {
public:
	LaserBlast(int screenTop, int screenBottom, float buzzLaser=0.1, float enemyLaser=0.05);
	void spawnFromBuzzy(ECEBuzzy& buzz);
	void spawnFromEnemy(ECEEnemy& enemy);
	
	bool checkBuzzyCollision(ECEBuzzy& buzz);
	bool checkEnemyCollision(ECEEnemy& enemy);

	void updateLasers();

	void drawAll(RenderWindow& window) const;

	void clear();

	//const Vector2f getPosition() const;
	//LaserBlast(const ECEBuzzy& buzz);
	//LaserBlast(const ECEEnemy& buzz);
	//void updatePosition();
	//bool shouldBeRemoved() const;

private:
	//Vector holding position of each laser
	std::vector<Vector2f> buzzLasers;
	std::vector<Vector2f> enemyLasers;
	float buzzLaserVelocity;
	float enemyLaserVelocity;

	float topBound;
	float bottomBound;

	//Overwride draw from sf::Drawable
	//virtual void draw(RenderTarget& target, RenderStates states) const override;
};