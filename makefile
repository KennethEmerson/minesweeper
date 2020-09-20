#compile all files
all: minesweeper.o logic.o view.o fileIO.o
	@echo "Linking all object files"
	gcc minesweeper.o logic.o view.o fileIO.o -lSDL2 -o minesweeper

#compile minesweeper.c only
minesweeper.o: minesweeper.c minesweeper.h
	@echo "compiling minesweeper.c"
	gcc minesweeper.c -c -o minesweeper.o

#compile logic.c only
logic.o: logic.c minesweeper.h
	@echo "compiling logic.c"
	gcc logic.c -c -o logic.o

#compile view.c only
view.o: view.c minesweeper.h
	@echo "compiling view.c"
	gcc view.c -c -o view.o

#compile fileIO.c only
fileIO.o: fileIO.c minesweeper.h
	@echo "compiling fileIO.c"
	gcc fileIO.c -c -o fileIO.o