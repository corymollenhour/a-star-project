/*
CORY MOLLENHOUR
CSCI 4350
OLA1
10/2/2018
*/

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>

#define LEFT	0
#define UP		1
#define RIGHT	2
#define DOWN	3

using namespace std;

int getRandom();
int moveZero(int move);
void printBoard(ofstream file);
bool sameAxis(int current);

int seed = 555;
int moves = 20;
char board[3][3] = { { '0','1','2' },{ '3','4','5' },{ '6','7','8' } };
int zeroX = 0;
int zeroY = 0;
int lastX = -10;
int lastY = -10;
int lastMove = 0;
int changeNum = 0;

int main(int argc, char* argv[]) {
	string inputFileName = "";
	char c;
	int i = 0;
	int j = 0;
	int randomNum = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> c;
			board[i][j] = c;
		}

	}

	seed = atoi(argv[1]);
	moves = atoi(argv[2]);
	srand(seed);

	randomNum = getRandom();

	for (int i = 0; i < moves; i++) {
		while (sameAxis(randomNum) || !moveZero(randomNum)) {
			randomNum = getRandom();
		}

		board[zeroY][zeroX] = '0';
	}
	

	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << board[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	return 0;
}

//Move the empty square (0) if the destination is valid
int moveZero(int move) {
	int isValid = 0;
	switch (move) {
	case LEFT:
		if (zeroX > 0 && (zeroX - 1 != lastX)) {
			lastX = zeroX;
			lastY = zeroY;
			board[zeroY][zeroX] = board[zeroY][zeroX - 1];
			zeroX -= 1;
			lastMove = move;
			isValid = 1;
		}
		break;
	case RIGHT:
		if (zeroX < 2 && (zeroX + 1 != lastX)) {
			lastX = zeroX;
			lastY = zeroY;
			board[zeroY][zeroX] = board[zeroY][zeroX + 1];
			zeroX += 1;
			lastMove = move;
			isValid = 1;
		}
		break;
	case UP:
		if (zeroY > 0 && (zeroY - 1 != lastY)) {
			lastX = zeroX;
			lastY = zeroY;
			board[zeroY][zeroX] = board[zeroY - 1][zeroX];
			zeroY -= 1;
			lastMove = move;
			isValid = 1;
		}
		break;
	case DOWN:
		if (zeroY < 2 && (zeroY + 1 != lastY)) {
			lastX = zeroX;
			lastY = zeroY;
			board[zeroY][zeroX] = board[zeroY + 1][zeroX];
			zeroY += 1;
			lastMove = move;
			isValid = 1;
		}
		break;
	default:
		isValid = 0;
	}
	return isValid;
}

//Return a random integer from 0 to 3
int getRandom() {
	return rand() % 4 + 0;
}

//Check if the last move is on the same axis as the current
bool sameAxis(int current) {
	if (lastMove % 2 == 0) {
		if (current % 2 == 0)
			return true;
		else
			return false;
	} else {
		if (current % 2 != 0)
			return true;
		else
			return false;
	}
	return false;
}
