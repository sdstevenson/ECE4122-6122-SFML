#include "ECEEnemy.h"
#include "Laser.h"

ECEEnemy::ECEEnemy(const int textureOption, const Vector2f setPos) {
	//Set texture based on inputs
	if (textureOption == 0) {
		texture.loadFromFile("graphics/bulldog.png");
		scale = Vector2f(0.25, 0.25);
	}
	else if (textureOption == 1) {
		texture.loadFromFile("graphics/clemson_tigers.png");
		scale = Vector2f(0.0833, 0.0833);
	}
	else {
		printf("ERROR: No texture set, defaulting to bulldog\n");
		texture.loadFromFile("graphics/bulldog.png");
		scale = Vector2f(0.25, 0.25);
	}

	//Set position and scale
	position = setPos;

	Vector2u textSize = texture.getSize();

	scaledWidth = textSize.x * scale.x;
	scaledHeight = textSize.y * scale.y;
	velocity = Vector2f(0.01, 0.005);

	setTexture(texture);
	setPosition(position);
	setScale(scale);
}

void ECEEnemy::moveRight() {
	position.x += velocity.x;
	setPosition(position);
}

void ECEEnemy::moveLeft() {
	position.x -= velocity.x;
	setPosition(position);
}

void ECEEnemy::moveUp() {
	position.y -= velocity.y;
	setPosition(position);
}

void ECEEnemy::moveDown() {
	position.y += velocity.y;
	setPosition(position);
}

bool ECEEnemy::shouldBeRemoved(const LaserBlast& laser) const {
	Vector2f laserPos = laser.getPosition();

	float minY = laserPos.y - 20;
	float maxY = laserPos.y;
	float x = laserPos.x;

	//Check if in the right y range
	if (maxY > position.y && minY < (position.y + scaledHeight)) {
		//Check if in the right x range
		if (x > position.x && x < (position.x + scaledWidth)) {
			//If so, we have a collisions, return true
			return true;
		}
	}
	return false;
}

bool ECEEnemy::atLeftWall() const {
	if (position.x <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool ECEEnemy::atRightWall() const {
	if (position.x + scaledWidth >= 800) {
		return true;
	}
	else {
		return false;
	}
}

Vector2f ECEEnemy::getPosition() const {
	return position;
}

float ECEEnemy::getScaledWidth() const {
	return scaledWidth;
}

float ECEEnemy::getScaledHeight() const {
	return scaledHeight;
}