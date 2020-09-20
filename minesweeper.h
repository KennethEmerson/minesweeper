
#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

	#include <SDL2/SDL.h>

	#define ROWMAX 20  //max of rows that can be used in game
	#define COLMAX 25  //max of columns that can be used in game
	#define MAXPLACEMINEATTEMPTS 1000 //max of unsuccesfull attempts allowed for placing mines

	#define IMAGE_HEIGHT 50
	#define IMAGE_WIDTH 50

	//logic.c functions
	int areOnlyMinesLeft(int row, int col, char (*mineMap)[col],char(*userMap)[col]);
	int doFlagsMatchMines(int row, int col, char (*mineMap)[col],char(*userMap)[col]);
	int checkCell(int row, int col, char (*mineMap)[col],char(*userMap)[col], int x, int y);
	void revealCells(int row, int col, char (*mineMap)[col],char(*userMap)[col],int x,int y);
	char countAdjMines(int row, int col, char (*map)[col],int x, int y);
	int countFlags(int row, int col, char (*map)[col]);
	int addFlag(int row, int col, char (*userMap)[col],int x, int y, int maxMines);
	void createEmptyMap(int row, int col, char (*map)[col]);
	void placeMines(int row, int col,char (*mineMap)[col], int mines,int exclX, int exclY, int maxIter);
	void composeSolution(int row, int col,char (*mineMap)[col],char (*userMap)[col]);
	
	//view.c functions
	void initialize_window(const char *title, int width, int height);
	void initialize_textures(void);
	void initialize_gui(int row, int col);
	void free_gui(void);
	int eventfilter(void* unused, SDL_Event *event);
	int readTilePressed(int *x, int *y,char *button);
	int findTexture(char symbol);
	void draw_tile(int x, int y,char symbol);
	void updateGUI(int row, int col,char (*map)[col]);
	void GuiMsgbox(int endgame);
	void updateGUI(int row, int col,char (*Map)[col]);

	//fileIO.C functions
	void writeGameToFile(char fileName[],int row, int col, int mines, char (*userMap)[col],char (*mineMap)[col]);
	int gameInit(int argc, char *argv[],char *filename,int *row,int *col, int * mines);
	int readParametersFromFile(char filename[],int *row, int *col, int *mines);
	int readMapsFromFile(char filename[],int row, int col,char (*userMap)[col],char (*mineMap)[col]);
#endif
