#include <stdio.h>
#include <stdlib.h>

#include "minesweeper.h"

int main(int argc, char *argv[]){

	char filename[50]= "gamelog.csv"; 	//default filename when no custom filename is given
	int row = 5;						//default value when no custom value is given
	int col = 5; 						//default value when no custom value is given
	int mines = 5;						//default value when no custom value is given
	
	//evaluate the command line parameters
	int useSavedFile = gameInit(argc, argv, filename, &row, &col, &mines);

	//if a filename was given, try to read the rows, cols and mines
	//if the file could not be read the default or command line values will be used
	if(useSavedFile){
		if(!readParametersFromFile(filename,&row, &col, &mines)){
			printf("file %s not found, new file will be created\n",filename);
			useSavedFile = 0;
		}
	}

	char mineMap[row][col];  			//contains the location of the mines
	char userMap[row][col];  			//contains the map presented to the user
	createEmptyMap(row, col, userMap); 	//usermap will be filled with 'C'
	createEmptyMap(row, col, mineMap); 	//minemap will be filled with 'C'
	
	int x,y;				 			//coordinates of cell
	char command;			 			//'L' or 'R' depending on mouse button pressed
	int endGame = 0;			 		//is end condition reached in game?
							 			// 0 = No, 1 = user lost, 
										// 2 = user wins, 
										// 3 = game ended by user

	
	//if a filename was passed, read maps from file
	if(useSavedFile){
		if(!readMapsFromFile(filename,row,col, userMap, mineMap)){
			useSavedFile = 0;
		}
	}

	initialize_gui(row, col);    					//create the GUI
	if(!useSavedFile){								//if no file was given
		updateGUI(row,col,userMap);					//show empty map
		endGame = readTilePressed(&x,&y,&command); 	//wait for selection of first tile
		if(endGame==0){								//if user hasn't ended the game
			placeMines(row,col,mineMap,mines,x,y,MAXPLACEMINEATTEMPTS);
			endGame = checkCell(row, col, mineMap,userMap,x,y); //update selected first tile
		}
	}

	//if all flags match all mines, user wins
	if(endGame==0){endGame = doFlagsMatchMines(row, col, mineMap,userMap);}
		//if all cells not containing mines are revealed, user wins
	if(endGame==0){endGame = areOnlyMinesLeft(row, col, mineMap,userMap);}

	while (endGame==0) {
		updateGUI(row,col,userMap);
		writeGameToFile(filename,row, col, mines, userMap, mineMap);
		endGame = readTilePressed(&x,&y,&command);
		switch(command){
			//if left mouse button is pressed
			case 'L': {endGame = checkCell(row, col, mineMap,userMap, x, y);break;}
			//if right button is pressed, a flag is added
			//when no more flags are available a messagebox is presented
			case 'R': if(addFlag(row, col, userMap,x, y, mines)){GuiMsgbox(4);}; break;
			default: break;
		}
		//if all flags match all mines, user wins
		if(endGame==0){endGame = doFlagsMatchMines(row, col, mineMap,userMap);}
		//if all cells not containing mines are revealed, user wins
		if(endGame==0){endGame = areOnlyMinesLeft(row, col, mineMap,userMap);}
	}
	//depending on endGame solution is presented and messagebox is shown
	//1: game over, 2: game won, 3: user ended game
	switch (endGame){
		case 1: composeSolution(row,col,mineMap,userMap);  
				updateGUI(row,col,userMap);
				GuiMsgbox(1); 
				break;
		case 2: composeSolution(row,col,mineMap,userMap);
				updateGUI(row,col,userMap);
				GuiMsgbox(2); 
				break;
		case 3: GuiMsgbox(3); break; //solution is not shown
	}	
	free_gui();   //deallocate memory
	return 0;
}