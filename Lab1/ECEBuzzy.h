#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdio>

using namespace sf;

class ECEEnemy;

class ECEBuzzy : public Sprite {
public:
    ECEBuzzy();
    void moveLeft();
    void moveRight();
    bool shouldBeRemoved(const ECEEnemy& enemy) const;
    Vector2f getPosition() const;

 private:
     Texture texture;
     Vector2f position;
     Vector2f scale;
     float leftBound;
     float rightBound;
};