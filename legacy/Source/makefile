# GNU make makefile
#
# Tools used:
#  Compile::Watcom Resource Compiler
#  Compile::GNU C
#  Make: GNU make
all : Maze.exe

Maze.exe : maze.obj maze.def maze.res
	gcc -Zomf maze.obj maze.def maze.res -o Maze.exe
	wrc maze.res

maze.obj : maze.c maze.h
	gcc -Wall -Zomf -c -O2 maze.c -o maze.obj

maze.res : maze.rc 
	wrc -r maze.rc

clean :
	rm -rf *exe *res *obj