#include <cstdio>
#include <string>
using namespace std;

enum class RunOptions {SEQ, THRD, OMP};

int main(int argc, char* argv[]) {
	int numThreads = 8;
	int cellSize = 5;
	int windowWidth = 800;
	int windowHeight = 600;
	RunOptions option = RunOptions::THRD;
	string optString;

	printf("argc: %d\n", argc);

	for (int i = 0; i < argc; ++i) {
		printf("%d: %s\n", i, argv[i]);
	}

	int temp;	//Temp int to hold user inputs
	string currArg;
	for (int i = 0; i < argc; ++i) {
		currArg = argv[i];
		if (currArg == "-n") {
			temp = stoi(argv[i+1]);
			if (temp < 2) {
				printf("Invalid num threads input!\n");
				return 1;
			}
			else {
				numThreads = temp;
			}
		}
		else if (currArg == "-c") {
			temp = stoi(argv[i + 1]);
			if (temp < 1) {
				printf("Invalid cell size input!\n");
				return 1;
			}
			else {
				cellSize = temp;
			}
		}
		else if (currArg == "-x") {
			temp = stoi(argv[i + 1]);
			if (temp < 0) {
				printf("Invalid window width input!\n");
				return 1;
			}
			else {
				windowWidth = temp;
			}
		}
		else if (currArg == "-y") {
			temp = stoi(argv[i + 1]);
			if (temp < 0) {
				printf("Invalid window height input!\n");
				return 1;
			}
			else {
				windowHeight = temp;
			}
		}
		else if (currArg == "-t") {
			string tempStr = argv[i + 1];
			if (tempStr == "SEQ") {
				option = RunOptions::SEQ;
				optString = "SEQ";
			}
			else if (tempStr == "THRD") {
				option = RunOptions::THRD;
				optString = "THRD";
			}
			else if (tempStr == "OMP") {
				option = RunOptions::OMP;
				optString = "OMP";
			}
			else {
				printf("Invalid run mode input!\n");
				return 1;
			}
		}
	}

	printf("Running with options: numThreads (%d), cellSize (%d), windowWidth (%d), windowHeight (%d), options (%s)\n", numThreads, cellSize, windowWidth, windowHeight, optString.c_str());


	return 0;
}