# Minesweeper
## Minesweeper Game C++ Project
Version control 2.11 (FINAL)
Last changed: 22 August 2017
created by Kelvin Benzali / 26445468 / kben19@student.monash.edu

## Program structure:
- main.cpp = main cpp files of the program in which controls all the UI of the program and minesweeper object
- Minesweeper.cpp = Minesweeper class file
- Minesweeper.h = Minesweeper header file



## ASSUMPTION:
- The game effectively can only be played within 2 and 30 number of rows and columns (as 30 rows and columns fit perfectly on 1920x1080 fullscreen resolution)
- The total number of the bombs can only be effectively played u to half of the total number of the nodes
- Save slot are available up to 3 slots to keep the game simple in terms of appearance and technical
- Save game automatically overwrite the previous saved game on the selected save slot
- Adjacent zero nodes are assumed to have same average distribution placements most of the time. Therefore, Breath First Search is more efficient than Depth First Search
- Accessors and mutators are not available for every variables to keep the game simple and because most of them are not used for current version


## Known Bugs:
- NONE
