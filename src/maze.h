/*
  Maze - Header file
  Copyright (C) 1988 James L. Dean
  Copyright (C) 2023 Martin Iturbide
  Copyright (C) 2026 OS2World
*/

#ifndef MAZE_H
#define MAZE_H

/* ============================================================================
   Resource IDs
   ============================================================================ */
#define ID_MAINMENU 1

/* ============================================================================
   Menu IDs - reserved ranges per plan.txt §6.4
   Game menu (100-199)
   ============================================================================ */
#define IDM_NEW      101
#define IDM_SOLVE    102
#define IDM_CLEAR    103
#define IDM_EXIT     104

/* Options / detail items (200-299) */
#define IDM_FRAME      201
#define IDM_SAVEONEXIT 202

/* Language items (300-399) */
#define IDM_LANG_EN  300
#define IDM_LANG_ES  301
#define IDM_LANG_NL  302
#define IDM_LANG_DE  303
#define IDM_LANG_FR  304
#define IDM_LANG_IT  305
#define IDM_LANG_LAST IDM_LANG_IT

/* Help items (900-999) */
#define IDM_HELP     901
#define IDM_ABOUT    902

/* ============================================================================
   Submenu cascade IDs (1000-1099) - used at runtime for MM_QUERYITEM /
   MM_SETITEMTEXT during language switching (plan.txt §6.4)
   ============================================================================ */
#define IDM_SUBMENU_GAME     1000
#define IDM_SUBMENU_OPTIONS  1001
#define IDM_SUBMENU_LANGUAGE 1002
#define IDM_SUBMENU_HELP     1003

/* ============================================================================
   Dialogs
   ============================================================================ */
#define IDD_HELPBOX   1
#define IDD_ABOUT     2

/* Help dialog static text control IDs */
#define IDD_HELP_T1   61
#define IDD_HELP_T2   62
#define IDD_HELP_T3   63
#define IDD_HELP_T4   64
#define IDD_HELP_T5   65
#define IDD_HELP_T6   66

#endif /* MAZE_H */
