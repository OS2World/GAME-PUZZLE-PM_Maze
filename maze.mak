cp=cl -c -W3 -AL -G2sw -Os -Zpei

.c.obj:
    $(cp) $*.c

.rc.res:
   rc -r $*.rc

maze.obj: maze.c maze.h

maze.res: maze.rc maze.h

maze.exe: maze.obj maze.res maze.lnk maze.def
    link @maze.lnk
    rc maze.res
