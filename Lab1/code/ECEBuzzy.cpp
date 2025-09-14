#include "ECEBuzzy.h"
#include "ECEEnemy.h"

ECEBuzzy::ECEBuzzy() {
    texture.loadFromFile("graphics/Buzzy_blue.png");
    position = Vector2f(50, 50);
    scale = Vector2f(0.25, 0.25);

    Vector2u textSize = texture.getSize();
    scaledWidth = textSize.x * scale.x;
    scaledHeight = textSize.y * scale.y;

    setTexture(texture);
    setPosition(position);
    setScale(scale);

    leftBound = 0;
    rightBound = 700;
}

void ECEBuzzy::moveLeft() {
    float currX = position.x;

    //Cant move out of the canvas
    if (currX > leftBound) {
        position.x -= 10;
        setPosition(position);
    }
}

void ECEBuzzy::moveRight() {
    float currX = position.x;
    if (currX < rightBound) {
        position.x += 10;
        setPosition(position);
    }
}

bool ECEBuzzy::shouldBeRemoved(const ECEEnemy& enemy) const {
    Vector2f enemyPos = enemy.getPosition();
    if (enemyPos.y < 145) {
        if ((enemyPos.x + enemy.getScaledWidth()) > position.x && enemyPos.x < (position.x + 90)) {
            return true;
        }
    }
    return false;
}

Vector2f ECEBuzzy::getPosition() const {
    return position;
}

float ECEBuzzy::getScaledWidth() const {
    return scaledWidth;
}

float ECEBuzzy::getScaledHeight() const {
    return scaledHeight;
}