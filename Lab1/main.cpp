#include <cstdio>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "ECEBuzzy.h"
#include "Laser.h"
#include "ECEEnemy.h"

using namespace sf;

enum class GameState { StartScreen, Running, GameOver };
enum class EnemyDir { Left, Right };

int main() {
	GameState state = GameState::StartScreen;
	EnemyDir enemyDir = EnemyDir::Left;

	//Setup screen
	VideoMode vm(800, 800);
	RenderWindow window(vm, "Hello SFML", Style::Default);

	//Setup text
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	Text startupText;
	startupText.setFont(font);
	startupText.setString("Welcome. Press enter to start.");
	startupText.setCharacterSize(24);
	startupText.setFillColor(Color::Yellow);
	startupText.setPosition(40, vm.height / static_cast<float>(2));

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("Game over. Press enter to go to startup.");
	gameOverText.setCharacterSize(24);
	gameOverText.setFillColor(Color::Yellow);
	gameOverText.setPosition(40, vm.height / static_cast<float>(2));

	//Setup required variables
	ECEBuzzy buzz = ECEBuzzy();
	LaserBlast lasers = LaserBlast(0, 1000);
	std::vector<std::unique_ptr<ECEEnemy>> enemies;	//Use unique ptr to prevent texture lifetime issues
	std::vector<int> enemiesToRemove;
	bool switchDir = false;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed) {
				switch (state) {

				case GameState::StartScreen:
					if (event.key.code == Keyboard::Enter) {
						//Reset game state
						lasers.clear();
						enemies.clear();
						enemiesToRemove.clear();
						//Populate enemies
						for (int row = 0; row < 4; row++) {
							for (int col = 0; col < 8; col++) {
								enemies.push_back(std::make_unique<ECEEnemy>(row % 2, Vector2f(40 + col * 80, 500 + row * 80)));
							}
						}
						state = GameState::Running;
					}
					break;

				case GameState::Running:
					if (event.key.code == Keyboard::Space) {
						lasers.spawnFromBuzzy(buzz);
					}
					if (event.key.code == Keyboard::Left) {
						buzz.moveLeft();
					}
					if (event.key.code == Keyboard::Right) {
						buzz.moveRight();
					}
					break;

				case GameState::GameOver:
;					if (event.key.code == Keyboard::Enter) {
						state = GameState::StartScreen;
					}
					break;

				}
			}
		}

		//Reset the screen
		window.clear(Color::Blue);

		//Game logic and drawing based on state
		switch (state) {

		case GameState::StartScreen:
			window.draw(startupText);
			break;
		case GameState::Running:
			//Buzz update
			window.draw(buzz);
			if (lasers.checkBuzzyCollision(buzz)) {
				state = GameState::GameOver;
				break;
			}


			switchDir = false;

			//Enemy update
			for (int i = 0; i < enemies.size(); i++) {
				ECEEnemy& currEnemy = *enemies.at(i);

				//Move enemies
				if (enemyDir == EnemyDir::Left) currEnemy.moveLeft();
				else currEnemy.moveRight();
				currEnemy.moveUp();
				window.draw(currEnemy);

				//Update which side to move towards on the next iteration
				if (currEnemy.atLeftWall() && enemyDir == EnemyDir::Left) {
					switchDir = true;
				}
				else if (currEnemy.atRightWall() && enemyDir == EnemyDir::Right) {
					switchDir = true;
				}

				//Check for collisions with laser
				if (lasers.checkEnemyCollision(currEnemy)) {
					enemiesToRemove.push_back(i);
					continue;
				}
				//Check for collisions with Buzz
				if (buzz.shouldBeRemoved(currEnemy)) {
					state = GameState::GameOver;
					break;
				}
				//Check if hit the top of the screen
				if (currEnemy.getPosition().y < 10) {
					state = GameState::GameOver;
					break;
				}
			}

			if (switchDir) {
				if (enemyDir == EnemyDir::Left) enemyDir = EnemyDir::Right;
				else enemyDir = EnemyDir::Left;
			}

			//Sort remove indices from highest to lowest and remove duplicates
			std::sort(enemiesToRemove.begin(), enemiesToRemove.end(), std::greater<int>());
			enemiesToRemove.erase(std::unique(enemiesToRemove.begin(), enemiesToRemove.end()), enemiesToRemove.end());

			//Remove enemies after the loop
			for (int idx : enemiesToRemove) {
				if (idx < enemies.size())
					enemies.erase(enemies.begin() + idx);
			}

			//If there are no more enemies, end the game
			if (enemies.size() == 0) {
				state = GameState::GameOver;
			}

			enemiesToRemove.clear();


			//Laser update
			lasers.updateLasers();
			lasers.drawAll(window);

			break;
		case GameState::GameOver:
			window.draw(gameOverText);
			lasers.clear();
			enemies.clear();
			break;

		}

		window.display();
	}
}