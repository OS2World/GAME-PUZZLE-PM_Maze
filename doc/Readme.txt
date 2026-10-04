MAZE 1.06
              A Puzzle Maze Game for OS/2 Presentation Manager

OVERVIEW
--------
Maze is a PM puzzle game that generates a maze and lets you walk through
it. The maze is always unique; resize the window or use the New Game
command to generate another one. The computer can solve the maze for
you, and you can clear your attempts and start over from the entrance.

The original game was written in 1988 by James L. Dean. Martin Iturbide
retargeted it to GCC on ArcaOS in 2023. This 1.06 release ports it to
the Open Watcom compiler and applies the OS/2 PM Games Standardization
Plan.

HOW TO PLAY
-----------
A maze appears in the window. Walls are grey, the floor is black.
The entrance is the lower left cell, the exit is the upper right cell.

When the maze is not yet solved:
  - Walk with the cursor keys, or the numeric keypad 2 (down), 4 (left),
    6 (right), 8 (up).
  - The green line marks your current path. Re-tracing a step turns it
    red (backtracking).
  - Reaching the upper right corner plays a rising beep sequence and
    solves the maze.
  - Bumping a wall or leaving the maze beeps.

CONTROLS
--------
  Ctrl+N        New Game (new maze, same window size)
  Ctrl+X        Exit the application
  Ctrl+F        Toggle Frame Controls (hide/show title bar and menu)
  Ctrl+S        Solve - let the computer find the solution
  Ctrl+C        Clear - remove your path marks
  Cursor keys   Walk the maze

  Game menu: New Game, Solve, Clear, Exit
  Options menu: Language, Frame Controls, Save settings on exit
  Help menu: Help, About

SETTINGS
--------
Language selection and the "Save settings on exit" choice are kept in
the file Maze.cfg in the working directory of the program. If you want
the program to remember your settings, check "Save settings on exit"
in the Options menu.

FILE LIST
---------
  Maze.exe            The program
  Readme              This file
  Changelog           Change history
  LICENSE             GNU GPL V3 license text
  Maze.cfg            Settings (created on exit when enabled)

DISCLAIMER
----------
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for details.

AUTHOR
------
  James L. Dean     Original author, 1988
  Martin Iturbide  GCC/ArcaOS port, 2023
  OS/2World        Open Watcom port, 2026