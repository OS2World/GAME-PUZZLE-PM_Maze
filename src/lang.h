/*
  Maze - Language support
  Copyright (C) 1988 James L. Dean
  Copyright (C) 2023 Martin Iturbide
  Copyright (C) 2026 OS2World
*/

#ifndef LANG_H
#define LANG_H

/* ============================================================================
   Language IDs (per plan.txt §8.1)
   ============================================================================ */
#define LANG_EN  0
#define LANG_ES  1
#define LANG_NL  2
#define LANG_DE  3
#define LANG_FR  4
#define LANG_IT  5
#define LANG_COUNT 6

/* ============================================================================
   String IDs (per plan.txt §8.2)
   ============================================================================ */
#define STR_APPNAME        0
#define STR_MENU_GAME      1
#define STR_MENU_NEW       2
#define STR_MENU_SOLVE     3
#define STR_MENU_CLEAR     4
#define STR_MENU_EXIT      5
#define STR_MENU_OPTIONS   6
#define STR_MENU_LANGUAGE  7
#define STR_MENU_FRAME     8
#define STR_MENU_SAVEONEXIT 9
#define STR_MENU_HELP      10
#define STR_MENU_ABOUT     11
#define STR_HELP_TITLE     12
#define STR_HELP_L1        13
#define STR_HELP_L2        14
#define STR_HELP_L3        15
#define STR_HELP_L4        16
#define STR_HELP_L5        17
#define STR_HELP_L6        18

#define STR_COUNT          19

extern int current_lang;
extern const char *lang_strings[LANG_COUNT][STR_COUNT];

#define tr(id) (lang_strings[current_lang][(id)])

#endif /* LANG_H */
