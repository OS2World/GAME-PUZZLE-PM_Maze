# GAME-PUZZLE-PM_Maze

Maze puzzle game for OS/2 Presentation Manager.

![PM Maze ScreenShot](/doc/wiki/PMMaze_001.png)

## LICENSE
* GNU GPL V3

## VERSION 1.06 (2026-10-04)

### Standardization
- Applied OS/2 PM Games Standardization Plan (plan.txt rev 2026-09-20)
- Standard folder layout: `src/`, `bin/`, `doc/`, `help/`, `legacy/`
- Ported from GCC to Open Watcom (`wcc386`, `wlink`, `wrc`) with `makefile.wat` + `compile-wat.cmd`
- BLDLEVEL embedded in C code (removed DESCRIPTION from .def per plan section 5)
- Standardized Game/Options/Help menu with reserved ID ranges, Ctrl+N / Ctrl+X / Ctrl+F accelerators
- Six-language support (EN/ES/NL/DE/FR/IT) with runtime menu switching and persisted setting
- About dialog per plan section 11 (Close button, centered credits)
- Frame Controls toggle (Ctrl+F) to hide/show title bar and menu
- Window sized and centered (1024x768, shrinks to fit smaller screens)
- Settings persisted to `Maze.cfg` in the working directory
- Documentation: `doc/Readme.txt`, `doc/Changelog.txt`, `doc/LICENSE.txt`

### Game Features (from original)
- Generates a unique maze sized to the window
- Walk the maze with cursor keys or the numeric keypad (2, 4, 6, 8)
- Clear removes your path marks
- Solve asks the computer to find the solution (green line)
- Backtracked steps are shown in red; winning beeps when you reach the exit

## COMPILE TOOLS
* Open Watcom C/C++ 1.9/2.0 (wcc386, wlink, wrc)
* OS/2 Toolkit 4.5 (os2tk45 headers)

## AUTHORS
* James L. Dean (original 1988)
* Martin Iturbide (2023 GCC/ArcaOS port)
* OS/2World (2026 Open Watcom port)

## LINKS
* https://www.os2world.com/games/index.php/native-games/puzzle/247-pm-maze
* https://github.com/OS2World/GAME-PUZZLE-PM_Maze