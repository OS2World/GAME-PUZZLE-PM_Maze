/*============================================
  MAZE.C : PM Puzzle Maze Game
  Version: 1.06
  License: GNU GPL V3 License
  Authors:
  - James L. Dean, 1988 (original)
  - Martin Iturbide, 2023
  - OS2World, 2026 (Open Watcom port, plan.txt standardization)
  ============================================*/
/*
 *  MAZE.C -- A Puzzle Maze Game.
 *
 *  Ported to Open Watcom C (wcc386) and updated to follow the
 *  "OS/2 PM GAMES STANDARDIZATION PLAN" (plan.txt). The original
 *  GCC-era source is preserved under legacy/Source/.
 */

static const char bldlevel[] =
    "@#James L. Dean:1.06#@##1## 04 Oct 2026 20:00:00      "
    "ARCAOS:::0::::@@Maze game\r\n\x1a";

#define INCL_BASE
#define INCL_PM
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maze.h"
#include "lang.h"

#define BACKTRACK_COLOR CLR_RED
#define FLOOR_COLOR     CLR_BLACK
#define SOLUTION_COLOR  CLR_GREEN
#define WALL_COLOR      CLR_PALEGRAY

/* ============================================================================
   Language strings (plan.txt §8.2) - defined here, declared extern in lang.h
   ============================================================================ */
const char *lang_strings[LANG_COUNT][STR_COUNT] =
  {
    /* English */
    {
      "Maze",
      "~Game",
      "~New Game\tCtrl+N",
      "~Solve\tCtrl+S",
      "~Clear\tCtrl+C",
      "E~xit\tCtrl+X",
      "~Options",
      "~Language",
      "~Frame Controls\tCtrl+F",
      "~Save settings on exit",
      "~Help",
      "~About...",
      "Maze Help",
      "Maze 1.06",
      "Resize the window or select New Game for a new maze.",
      "Use the cursor keys (or 2, 4, 6, 8) to walk an unsolved maze.",
      "Select Clear to remove your path marks.",
      "Select Solve to have the computer solve the maze.",
      "Reach the upper right corner to finish."
    },
    /* Spanish */
    {
      "Maze",
      "~Juego",
      "~Nuevo Juego\tCtrl+N",
      "~Resolver\tCtrl+S",
      "~Limpiar\tCtrl+C",
      "S~alir\tCtrl+X",
      "~Opciones",
      "~Idioma",
      "~Controles Marco\tCtrl+F",
      "~Guardar al salir",
      "~Ayuda",
      "~About...",
      "Ayuda de Maze",
      "Maze 1.06",
      "Cambie el tamano de la ventana o elija Nuevo Juego para un nuevo laberinto.",
      "Use las teclas de cursor (o 2, 4, 6, 8) para recorrer un laberinto sin resolver.",
      "Elija Limpiar para borrar sus marcas de recorrido.",
      "Elija Resolver para que la computadora resuelva el laberinto.",
      "Llegue a la esquina superior derecha para terminar."
    },
    /* Dutch */
    {
      "Maze",
      "~Spel",
      "~Nieuw Spel\tCtrl+N",
      "~Los Oplossen\tCtrl+S",
      "~Wissen\tCtrl+C",
      "~Afsluiten\tCtrl+X",
      "~Opties",
      "~Taal",
      "~Vensterbesturing\tCtrl+F",
      "~Opslaan bij afsluiten",
      "~Help",
      "~About...",
      "Maze Help",
      "Maze 1.06",
      "Pas het venster aan of kies Nieuw Spel voor een nieuw doolhof.",
      "Gebruik de pijltjestoetsen (of 2, 4, 6, 8) om een onopgelost doolhof te lopen.",
      "Kies Wissen om uw routemarkeringen te verwijderen.",
      "Kies Los Oplossen om de computer het doolhof te laten oplossen.",
      "Bereik de rechterbovenhoek om te eindigen."
    },
    /* German */
    {
      "Maze",
      "~Spiel",
      "~Neues Spiel\tCtrl+N",
      "~Loesen\tCtrl+S",
      "~Leeren\tCtrl+C",
      "~Beenden\tCtrl+X",
      "~Optionen",
      "~Sprache",
      "~Fenstersteuerung\tCtrl+F",
      "~Beim Beenden Speichern",
      "~Hilfe",
      "~About...",
      "Maze Hilfe",
      "Maze 1.06",
      "Aendern Sie die Fenstergroesse oder waehlen Sie Neues Spiel fuer ein neues Labyrinth.",
      "Benutzen Sie die Pfeiltasten (oder 2, 4, 6, 8) zum Wanden in einem ungeloesten Labyrinth.",
      "Waehlen Sie Leeren um Ihre Markierungen zu entfernen.",
      "Waehlen Sie Loesen damit der Computer das Labyrinth loest.",
      "Erreichen Sie die obere rechte Ecke zum Beenden."
    },
    /* French */
    {
      "Maze",
      "~Jeu",
      "~Nouvelle Partie\tCtrl+N",
      "~Resoudre\tCtrl+S",
      "~Effacer\tCtrl+C",
      "~Quitter\tCtrl+X",
      "~Options",
      "~Langue",
      "~Controles Fenetre\tCtrl+F",
      "~Sauver en Quittant",
      "~Aide",
      "~About...",
      "Aide de Maze",
      "Maze 1.06",
      "Redimensionnez la fenetre ou choisissez Nouvelle Partie pour un nouveau labyrinthe.",
      "Utilisez les touches curseur (ou 2, 4, 6, 8) pour parcourir un labyrinthe non resolu.",
      "Choisissez Effacer pour retirer vos marques de parcours.",
      "Choisissez Resoudre pour que l'ordinateur resout le labyrinthe.",
      "Atteignez le coin superieur droit pour terminer."
    },
    /* Italian */
    {
      "Maze",
      "~Gioco",
      "~Nuova Partita\tCtrl+N",
      "~Risolvi\tCtrl+S",
      "~Cancella\tCtrl+C",
      "E~sci\tCtrl+X",
      "~Opzioni",
      "~Lingua",
      "~Controlli Cornice\tCtrl+F",
      "~Salva all'Uscita",
      "~Aiuto",
      "~About...",
      "Aiuto Maze",
      "Maze 1.06",
      "Ridimensionare la finestra o scegliere Nuova Partita per un nuovo labirinto.",
      "Usare i tasti freccia (o 2, 4, 6, 8) per percorrere un labirinto non risolto.",
      "Scegliere Cancella per rimuovere i segni del percorso.",
      "Scegliere Risolvi per far risolvere il labirinto al computer.",
      "Raggiungere l'angolo in alto a destra per terminare."
    }
  };

/* ============================================================================
   Global state
   ============================================================================ */
int  current_lang = LANG_EN;
BOOL fSaveOnExit  = TRUE;

HWND hwndFrame   = NULLHANDLE;
HWND hwndObject  = NULLHANDLE;   /* parking window for frame controls */
HWND hwndTitleBar = NULLHANDLE;
HWND hwndSysMenu  = NULLHANDLE;
HWND hwndMinMax   = NULLHANDLE;
HWND hwndMenuBar  = NULLHANDLE;
BOOL bFrameHidden = FALSE;

typedef struct ROWREC /* rr */
                 {
                   char          *pchRowPtr;
                   struct ROWREC *prrPredecessorPtr;
                   struct ROWREC *prrSuccessorPtr;
                 } *pROWREC;

typedef struct STACK1REC /* s1 */
                 {
                   unsigned char    chIndex1;
                   struct STACK1REC *ps1NextPtr;
                 } *pSTACK1REC;

typedef struct STACK2REC /* s2 */
                 {
                   unsigned char    chIndex1;
                   unsigned char    chIndex2;
                   struct STACK2REC *ps2NextPtr;
                 } *pSTACK2REC;

/* ============================================================================
   Forward declarations
   ============================================================================ */
int main(void);
MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HelpDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
void set_language(HWND, int);
static void load_settings(void);
static void save_settings(void);
static HWND GetSubmenu(HWND, USHORT);
static void SendSetCheck(HWND, USHORT, BOOL);
static void CreateMaze(int *, int *, pROWREC *, pROWREC *, int *);
static void DestroyMaze(pROWREC *, pROWREC *);
static void ClearPaths(pROWREC *, int *);
static void SizeMaze(int *, int *, pROWREC *, pROWREC *, int *, int *,
                     int *, int *, int *, int *, int *, int *, int *,
                     int *, int *, int *);
static void PaintMaze(pROWREC *, int *, int *, int *, int *, int *, int *,
                      int *, HPS, int *);
static void OptionallyHaveComputerSolve(pROWREC *, pROWREC *, int *, int *,
                                        int *, int *);

/* ============================================================================
   Settings persistence (plan.txt §10)
   ============================================================================ */
static void load_settings(void)
{
    FILE *fp;
    int settings[2];

    settings[0] = 1;              /* saveonexit */
    settings[1] = LANG_EN;        /* current_lang */

    fp = fopen("Maze.cfg", "rb");
    if (fp != NULL) {
        (void) fread(settings, sizeof(int), 2, fp);
        fclose(fp);
    }

    fSaveOnExit = (settings[0] == 1);
    current_lang = settings[1];
    if (current_lang < 0 || current_lang >= LANG_COUNT)
        current_lang = LANG_EN;
}

static void save_settings(void)
{
    FILE *fp;
    int settings[2];

    if (!fSaveOnExit)
        return;

    settings[0] = 1;
    settings[1] = current_lang;

    fp = fopen("Maze.cfg", "wb");
    if (fp != NULL) {
        (void) fwrite(settings, sizeof(int), 2, fp);
        fclose(fp);
    }
}

/* ============================================================================
   Menu helpers (plan.txt §8.4)
   ============================================================================ */
static HWND GetSubmenu(HWND hmenu, USHORT id)
{
    MENUITEM mi;

    mi.iPosition = 0;
    if (WinSendMsg(hmenu, MM_QUERYITEM, MPFROM2SHORT(id, TRUE), MPFROMP(&mi)))
        return mi.hwndSubMenu;
    return (HWND)NULLHANDLE;
}

static void SendSetCheck(HWND hmenu, USHORT id, BOOL check)
{
    WinSendMsg(hmenu, MM_SETITEMATTR, MPFROM2SHORT(id, TRUE),
               MPFROM2SHORT(MIA_CHECKED, check ? MIA_CHECKED : 0));
}

void set_language(HWND hmenu, int lang)
{
    int i;
    HWND hGame, hOptions, hHelp, hLang;

    if (lang < 0 || lang >= LANG_COUNT)
        lang = LANG_EN;
    current_lang = lang;
    if (hmenu == NULLHANDLE)
        return;

    hGame = GetSubmenu(hmenu, IDM_SUBMENU_GAME);
    hOptions = GetSubmenu(hmenu, IDM_SUBMENU_OPTIONS);
    hHelp = GetSubmenu(hmenu, IDM_SUBMENU_HELP);
    hLang = GetSubmenu(hmenu, IDM_SUBMENU_LANGUAGE);

    WinSendMsg(hmenu, MM_SETITEMTEXT,
               MPFROM2SHORT(IDM_SUBMENU_GAME, TRUE), MPFROMP((PSZ)tr(STR_MENU_GAME)));
    WinSendMsg(hmenu, MM_SETITEMTEXT,
               MPFROM2SHORT(IDM_SUBMENU_OPTIONS, TRUE), MPFROMP((PSZ)tr(STR_MENU_OPTIONS)));
    WinSendMsg(hmenu, MM_SETITEMTEXT,
               MPFROM2SHORT(IDM_SUBMENU_HELP, TRUE), MPFROMP((PSZ)tr(STR_MENU_HELP)));

    if (hGame != NULLHANDLE) {
        WinSendMsg(hGame, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_NEW, TRUE), MPFROMP((PSZ)tr(STR_MENU_NEW)));
        WinSendMsg(hGame, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_SOLVE, TRUE), MPFROMP((PSZ)tr(STR_MENU_SOLVE)));
        WinSendMsg(hGame, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_CLEAR, TRUE), MPFROMP((PSZ)tr(STR_MENU_CLEAR)));
        WinSendMsg(hGame, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_EXIT, TRUE), MPFROMP((PSZ)tr(STR_MENU_EXIT)));
    }

    if (hOptions != NULLHANDLE) {
        WinSendMsg(hOptions, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_SUBMENU_LANGUAGE, TRUE), MPFROMP((PSZ)tr(STR_MENU_LANGUAGE)));
        WinSendMsg(hOptions, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_FRAME, TRUE), MPFROMP((PSZ)tr(STR_MENU_FRAME)));
        WinSendMsg(hOptions, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_SAVEONEXIT, TRUE), MPFROMP((PSZ)tr(STR_MENU_SAVEONEXIT)));
        SendSetCheck(hOptions, IDM_SAVEONEXIT, fSaveOnExit);
    }

    if (hHelp != NULLHANDLE) {
        WinSendMsg(hHelp, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_HELP, TRUE), MPFROMP((PSZ)tr(STR_MENU_HELP)));
        WinSendMsg(hHelp, MM_SETITEMTEXT,
                   MPFROM2SHORT(IDM_ABOUT, TRUE), MPFROMP((PSZ)tr(STR_MENU_ABOUT)));
    }

    for (i = 0; i < LANG_COUNT; i++)
        SendSetCheck(hLang, (USHORT)(IDM_LANG_EN + i), (i == lang));
}

/* ============================================================================
   Main program
   ============================================================================ */
int main(void)
{
    ULONG       ctldata;
    HAB         hab;
    HMQ         hmq;
    HWND        hwndClient;
    QMSG        qmsg;
    LONG        cxScreen;
    LONG        cyScreen;
    LONG        winW;
    LONG        winH;
    LONG        x;
    LONG        y;
    static CHAR szClientClass[] = "Maze";

    hab = WinInitialize(0);
    if (hab == NULLHANDLE)
        return 1;
    hmq = WinCreateMsgQueue(hab, 0);
    if (hmq == NULLHANDLE) {
        WinTerminate(hab);
        return 1;
    }

    load_settings();

    if (!WinRegisterClass(hab, (PCH)szClientClass, (PFNWP)ClientWndProc,
                          CS_SYNCPAINT | CS_SIZEREDRAW, 0)) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      "Cannot register window class", "Maze",
                      0, MB_OK | MB_ERROR);
        WinDestroyMsgQueue(hmq);
        WinTerminate(hab);
        return 1;
    }

ctldata = FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU | FCF_MINMAX |
              FCF_TASKLIST | FCF_SIZEBORDER | FCF_SHELLPOSITION | FCF_ACCELTABLE |
              FCF_ICON;
    hwndFrame = WinCreateStdWindow(HWND_DESKTOP, 0, &ctldata,
                                   (PCH)szClientClass, (PSZ)tr(STR_APPNAME),
                                   0L, (HMODULE)NULLHANDLE, ID_MAINMENU,
                                   (PHWND)&hwndClient);
    if (hwndFrame == NULLHANDLE) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      "Cannot create main window", "Maze",
                      0, MB_OK | MB_ERROR);
        WinDestroyMsgQueue(hmq);
        WinTerminate(hab);
        return 1;
    }

    /* Capture frame-control handles AFTER the frame exists (plan.txt §16.10)
       and create the parking window used to hide them. */
    hwndTitleBar = WinWindowFromID(hwndFrame, FID_TITLEBAR);
    hwndSysMenu  = WinWindowFromID(hwndFrame, FID_SYSMENU);
    hwndMinMax   = WinWindowFromID(hwndFrame, FID_MINMAX);
    hwndMenuBar  = WinWindowFromID(hwndFrame, FID_MENU);
    hwndObject   = WinCreateWindow(HWND_OBJECT, WC_FRAME, "",
                                   0L, 0, 0, 0, 0,
                                   NULLHANDLE, HWND_TOP, 0, NULL, NULL);

    /* Size and center the window (plan.txt §9):
       target 1024x768, shrink to fit smaller screens. */
    cxScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    cyScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
    winW = (cxScreen >= 1024L) ? 1024L : cxScreen;
    winH = (cyScreen >= 768L)  ? 768L  : cyScreen;
    x = (cxScreen - winW) / 2;
    y = (cyScreen - winH) / 2;
    WinSetWindowPos(hwndFrame, HWND_TOP, x, y, winW, winH,
                    SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW);

    while (WinGetMsg(hab, (PQMSG)&qmsg, (HWND)NULL, 0, 0))
WinDispatchMsg(hab, (PQMSG)&qmsg);

    save_settings();

    if (bFrameHidden) {
        WinSetParent(hwndTitleBar, hwndFrame, FALSE);
        WinSetParent(hwndSysMenu, hwndFrame, FALSE);
        WinSetParent(hwndMinMax, hwndFrame, FALSE);
        WinSetParent(hwndMenuBar, hwndFrame, FALSE);
    }

    if (hwndObject != NULLHANDLE)
        WinDestroyWindow(hwndObject);
    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}

/* ============================================================================
   Frame Controls helper (plan.txt §16.10) - called from WM_COMMAND.
   ============================================================================ */
static void ToggleFrameControls(HWND hwnd)
{
    HWND hwndList[4];
    int i;

    bFrameHidden = !bFrameHidden;

    hwndList[0] = hwndTitleBar;
    hwndList[1] = hwndSysMenu;
    hwndList[2] = hwndMinMax;
    hwndList[3] = hwndMenuBar;

    for (i = 0; i < 4; i++)
        WinSetParent(hwndList[i],
                     bFrameHidden ? hwndObject : hwndFrame, FALSE);

    WinSendMsg(hwndFrame, WM_UPDATEFRAME,
               MPFROMLONG(FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_MENU),
               NULL);
    WinInvalidateRect(hwndFrame, NULL, TRUE);
    WinUpdateWindow(hwndFrame);
}

/* ============================================================================
   Client window procedure
   ============================================================================ */
MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    static int     aiDeltaX [4] [24];
    static int     aiDeltaY [4] [24];
    static int     aiRN [8];
    static HWND    hMenuMain;
           HPS     hPS;
           int     iDeltaIndex1 = 0;
    static int     iFatalError;
    static int     iMagnitudeDeltaX;
    static int     iMagnitudeDeltaY;
    static int     iMaxX;
    static int     iMaxY;
    static int     iNumColumns;
    static int     iNumRows;
           int     iPassageFound;
    static int     iSolved;
    static int     iTwiceMagnitudeDeltaX;
    static int     iTwiceMagnitudeDeltaY;
    static int     iX;
    static int     iXMax;
    static int     iYMax;
           int     iXNext;
           USHORT  cmd;
    static pROWREC prrCurrentPtr;
           pROWREC prrNextPtr;
    static pROWREC prrRowHead;
    static pROWREC prrRowTail;
    static POINTL  ptlPosition;
           ULONG   usFrequency;

    switch (msg)
      {
        case WM_CREATE:
          iSolved = TRUE;
          iMaxX = 0;
          iMaxY = 0;
          CreateMaze(&aiDeltaX[0][0], &aiDeltaY[0][0],
                     &prrRowHead, &prrRowTail, &iFatalError);
          hMenuMain = WinWindowFromID(WinQueryWindow(hwnd, QW_PARENT),
                                      FID_MENU);
          set_language(hMenuMain, current_lang);
          break;

        case WM_CHAR:
          if ((!iSolved) && (!iFatalError)
          &&  (iMaxX >= 20) && (iMaxY >= 20)
          &&  (!((ULONG)KC_KEYUP & (ULONG)mp1))) {
              iPassageFound = TRUE;
              if ((ULONG)KC_CHAR & (ULONG)mp1) {
                  switch (SHORT1FROMMP(mp2)) {
                    case '8':
                      iDeltaIndex1 = 1;
                      break;
                    case '4':
                      iDeltaIndex1 = 2;
                      break;
                    case '6':
                      iDeltaIndex1 = 0;
                      break;
                    case '2':
                      iDeltaIndex1 = 3;
                      break;
                    default:
                      iPassageFound = FALSE;
                      break;
                  }
              } else {
                  if ((ULONG)KC_VIRTUALKEY & (ULONG)mp1) {
                      switch (SHORT2FROMMP(mp2)) {
                        case VK_UP:
                          iDeltaIndex1 = 1;
                          break;
                        case VK_LEFT:
                          iDeltaIndex1 = 2;
                          break;
                        case VK_RIGHT:
                          iDeltaIndex1 = 0;
                          break;
                        case VK_DOWN:
                          iDeltaIndex1 = 3;
                          break;
                        default:
                          iPassageFound = FALSE;
                          break;
                      }
                  } else {
                      iPassageFound = FALSE;
                  }
              }
              if (iPassageFound) {
                  switch (aiDeltaY[iDeltaIndex1][0]) {
                    case -1:
                      iXNext = iX;
                      prrNextPtr = prrCurrentPtr->prrPredecessorPtr;
                      break;
                    case 1:
                      iXNext = iX;
                      prrNextPtr = prrCurrentPtr->prrSuccessorPtr;
                      break;
                    default:
                      iXNext = iX + aiDeltaX[iDeltaIndex1][0];
                      prrNextPtr = prrCurrentPtr;
                      break;
                  }
                  if (*((prrNextPtr->pchRowPtr) + iXNext) == 'W') {
                      iPassageFound = FALSE;
                  } else {
                      if (prrNextPtr->prrPredecessorPtr == NULL) {
                          iPassageFound = FALSE;
                      } else {
                          hPS = WinGetPS(hwnd);
                          GpiMove(hPS, &ptlPosition);
                          if (*((prrNextPtr->pchRowPtr) + iXNext) == 'S') {
                              GpiSetColor(hPS, BACKTRACK_COLOR);
                              *((prrCurrentPtr->pchRowPtr) + iX) = 'A';
                              *((prrNextPtr->pchRowPtr) + iXNext) = 'A';
                          } else {
                              GpiSetColor(hPS, SOLUTION_COLOR);
                              *((prrNextPtr->pchRowPtr) + iXNext) = 'S';
                          }
                          switch (aiDeltaY[iDeltaIndex1][0]) {
                            case -1:
                              prrNextPtr = prrNextPtr->prrPredecessorPtr;
                              ptlPosition.y -= iTwiceMagnitudeDeltaY;
                              break;
                            case 1:
                              prrNextPtr = prrNextPtr->prrSuccessorPtr;
                              if (prrNextPtr == NULL)
                                  ptlPosition.y += iMagnitudeDeltaY;
                              else
                                  ptlPosition.y += iTwiceMagnitudeDeltaY;
                              break;
                            default:
                              iXNext += aiDeltaX[iDeltaIndex1][0];
                              ptlPosition.x +=
                                  (iTwiceMagnitudeDeltaX * aiDeltaX[iDeltaIndex1][0]);
                              break;
                          }
                          GpiLine(hPS, &ptlPosition);
                          WinReleasePS(hPS);
                          prrCurrentPtr = prrNextPtr;
                          iX = iXNext;
                          if (prrCurrentPtr == NULL) {
                              iSolved = TRUE;
                              usFrequency = 10;
                              for (iDeltaIndex1 = 1; iDeltaIndex1 <= 100;
                                   iDeltaIndex1++) {
                                  DosBeep(usFrequency, 56);
                                  usFrequency += 10;
                              }
                          } else {
                              *((prrCurrentPtr->pchRowPtr) + iX) = 'S';
                          }
                      }
                  }
              }
              if ((!iPassageFound)
              &&  (SHORT2FROMMP(mp2) != VK_NUMLOCK)
              &&  (SHORT2FROMMP(mp2) != VK_ALT))
                  DosBeep(120, 333);
          }
          return (MRESULT)1;

        case WM_COMMAND:
          cmd = COMMANDMSG(&msg)->cmd;
          switch (cmd) {
            case IDM_CLEAR:
              if (!iFatalError) {
                  ClearPaths(&prrRowHead, &iNumColumns);
                  ptlPosition.x = iMagnitudeDeltaX;
                  ptlPosition.y = iMagnitudeDeltaY;
                  prrCurrentPtr = prrRowHead->prrSuccessorPtr;
                  iX = 1;
                  iSolved = FALSE;
              }
              WinInvalidateRect(hwnd, NULL, FALSE);
              break;
            case IDM_NEW:
              if ((iMaxX >= 20) && (iMaxY >= 20)) {
                  SizeMaze(&aiDeltaX[0][0], &aiDeltaY[0][0],
                           &prrRowHead, &prrRowTail,
                           &iMagnitudeDeltaX, &iMagnitudeDeltaY,
                           &iMaxX, &iMaxY, &iNumColumns, &iNumRows, &aiRN[0],
                           &iTwiceMagnitudeDeltaX, &iTwiceMagnitudeDeltaY,
                           &iXMax, &iYMax, &iFatalError);
                  if (!iFatalError) {
                      ptlPosition.x = iMagnitudeDeltaX;
                      ptlPosition.y = iMagnitudeDeltaY;
                      prrCurrentPtr = prrRowHead->prrSuccessorPtr;
                      iX = 1;
                  }
              }
              iSolved = FALSE;
              WinInvalidateRect(hwnd, NULL, FALSE);
              break;
            case IDM_SOLVE:
              if ((!iFatalError) && (iMaxX >= 20) && (iMaxY >= 20)) {
                  ClearPaths(&prrRowHead, &iNumColumns);
                  OptionallyHaveComputerSolve(&prrRowHead, &prrRowTail,
                                              &aiDeltaX[0][0], &aiDeltaY[0][0],
                                              &iNumColumns, &iFatalError);
                  iSolved = TRUE;
              }
              WinInvalidateRect(hwnd, NULL, FALSE);
              break;
            case IDM_EXIT:
              WinPostMsg(hwndFrame, WM_CLOSE, (MPARAM)0, (MPARAM)0);
              break;
            case IDM_HELP:
              WinDlgBox(HWND_DESKTOP, hwnd, HelpDlgProc, NULLHANDLE,
                        IDD_HELPBOX, NULL);
              break;
            case IDM_ABOUT:
              WinDlgBox(HWND_DESKTOP, hwnd, AboutDlgProc, NULLHANDLE,
                        IDD_ABOUT, NULL);
              break;
            case IDM_FRAME:
              ToggleFrameControls(hwnd);
              SendSetCheck(GetSubmenu(hMenuMain, IDM_SUBMENU_OPTIONS),
                           IDM_FRAME, bFrameHidden);
              break;
            case IDM_SAVEONEXIT:
              fSaveOnExit = !fSaveOnExit;
              SendSetCheck(GetSubmenu(hMenuMain, IDM_SUBMENU_OPTIONS),
                           IDM_SAVEONEXIT, fSaveOnExit);
              break;
            default:
              if ((cmd >= IDM_LANG_EN) && (cmd <= IDM_LANG_LAST))
                  set_language(hMenuMain, cmd - IDM_LANG_EN);
              break;
          }
          break;

        case WM_SIZE:
          iSolved = FALSE;
          iMaxX = SHORT1FROMMP(mp2) - 1;
          iMaxY = SHORT2FROMMP(mp2) - 1;
          if ((iMaxX >= 20) && (iMaxY >= 20)) {
              SizeMaze(&aiDeltaX[0][0], &aiDeltaY[0][0], &prrRowHead,
                       &prrRowTail, &iMagnitudeDeltaX, &iMagnitudeDeltaY,
                       &iMaxX, &iMaxY, &iNumColumns, &iNumRows, &aiRN[0],
                       &iTwiceMagnitudeDeltaX, &iTwiceMagnitudeDeltaY,
                       &iXMax, &iYMax, &iFatalError);
              if (!iFatalError) {
                  ptlPosition.x = iMagnitudeDeltaX;
                  ptlPosition.y = iMagnitudeDeltaY;
                  prrCurrentPtr = prrRowHead->prrSuccessorPtr;
                  iX = 1;
              }
              iSolved = FALSE;
          }
          break;

        case WM_ERASEBACKGROUND:
          return (MRESULT)TRUE;

        case WM_PAINT:
          hPS = WinBeginPaint(hwnd, (HPS)NULL, (PRECTL)NULL);
          if ((iMaxX >= 20) && (iMaxY >= 20)) {
              PaintMaze(&prrRowHead, &iNumColumns, &iMagnitudeDeltaX,
                        &iMagnitudeDeltaY, &iTwiceMagnitudeDeltaX,
                        &iTwiceMagnitudeDeltaY, &iXMax, &iYMax, hPS,
                        &iFatalError);
          }
          WinEndPaint(hPS);
          break;

        case WM_DESTROY:
          DestroyMaze(&prrRowHead, &prrRowTail);
          break;

        default:
          return WinDefWindowProc(hwnd, msg, mp1, mp2);
      }
    return 0L;
}

/* ============================================================================
   Help dialog procedure - text is filled from language strings
   ============================================================================ */
MRESULT EXPENTRY HelpDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg)
      {
        case WM_INITDLG:
          WinSetWindowText(hwnd, (PSZ)tr(STR_HELP_TITLE));
          WinSetDlgItemText(hwnd, IDD_HELP_T1, (PSZ)tr(STR_HELP_L1));
          WinSetDlgItemText(hwnd, IDD_HELP_T2, (PSZ)tr(STR_HELP_L2));
          WinSetDlgItemText(hwnd, IDD_HELP_T3, (PSZ)tr(STR_HELP_L3));
          WinSetDlgItemText(hwnd, IDD_HELP_T4, (PSZ)tr(STR_HELP_L4));
          WinSetDlgItemText(hwnd, IDD_HELP_T5, (PSZ)tr(STR_HELP_L5));
          WinSetDlgItemText(hwnd, IDD_HELP_T6, (PSZ)tr(STR_HELP_L6));
          return (MRESULT)FALSE;

        case WM_COMMAND:
          switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
            case DID_CANCEL:
              WinDismissDlg(hwnd, TRUE);
              return 0L;
          }
          break;
      }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* ============================================================================
   About dialog procedure (plan.txt §11)
   ============================================================================ */
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg)
      {
        case WM_COMMAND:
          switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
            case DID_CANCEL:
              WinDismissDlg(hwnd, TRUE);
              return 0L;
          }
          break;
      }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* ============================================================================
   Maze generation and solving (ported from original, ANSI prototypes)
   ============================================================================ */
static void CreateMaze(int *piDeltaX, int *piDeltaY, pROWREC *pprrRowHead,
                       pROWREC *pprrRowTail, int *piFatalError)
{
    int iDeltaIndex1a;
    int iDeltaIndex1b;
    int iDeltaIndex1c;
    int iDeltaIndex1d;
    int iDeltaIndex2;

    *piFatalError = FALSE;
    *piDeltaX = 1;
    *(piDeltaY + 24) = 1;
    *(piDeltaX + 48) = -1;
    *(piDeltaY + 72) = -1;
    *piDeltaY = 0;
    *(piDeltaX + 24) = 0;
    *(piDeltaY + 48) = 0;
    *(piDeltaX + 72) = 0;
    iDeltaIndex2 = -1;
    for (iDeltaIndex1a = 0; iDeltaIndex1a < 4; iDeltaIndex1a++)
        for (iDeltaIndex1b = 0; iDeltaIndex1b < 4; iDeltaIndex1b++)
            if (iDeltaIndex1a != iDeltaIndex1b)
                for (iDeltaIndex1c = 0; iDeltaIndex1c < 4; iDeltaIndex1c++)
                    if ((iDeltaIndex1a != iDeltaIndex1c)
                    &&  (iDeltaIndex1b != iDeltaIndex1c))
                        for (iDeltaIndex1d = 0; iDeltaIndex1d < 4;
                             iDeltaIndex1d++)
                            if ((iDeltaIndex1a != iDeltaIndex1d)
                            &&  (iDeltaIndex1b != iDeltaIndex1d)
                            &&  (iDeltaIndex1c != iDeltaIndex1d)) {
                                iDeltaIndex2 = iDeltaIndex2 + 1;
                                *(piDeltaX + (24 * iDeltaIndex1a + iDeltaIndex2))
                                    = *piDeltaX;
                                *(piDeltaY + (24 * iDeltaIndex1a + iDeltaIndex2))
                                    = *piDeltaY;
                                *(piDeltaX + (24 * iDeltaIndex1b + iDeltaIndex2))
                                    = *(piDeltaX + 24);
                                *(piDeltaY + (24 * iDeltaIndex1b + iDeltaIndex2))
                                    = *(piDeltaY + 24);
                                *(piDeltaX + (24 * iDeltaIndex1c + iDeltaIndex2))
                                    = *(piDeltaX + 48);
                                *(piDeltaY + (24 * iDeltaIndex1c + iDeltaIndex2))
                                    = *(piDeltaY + 48);
                                *(piDeltaX + (24 * iDeltaIndex1d + iDeltaIndex2))
                                    = *(piDeltaX + 72);
                                *(piDeltaY + (24 * iDeltaIndex1d + iDeltaIndex2))
                                    = *(piDeltaY + 72);
                            }
    *pprrRowHead = NULL;
    *pprrRowTail = NULL;
}

static void SizeMaze(int *piDeltaX, int *piDeltaY, pROWREC *pprrRowHead,
                     pROWREC *pprrRowTail, int *piMagnitudeDeltaX,
                     int *piMagnitudeDeltaY, int *piMaxX, int *piMaxY,
                     int *piNumColumns, int *piNumRows, int *piRN,
                     int *piTwiceMagnitudeDeltaX, int *piTwiceMagnitudeDeltaY,
                     int *piXMax, int *piYMax, int *piFatalError)
{
    DATETIME   dateSeed;
    int        iColumnNum;
    int        iDeltaIndex1;
    int        iDeltaIndex2;
    int        iDigit;
    int        iDigitNum;
    int        iFinished;
    int        iRecurse;
    int        iRNIndex1;
    int        iRNIndex2;
    int        iRowNum;
    int        iSum;
    int        iTemInt;
    int        iX;
    int        iXNext;
    int        iXOut;
    int        iY;
    int        iYOut;
    char       *pchColumnPtr;
    pROWREC    prrCurrentPtr;
    pROWREC    prrNextPtr;
    pROWREC    prrPreviousPtr;
    pSTACK2REC ps2StackHead;
    pSTACK2REC ps2StackPtr;

    DosGetDateTime(&dateSeed);
    *piRN = dateSeed.year % 29;
    *(piRN + 1) = dateSeed.month;
    *(piRN + 2) = dateSeed.day % 29;
    *(piRN + 3) = dateSeed.hours;
    *(piRN + 4) = dateSeed.minutes % 29;
    *(piRN + 5) = dateSeed.seconds % 29;
    *(piRN + 6) = dateSeed.hundredths % 29;
    *(piRN + 7) = 0;
    *piNumColumns = (*piMaxX) / 10;
    *piNumRows = (*piMaxY) / 10;
    *piMagnitudeDeltaX = (*piMaxX) / (*piNumColumns) / 2;
    *piTwiceMagnitudeDeltaX = (*piMagnitudeDeltaX) + (*piMagnitudeDeltaX);
    *piMagnitudeDeltaY = (*piMaxY) / (*piNumRows) / 2;
    *piTwiceMagnitudeDeltaY = (*piMagnitudeDeltaY) + (*piMagnitudeDeltaY);
    *piXMax = *piTwiceMagnitudeDeltaX * (*piNumColumns);
    *piYMax = *piTwiceMagnitudeDeltaY * (*piNumRows);
    while (*pprrRowHead != NULL) {
        free((*pprrRowHead)->pchRowPtr);
        prrPreviousPtr = *pprrRowHead;
        *pprrRowHead = (*pprrRowHead)->prrSuccessorPtr;
        free((char *)prrPreviousPtr);
    }
    if ((*pprrRowHead = (struct ROWREC *)
         malloc((unsigned)sizeof(struct ROWREC))) == NULL)
        *piFatalError = TRUE;
    else {
        (*pprrRowHead)->prrPredecessorPtr = NULL;
        (*pprrRowHead)->prrSuccessorPtr = NULL;
        *pprrRowTail = *pprrRowHead;
        if (((*pprrRowHead)->pchRowPtr =
             malloc((unsigned)2 * (*piNumColumns) + 1)) == NULL)
            *piFatalError = TRUE;
        else {
            pchColumnPtr = (*pprrRowHead)->pchRowPtr;
            for (iColumnNum = 0; iColumnNum < 2 * (*piNumColumns) + 1;
                 iColumnNum++) {
                *pchColumnPtr = 'W';
                pchColumnPtr++;
            }
        }
    }
    iRowNum = 1;
    while ((iRowNum <= *piNumRows) && (!*piFatalError)) {
        if ((prrCurrentPtr = (struct ROWREC *)
             malloc((unsigned)sizeof(struct ROWREC))) == NULL)
            *piFatalError = TRUE;
        else {
            (*pprrRowTail)->prrSuccessorPtr = prrCurrentPtr;
            prrCurrentPtr->prrPredecessorPtr = *pprrRowTail;
            prrCurrentPtr->prrSuccessorPtr = NULL;
            *pprrRowTail = prrCurrentPtr;
            if ((prrCurrentPtr->pchRowPtr =
                 malloc((unsigned)2 * (*piNumColumns) + 1)) == NULL)
                *piFatalError = TRUE;
            else {
                pchColumnPtr = prrCurrentPtr->pchRowPtr;
                for (iColumnNum = 0; iColumnNum < 2 * (*piNumColumns) + 1;
                     iColumnNum++) {
                    *pchColumnPtr = 'W';
                    pchColumnPtr++;
                }
            }
        }
        if ((prrCurrentPtr = (struct ROWREC *)
             malloc((unsigned)sizeof(struct ROWREC))) == NULL)
            *piFatalError = TRUE;
        else {
            (*pprrRowTail)->prrSuccessorPtr = prrCurrentPtr;
            prrCurrentPtr->prrPredecessorPtr = *pprrRowTail;
            prrCurrentPtr->prrSuccessorPtr = NULL;
            *pprrRowTail = prrCurrentPtr;
            if ((prrCurrentPtr->pchRowPtr =
                 malloc((unsigned)2 * (*piNumColumns) + 1)) == NULL)
                *piFatalError = TRUE;
            else {
                pchColumnPtr = prrCurrentPtr->pchRowPtr;
                for (iColumnNum = 0; iColumnNum < 2 * (*piNumColumns) + 1;
                     iColumnNum++) {
                    *pchColumnPtr = 'W';
                    pchColumnPtr++;
                }
            }
        }
        iRowNum++;
    }
    iSum = 0;
    for (iDigitNum = 1; iDigitNum <= 3; iDigitNum++) {
        iDigit = *piRN;
        iRNIndex1 = 0;
        for (iRNIndex2 = 1; iRNIndex2 < 8; iRNIndex2++) {
            iTemInt = *(piRN + iRNIndex2);
            *(piRN + iRNIndex1) = iTemInt;
            iRNIndex1++;
            iDigit += iTemInt;
            if (iDigit >= 29)
                iDigit -= 29;
        }
        *(piRN + 7) = iDigit;
        iSum = 29 * iSum + iDigit;
    }
    iX = 2 * (iSum % (*piNumColumns)) + 1;
    iSum = 0;
    for (iDigitNum = 1; iDigitNum <= 3; iDigitNum++) {
        iDigit = *piRN;
        iRNIndex1 = 0;
        for (iRNIndex2 = 1; iRNIndex2 < 8; iRNIndex2++) {
            iTemInt = *(piRN + iRNIndex2);
            *(piRN + iRNIndex1) = iTemInt;
            iRNIndex1++;
            iDigit += iTemInt;
            if (iDigit >= 29)
                iDigit -= 29;
        }
        *(piRN + 7) = iDigit;
        iSum = 29 * iSum + iDigit;
    }
    iY = 2 * (iSum % (*piNumRows)) + 1;
    prrCurrentPtr = *pprrRowHead;
    for (iYOut = 0; iYOut < iY; iYOut++)
        prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
    iFinished = FALSE;
    iRecurse = TRUE;
    ps2StackHead = NULL;
    while ((!iFinished) && (!*piFatalError)) {
        if (iRecurse) {
            *((prrCurrentPtr->pchRowPtr) + iX) = ' ';
            iDeltaIndex1 = 0;
            do {
                iDeltaIndex2 = *piRN;
                iRNIndex1 = 0;
                for (iRNIndex2 = 1; iRNIndex2 < 8; iRNIndex2++) {
                    iTemInt = *(piRN + iRNIndex2);
                    *(piRN + iRNIndex1) = iTemInt;
                    iRNIndex1++;
                    iDeltaIndex2 += iTemInt;
                    if (iDeltaIndex2 >= 29)
                        iDeltaIndex2 -= 29;
                }
                *(piRN + 7) = iDeltaIndex2;
            } while (iDeltaIndex2 >= 24);
            iRecurse = FALSE;
        }
        while ((iDeltaIndex1 < 4)
        &&     (!iRecurse)
        &&     (!*piFatalError)) {
            iXNext = iX + 2 * (*(piDeltaX + (24 * iDeltaIndex1 + iDeltaIndex2)));
            if ((iXNext <= 0) || (iXNext >= 2 * (*piNumColumns)))
                iDeltaIndex1++;
            else {
                switch (*(piDeltaY + (24 * iDeltaIndex1 + iDeltaIndex2))) {
                  case -1:
                    prrNextPtr = (prrCurrentPtr->prrPredecessorPtr)->
                                 prrPredecessorPtr;
                    break;
                  case 1:
                    prrNextPtr = (prrCurrentPtr->prrSuccessorPtr)->
                                 prrSuccessorPtr;
                    break;
                  default:
                    prrNextPtr = prrCurrentPtr;
                    break;
                }
                if (prrNextPtr == NULL)
                    iDeltaIndex1++;
                else {
                    if (*((prrNextPtr->pchRowPtr) + iXNext) == 'W') {
                        if (iX == iXNext) {
                            if ((*(piDeltaY
                                 + (24 * iDeltaIndex1 + iDeltaIndex2))) == 1)
                                *(((prrCurrentPtr->prrSuccessorPtr)
                                   ->pchRowPtr) + iX) = ' ';
                            else
                                *(((prrCurrentPtr->prrPredecessorPtr)
                                   ->pchRowPtr) + iX) = ' ';
                        } else {
                            iXOut = (iX + iXNext) / 2;
                            *((prrCurrentPtr->pchRowPtr) + iXOut) = ' ';
                        }
                        iX = iXNext;
                        prrCurrentPtr = prrNextPtr;
                        if ((ps2StackPtr = (struct STACK2REC *)
                             malloc((unsigned)sizeof(struct STACK2REC)))
                            == NULL)
                            *piFatalError = TRUE;
                        else {
                            ps2StackPtr->ps2NextPtr = ps2StackHead;
                            ps2StackHead = ps2StackPtr;
                            ps2StackHead->chIndex1 =
                                (unsigned char)iDeltaIndex1;
                            ps2StackHead->chIndex2 =
                                (unsigned char)iDeltaIndex2;
                            iRecurse = TRUE;
                        }
                    } else {
                        iDeltaIndex1++;
                    }
                }
            }
        }
        if ((!iRecurse) && (!*piFatalError)) {
            iDeltaIndex1 = (int)ps2StackHead->chIndex1;
            iDeltaIndex2 = (int)ps2StackHead->chIndex2;
            ps2StackPtr = ps2StackHead;
            ps2StackHead = ps2StackHead->ps2NextPtr;
            free((char *)ps2StackPtr);
            if (ps2StackHead == NULL)
                iFinished = TRUE;
            else
                switch (*(piDeltaY + (24 * iDeltaIndex1 + iDeltaIndex2))) {
                  case -1:
                    prrCurrentPtr = (prrCurrentPtr->prrSuccessorPtr)->
                                    prrSuccessorPtr;
                    break;
                  case 1:
                    prrCurrentPtr = (prrCurrentPtr->prrPredecessorPtr)->
                                    prrPredecessorPtr;
                    break;
                  default:
                    iX -= (2 * (*(piDeltaX + (24 * iDeltaIndex1 + iDeltaIndex2))));
                    break;
                }
}
    }
    while (ps2StackHead != NULL) {
        ps2StackPtr = ps2StackHead;
        ps2StackHead = ps2StackHead->ps2NextPtr;
        free((char *)ps2StackPtr);
    }
    if (!*piFatalError) {
        *(((*pprrRowHead)->pchRowPtr) + 1) = 'S';
        *((((*pprrRowHead)->prrSuccessorPtr)->pchRowPtr) + 1) = 'S';
        *(((*pprrRowTail)->pchRowPtr) + (2 * (*piNumColumns) - 1)) = ' ';
    }
}

static void PaintMaze(pROWREC *pprrRowHead, int *piNumColumns,
                      int *piMagnitudeDeltaX, int *piMagnitudeDeltaY,
                      int *piTwiceMagnitudeDeltaX, int *piTwiceMagnitudeDeltaY,
                      int *piXMax, int *piYMax, HPS hPS, int *piFatalError)
{
    char    chPenColor;
    int     iColumnNum;
    int     iEven;
    char    *pchPixelPtr;
    pROWREC prrCurrentPtr;
    POINTL  ptlEndingPosition;
    POINTL  ptlStartingPosition;

    if (*piFatalError) {
        GpiSetColor(hPS, BACKTRACK_COLOR);
        ptlStartingPosition.x = 0;
        ptlStartingPosition.y = 0;
        ptlEndingPosition.x = *piXMax;
        ptlEndingPosition.y = *piYMax;
        GpiMove(hPS, &ptlStartingPosition);
        GpiBox(hPS, DRO_FILL, &ptlEndingPosition, 0L, 0L);
        DosBeep(60, 333);
    } else {
        GpiSetColor(hPS, FLOOR_COLOR);
        ptlStartingPosition.x = 0;
        ptlStartingPosition.y = 0;
        ptlEndingPosition.x = *piXMax;
        ptlEndingPosition.y = *piYMax;
        GpiMove(hPS, &ptlStartingPosition);
        GpiBox(hPS, DRO_FILL, &ptlEndingPosition, 0L, 0L);
        prrCurrentPtr = *pprrRowHead;
        iEven = TRUE;
        ptlStartingPosition.y = 0;
        ptlEndingPosition.y = 0;
        while (prrCurrentPtr != NULL) {
            if (iEven) {
                pchPixelPtr = (prrCurrentPtr->pchRowPtr) + 1;
                ptlStartingPosition.x = 0;
                ptlEndingPosition.x = (*piTwiceMagnitudeDeltaX);
                GpiSetColor(hPS, WALL_COLOR);
                chPenColor = ' ';
                for (iColumnNum = 1; iColumnNum <= *piNumColumns;
                     iColumnNum++) {
                    if (*pchPixelPtr == 'W') {
                        if (chPenColor != 'W') {
                            chPenColor = 'W';
                            GpiMove(hPS, &ptlStartingPosition);
                        }
                    } else {
                        if (chPenColor == 'W') {
                            GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = ' ';
                        }
                    }
                    ptlStartingPosition.x = ptlEndingPosition.x;
                    ptlEndingPosition.x += (*piTwiceMagnitudeDeltaX);
                    pchPixelPtr += 2;
                }
                if (chPenColor == 'W')
                    GpiLine(hPS, &ptlStartingPosition);
                ptlStartingPosition.y += (*piMagnitudeDeltaY);
                ptlEndingPosition.y = ptlStartingPosition.y;
            } else {
                pchPixelPtr = (prrCurrentPtr->pchRowPtr);
                ptlStartingPosition.x = -(*piMagnitudeDeltaX);
                ptlEndingPosition.x = (*piMagnitudeDeltaX);
                chPenColor = ' ';
                for (iColumnNum = 1; iColumnNum <= *piNumColumns;
                     iColumnNum++) {
                    switch (*pchPixelPtr) {
                      case 'A':
                        if (chPenColor != 'A') {
                            if (chPenColor != ' ')
                                GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = 'A';
                            GpiSetColor(hPS, BACKTRACK_COLOR);
                            GpiMove(hPS, &ptlStartingPosition);
                        }
                        break;
                      case 'S':
                        if (chPenColor != 'S') {
                            if (chPenColor != ' ')
                                GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = 'S';
                            GpiSetColor(hPS, SOLUTION_COLOR);
                            GpiMove(hPS, &ptlStartingPosition);
                        }
                        break;
                      default:
                        if (chPenColor != ' ') {
                            GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = ' ';
                        }
                        break;
                    }
                    ptlStartingPosition.x = ptlEndingPosition.x;
                    ptlEndingPosition.x += (*piTwiceMagnitudeDeltaX);
                    pchPixelPtr += 2;
                }
                if (chPenColor != ' ')
                    GpiLine(hPS, &ptlStartingPosition);
                ptlStartingPosition.y += (*piMagnitudeDeltaY);
                ptlEndingPosition.y = ptlStartingPosition.y;
            }
            iEven = !iEven;
            prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
        }
        ptlStartingPosition.x = 0;
        ptlEndingPosition.x = 0;
        for (iColumnNum = 1; iColumnNum <= *piNumColumns; iColumnNum++) {
            ptlStartingPosition.y = 0;
            ptlEndingPosition.y = *piTwiceMagnitudeDeltaY;
            prrCurrentPtr = *pprrRowHead;
            iEven = TRUE;
            chPenColor = ' ';
            GpiSetColor(hPS, WALL_COLOR);
            while (prrCurrentPtr != NULL) {
                if (!iEven) {
                    pchPixelPtr = (prrCurrentPtr->pchRowPtr)
                                  + 2 * (iColumnNum - 1);
                    if (*pchPixelPtr == 'W') {
                        if (chPenColor != 'W') {
                            chPenColor = 'W';
                            GpiMove(hPS, &ptlStartingPosition);
                        }
                    } else {
                        if (chPenColor == 'W') {
                            GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = ' ';
                        }
                    }
                    ptlStartingPosition.y = ptlEndingPosition.y;
                    ptlEndingPosition.y += (*piTwiceMagnitudeDeltaY);
                }
                iEven = !iEven;
                prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
            }
            if (chPenColor == 'W')
                GpiLine(hPS, &ptlStartingPosition);
            ptlStartingPosition.x += (*piMagnitudeDeltaX);
            ptlEndingPosition.x = ptlStartingPosition.x;
            ptlStartingPosition.y = 0;
            ptlEndingPosition.y = (*piMagnitudeDeltaY);
            prrCurrentPtr = *pprrRowHead;
            iEven = TRUE;
            chPenColor = ' ';
            while (prrCurrentPtr != NULL) {
                if (iEven) {
                    pchPixelPtr = (prrCurrentPtr->pchRowPtr)
                                  + 2 * iColumnNum - 1;
                    switch (*pchPixelPtr) {
                      case 'A':
                        if (chPenColor != 'A') {
                            if (chPenColor != ' ')
                                GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = 'A';
                            GpiSetColor(hPS, BACKTRACK_COLOR);
                            GpiMove(hPS, &ptlStartingPosition);
                        }
                        break;
                      case 'S':
                        if (chPenColor != 'S') {
                            if (chPenColor != ' ')
                                GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = 'S';
                            GpiSetColor(hPS, SOLUTION_COLOR);
                            GpiMove(hPS, &ptlStartingPosition);
                        }
                        break;
                      default:
                        if (chPenColor != ' ') {
                            GpiLine(hPS, &ptlStartingPosition);
                            chPenColor = ' ';
                        }
                        break;
                    }
                    ptlStartingPosition.y = ptlEndingPosition.y;
                    ptlEndingPosition.y += (*piTwiceMagnitudeDeltaX);
                    if (ptlEndingPosition.y > *piYMax)
                        ptlEndingPosition.y = *piYMax;
                }
                iEven = !iEven;
                prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
            }
            if (chPenColor != ' ')
                GpiLine(hPS, &ptlStartingPosition);
            ptlStartingPosition.x += (*piMagnitudeDeltaX);
            ptlEndingPosition.x = ptlStartingPosition.x;
        }
        ptlStartingPosition.x = *piXMax;
        ptlEndingPosition.x = *piXMax;
        ptlStartingPosition.y = 0;
        ptlEndingPosition.y = *piYMax;
        GpiSetColor(hPS, WALL_COLOR);
        GpiMove(hPS, &ptlStartingPosition);
        GpiLine(hPS, &ptlEndingPosition);
    }
}

static void DestroyMaze(pROWREC *pprrRowHead, pROWREC *pprrRowTail)
{
    pROWREC prrPreviousPtr;

    while (*pprrRowHead != NULL) {
        free((*pprrRowHead)->pchRowPtr);
        prrPreviousPtr = *pprrRowHead;
        *pprrRowHead = (*pprrRowHead)->prrSuccessorPtr;
        free((char *)prrPreviousPtr);
    }
    *pprrRowTail = NULL;
}

static void ClearPaths(pROWREC *pprrRowHead, int *piNumColumns)
{
    int     iX;
    pROWREC prrCurrentPtr;

    prrCurrentPtr = *pprrRowHead;
    while (prrCurrentPtr != NULL) {
        for (iX = 1; iX < 2 * (*piNumColumns); iX++)
            if (*((prrCurrentPtr->pchRowPtr) + iX) != 'W')
                *((prrCurrentPtr->pchRowPtr) + iX) = ' ';
        prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
    }
    *(((*pprrRowHead)->pchRowPtr) + 1) = 'S';
    *((((*pprrRowHead)->prrSuccessorPtr)->pchRowPtr) + 1) = 'S';
}

static void OptionallyHaveComputerSolve(pROWREC *pprrRowHead,
                                        pROWREC *pprrRowTail,
                                        int *piDeltaX, int *piDeltaY,
                                        int *piNumColumns, int *piFatalError)
{
    int           iFinished;
    int           iRecurse;
    int           iX;
    int           iXNext;
    pROWREC       prrCurrentPtr;
    pROWREC       prrNextPtr;
    pSTACK1REC    ps1StackHead;
    pSTACK1REC    ps1StackPtr;
    unsigned char uchDeltaIndex1 = 0;

    iX = 1;
    prrCurrentPtr = (*pprrRowHead)->prrSuccessorPtr;
    prrNextPtr = prrCurrentPtr->prrSuccessorPtr;
    iFinished = FALSE;
    iRecurse = TRUE;
    ps1StackHead = NULL;
    while ((!iFinished) && (!*piFatalError)) {
        if (iRecurse) {
            uchDeltaIndex1 = 0;
            iRecurse = FALSE;
        }
        while ((uchDeltaIndex1 < 4)
        &&     (!iFinished)
        &&     (!iRecurse)
        &&     (!*piFatalError)) {
            switch (*(piDeltaY + (24 * uchDeltaIndex1))) {
              case -1:
                iXNext = iX;
                prrNextPtr = prrCurrentPtr->prrPredecessorPtr;
                break;
              case 1:
                iXNext = iX;
                prrNextPtr = prrCurrentPtr->prrSuccessorPtr;
                break;
              default:
                prrNextPtr = prrCurrentPtr;
                iXNext = iX + (*(piDeltaX + (24 * uchDeltaIndex1)));
                break;
            }
            if (*((prrNextPtr->pchRowPtr) + iXNext) == ' ') {
                *((prrNextPtr->pchRowPtr) + iXNext) = 'S';
                switch (*(piDeltaY + (24 * uchDeltaIndex1))) {
                  case -1:
                    prrNextPtr = prrNextPtr->prrPredecessorPtr;
                    break;
                  case 1:
                    prrNextPtr = prrNextPtr->prrSuccessorPtr;
                    break;
                  default:
                    iXNext += (*(piDeltaX + (24 * uchDeltaIndex1)));
                    break;
                }
                if (prrNextPtr != NULL) {
                    *((prrNextPtr->pchRowPtr) + iXNext) = 'S';
                    iX = iXNext;
                    prrCurrentPtr = prrNextPtr;
                    if ((ps1StackPtr = (struct STACK1REC *)
                         malloc((unsigned)sizeof(struct STACK1REC)))
                        == NULL)
                        *piFatalError = TRUE;
                    else {
                        ps1StackPtr->ps1NextPtr = ps1StackHead;
                        ps1StackHead = ps1StackPtr;
                        ps1StackHead->chIndex1 = uchDeltaIndex1;
                        iRecurse = TRUE;
                    }
                } else {
                    iFinished = TRUE;
                }
            } else {
                uchDeltaIndex1++;
            }
        }
        if ((uchDeltaIndex1 >= 4) && (!*piFatalError)) {
            *((prrCurrentPtr->pchRowPtr) + iX) = ' ';
            iXNext = iX;
            prrNextPtr = prrCurrentPtr;
            uchDeltaIndex1 = ps1StackHead->chIndex1;
            ps1StackPtr = ps1StackHead;
            ps1StackHead = ps1StackHead->ps1NextPtr;
            free((char *)ps1StackPtr);
            switch (*(piDeltaY + (24 * uchDeltaIndex1))) {
              case -1:
                prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
                *((prrCurrentPtr->pchRowPtr) + iX) = ' ';
                prrCurrentPtr = prrCurrentPtr->prrSuccessorPtr;
                break;
              case 1:
                prrCurrentPtr = prrCurrentPtr->prrPredecessorPtr;
                *((prrCurrentPtr->pchRowPtr) + iX) = ' ';
                prrCurrentPtr = prrCurrentPtr->prrPredecessorPtr;
                break;
              default:
                iX -= (*(piDeltaX + (24 * uchDeltaIndex1)));
                *((prrCurrentPtr->pchRowPtr) + iX) = ' ';
                iX -= (*(piDeltaX + (24 * uchDeltaIndex1)));
                break;
            }
            uchDeltaIndex1++;
        }
    }
    if (!*piFatalError)
        *(((*pprrRowTail)->pchRowPtr) + (2 * (*piNumColumns) - 1)) = 'S';
    while (ps1StackHead != NULL) {
        ps1StackPtr = ps1StackHead;
        ps1StackHead = ps1StackHead->ps1NextPtr;
        free((char *)ps1StackPtr);
    }
}
