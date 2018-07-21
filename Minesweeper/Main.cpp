#include <iostream>
#include <fstream>
#include <string>

#include "minesweeper.h"

using namespace std;

//function that receives a string and ensures that the inputted string is a single char
//input: string
//output: char
char stringInputErrorHandle(string input) {
	while (input.length() > 1) {
		cout << "Input must be a single character." << endl;
		cout << "Input : ";
		cin >> input;
	}
	return input[0];
}

//function that receives a string and ensures that the inputted string is an integer number
//input: string
//output: positive integer number
int intInputErrorHandle(string input) {
	int num = 0;
	for (int i = 0; i < input.length(); i++) {
		//check if the current char is a numeric character
		if (input[i] - '0' < 0 || input[i] - '0' > 9) {
			return -1;
		}
		else {
			//mathematical transformation formula from ASCII character into numeric
			//num = num + (input[i] x 10^(input.length - i - 1))
			num += (input[i] - '0') * pow(10, (input.length() - i - 1));
		}
	}
	return num;
}


//function that provides a border output for the console
void borderInterface() {
	cout << endl;
	cout << "-------------------------------------------" << endl;
	cout << endl;
}

//function that interact with user and choose a save slot
//requirements: string package
//output: a string represents the save slot file name
string saveGame() {
	int slot;
	string response, saveFile;
	cout << "Select the save slot:\n";
	cout << "Save slot (1)\nSave slot (2)\nSave slot (3)\nInput: ";
	cin >> response;
	slot = intInputErrorHandle(response);
	//save slot input error handle
	while (slot <= 0 || slot > 3) {
		cout << "Save slot does not exist" << endl << endl;
		cout << "Select the save slot:\n";
		cout << "Save slot (1)\nSave slot (2)\nSave slot (3)\nInput: ";
		cin >> response;
		slot = intInputErrorHandle(response);
	}

	//determines the save slot file name
	if (slot == 1) {
		saveFile = "saveSlot1.txt";
	}
	else if (slot == 2) {
		saveFile = "saveSlot2.txt";
	}
	else if (slot == 3) {
		saveFile = "saveSlot3.txt";
	}
	return saveFile;
}

//function that controls the minesweeper game process from start to the end
//function provides interactions between the game object with the user
//requirements: Minesweeper class, string package
//input: integer of columns, integer of row, integer of bomb, and string for save filename
void playGame(int col, int row, int bomb, string filename) {
	Minesweeper myGame(col, row, bomb, filename);
	int inputColumn, inputRow, gameEnd = 0;
	string input, saveFile;
	char action;

	//loop while the game state is still ongoing
	while (gameEnd == 0) {
		borderInterface();
		cout << "You have " << myGame.getMarkCount() << " bombs left to mark!" << endl << endl;
		myGame.displayBoard();
		cout << "What would you like to do? (S)ave, (R)eveal, (M)ark, (U)nmark or (E)xit: ";
		cin >> input;
		action = stringInputErrorHandle(input);
		//play game interface input error handle
		while (action != 'S' && action != 's' && action != 'R' && action != 'r' && action != 'M' && action != 'm' && action != 'U' && action != 'u' && action != 'E' && action != 'e') {
			cout << "Invalid Input" << endl;
			cout << "What would you like to do? (S)ave, (R)eveal, (M)ark, (U)nmark or (E)xit: ";
			cin >> input;
			action = stringInputErrorHandle(input);
		}

		if (action == 'S' || action == 's') {	//save game
			saveFile = saveGame();
			gameEnd = 2;
		}
		else if (action == 'E' || action == 'e') {	//exit game
			return;
		}
		else {
			cout << "Please input the row and the column:" << endl;

			cout << "Row: ";
			cin >> input;
			inputRow = intInputErrorHandle(input);
			//input row error handle
			while (inputRow < 0 || inputRow >= row) {
				cout << "Inputted row does not exist." << endl;
				cout << "Row: ";
				cin >> input;
				inputRow = intInputErrorHandle(input);
			}

			cout << "Column: ";
			cin >> input;
			inputColumn = intInputErrorHandle(input);
			//input column error handle
			while (inputColumn < 0 || inputColumn >= col) {
				cout << "Inputted column does not exist." << endl;
				cout << "Column: ";
				cin >> input;
				inputColumn = intInputErrorHandle(input);
			}

			gameEnd = myGame.openNode(action, inputColumn, inputRow);
		}
	}
	//check the game status if the game either win, lose, game saved or still ongoing
	if (gameEnd == -1) {
		borderInterface();
		cout << "You Lose" << endl;
		myGame.displayBoard();
		system("pause");
	}
	else if (gameEnd == 1) {
		borderInterface();
		cout << "You Win" << endl;
		myGame.displayBoard();
		system("pause");
	}
	else if (gameEnd == 2) {
		borderInterface();
		myGame.saveBoard(saveFile);
		cout << "Game Saved" << endl;
		system("pause");
	}
}

//function that provides the new game user interface
//function provides the setting of the game before starting the game
//requirements: string package, playGame function
void newGame() {
	//the default value of the game setting
	int column = 10, row = 10, bomb = 20, limitBomb = (row*column) / 2;
	string response;
	char input = '0';
	while (input != 'P' && input != 'p' && input != 'E' && input != 'e') {
		borderInterface();
		cout << "Start a new game.\n" << endl;
		cout << "Settings:\n";
		cout << "(C)olumn : " << column << endl;
		cout << "(R)ow : " << row << endl;
		cout << "(B)omb : " << bomb << endl;
		cout << "(P)lay game" << endl;
		cout << "(E)xit to main menu" << endl;
		cout << "Input : ";
		cin >> response;

		input = stringInputErrorHandle(response);

		if (input == 'C' || input == 'c') {		//set column
			cout << "Enter new column: ";
			cin >> response;
			column = intInputErrorHandle(response);
			//column input error handle
			while (column < 2 || column > 30) {
				cout << "Column must be bigger than 1 and less than 30" << endl;
				cout << "Enter new column: ";
				cin >> response;
				column = intInputErrorHandle(response);
			}
			limitBomb = (row*column) / 2;
		}
		else if (input == 'R' || input == 'r') {	//set row
			cout << "Enter new row: ";
			cin >> response;
			row = intInputErrorHandle(response);
			//row input error handle
			while (row < 2 || row > 30) {
				cout << "Column must be bigger than 1 and less than 30" << endl;
				cout << "Enter new row: ";
				cin >> response;
				row = intInputErrorHandle(response);
			}
			limitBomb = (row*column) / 2;
		}
		else if (input == 'B' || input == 'b') {	//set bomb
			cout << "Enter new bomb: ";
			cin >> response;
			bomb = intInputErrorHandle(response);
			//bomb input error handle
			while (bomb < 1 || bomb > limitBomb) {
				cout << "Bomb must be bigger than 0 or less than " << limitBomb << endl;
				cout << "Enter new bomb: ";
				cin >> response;
				bomb = intInputErrorHandle(response);
			}
		}
		else if (input == 'P' || input == 'p') {	//play game
			if (bomb > limitBomb) {
				cout << "Number of bomb must not be higher than ((row * column)/2)" << endl;
				input = 'B';
			}
			else {
				playGame(column, row, bomb, "None");
			}
		}
		//New game setting user input error handle
		else if (input != 'E' && input != 'e') {
			cout << "Input is invalid" << endl << endl;
		}
	}
}

//function that provides the load game user interface
//function provides the state of previous saved game for user to continue the game
//requirements: fstream package, string package, playGame function, saveSlot1.txt, saveSlot2.txt, saveSlot3.txt
void loadGame() {
	ifstream afile;
	int ro = 0, col, bo, input;
	string loadFile, response;

	borderInterface();
	cout << "Choose a save slot to load:\nSave Slot (1)\nSave Slot (2)\nSave Slot(3)\nInput: ";
	cin >> response;
	input = intInputErrorHandle(response);
	//user input error handle
	while (input <= 0 || input > 3) {
		cout << "Invalid input number" << endl << endl;
		cout << "Choose a save slot:\nSave Slot (1)\nSave Slot (2)\nSave Slot(3)\nInput: ";
		cin >> response;
		input = intInputErrorHandle(response);
	}

	if (input == 1) {
		loadFile = "saveSlot1.txt";
	}
	else if (input == 2) {
		loadFile = "saveSlot2.txt";
	}
	else if (input == 3) {
		loadFile = "saveSlot3.txt";
	}
	afile.open(loadFile);
	afile >> ro;
	//play the game with the loaded game into playGame function
	if (ro > 0) {
		afile >> col;
		afile >> bo;
		playGame(ro, col, bo, loadFile);
	}
	afile.close();
	//if the save slot is empty, recursively called the function again
	if (ro == 0) {
		cout << "Save slot is empty." << endl;
		loadGame();
	}
}

//main function of the program
//This function is the start and main position when the program executed
//The function controls other function as a whole program
//function provides the main menu interface of Minesweeper game
int main() {
	string option = "0";

	//loop while user does not choose 3 or exit game
	while (option != "3") {
		borderInterface();
		cout << "Welcome to Minesweeper!\n" << endl;
		cout << "1. New game (1)\n";
		cout << "2. Load game (2)\n";
		cout << "3. Exit game (3)\n";
		cout << "Input: ";
		cin >> option;
		if (option == "1") {
			newGame();
		}
		else if (option == "2") {
			loadGame();
		}
		else {
			//user input error handle
			cout << "Invalid input." << endl;
		}
	};
};



