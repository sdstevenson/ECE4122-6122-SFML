#include "Laser.h"
#include "ECEBuzzy.h"
#include "ECEEnemy.h"

using namespace sf;

LaserBlast::LaserBlast(int screenTop, int screenBottom, float buzzLaser, float enemyLaser) {
	topBound = screenTop;
	bottomBound = screenBottom;
	buzzLaserVelocity = buzzLaser;
	enemyLaserVelocity = enemyLaser;
}

void LaserBlast::spawnFromBuzzy(ECEBuzzy& buzz) {
	const Vector2f buzzPosition = buzz.getPosition();

	buzzLasers.push_back(Vector2f(buzzPosition.x + 40, 160));
}

void LaserBlast::spawnFromEnemy(ECEEnemy& enemy) {
	const Vector2f enemyPosition = enemy.getPosition();

	enemyLasers.push_back(Vector2f(enemyPosition.x - 40, enemyPosition.y));
}

bool LaserBlast::checkBuzzyCollision(ECEBuzzy& buzz) {
	for (int i = 0; i < enemyLasers.size(); i++) {
		Vector2f position = buzz.getPosition();

		float minY = enemyLasers.at(i).y - 20;
		float maxY = enemyLasers.at(i).y;
		float x = enemyLasers.at(i).x;

		//Check if in the right y range
		if (minY < position.y + buzz.getScaledWidth()) {
			if (x > position.x && x < (position.x + buzz.getScaledWidth())) {
				//If so, we have a collisions. Remove laser and return true.
				enemyLasers.erase(enemyLasers.begin() + i);
				return true;
			}
		}
	}
	return false;
}

bool LaserBlast::checkEnemyCollision(ECEEnemy& enemy) {
	for (int i = 0; i < buzzLasers.size(); ++i) {
		Vector2f position = enemy.getPosition();

		float minY = buzzLasers.at(i).y - 20;
		float maxY = buzzLasers.at(i).y;
		float x = buzzLasers.at(i).x;

		//Check if in the right y range
		if (maxY > position.y && minY < (position.y + enemy.getScaledHeight())) {
			//Check if in the right x range
			if (x > position.x && x < (position.x + enemy.getScaledWidth())) {
				//If so, we have a collisions. Remove laser and return true.
				buzzLasers.erase(buzzLasers.begin() + i);
				return true;
			}
		}
	}
	return false;
}

void LaserBlast::updateLasers() {
	std::vector<int> buzzLasersToRemove;
	std::vector<int> enemyLasersToRemove;
	for (int i = 0; i < buzzLasers.size(); ++i) {
		Vector2f& currLaser = buzzLasers.at(i);
		currLaser.y += buzzLaserVelocity;
		if (currLaser.y >= bottomBound) {
			buzzLasersToRemove.push_back(i);
		}
	}

	for (int i = 0; i < enemyLasers.size(); ++i) {
		Vector2f& currLaser = enemyLasers.at(i);
		currLaser.y -= enemyLaserVelocity;
		if (currLaser.y <= topBound) {
			enemyLasersToRemove.push_back(i);
		}
	}

	//Sort and remove lasers
	std::sort(buzzLasersToRemove.begin(), buzzLasersToRemove.end(), std::greater<int>());

	std::sort(enemyLasersToRemove.begin(), enemyLasersToRemove.end(), std::greater<int>());

	//Remove lasers and enemies after the loop
	for (int idx : buzzLasersToRemove) {
		if (idx < buzzLasers.size())
			buzzLasers.erase(buzzLasers.begin() + idx);
	}
	for (int idx : enemyLasersToRemove) {
		if (idx < enemyLasers.size())
			enemyLasers.erase(enemyLasers.begin() + idx);
	}
}

void LaserBlast::drawAll(RenderWindow& window) const {
	for (int i = 0; i < buzzLasers.size(); ++i) {
		const Vector2f& currLaser = buzzLasers.at(i);
		Vertex line[] =
		{
			Vertex(currLaser, Color::Red),
			Vertex(Vector2f(currLaser.x, currLaser.y - 20), Color::Red)
		};
		window.draw(line, 2, Lines, RenderStates::Default);
	}

	for (int i = 0; i < enemyLasers.size(); ++i) {
		const Vector2f& currLaser = enemyLasers.at(i);
		Vertex line[] =
		{
			Vertex(currLaser, Color::Red),
			Vertex(Vector2f(currLaser.x, currLaser.y - 20), Color::Red)
		};
		window.draw(line, 2, Lines, RenderStates::Default);
	}
}

void LaserBlast::clear() {
	buzzLasers.clear();
	enemyLasers.clear();
}

/*
LaserBlast::LaserBlast(const ECEBuzzy& buzz) {
	const Vector2f buzzPosition = buzz.getPosition();

	//Use constant y value
	position = Vector2f(buzzPosition.x + 40, 160);

	//Increase to change speed
	yVelocity = 0.1;

	//How far down a laser blast can go before dissapearing
	topBound = 0;
	bottomBound = 1000;
}

LaserBlast::LaserBlast(const ECEEnemy& buzz) {
	const Vector2f buzzPosition = buzz.getPosition();

	//Use constant y value
	position = Vector2f(buzzPosition.x + 40, 160);

	//Increase to change speed
	yVelocity = -0.1;

	//How far down a laser blast can go before dissapearing
	topBound = 0;
	bottomBound = 1000;
}

const Vector2f LaserBlast::getPosition() const {
	return position;
}

void LaserBlast::updatePosition() {
	position.y += yVelocity;
}

bool LaserBlast::shouldBeRemoved() const {
	if (position.y > bottomBound || position.y < topBound)
		return true;
	else
		return false;
}

void LaserBlast::draw(RenderTarget& target, RenderStates states) const {
	Vertex line[] =
	{
		Vertex(position, Color::Red),
		Vertex(Vector2f(position.x, position.y - 20), Color::Red)
	};
	target.draw(line, 2, Lines, states);
}
*/