# Makefile for Maze (Open Watcom on OS/2)
# Compatible with wmake 2.0.1 on ArcaOS

# ============================================================================
# Configuration
# ============================================================================

NAME    = Maze
SRCDIR  = src
BINDIR  = bin

!ifndef WATCOM
WATCOM  = C:\WATCOM
!endif

!ifndef OS2TK
OS2TK   = C:\OS2TK45
!endif

# ============================================================================
# Tools
# ============================================================================

CC      = wcc386
LINK    = wlink
RC      = wrc

# ============================================================================
# Flags (per plan.txt §2)
# ============================================================================

CFLAGS  = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0
CFLAGS  = $(CFLAGS) -i=$(OS2TK)\h -i=$(SRCDIR)

RCFLAGS = -r -bt=os2 -i=$(OS2TK)\h -i=$(SRCDIR)

LFLAGS  = system os2v2 pm
LFLAGS  = $(LFLAGS) option stack=65536
LFLAGS  = $(LFLAGS) option map=$(BINDIR)\$(NAME).map

# ============================================================================
# Source files
# ============================================================================

MAZE_OBJ = $(BINDIR)\maze.obj

RCFILE  = $(SRCDIR)\$(NAME).rc
ROBJ    = $(BINDIR)\$(NAME).res
DEFFILE = $(SRCDIR)\$(NAME).def

# ============================================================================
# Targets
# ============================================================================

all : $(BINDIR)\$(NAME).exe .SYMBOLIC

$(BINDIR) :
	@if not exist $(BINDIR) mkdir $(BINDIR)

$(BINDIR)\$(NAME).exe : $(MAZE_OBJ) $(ROBJ) $(DEFFILE)
	@echo Linking $(NAME).exe...
	@$(LINK) $(LFLAGS) name $(BINDIR)\$(NAME).exe file $(MAZE_OBJ) library os2386.lib
	@echo Binding resources...
	@$(RC) -q -bt=os2 -fe=$(BINDIR)\$(NAME).exe $(ROBJ) $(BINDIR)\$(NAME).exe
	@if exist $(BINDIR)\$(NAME).exe echo BUILD OK

$(MAZE_OBJ) : $(SRCDIR)\maze.c $(SRCDIR)\maze.h $(SRCDIR)\lang.h $(BINDIR)
	@echo Compiling $<
	@$(CC) $(CFLAGS) -fo=$(MAZE_OBJ) $(SRCDIR)\maze.c

$(ROBJ) : $(RCFILE) $(SRCDIR)\maze.h $(SRCDIR)\maze.ico $(BINDIR)
	@echo Compiling resources...
	@$(RC) $(RCFLAGS) -fo=$(ROBJ) $(RCFILE)

clean : .SYMBOLIC
	@if exist $(BINDIR)\maze.obj del $(BINDIR)\maze.obj >nul
	@if exist $(BINDIR)\$(NAME).res del $(BINDIR)\$(NAME).res >nul
	@if exist $(BINDIR)\$(NAME).exe del $(BINDIR)\$(NAME).exe >nul
	@if exist $(BINDIR)\$(NAME).map del $(BINDIR)\$(NAME).map >nul
	@echo Clean complete