#include "Laser.h"
#include "ECEBuzzy.h"
#include "ECEEnemy.h"

using namespace sf;

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