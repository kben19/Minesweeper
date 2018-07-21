#include "Minesweeper.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

//empty constructor
Minesweeper::Minesweeper() {
}

//Minesweeper constructor
Minesweeper::Minesweeper(int col, int ro, int bo, string filename) {
	//seed the random function with current time
	srand(time(NULL));

	//initialize the board table
	//Using pointer to make table of array dynamically
	board = new string*[ro];
	for (int i = 0; i < ro; i++) {
		board[i] = new string[col];
	}

	//check if the game loaded from save file
	//then initialize all variables, bombs and board values
	if (filename != "None") {
		loadBoard(filename);
	}
	else {
		column = col;
		row = ro;
		bomb = bo;
		markCount = bomb;
		totalNodes = column * row;

		resetBoard();
		initializedBomb(bomb);
	}
}

//Accessor mark count variable
//output: integer represent markCount
int Minesweeper::getMarkCount() {
	return markCount;
}

//save current state of the board to the save slot
//requirements: string package, ofstream package
//input: string represents the filename
void Minesweeper::saveBoard(string fileName) {
	ofstream afile;
	afile.open(fileName, ofstream::out | ofstream::trunc);
	afile << row << " " << column << " " << bomb << " " << revealedNodes << " " << markCount << " " << falseMark << endl;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			afile << board[i][j] << " ";
		}
		afile << endl;
	}
	afile.close();
}

//load the state of the game from saved game and initialize it
//requirements: string package, ifstream package
//input: string represents the filename
void Minesweeper::loadBoard(string fileName) {
	ifstream afile;
	afile.open(fileName);
	afile >> row;
	afile >> column;
	afile >> bomb;
	afile >> revealedNodes;
	afile >> markCount;
	afile >> falseMark;
	totalNodes = column * row;

	//initialize the board values with saved board
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			afile >> board[i][j];
		}
	}
	afile.close();
}

//display the state of the board to the console
void Minesweeper::displayBoard() {
	char temp;

	//Print the column legends
	cout << "   ";
	for (int i = 0; i < column; i++) {
		cout << i;
		if (i < 10) {	//change the spacing based on the digit of the legends
			cout << "  ";
		}
		else {
			cout << " ";
		}
	}
	cout << endl << "   ";
	for (int i = 0; i < column; i++) {
		cout << "-- ";
	}
	cout << endl;

	//Print the board and its value
	for (int i = 0; i < row; i++) {
		cout << i;
		if (i < 10) {	//change the spacing based on the digit of the legends
			cout << " |";
		}
		else {
			cout << "|";
		}
		for (int j = 0; j < column; j++) {
			cout << " ";
			if (board[i][j][1] == 'H') {	//hidden nodes
				temp = ' ';
			}
			else if (board[i][j][1] == 'R') {	//revealed nodes
				temp = board[i][j][0];
			}
			else if (board[i][j][1] == 'M') {	//marked nodes
				temp = 'X';
			}
			cout << temp << "|";
		}
		cout << endl << "   ";
		for (int k = 0; k < column; k++) {
			cout << "-- ";
		}
		cout << endl;
	}
}

//open a node of a board and reacts accordingly
//input: a char represents the action, integer of column, integer of row
//output: an integer represent status of the game (0 means ongoing, 1 means win, and -1 means lose)
int Minesweeper::openNode(char action, int col, int ro) {
	int status = 0;

	if (action == 'R' || action == 'r') {	//reveal a node
		if (board[ro][col][0] == '0') {
			//open all the surrounding zero with breath first search function
			openZeroBFS(ro, col);
			status = 0;
		}
		else if (board[ro][col][1] != 'H' || board[ro][col][1] == 'M') {
			cout << "This node cannot be revealed" << endl;
		}
		else {
			board[ro][col][1] = 'R';
			revealedNodes += 1;
			//check if the revealed nodes is a bomb or not
			if (board[ro][col][0] == 'B') {
				status = -1;
			}
			else {
				status = 0;
			}
		}
	}
	else if (action == 'M' || action == 'm') {	//marked a node
		if (markCount == 0) {
			cout << "You don't have any mark left." << endl;
		}
		else if (board[ro][col][1] != 'H' || board[ro][col][1] == 'M') {
			cout << "This node cannot be marked" << endl;
		}
		else {
			board[ro][col][1] = 'M';
			revealedNodes += 1;
			markCount -= 1;
			//check if the marked nodes is a bomb as a falseMark
			//NOTE: the false mark is not used right now but it will be useful for future development
			if (board[ro][col][0] != 'B') {
				falseMark += 1;
			}
		}
		status = 0;
	}
	else if (action == 'U' || action == 'u') {	//unmarked a node
		if (board[ro][col][1] != 'M') {
			cout << "This node is not marked" << endl;
		}
		else {
			board[ro][col][1] = 'H';
			revealedNodes -= 1;
			markCount += 1;
		}
		if (board[ro][col][0] != 'B') {
			falseMark -= 1;
		}
		status = 0;
	}

	//check the win condition if all the nodes are opened
	if (revealedNodes == totalNodes) {
		status = 1;
	}

	return status;
}

//reset the board values with hidden zero value
void Minesweeper::resetBoard() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			board[i][j] = "0H";
		}
	}
}

//initialize the bombs to the board with random function
//requirements: seeded random function, can only be called once after the constructor
//input: integer represents the total bomb
void Minesweeper::initializedBomb(int bo) {
	int randomCol, randomRow, counter;
	counter = 0;
	while (counter < bo) {
		//get random value of row and column
		randomCol = rand() % column;
		randomRow = rand() % row;

		if (board[randomRow][randomCol][0] != 'B') {
			board[randomRow][randomCol][0] = 'B';
			initializedNumber(randomRow, randomCol);
		}
		else {	//if the selected row and column has existing bomb, counter must be unaffected
			counter -= 1;
		}
		counter += 1;
	}// end while loop
}// end function

//initialize the number on the surrounding node that represent as a bomb
//requirement: the inputted row and column node must represent a bomb
//input: integer of row, integer of column
void Minesweeper::initializedNumber(int ro, int col) {
	int temp;
	char numChar;
	//loop a permutation of {-1, 0, 1} two digit numbers to represent the surrounding nodes position
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (ro + i >= 0 && col + j >= 0 && ro + i < row && col + j < column) {
				if (board[ro + i][col + j][0] != 'B') {
					temp = board[ro + i][col + j][0];	//get the ASCII value
					temp += 1;
					numChar = temp;	//put back the char after incremented ASCII value
					board[ro + i][col + j][0] = numChar;
				}// end inner if
			}// end outer if
		}// end inner for loop
	}// end outer for loop
}// end function

//function that reveal all surrounding adjacent nodes of a certain node
//input: integer of row and integer of column
//output: a boolean, true if any zero revealed, false otherwise
bool Minesweeper::openZeroVicinity(int ro, int col) {
	bool openedZero = false;
	if (board[ro][col][0] == 'B') {
		return false;
	}
	//loop a permutation of {-1, 0, 1} two digit numbers to represent the surrounding nodes position({0,0} excluded)
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (ro + i >= 0 && col + j >= 0 && ro + i < row && col + j < column && (i != 0 || j != 0)) {
				if (board[ro + i][col + j][1] == 'H' && board[ro][col][0] != 'B') {
					board[ro + i][col + j][1] = 'R';
					revealedNodes += 1;
					if (board[ro][col][0] == '0') {
						openedZero = true;
					}
				}// end inner if
			}// end outer if
		}// end inner for loop
	}// end outer for loop
	return openedZero;
}// end function

//function of opening all adjacent zero by using Breath First Search method
//it works as revealing adjacent nodes for every node in every level of BFS depth
//stop the loop once all the node in a certain level of depth does not reveal another zero
//input: integer of row and integer of column
void Minesweeper::openZeroBFS(int ro, int col) {
	int startRow, endRow, startColumn, endColumn, secondLoop = 30, limitLoop = 0;
	bool openedNodes, tempBool;
	startRow = ro - 1;
	endRow = ro + 1;
	startColumn = col - 1;
	endColumn = col + 1;
	openedNodes = openZeroVicinity(ro, col);
	//loop the BFS for two times, first for anticlockwise direction and second for clockwise direction
	//Doing this to ensure all the zero nodes are open correctly
	for (int k = 0; k < 2; k++) {
		//loop until there is no opened zero left
		//or the second loop is the same number of the first loop
		while (openedNodes || secondLoop < limitLoop) {
			openedNodes = false;
			//left column loop for first loop, right column loop for second loop
			for (int i = startRow; i < endRow; i++) {
				if (i >= 0 && i < row && startColumn >= 0 && startColumn < column) {
					if (board[i][startColumn] == "0R") {
						tempBool = openZeroVicinity(i, startColumn);
						if (tempBool) {
							openedNodes = true;
						}
					}
				}
			}
			//bottom row loop from left to right for first loop, right to left for second loop
			//This can be achieved by ternary operator function
			for (int i = startColumn; ((k == 0) ? i : endColumn) < ((k == 0) ? endColumn : i); (k == 0) ? i++ : i--) {
				if (i >= 0 && i < column && endRow >= 0 && endRow < row) {
					if (board[endRow][i] == "0R") {
						tempBool = openZeroVicinity(endRow, i);
						if (tempBool) {
							openedNodes = true;
						}
					}
				}
			}
			//right column loop for first loop, left column loop for second loop
			for (int i = endRow; i > startRow; i--) {
				if (i >= 0 && i < row && endColumn >= 0 && endColumn < column) {
					if (board[i][endColumn] == "0R") {
						tempBool = openZeroVicinity(i, endColumn);
						if (tempBool) {
							openedNodes = true;
						}
					}
				}
			}
			//upper row loop from right to left for first loop, left to right for second loop
			//This can be achieved by ternary operator function
			for (int i = endColumn; ((k == 0) ? i : startColumn) > ((k == 0) ? startColumn : i); (k == 0) ? i-- : i++) {
				if (i >= 0 && i < column && startRow >= 0 && startRow < row) {
					if (board[startRow][i] == "0R") {
						tempBool = openZeroVicinity(startRow, i);
						if (tempBool) {
							openedNodes = true;
						}
					}
				}
			}
			startRow = startRow - 1;
			endRow = endRow + 1;
			startColumn = (k == 0) ? startColumn - 1 : startColumn + 1;	//ternary function to distinguish the loop
			endColumn = (k == 0) ? endColumn + 1 : endColumn - 1;
			secondLoop += 1;	//collect the total number of first loop AND counter for second loop
		}//end while
		//set the loop property for second loop
		startRow = ro - 1;
		endRow = ro + 1;
		startColumn = col + 1;
		endColumn = col - 1;

		limitLoop = secondLoop - 30;
		secondLoop = 0;
	}//end for loop
}


