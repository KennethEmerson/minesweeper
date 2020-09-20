# minesweeper
a simple minesweeper game written in C

the game accepts the following command line parameters:

    -w <int>        : the required number of columns (must be between 1 and COLMAX = 25)
    -h <int>        : the required number of rows (must be between 1 and ROWMAX = 20) 
    -m <int>        : the required number of mines
    -f <filename>   : the file to log the game, 
                      when an existing and valid gamelog file is given
                      the game will be loaded. The user can then continue the
                      game from the last logged state. 
                      Otherwise a new file will be created with the 
                      corresponding filename and the game will start with the passed 
                      parameters or the default parameters when none were passed as
                      command line parameter.

When no command line parameters are passed, a standard map of 5X5 with 5 mines is presented.

example: ./minesweeper -w 6 -h 7 -m 6

When a new game is started an empty map will be presented and the user will be able to
select a starting tile.
This command will reveal the selected tile. In the background all mines
will be randomly distributed in the other cells.

from then on when the user presses the left mouse button on a blank cell,
the number of mines surrounding the cell will be shown. If the cell contains
a mine the player has lost the game. If the number of mines surrounding
the cell is zero, the surrounding cells are also revealed.
    
When the user presses the right mouse button on a blank cell, a flag will be planted
in the cell marking a potential mine. The player can only plant as many flags 
as there are mines in the game, otherwise an error message will be shown.
The player can remove a flag by pressing the right mouse button on the cell containing
the flag.

During the game, the last gamestate will be logged in the gamelog.csv file or 
an other file if the filename was passed as a command line parameter. 
This file can be used to load the game at a later stage if the user ends the game by
closing the game window.

During the complete course of the game,  the user can end the game by closing the game window.

To win the game the player must reveal all cells not containing mines or
correctly place all flags in the cells containing mines. 

