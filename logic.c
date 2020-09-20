#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "minesweeper.h"

//-------------------------------------------------------------------------------------
//createEmptyMap: fills 2D [char] array with ' ' 
//-------------------------------------------------------------------------------------

void createEmptyMap(int row, int col, char (*map)[col]){
	int i;
	for (i=0; i<row; i++){
		memset(map[i], 'C', col * sizeof(map[i][0]));
	}
}

//-------------------------------------------------------------------------------------
//placeMines: randomly places the given amount of mines in the map.
//            To prevent a potential eternal loop, the amount of
//			  non succesfull attempts is limited to maxIter	 
//-------------------------------------------------------------------------------------

void placeMines(int row, int col,char (*mineMap)[col], int mines,int exclX, int exclY, int maxIter){
	int counter = 0;
	int candRow,candCol;
	int iter = 0;  //counts the amount of iterations to prevent eternal loop
	int succesFlag = 0;
	srand (time(NULL));

	//repeat until all mines are placed on map
	while (counter<mines){
		candRow = (int)(((double)rand()/RAND_MAX)*row); //randomly choose candidate row
		candCol = (int)(((double)rand()/RAND_MAX)*col); //randomly choose candidate col

		//if the randomly selected cell already contains a mine or when the cell has
		//coord exclX exclY, the cell is ignored
		if(mineMap[candRow][candCol] != 'M' && !(candRow == exclX && candCol == exclY)){
			mineMap[candRow][candCol] = 'M';
			counter++;
		}
		else iter++;
		
		//checks if max amount of attempts is exceeded
		if (iter == maxIter){
			fprintf(stderr,"placing mines failed after %d unsuccesfull attempts\n", iter);
			exit(1);
		}
	}
	
}


//-------------------------------------------------------------------------------------
// checkCell: checks if cell contains a mine, if so a '+' is put in the cell and 1 is
//            returned to indicate the user has lost the game.
//			  if not, revealCells will be called to reveal all relevant cells
//-------------------------------------------------------------------------------------

int checkCell(int row, int col, char (*mineMap)[col],char(*userMap)[col], int x, int y){

	void revealCells(int row, int col, char (*mineMap)[col],char(*userMap)[col],int x,int y);
	int endGame = 0;

	if(mineMap[x][y]=='M'){
		userMap[x][y] ='E';
		endGame = 1;
	}
	else{
		revealCells(row,col,mineMap, userMap,x,y);
	}

	return endGame;
}


//-------------------------------------------------------------------------------------
//revealCells: reveals the amount of adjacent mines. If amount = 0 all adjacent cells 
//             are recursively revealed with their corresponding amount of adjacent mines
//-------------------------------------------------------------------------------------

void revealCells(int row, int col, char (*mineMap)[col],char(*userMap)[col],int x,int y){

	char minesCount = countAdjMines(row, col, mineMap, x, y);
	userMap[x][y]= minesCount;

	//when no adjacent mines are present
	if(minesCount=='0'){
		int lowBoundX, upBoundX, lowBoundY, upBoundY;
		int i,j;

		// set boundaries for checking adjacent cells
		if(x==0) lowBoundX = 0; else lowBoundX = x-1;   //is cell at left border
		if(x==row-1) upBoundX = x; else upBoundX = x+1; //is cell at upper border
		if(y==0) lowBoundY = 0; else lowBoundY = y-1;   //is cell at lower border
		if(y==col-1) upBoundY = y; else upBoundY = y+1; //is cell at right border

		//recursively reveals all cells within boundaries
		for(i=lowBoundX;i<=upBoundX;i++){
			for(j=lowBoundY;j<=upBoundY;j++){
				if(userMap[i][j]== 'C' && !(i==x && j==y)) revealCells(row,col,mineMap,userMap,i,j);
				}
			}
		}
	//if adjacent mines are present put amount in cell
	else userMap[x][y] = minesCount;

}

//-------------------------------------------------------------------------------------
//countAdjMines: counts all mines surrounding the given cell and returns amount as char
//-------------------------------------------------------------------------------------

char countAdjMines(int row, int col, char (*map)[col],int x, int y){
	int adjMines = 0;
	int lowBoundX, upBoundX, lowBoundY, upBoundY;
	int i,j;

	// set boundaries for checking adjacent cells
	if(x==0) lowBoundX = 0; else lowBoundX = x-1;    //is cell at left border
	if(x==row-1) upBoundX = x; else upBoundX = x+1;  //is cell at upper border
	if(y==0) lowBoundY = 0; else lowBoundY = y-1;    //is cell at lower border
	if(y==col-1) upBoundY = y; else upBoundY = y+1;  //is cell at right border

	for(i=lowBoundX;i<=upBoundX;i++){
		for(j=lowBoundY;j<=upBoundY;j++){
			if(map[i][j]== 'M' && !(i==x && j==y)) adjMines++;
		}
	}
	return (adjMines +'0');
}

//-------------------------------------------------------------------------------------
//countFlags: counts all flags placed by user
//-------------------------------------------------------------------------------------

int countFlags(int row, int col, char (*map)[col]){
	int i,j;
	int minesUsed = 0;

	for(i=0;i<row; i++){
		for(j=0;j<col;j++){
			if(map[i][j]=='F') minesUsed++;
		}
	}
	return minesUsed;
}

//-------------------------------------------------------------------------------------
//addFlag: adds a flag in the corresponding cell or removes it when a flag was already
//         present.
//		   when no more flags are available, 1 is returned and no flag is placed
//		   on the map
//-------------------------------------------------------------------------------------

int addFlag(int row, int col, char (*userMap)[col],int x, int y, int maxMines){
	int noMoreFlags = 0;
	if(userMap[x][y]!='F'){
		if (countFlags(row,col,userMap)< maxMines) userMap[x][y]='F';	
		else noMoreFlags = 1;		
	}
	else userMap[x][y] = 'C';
	return noMoreFlags;	
}

//-------------------------------------------------------------------------------------
// areOnlyMinesLeft: checks if only cells with mines are left on the usermap.
//                   is so returns 2 (=victory) else returns 0
//-------------------------------------------------------------------------------------

int areOnlyMinesLeft(int row, int col, char (*mineMap)[col],char(*userMap)[col]){
	int endGame = 2;
	int i,j;

	for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				if(!((mineMap[i][j]== 'M' && userMap[i][j]=='C') ||
						(mineMap[i][j]== 'M' && userMap[i][j]=='F') ||
						(mineMap[i][j]== 'C' && userMap[i][j]!='C' && userMap[i][j]!='F'))) endGame =0;
			}
	}
	return endGame;
}

//-------------------------------------------------------------------------------------
// doFlagsMatchMines: checks if all flags are positioned correctly on the mines
//                    is so returns 2 (=victory) else returns 0
//-------------------------------------------------------------------------------------

int doFlagsMatchMines(int row, int col, char (*mineMap)[col],char(*userMap)[col]){
	int endGame = 2;
	int i,j;

	for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				if(!((mineMap[i][j]== 'M' && userMap[i][j]=='F') ||
						(mineMap[i][j]== 'C' && userMap[i][j]!='F'))) endGame =0;
			}
	}
	return endGame;
}

//-------------------------------------------------------------------------------------
// composeSolution: fills the usermap with all correct placed flags,
//					all incorrect placed flags,	mines not discovered,
//					and the amount of adjacent mines per cell
//-------------------------------------------------------------------------------------

void composeSolution(int row, int col,char (*mineMap)[col],char (*userMap)[col]){
	int i,j;
	// fill usermap with solution including correct and incorrect placed flags
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(mineMap[i][j]=='M' && userMap[i][j]=='F') userMap[i][j] ='G';
			else if(mineMap[i][j]!='M' && userMap[i][j]=='F') userMap[i][j] ='W';
			else if(mineMap[i][j]=='M' && userMap[i][j]=='C') userMap[i][j] ='M';
			else if (userMap[i][j]=='C') revealCells(row, col, mineMap,userMap, i,j);
		}
	}
}
//-------------------------------------------------------------------------------------