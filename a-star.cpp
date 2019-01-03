/*
	Cory Mollenhour
	CSCI 4350 - Joshua Phillips
	Due: 10/2/2018 11:00 PM
	A* 8-puzzle solver
*/

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <queue>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <time.h>

#define UP		0
#define LEFT	1
#define DOWN	2
#define RIGHT	3

using namespace std;

struct Node {
	int board[3][3];
	Node * parent;
	int zeroX, zeroY;
	int cost, depth;
	int config[9] = { 0 };
	int configID;
	int id;
};

int startBoard[3][3];
int goalCosts[9][2] = { { 0,0 },{ 0,1 },{ 0,2 },{ 1,0 },{ 1,1 },{ 1,2 },{ 2,0 },{ 2,1 },{ 2,2 } };
int goalConfig[9] = { 0,1,2,3,4,5,6,7,8 };
unordered_map<int, Node*> closedList;
int changeNum = 0;
int i = 0;
int j = 0;
int heuristic = 1;
int newX, newY;
int sortedID = 0;
int currentDepth = 0;
int depthLimit = 1000000;
void solvePuzzle(int initial[3][3], int x, int y);

//Statistics
int nodesVisited = 0;		//nodes expanded / visted (V)
int nodesInOpenList = 0;	// Keep track of nodes in frontier
int nodesInMemory = 0;		//Closed list + open list size (N)
int depthOfSolution = 0;	//Depth of optimal solution (d)
float branchingFactor = 0.00;	//approximate b^d

//Function Definitions
int getCost(int board[3][3]);
int getDisplacedCost(Node * node);
int getManhattenCost(Node * node);
int getCustomCost(int board[3][3]);
bool isValid(int move, int x, int y);
void printBoard(int board[3][3]);
bool hasGoalState(Node * node);
Node * newNode(int board[3][3], int zeroX, int zeroY, int depth, Node* parent);
int moveZero(int move, int zeroX, int zeroY);

//Check to make sure move is valid
bool isValid(int move, int x, int y) {
	newX = x;
	newY = y;
	bool isValid = false;
	switch (move) {
	case UP:
		if (y > 0) {
			newY--;
			isValid = true;
		}
		break;
	case LEFT:
		if (x > 0) {
			newX--;
			isValid = true;
		}
		break;
	case DOWN:
		if (y < 2) {
			newY++;
			isValid = true;
		}
		break;
	case RIGHT:
		if (x < 2) {
			newX++;
			isValid = true;
		}
		break;
	default:
		return false;
	}
	if (isValid)
		return true;
	else
		return false;
}

bool isOnClosedList(Node * node) {
	unordered_map<int, Node*>::const_iterator foundNode = closedList.find(node->configID);
	if (foundNode == closedList.end())
		return false;
	return true;
}

void printBoard(int board[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << board[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

void printSolution(Node * node) {
	if (node->parent != NULL) {
		printSolution(node->parent);
		printBoard(node->board);
	}

}

Node * newNode(int board[3][3], int zeroX, int zeroY, int depth, Node* parent) {
	Node * node = new Node();
	node->parent = parent;
	memcpy(node->board, board, sizeof node->board);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			node->board[i][j] = board[i][j];
		}
	}
		
	node->zeroX = newX;
	node->zeroY = newY;
	node->id = sortedID++;
	swap(node->board[zeroY][zeroX], node->board[newY][newX]);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			node->config[i * 3 + j] = node->board[i][j];
			node->configID += node->board[i][j];
		}
	}
	for (int i = 0; i < 9; i++) {
		node->configID = 10 * node->configID + node->config[i];
	}

	switch (heuristic) {
		case 0: {
			node->cost = getCost(node->board);
			break;
		}
		case 1: {
			node->cost = getDisplacedCost(node);
			break;
		}
		case 2: {
			node->cost = getManhattenCost(node);
			break;
		}
		case 4: {
			node->cost = getCustomCost(node->board);
			break;
		}
		default:
			break;
	}
	node->depth = depth;

	return node;
}

int getCost(int board[3][3]) {
	return 0;
}

int getDisplacedCost(Node * node) {
	int count = 0;
	for (int i = 0; i < 9; i++) {
		if (node->config[i] != goalConfig[i] && node->config[i] != 0)
			count++;
	}
	return count;
}

int getManhattenCost(Node * node) {
	int count = 0;
	int numberOnBoard;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (node->board[i][j] != i*3+j && node->board[i][j] != 0) {
				numberOnBoard = node->board[i][j];
				count += abs(i - goalCosts[numberOnBoard][0]);
				count += abs(j - goalCosts[numberOnBoard][1]);
			}
		}
	}
	return count;
}

int getCustomCost(int board[3][3]) {
	return 0;
}

// Comparison object to be used to order the heap 
struct comp
{
	bool operator()(const Node* leftSide, const Node* rightSide) const
	{
		int leftCost = leftSide->cost + leftSide->depth;
		int rightCost = rightSide->cost + rightSide->depth;
		if (leftCost == rightCost) {
			if (leftSide->id < rightSide->id) //Tie breaker
				return true;
			else
				return false;
		} else {
			if (leftCost > rightCost)
				return true;
			else
				return false;
		}
	}
};

void solvePuzzle(int initial[3][3], int x, int y){
	priority_queue<Node*, vector<Node*>, comp> pq;
	newX = x;
	newY = y;

	Node* root = newNode(initial, x, y, 0, NULL);
	pq.push(root);
	nodesInOpenList++;
	printBoard(initial);
	printf("Finding solution.\n");
	while (!pq.empty()){
		Node* min = pq.top();
		nodesVisited++;
		int originalX = min->zeroX;
		int originalY = min->zeroY;		
		pq.pop();
		nodesInOpenList--;
		if (!hasGoalState(min)) {
			for (int i = 0; i < 4; i++) {
				if (isValid(i, originalX, originalY)) {
					Node* child = newNode(min->board, originalX, originalY, min->depth+1, min);
					if (!isOnClosedList(child)) {
						pq.push(child);
						nodesInOpenList++;
					} else {
						delete child;
					}
				}
			}
			currentDepth++;
		} else {
			depthOfSolution = min->depth;
			printSolution(min);
			printf("FOUND GOAL");
			break;
		}
		closedList.emplace(min->configID, min);
	}
}

//Check if current board matches goal state
bool hasGoalState(Node * node) {
	for (int i = 0; i < 9; i++) {
		if (node->config[i] != goalConfig[i])
			return false;
	}
	return true;
}

int main(int argc, char* argv[]) {
	string inputFileName = "";
	string line;
	string boardString;
	char boardChars[3][3];
	char c;
	int startX = 0;
	int startY = 0;
	int randomNum = 0;
	clock_t t;
	int numChars = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> c;
			boardChars[i][j] = c;
		}
		
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			startBoard[i][j] = (int)boardChars[i][j]-48;
			if (startBoard[i][j] == 0) {
				startX = j;
				startY = i;
			}
		}
	}

	cout << endl << endl;

	t = clock();
	printf("Calculating...\n");
	solvePuzzle(startBoard, startX, startY);
	t = clock() - t;
	printf("%f seconds.\n\n", ((float)t) / CLOCKS_PER_SEC);

	nodesInMemory = closedList.size() + nodesInOpenList;
	branchingFactor = pow(nodesInMemory, (1.0 / depthOfSolution));
	printf("V: %i	Nodes Visited\n", nodesVisited);
	printf("N: %i	Nodes In Memory\n", nodesInMemory);
	printf("d: %i	Depth of Solution\n", depthOfSolution);
	printf("b: %f	Branch Factor\n\n", branchingFactor);

	cout << endl;
	return 0;
}
