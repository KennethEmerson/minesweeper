#include <stdio.h>
#include "minesweeper.h"

//-------------------------------------------------------------------------------------
// gameInit: validates and evaluates the command line parameters
//           When an incorrect parameter is given an error will be given and the
//           program will be terminated
//           If a filename was given, 1 will be returned, otherwise 0 will be returned
//-------------------------------------------------------------------------------------
int gameInit(int argc, char *argv[],char *filename,int *row,int *col, int *mines){

    int tempWidth = *row;
    int tempHeight = *col;
    int tempMines = *mines;
    int useSavedFile = 0;
    int correctInput = 1;
    if(argc>1){
		for (int i = 1; i < argc; i+=2){    
            if((argv[i][0]== '-') && ((i+1)<argc)){                     //check for parameter id
                if(argv[i][2] == '\0'){                                 //check if parameter id is one char long
                    switch (argv[i][1]){                                //check which parameter id was given
                        case 'W':
                        case 'w': tempWidth = atoi(argv[i+1]);break;      //first value after parameter id to int (zero if invalid)
                        case 'H':
                        case 'h': tempHeight = atoi(argv[i+1]);break;      //first value after parameter id to int (zero if invalid)
                        case 'M':
                        case 'm': tempMines = atoi(argv[i+1]);break;    //first value after parameter id to int (zero if invalid)
                        case 'F':
                        case 'f': useSavedFile = 1;                     //a filename is given
                                  strcpy(filename,argv[i+1]);break;     
                        default: correctInput = 0;
                        }
                }
                else correctInput = 0;
            }
            else correctInput = 0;
        }
    } 
    
    //if invalid input or zero was given for row, col or mines the program will terminate
    if(!(correctInput && tempWidth && tempHeight && tempMines)){
        fprintf(stderr,"unkown command, program terminated\n");
        exit(1);
    }
    //check if row/col are between zero and limits
    if((tempWidth<0 || tempWidth>COLMAX || tempHeight<0 || tempHeight > ROWMAX || tempMines<0)){
        fprintf(stderr,"values not valid, program terminated\n");
        exit(1);
    }
    //check if a valid amount of mines is given
    if (tempMines >((tempWidth*tempHeight)-1)){
		fprintf(stderr,"number of mines exceeds available map tiles, program terminated\n");
		exit(1);
	}
    //set the values
    *row = tempWidth; *col = tempHeight; *mines = tempMines;
    return useSavedFile;
}


//-------------------------------------------------------------------------------------
// writeGameToFile: writes the gamestate to the given file as a ; separated file
//                  first row contains row;col;mines
//                  second row contains usermap array values
//                  third row contains minemap array values
//-------------------------------------------------------------------------------------

void writeGameToFile(char fileName[],int row, int col, int mines, char (*userMap)[col],char (*mineMap)[col]){
    FILE *file;
    file = fopen(fileName, "w");
    //write row;col;mines to first row
    fprintf(file,"%d;%d;%d;\n",row,col,mines);
    
    //write usermap array values to second row
    for(int i=0; i<row;i++){
        for(int j = 0;j<col;j++){
            fprintf(file,"%c;",userMap[i][j]);
        }
    }
    
    //write minemap array values to third row
    fprintf(file,"\n");
    for(int i=0; i<row;i++){
        for(int j = 0;j<col;j++){
            fprintf(file,"%c;",mineMap[i][j]);
        }
    }
    fclose(file);
}

//-------------------------------------------------------------------------------------
//readParametersFromFile: reads the row;col;mines from the first row
//                        if file doesn't exist 0 is returned else 1 is returned
//-------------------------------------------------------------------------------------

int readParametersFromFile(char filename[],int *row, int *col, int *mines){
    int fileExists = 1;  
    char *token;
    FILE *file;
    file = fopen(filename, "r");
    if (file != NULL) {
        char line[12];
        fscanf(file, "%s", line);   //read first line
        token = strtok(line, ";");  //tokenize the line and read first item
        *row = atoi(token);         
        token = strtok(NULL, ";");  //read second item from token
        *col = atoi(token);
        token = strtok(NULL, ";"); //read third item from token
        *mines = atoi(token);
        fclose(file);    
    }
    else fileExists = 0;
    return fileExists;  
}
//-------------------------------------------------------------------------------------
// readMapsFromFile: reads the usermap and minemap array data from the file
//                   if file doesn't exist 0 is returned else 1 is returned
//-------------------------------------------------------------------------------------

int readMapsFromFile(char filename[],int row, int col,char (*userMap)[col],char (*mineMap)[col]){
    int fileExists = 1;  
    char *token;
    FILE *file;
    file = fopen(filename, "r");
    if (file != NULL) {
        char line[(row*col*3)];
        fscanf(file, "%s", line);   //read first line with row;col;mines, result is ignored
        
        fscanf(file, "%s", line);   //read second line with usermap array values
        token = strtok(line, ";");  //tokenize the line and read first item
        userMap[0][0] = token[0];   //set first cell value from usermap
        for(int i=0; i<row;i++){    
            for(int j = 0;j<col;j++){
                if(i || j){
                     token = strtok(NULL, ";"); //read next item from token
                     userMap[i][j] = token[0];
                } 
            }
        }     
        fscanf(file, "%s", line);   //read third line with usermap array values
        token = strtok(line, ";");  //tokenize the line and read first item
        mineMap[0][0] = token[0];   //set first cell value from minemap
        for(int i=0; i<row;i++){
            for(int j = 0;j<col;j++){
                if(i || j){
                     token = strtok(NULL, ";"); //read next item from token
                     mineMap[i][j] = token[0];
                } 
            }
        }
        fclose(file);    
    }
    else fileExists = 0;
    return fileExists;  
}
