#pragma once
#include <string>


class Minesweeper {
public:
	Minesweeper();
	Minesweeper(int column, int row, int bomb, std::string filename);
	void saveBoard(std::string fileName);
	void displayBoard();
	int getMarkCount();
	int openNode(char action, int column, int row);

private:
	std::string** board;
	int column, row, bomb, mark, markCount, totalNodes, revealedNodes = 0, falseMark = 0;
	void loadBoard(std::string fileName);
	void initializedBomb(int bomb);
	void initializedNumber(int ro, int col);
	void resetBoard();
	void openZeroBFS(int ro, int col);
	bool openZeroVicinity(int ro, int col);
};