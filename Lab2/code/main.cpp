#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum class RunOptions {SEQ, THRD, OMP};

int getUserInput(int argc, char* argv[], int* numThreads, int* cellSize, int* windowWidth, int* windowHeight, RunOptions* option) {
	int temp;	//Temp int to hold user inputs
	string currArg;
	string optString;

	for (int i = 0; i < argc; ++i) {
		currArg = argv[i];
		if (currArg == "-n") {
			temp = stoi(argv[i + 1]);
			if (temp < 2) {
				printf("Invalid num threads input!\n");
				return 1;
			}
			else {
				*numThreads = temp;
			}
		}
		else if (currArg == "-c") {
			temp = stoi(argv[i + 1]);
			if (temp < 1) {
				printf("Invalid cell size input!\n");
				return 1;
			}
			else {
				*cellSize = temp;
			}
		}
		else if (currArg == "-x") {
			temp = stoi(argv[i + 1]);
			if (temp < 0) {
				printf("Invalid window width input!\n");
				return 1;
			}
			else {
				*windowWidth = temp;
			}
		}
		else if (currArg == "-y") {
			temp = stoi(argv[i + 1]);
			if (temp < 0) {
				printf("Invalid window height input!\n");
				return 1;
			}
			else {
				*windowHeight = temp;
			}
		}
		else if (currArg == "-t") {
			string tempStr = argv[i + 1];
			if (tempStr == "SEQ") {
				*option = RunOptions::SEQ;
				optString = "SEQ";
			}
			else if (tempStr == "THRD") {
				*option = RunOptions::THRD;
				optString = "THRD";
			}
			else if (tempStr == "OMP") {
				*option = RunOptions::OMP;
				optString = "OMP";
			}
			else {
				printf("Invalid run mode input!\n");
				return 1;
			}
		}
	}

	//printf("Running with options: numThreads (%d), cellSize (%d), windowWidth (%d), windowHeight (%d), options (%s)\n", *numThreads, *cellSize, *windowWidth, *windowHeight, optString.c_str());
	return 0;
}

void drawSquare(int x, int y, int cellSize, RenderWindow* window) {
	RectangleShape square(sf::Vector2f(cellSize, cellSize));
	square.setFillColor(sf::Color::White);
	square.setPosition(x * cellSize, y * cellSize);
	window->draw(square);
}

int seqGameOfLife(vector<vector<int>>* gameMatrix, int numThreads, int cellSize, int windowWidth, int windowHeight) {
	//Set up the game based on args
	VideoMode vm(windowWidth, windowHeight);
	RenderWindow window(vm, "Game of Life -SFML", Style::Default);
	
	//Set up timer
	int generationNum = 0;
	chrono::nanoseconds totalTime(0);

	Event event;	//Event to check user inputs
	int rows = gameMatrix->size();
	int cols = gameMatrix->at(0).size();

	do {
		//Check for user inputs
		window.pollEvent(event);

		//Get time to calculate this iteration
		auto start = chrono::high_resolution_clock::now();

		//Update gameMatrix
		vector<vector<int>> nextGameMatrix(rows, vector<int>(cols, 0));		//Create a new game state without updating curr
		for (int r = 0; r < rows; ++r) {
			for (int c = 0; c < cols; ++c) {
				int numNeighbors = 0;
				//Check around each cell for neighboring cells
				for (int dr = -1; dr <= 1; ++dr) {
					for (int dc = -1; dc <= 1; ++dc) {
						if (dr == 0 && dc == 0) continue;
						int rowToCheck = (int)r + dr;
						int colToCheck = (int)c + dc;
						if (rowToCheck >= 0 && rowToCheck < (int)rows && colToCheck >= 0 && colToCheck < (int)cols) {
							if ((*gameMatrix)[rowToCheck][colToCheck] != 0) numNeighbors++;
						}
					}
				}

				int cur = (*gameMatrix)[r][c];
				if (cur != 0) {
					//If alive, die if numNeighbors is not 2 or 3
					nextGameMatrix[r][c] = (numNeighbors == 2 || numNeighbors == 3) ? 1 : 0;
				}
				else {
					//If dead, revive if numNeighbors is 3
					nextGameMatrix[r][c] = (numNeighbors == 3) ? 1 : 0;
				}
			}
		}

		//Set game matrix to the newly calculate one
		*gameMatrix = std::move(nextGameMatrix);

		//Get end time of iteration calculation and add to overal duration
		auto stop = chrono::high_resolution_clock::now();
		generationNum++;
		totalTime += (stop - start);

		//Print every 100th generation
		if ((generationNum % 100) == 0) {
			auto avgMs = chrono::duration_cast<std::chrono::milliseconds>(totalTime).count();
			printf("100 generations took %lld microseconds with single thread.\n", (long long)avgMs);
			totalTime = chrono::nanoseconds(0);
		}

		//Draw gameMatrix
		window.clear(sf::Color::Black);		//Make sure to reset the screen
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				if ((*gameMatrix)[r][c] != 0) {
					drawSquare(c, r, cellSize, &window);
				}
			}
		}

		//Continue while window is open, and user has not pressed escape
		window.display();
	} while (window.isOpen()
		&& event.type != Event::Closed
		&& event.type == Event::KeyPressed ? event.key.code != Keyboard::Escape : true);

	//Clear up the window on exit
	window.close();
	return 0;
}

int thrdGameOfLife(int numThreads, int cellSize, int windowWidth, int windowHeight) {
	return 0;
}

int ompGameOfLife(int numThreads, int cellSize, int windowWidth, int windowHeight) {
	return 0;
}

int main(int argc, char* argv[]) {
	//Define game args
	int numThreads = 8;
	int cellSize = 5;
	int windowWidth = 800;
	int windowHeight = 600;
	RunOptions option = RunOptions::THRD;

	//Collect user input, ending if there is an error
	if (getUserInput(argc, argv, &numThreads, &cellSize, &windowWidth, &windowHeight, &option)) {
		printf("Error collecting user inputs. Invalid args.");
		return 1;
	}

	//Define game state trackers
	vector<vector<int>> gameMatrix(windowHeight / cellSize, vector<int>(windowWidth / cellSize, 0));

	//Initialize the underlying matrix, ~every cellOccurance - th cell should be set to alive
	int cellOccurance = 3;
	int numAlive = 0;
	srand(time(0));		//Seed the random number generator
	for (int row = 0; row < gameMatrix.size(); row++) {
		for (int col = 0; col < gameMatrix[0].size(); col++) {
			if (rand() % (cellOccurance + 1) == cellOccurance) {
				gameMatrix[row][col] = 1;
				numAlive++;
			}
		}
	}
	if (option == RunOptions::SEQ) {
		seqGameOfLife(&gameMatrix, numThreads, cellSize, windowWidth, windowHeight);
	}
	else if (option == RunOptions::THRD) {
		thrdGameOfLife(numThreads, cellSize, windowWidth, windowHeight);
	}
	else if (option == RunOptions::OMP) {
		ompGameOfLife(numThreads, cellSize, windowWidth, windowHeight);
	}

	return 0;
}