/*
Author: Samir Stevenson
Class: ECE4122
Last Data Modified: 10/1/2025

Description: John Conway's Game of Life, implemented sequentially, with threads, and using OMP for threading.

*/

#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

//Threading imports
#include <thread>
#include <mutex>
#include "threadPool.hpp"

//SFML imports
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum class RunOptions {SEQ, THRD, OMP};

/*
Gets user input, and assigns values to passed in pointers as parsed. Returns status as an int.

Inputs
	int argc: User args count
	char* argv[]: User args

Outputs
	int* numThreads: Pointer to numThreads variable to set
	int* cellSize: Pointer to cellSize variable to set
	int* windowWidth: Pointer to windowWidth variable to set
	int* windowHeight: Pointer to windowHeight variable to set
	RunOptions* option: Pointer to RunOptions enum to set

Returns
	int: Status, 1 on error 0 on success

*/
int getUserInput(int argc, char* argv[], int* numThreads, int* cellSize, int* windowWidth, int* windowHeight, RunOptions* option) {
	int temp;	//Temp int to hold user inputs
	string currArg;
	string optString;

	for (int i = 0; i < argc; ++i) {
		//Loop through each arg, checking if valid and setting as needed.
		//Return 1 on error

		//Get the current argument once
		currArg = argv[i];
		if (currArg == "-n") {
			//Check for numThreads
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
			//Check for cellSize
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
			//Check for windowWidth
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
			//Check for windowHeight
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
			//Check for optString, which mode to execute in
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

	return 0;
}

/*
Draws a single square, cell, on screen.

Inputs
	int x: X position to draw at
	int y: Y position to draw at
	int cellSize: Size of the cell to draw
	RenderWindow* window: Pointer to the window to draw on

Outputs
	None

Returns
	void
	
*/
void drawSquare(int x, int y, int cellSize, RenderWindow* window) {
	RectangleShape square(sf::Vector2f(cellSize, cellSize));
	square.setFillColor(sf::Color::White);
	square.setPosition(x * cellSize, y * cellSize);
	window->draw(square);
}


/*
Sequential implementation of Game of Life.

Inputs
	vector<vector<int>>* gameMatrix: Pointer to the game matrix to update
	int cellSize: Size of each cell in pixels
	int windowWidth: Width of the window in pixels
	int windowHeight: Height of the window in pixels

Outputs
	None

Returns
	int: Status, 0 on success
*/
int seqGameOfLife(vector<vector<int>>* gameMatrix, int cellSize, int windowWidth, int windowHeight) {
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

				int currCell = (*gameMatrix)[r][c];
				if (currCell != 0) {
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


/*
Threaded implementation of Game of Life.

Inputs
	vector<int>* gameMatrix: Pointer to the flattened game matrix to update
	size_t rows: Number of rows in the game matrix
	size_t cols: Number of columns in the game matrix
	int numThreads: Number of threads to use
	int cellSize: Size of each cell in pixels
	int windowWidth: Width of the window in pixels
	int windowHeight: Height of the window in pixels

Outputs
	None

Returns
	int: Status, 0 on success
*/
int thrdGameOfLife(vector<int>* gameMatrix, size_t rows, size_t cols, int numThreads, int cellSize, int windowWidth, int windowHeight) {
	//Set up the game based on args
	VideoMode vm(windowWidth, windowHeight);
	RenderWindow window(vm, "Game of Life -SFML", Style::Default);

	//Set up thread pool
	ThreadPool pool(numThreads);

	//Set up timer
	int generationNum = 0;
	chrono::nanoseconds totalTime(0);

	Event event;	//Event to check user inputs

	do {
		//Check for user inputs
		window.pollEvent(event);

		//Get time to calculate this iteration
		auto start = chrono::high_resolution_clock::now();

		//Update gameMatrix
		vector<int> nextGameMatrix(rows * cols, 0);	//Create a next game state, so we can update without updating the current

		//Each thread processes a chunk
		size_t rowStart = 0;
		for (size_t row = 0; row < rows; row++) {
			pool.enqueueTask([gameMatrix, &nextGameMatrix, row, rows, cols]() {
				//Loop through each element on this row
				for (int col = 0; col < cols; col++) {
					//Reset numNeighbors per cell
					int numNeighbors = 0;

					int index = row * cols + col;

					#pragma region NeighborsCheck
					//Check around each cell for neighboring cells - hardcoded 8 neighbors
					int nr, nc;
					// (r-1, c-1)
					nr = row - 1; nc = col - 1;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index - cols - 1] != 0) ++numNeighbors;
					}
					// (r-1, c)
					nr = row - 1; nc = col;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index - cols] != 0) ++numNeighbors;
					}
					// (r-1, c+1)
					nr = row - 1; nc = col + 1;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index - cols + 1] != 0) ++numNeighbors;
					}
					// (r, c-1)
					nr = row; nc = col - 1;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index - 1] != 0) ++numNeighbors;
					}
					// (r, c+1)
					nr = row; nc = col + 1;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index + 1] != 0) ++numNeighbors;
					}
					// (r+1, c-1)
					nr = row + 1; nc = col - 1;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index + cols - 1] != 0) ++numNeighbors;
					}
					// (r+1, c)
					nr = row + 1; nc = col;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index + cols] != 0) ++numNeighbors;
					}
					// (r+1, c+1)
					nr = row + 1; nc = col + 1;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
						if ((*gameMatrix)[index + cols + 1] != 0) ++numNeighbors;
					}
					#pragma endregion


					int currCell = (*gameMatrix)[index];
					if (currCell != 0) {
						//If alive, die if numNeighbors is not 2 or 3
						nextGameMatrix[index] = (numNeighbors == 2 || numNeighbors == 3) ? 1 : 0;
					}
					else {
						//If dead, revive if numNeighbors is 3
						nextGameMatrix[index] = (numNeighbors == 3) ? 1 : 0;
					}
				}
			});
		}

		//Wait for threads to end
		pool.waitForAll();

		//Set game matrix to the newly calculate one
		*gameMatrix = std::move(nextGameMatrix);

		//Get end time of iteration calculation and add to overal duration
		auto stop = chrono::high_resolution_clock::now();
		generationNum++;
		totalTime += (stop - start);

		//Print every 100th generation
		if ((generationNum % 100) == 0) {
			auto avgMs = chrono::duration_cast<std::chrono::milliseconds>(totalTime).count();
			printf("100 generations took %lld microseconds with %d std::threads.\n", (long long)avgMs, numThreads);
			totalTime = chrono::nanoseconds(0);
		}

		//Draw gameMatrix
		window.clear(sf::Color::Black);		//Make sure to reset the screen
		for (size_t r = 0; r < rows; r++) {
			for (size_t c = 0; c < cols; c++) {
				if ((*gameMatrix)[r * cols + c] != 0) {
					drawSquare(c, r, cellSize, &window);
				}
			}
		}

		//Continue while window is open, and user has not pressed escape
		window.display();
	} while (window.isOpen()
		&& event.type != Event::Closed
		&& event.type == Event::KeyPressed ? event.key.code != Keyboard::Escape : true);

	//Clear up the window on exit. Thread pool gets destroyed when out of scope. 
	window.close();
	return 0;
}


/*
OMP threaded implementation of Game of Life.

Inputs
	vector<int>* gameMatrix: Pointer to the flattened game matrix to update
	size_t rows: Number of rows in the game matrix
	size_t cols: Number of columns in the game matrix
	int numThreads: Number of threads to use
	int cellSize: Size of each cell in pixels
	int windowWidth: Width of the window in pixels
	int windowHeight: Height of the window in pixels

Outputs
	None

Returns
	int: Status, 0 on success
*/
int ompGameOfLife(vector<int>* gameMatrix, size_t rows, size_t cols, int numThreads, int cellSize, int windowWidth, int windowHeight) {
	//Set up the game based on args
	VideoMode vm(windowWidth, windowHeight);
	RenderWindow window(vm, "Game of Life -SFML", Style::Default);

	//Set up timer
	int generationNum = 0;
	chrono::nanoseconds totalTime(0);

	Event event;	//Event to check user inputs

	do {
		//Check for user inputs
		window.pollEvent(event);

		//Get time to calculate this iteration
		auto start = chrono::high_resolution_clock::now();

		//Update gameMatrix
		vector<int> nextGameMatrix(rows * cols, 0);	//Create a next game state, so we can update without updating the current

		//Each thread processes a chunk
		size_t rowStart = 0;
		for (size_t row = 0; row < rows; row++) {
			//Loop through each element on this row
			#pragma omp parallel for num_threads(numThreads)
			for (int col = 0; col < cols; col++) {
				//Reset numNeighbors per cell
				int numNeighbors = 0;

				int index = row * cols + col;

				#pragma region NeighborsCheck
				//Check around each cell for neighboring cells - hardcoded 8 neighbors
				int nr, nc;
				// (r-1, c-1)
				nr = row - 1; nc = col - 1;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index - cols - 1] != 0) ++numNeighbors;
				}
				// (r-1, c)
				nr = row - 1; nc = col;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index - cols] != 0) ++numNeighbors;
				}
				// (r-1, c+1)
				nr = row - 1; nc = col + 1;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index - cols + 1] != 0) ++numNeighbors;
				}
				// (r, c-1)
				nr = row; nc = col - 1;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index - 1] != 0) ++numNeighbors;
				}
				// (r, c+1)
				nr = row; nc = col + 1;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index + 1] != 0) ++numNeighbors;
				}
				// (r+1, c-1)
				nr = row + 1; nc = col - 1;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index + cols - 1] != 0) ++numNeighbors;
				}
				// (r+1, c)
				nr = row + 1; nc = col;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index + cols] != 0) ++numNeighbors;
				}
				// (r+1, c+1)
				nr = row + 1; nc = col + 1;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
					if ((*gameMatrix)[index + cols + 1] != 0) ++numNeighbors;
				}
				#pragma endregion


				int currCell = (*gameMatrix)[index];
				if (currCell != 0) {
					//If alive, die if numNeighbors is not 2 or 3
					nextGameMatrix[index] = (numNeighbors == 2 || numNeighbors == 3) ? 1 : 0;
				}
				else {
					//If dead, revive if numNeighbors is 3
					nextGameMatrix[index] = (numNeighbors == 3) ? 1 : 0;
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
			printf("100 generations took %lld microseconds with %d OMP threads.\n", (long long)avgMs, numThreads);
			totalTime = chrono::nanoseconds(0);
		}

		//Draw gameMatrix
		window.clear(sf::Color::Black);		//Make sure to reset the screen
		for (size_t r = 0; r < rows; r++) {
			for (size_t c = 0; c < cols; c++) {
				if ((*gameMatrix)[r * cols + c] != 0) {
					drawSquare(c, r, cellSize, &window);
				}
			}
		}

		//Continue while window is open, and user has not pressed escape
		window.display();
	} while (window.isOpen()
		&& event.type != Event::Closed
		&& event.type == Event::KeyPressed ? event.key.code != Keyboard::Escape : true);

	//Clear up the window on exit. Thread pool gets destroyed when out of scope. 
	window.close();
	return 0;
}


/*
Main loop, collects user input, initializes the game state, and calls the appropriate implementation.

Inputs
	int argc: User args count
	char* argv[]: User args
	
Outputs
	None

Returns
	int: Status, 0 on success, 1 on error
*/
int main(int argc, char* argv[]) {
	//Define game args
	int numThreads = 8;
	int cellSize = 5;
	int windowWidth = 800;
	int windowHeight = 600;
	RunOptions option = RunOptions::THRD;

	//Collect user input, end program if there is an error
	if (getUserInput(argc, argv, &numThreads, &cellSize, &windowWidth, &windowHeight, &option)) {
		printf("Error collecting user inputs. Invalid args.");
		return 1;
	}

	//Define game state trackers
	size_t rows = (windowHeight / cellSize);
	size_t cols = (windowWidth / cellSize);
	vector<vector<int>> gameMatrix(rows, vector<int>(cols, 0));
	vector<int> flattenedGameMatrix(rows * cols, 0);

	//Initialize the underlying matrix, every cellOccurance(th) cell should be set to alive
	int cellOccurance = 3;
	int numAlive = 0;
	srand(time(0));		//Seed the random number generator
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			if (rand() % (cellOccurance + 1) == cellOccurance) {
				gameMatrix[row][col] = 1;
				flattenedGameMatrix[row * cols + col] = 1;
				numAlive++;
			}
		}
	}
	if (option == RunOptions::SEQ) {
		seqGameOfLife(&gameMatrix, cellSize, windowWidth, windowHeight);
	}
	else if (option == RunOptions::THRD) {
		thrdGameOfLife(&flattenedGameMatrix, rows, cols, numThreads, cellSize, windowWidth, windowHeight);
	}
	else if (option == RunOptions::OMP) {
		ompGameOfLife(&flattenedGameMatrix, rows, cols, numThreads, cellSize, windowWidth, windowHeight);
	}

	return 0;
}