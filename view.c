#include <stdio.h>
#include <stdlib.h>
#include "minesweeper.h"

static SDL_Renderer *renderer;
static SDL_Window *window;  //holds the main window

//struct tile contains the texture tile and the corresponding ID character 
struct tile {
    char name;
    SDL_Texture *texture;
};

//tileNames contains all possible tile ID characters
static char tileNames[] = {'0','1','2','3','4','5','6','7','8','C','F','M','E','W','G'};
//tiles is array containing the struct tile for all available tiles
static struct tile tiles[sizeof(tileNames) / sizeof(char)];

//-------------------------------------------------------------------------------------
// initialize_window: creates the initial window and the renderer
//-------------------------------------------------------------------------------------

void initialize_window(const char *title, int width, int height) {

    // check if SDL can be initialised or re
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//create the window 
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                width, height, SDL_WINDOW_SHOWN);

    //check if the window could be created
	if (window == NULL) {
		printf("Couldn't set screen mode to required dimensions: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

//-------------------------------------------------------------------------------------
// initialize_textures: loads all imagefiles and builds the textures.
//                      textures are saved in the "static struct tile tiles" array
//                      if one file can't be found a error message is generated and the
//                      program ends.
//-------------------------------------------------------------------------------------

void initialize_textures(void) {
    char buff[15];
    char prefix[] ="Images/";
    char suffix[] = ".bmp";
	for(int i = 0; i < (sizeof(tileNames) / sizeof(char)); i++) {
        tiles[i].name = tileNames[i];

        snprintf(buff, 15, "%s%c%s",prefix,tileNames[i],suffix);
        SDL_Surface* texture_path = SDL_LoadBMP(buff);
        if (texture_path){
            tiles[i].texture = SDL_CreateTextureFromSurface(renderer, texture_path);
        }
        else {
            printf("imagefile not found: %s\n", SDL_GetError());
		exit(1);
        }
        SDL_FreeSurface(texture_path);
    }
}

//-------------------------------------------------------------------------------------
// eventfilter: defines the SDL eventfilter to only allow SDL_QUIT and 
//              SDL_MOUSEBUTTONDOWN events to be pushed to the event queue
//              in case of both events 1 is returned, else 0 is returned
//-------------------------------------------------------------------------------------

int eventfilter(void* unused, SDL_Event *event) {
	
	switch ((*event).type) {
	case SDL_QUIT:
		return 1;
		break;

	case SDL_MOUSEBUTTONDOWN:
		return 1;
		break;
	default: return 0;
	}
}	

//-------------------------------------------------------------------------------------
// initialize_gui: initialises the event filter, the window 
//                 and the textures of the tiles                
//-------------------------------------------------------------------------------------

void initialize_gui(int row, int col) {
	SDL_SetEventFilter(eventfilter,NULL);
    initialize_window("Minesweeper", row * IMAGE_WIDTH, col * IMAGE_HEIGHT);
	initialize_textures();
}

//-------------------------------------------------------------------------------------
// readTilePressed: waits for a new event on the event queue and checks if this 
//                  corresponds to the user closing the game window (SDL_QUIT)
//                  or the user pushing the left or right mouse button on the window
//                  If the game window is closed, 1 is returned (= game ended).
//                  If the left or right mouse button is pushed, the x and y value is
//                  adjusted and button is set to L or R. the function then returns 0
//-------------------------------------------------------------------------------------

int readTilePressed(int *x, int *y,char *button) {
	SDL_Event event;
    int endGame = 0;

	while (! SDL_PollEvent(&event)) {}

	switch (event.type) {
        case SDL_QUIT: 
            *x = 0;
            *y = 0;
            *button = 'Q';
            endGame = 3;
            break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button){
                case SDL_BUTTON_LEFT:       
                    *x = (event.button.x)/IMAGE_WIDTH;
                    *y = (event.button.y)/IMAGE_HEIGHT;
                    *button = 'L';
                    endGame = 0;
                    break;
        
                case SDL_BUTTON_RIGHT:       
                    *x = (event.button.x)/IMAGE_WIDTH;
                    *y = (event.button.y)/IMAGE_HEIGHT;
                    *button = 'R';
                    endGame = 0;
                    break;
            }
    }
    return endGame;
}

//-------------------------------------------------------------------------------------
// findTexture: searches the corresponding index in the tiles array
//              for the tile ID character given
//-------------------------------------------------------------------------------------

int findTexture(char symbol){
    int temp = 0;
    for (int i=0;i <(sizeof(tileNames) / sizeof(char));i++){
        if(symbol == tiles[i].name) temp = i;
    }
    return temp;
}

//-------------------------------------------------------------------------------------
// draw_tile: receives the ID character, takes the corresponding tile 
//            from the tiles array and places this on the x,y location of the window
//-------------------------------------------------------------------------------------

void draw_tile(int x, int y,char symbol){	
	
	SDL_Rect rectangle = { x, y, IMAGE_WIDTH, IMAGE_HEIGHT }; 
	SDL_RenderCopy(renderer,tiles[findTexture(symbol)].texture, NULL, &rectangle);
}

//-------------------------------------------------------------------------------------
// updateGUI: refreshes the GUI by placing all the corresponding tiles in the window
//            so the window corresponds to the tile ID characters in the given map array
//------------------------------------------------------------------------------------- 
void updateGUI(int row, int col,char (*map)[col]){
    SDL_RenderClear(renderer);  //clears renderer
    for(int i =0; i < row; i++){
        for(int j =0; j < col; j++){
            draw_tile(i*IMAGE_WIDTH,j*IMAGE_HEIGHT,map[i][j]);
        }
    }
    SDL_RenderPresent(renderer);                                    
}

//-------------------------------------------------------------------------------------
// GuiMsgbox: shows a messagebox. the messageSelector is used to select the message.
//-------------------------------------------------------------------------------------

void GuiMsgbox(int messageSelector){
	const char *message;
    SDL_Window *parent;
    parent = NULL;
    
    //select the correct message
    switch(messageSelector){
        case 1:  message = "GAME OVER!\n";break;
        case 2:  message = "Congratulations\n you won!\n";break;
        case 3:  message = "game ended by user";break;
        case 4:  message = "no more flags left";parent = window; break;
        default: message = "";break;
    }

    //define OK button
    const SDL_MessageBoxButtonData buttons[] = {{0, 0, "OK" }};
    
    //define colorscheme of messagebox
    const SDL_MessageBoxColorScheme colorScheme = {
        {{255,0,0},{0,255,0},{255,255,0},{0,0,255},{255,0,255}}};
    
    //define the messsagebox with message, buttons and colorscheme
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, parent,"Minesweeper", 
        message, SDL_arraysize(buttons), buttons, &colorScheme};
    
    //shows messagebox
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");   
    }  
}

//-------------------------------------------------------------------------------------
// free_gui: deallocates the SDL_textures, window and renderer from memory               
//-------------------------------------------------------------------------------------

void free_gui(void) {
	for(int i = 0; i < (sizeof(tileNames) / sizeof(char)); i++) {
        SDL_DestroyTexture(tiles[i].texture);
    }
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

//-------------------------------------------------------------------------------------