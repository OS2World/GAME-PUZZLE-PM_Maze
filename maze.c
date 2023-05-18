//==========================================
// MAZE.C : PM Puzzle Maze Game
// Version: 1.05
// License: GNU GPL V3 License
// Authors:
// - Martin Iturbide, 2023
// - James L. Dean, 1988
//==========================================
/*
 *  MAZE.C -- A Puzzle Maze Game. 
*/
#define INCL_PM
#include <os2.h>
#include <stdlib.h>
#include <malloc.h>
#include "maze.h"

#define BACKTRACK_COLOR CLR_RED
#define FLOOR_COLOR     CLR_BLACK
#define SOLUTION_COLOR  CLR_GREEN
#define WALL_COLOR      CLR_PALEGRAY

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

static void      ClearPaths(pROWREC *,int *);
MRESULT EXPENTRY ClientWndProc(HWND,ULONG,MPARAM,MPARAM);
static void      CreateMaze(int *,int *,pROWREC *,pROWREC *,
                  int *);
static void      DestroyMaze(pROWREC *,pROWREC *);
MRESULT EXPENTRY HelpProc(HWND,ULONG,MPARAM,MPARAM);
int main(void);
static void      OptionallyHaveComputerSolve(pROWREC *,
                  pROWREC *,int *,int *,int *,int *);
static void      PaintMaze(pROWREC *,int *,int *,int *,int *,int *,
                  int *,int *,HPS,int *);
static void      SizeMaze(int *,int *,pROWREC *,pROWREC *,
                  int *,int *,int *,int *,int *,int *,int *,int *,int *,
                  int *,int *,int *);

int main(void)
  {
    ULONG       ctldata;
    HAB         hAB;
    HMQ         hmq;
    HWND        hwndClient;
    HWND        hwndFrame;
    QMSG        qmsg;
    static CHAR szClientClass [] = "Maze";

    hAB=WinInitialize(0);
    hmq=WinCreateMsgQueue(hAB,0);
    WinRegisterClass(hAB,(PCH) szClientClass,(PFNWP) ClientWndProc,
     CS_SYNCPAINT|CS_SIZEREDRAW,0);
    ctldata=FCF_STANDARD & ~FCF_ICON;
    hwndFrame=WinCreateStdWindow(HWND_DESKTOP,
     WS_VISIBLE,&ctldata,
     (PCH) szClientClass,NULL,0L,(HMODULE) NULL,ID_MAINMENU,
     (HWND FAR *) &hwndClient);
    WinShowWindow(hwndFrame,TRUE);
    while (WinGetMsg(hAB,(PQMSG) &qmsg,(HWND) NULL,0,0))
     WinDispatchMsg(hAB,(PQMSG) &qmsg);
    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue(hmq);
    WinTerminate(hAB);
    return(0);
  }

MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG msg,MPARAM mp1,
 MPARAM mp2)
  {
    static int     aiDeltaX [4] [24];
    static int     aiDeltaY [4] [24];
    static int     aiRN [8];
           HPS     hPS;
           // HWND    hwndMenu = 0;
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
    static pROWREC prrCurrentPtr;
           pROWREC prrNextPtr;
    static pROWREC prrRowHead;
    static pROWREC prrRowTail;
    static POINTL  ptlPosition;
           ULONG  usFrequency;

    switch (msg)
      {
        case WM_CREATE:
          iSolved=TRUE;
          iMaxX=0;
          iMaxY=0;
          CreateMaze(&aiDeltaX[0][0],&aiDeltaY[0][0],
           &prrRowHead,&prrRowTail,&iFatalError);
          break;
        case WM_CHAR:
          if ((! iSolved) && (! iFatalError)
          &&  (iMaxX >= 20) && (iMaxY >= 20)
          &&  (! ((ULONG) KC_KEYUP & (ULONG) mp1)))
            {
              iPassageFound=TRUE;
              if ((ULONG) KC_CHAR & (ULONG) mp1)
                switch (SHORT1FROMMP(mp2))
                  {
                    case '8':
                      iDeltaIndex1=1;
                      break;
                    case '4':
                      iDeltaIndex1=2;
                      break;
                    case '6':
                      iDeltaIndex1=0;
                      break;
                    case '2':
                      iDeltaIndex1=3;
                      break;
                    default:
                      iPassageFound=FALSE;
                      break;
                  }
              else
                {
                  if ((ULONG) KC_VIRTUALKEY & (ULONG) mp1)
                    switch (SHORT2FROMMP(mp2))
                      {
                        case VK_UP:
                          iDeltaIndex1=1;
                          break;
                        case VK_LEFT:
                          iDeltaIndex1=2;
                          break;
                        case VK_RIGHT:
                          iDeltaIndex1=0;
                          break;
                        case VK_DOWN:
                          iDeltaIndex1=3;
                          break;
                          break;
                        default:
                          iPassageFound=FALSE;
                          break;
                      }
                }
              if (iPassageFound)
                {
                  switch (aiDeltaY[iDeltaIndex1][0])
                    {
                      case -1:
                        iXNext=iX;
                        prrNextPtr=prrCurrentPtr->prrPredecessorPtr;
                        break;
                      case 1:
                        iXNext=iX;
                        prrNextPtr=prrCurrentPtr->prrSuccessorPtr;
                        break;
                      default:
                        iXNext=iX+aiDeltaX[iDeltaIndex1][0];
                        prrNextPtr=prrCurrentPtr;
                        break;
                    }
                  if (*((prrNextPtr->pchRowPtr)+iXNext) == 'W')
                    iPassageFound=FALSE;
                  else
                    if (prrNextPtr->prrPredecessorPtr == NULL)
                      iPassageFound=FALSE;
                    else
                      {
                        hPS=WinGetPS(hwnd);
                        GpiMove(hPS,&ptlPosition);
                        if (*((prrNextPtr->pchRowPtr)+iXNext) == 'S')
                          {
                            GpiSetColor(hPS,BACKTRACK_COLOR);
                            *((prrCurrentPtr->pchRowPtr)+iX)='A';
                            *((prrNextPtr->pchRowPtr)+iXNext)='A';
                          }
                        else
                          {
                            GpiSetColor(hPS,SOLUTION_COLOR);
                            *((prrNextPtr->pchRowPtr)+iXNext)='S';
                          }
                        switch (aiDeltaY[iDeltaIndex1][0])
                          {
                            case -1:
                              prrNextPtr=prrNextPtr->prrPredecessorPtr;
                              ptlPosition.y-=iTwiceMagnitudeDeltaY;
                              break;
                            case 1:
                              prrNextPtr=prrNextPtr->prrSuccessorPtr;
                              if (prrNextPtr == NULL)
                                ptlPosition.y+=iMagnitudeDeltaY;
                              else
                                ptlPosition.y+=iTwiceMagnitudeDeltaY;
                              break;
                            default:
                              iXNext+=aiDeltaX[iDeltaIndex1][0];
                              ptlPosition.x+=(iTwiceMagnitudeDeltaX
                               *aiDeltaX[iDeltaIndex1][0]);
                              break;
                          }
                        GpiLine(hPS,&ptlPosition);
                        WinReleasePS(hPS);
                        prrCurrentPtr=prrNextPtr;
                        iX=iXNext;
                        if (prrCurrentPtr == NULL)
                          {
                            iSolved=TRUE;
                            usFrequency=10;
                            for (iDeltaIndex1=1; iDeltaIndex1 <= 100;
                             iDeltaIndex1++)
                              {
                                DosBeep(usFrequency,56);
                                usFrequency+=10;
                              }
                          }
                        else
                          *((prrCurrentPtr->pchRowPtr)+iX)='S';
                      }
                }
              if ((! iPassageFound)
              &&  (SHORT2FROMMP(mp2) != VK_NUMLOCK)
              &&  (SHORT2FROMMP(mp2) != VK_ALT))
                DosBeep(120,333);
            }
          return((MRESULT) 1);
          break;
        case WM_COMMAND:
          // hwndMenu=WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT), FID_MENU);
          switch (COMMANDMSG(&msg)->cmd)
            {
              case IDM_CLEAR:
                if (! iFatalError)
                  {
                    ClearPaths(&prrRowHead,&iNumColumns);
                    ptlPosition.x=iMagnitudeDeltaX;
                    ptlPosition.y=iMagnitudeDeltaY;
                    prrCurrentPtr=prrRowHead->prrSuccessorPtr;
                    iX=1;
                    iSolved=FALSE;
                  }
                WinInvalidateRect(hwnd,NULL,FALSE);
                break;
              case IDM_NEW:
                if ((iMaxX >= 20) && (iMaxY >= 20))
                  {
                    SizeMaze(&aiDeltaX[0][0],&aiDeltaY[0][0],
                     &prrRowHead,&prrRowTail,
                     &iMagnitudeDeltaX,&iMagnitudeDeltaY,
                     &iMaxX,&iMaxY,&iNumColumns,&iNumRows,&aiRN[0],
                     &iTwiceMagnitudeDeltaX,&iTwiceMagnitudeDeltaY,
                     &iXMax,&iYMax,&iFatalError);
                    if (! iFatalError)
                      {
                        ptlPosition.x=iMagnitudeDeltaX;
                        ptlPosition.y=iMagnitudeDeltaY;
                        prrCurrentPtr=prrRowHead->prrSuccessorPtr;
                        iX=1;
                      }
                  }
                iSolved=FALSE;
                WinInvalidateRect(hwnd,NULL,FALSE);
                break;
              case IDM_SOLVE:
                if ((! iFatalError) && (iMaxX >= 20) && (iMaxY >= 20))
                  {
                    ClearPaths(&prrRowHead,&iNumColumns);
                    OptionallyHaveComputerSolve(&prrRowHead,&prrRowTail,
                     &aiDeltaX[0][0],&aiDeltaY[0][0],&iNumColumns,
                     &iFatalError);
                    iSolved=TRUE;
                  }
                WinInvalidateRect(hwnd,NULL,FALSE);
                break;
              case IDM_HELP:
                WinDlgBox(HWND_DESKTOP,hwnd,HelpProc,NULLHANDLE,IDD_HELPBOX,
                 NULL);
                break;
              default:
                break;
            }
          break;
        case WM_SIZE:
          iSolved=FALSE;
          iMaxX=SHORT1FROMMP(mp2)-1;
          iMaxY=SHORT2FROMMP(mp2)-1;
          if ((iMaxX >= 20) && (iMaxY >= 20))
            {
              SizeMaze(&aiDeltaX[0][0],&aiDeltaY[0][0],&prrRowHead,
               &prrRowTail,&iMagnitudeDeltaX,&iMagnitudeDeltaY,
               &iMaxX,&iMaxY,&iNumColumns,&iNumRows,&aiRN[0],
               &iTwiceMagnitudeDeltaX,&iTwiceMagnitudeDeltaY,
               &iXMax,&iYMax,&iFatalError);
              if (! iFatalError)
                {
                  ptlPosition.x=iMagnitudeDeltaX;
                  ptlPosition.y=iMagnitudeDeltaY;
                  prrCurrentPtr=prrRowHead->prrSuccessorPtr;
                  iX=1;
                }
              iSolved=FALSE;
            }
          break;
        case WM_ERASEBACKGROUND:
          return ((MRESULT)TRUE) ;
          break;
        case WM_PAINT:
          hPS=WinBeginPaint(hwnd,(HPS) NULL,(PWRECT) NULL);
          if ((iMaxX >= 20) && (iMaxY >= 20))
            PaintMaze(&prrRowHead,&iNumColumns,&iMagnitudeDeltaX,
             &iMagnitudeDeltaY,&iTwiceMagnitudeDeltaX,
             &iTwiceMagnitudeDeltaY,&iXMax,&iYMax,hPS,&iFatalError);
          WinEndPaint(hPS);
          break;
        case WM_DESTROY:
          DestroyMaze(&prrRowHead,&prrRowTail);
          break;
        default:
          return(WinDefWindowProc(hwnd,msg,mp1,mp2));
          break;
      }
    return(0L);
  }

MRESULT EXPENTRY HelpProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
  {
    switch (msg)
      {
        case WM_COMMAND:
          switch (COMMANDMSG(&msg)->cmd)
            {
              case DID_OK:
              case DID_CANCEL:
                WinDismissDlg(hwnd,TRUE);
                break;
              default:
                break;
            }
          break;
        default:
          return(WinDefDlgProc(hwnd,msg,mp1,mp2));
      }
    return(0);
  }

static void CreateMaze(piDeltaX,piDeltaY,pprrRowHead,pprrRowTail,
 piFatalError)
  int     *piDeltaX;
  int     *piDeltaY;
  pROWREC *pprrRowHead;
  pROWREC *pprrRowTail;
  int     *piFatalError;
    {
      int iDeltaIndex1a;
      int iDeltaIndex1b;
      int iDeltaIndex1c;
      int iDeltaIndex1d;
      int iDeltaIndex2;

      *piFatalError=FALSE;
      *piDeltaX=1;
      *(piDeltaY+24)=1;
      *(piDeltaX+48)=-1;
      *(piDeltaY+72)=-1;
      *piDeltaY=0;
      *(piDeltaX+24)=0;
      *(piDeltaY+48)=0;
      *(piDeltaX+72)=0;
      iDeltaIndex2=-1;
      for (iDeltaIndex1a=0; iDeltaIndex1a < 4; iDeltaIndex1a++)
        for (iDeltaIndex1b=0; iDeltaIndex1b < 4; iDeltaIndex1b++)
          if (iDeltaIndex1a != iDeltaIndex1b)
            for (iDeltaIndex1c=0; iDeltaIndex1c < 4;
             iDeltaIndex1c++)
              if ((iDeltaIndex1a != iDeltaIndex1c)
              &&  (iDeltaIndex1b != iDeltaIndex1c))
                for (iDeltaIndex1d=0; iDeltaIndex1d < 4;
                 iDeltaIndex1d++)
                  if ((iDeltaIndex1a != iDeltaIndex1d)
                  &&  (iDeltaIndex1b != iDeltaIndex1d)
                  &&  (iDeltaIndex1c != iDeltaIndex1d))
                    {
                      iDeltaIndex2=iDeltaIndex2+1;
                      *(piDeltaX+(24*iDeltaIndex1a+iDeltaIndex2))
                       =*piDeltaX;
                      *(piDeltaY+(24*iDeltaIndex1a+iDeltaIndex2))
                       =*piDeltaY;
                      *(piDeltaX+(24*iDeltaIndex1b+iDeltaIndex2))
                       =*(piDeltaX+24);
                      *(piDeltaY+(24*iDeltaIndex1b+iDeltaIndex2))
                       =*(piDeltaY+24);
                      *(piDeltaX+(24*iDeltaIndex1c+iDeltaIndex2))
                       =*(piDeltaX+48);
                      *(piDeltaY+(24*iDeltaIndex1c+iDeltaIndex2))
                       =*(piDeltaY+48);
                      *(piDeltaX+(24*iDeltaIndex1d+iDeltaIndex2))
                       =*(piDeltaX+72);
                      *(piDeltaY+(24*iDeltaIndex1d+iDeltaIndex2))
                       =*(piDeltaY+72);
                    }
      *pprrRowHead=NULL;
      *pprrRowTail=NULL;
      return;
    }

static void SizeMaze(piDeltaX,piDeltaY,pprrRowHead,pprrRowTail,
 piMagnitudeDeltaX,piMagnitudeDeltaY,piMaxX,piMaxY,piNumColumns,
 piNumRows,piRN,piTwiceMagnitudeDeltaX,piTwiceMagnitudeDeltaY,
 piXMax,piYMax,piFatalError)
  int     *piDeltaX;
  int     *piDeltaY;
  pROWREC *pprrRowHead;
  pROWREC *pprrRowTail;
  int     *piMagnitudeDeltaX;
  int     *piMagnitudeDeltaY;
  int     *piMaxX;
  int     *piMaxY;
  int     *piNumColumns;
  int     *piNumRows;
  int     *piRN;
  int     *piTwiceMagnitudeDeltaX;
  int     *piTwiceMagnitudeDeltaY;
  int     *piXMax;
  int     *piYMax;
  int     *piFatalError;
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
      *piRN=dateSeed.year%29;
      *(piRN+1)=dateSeed.month;
      *(piRN+2)=dateSeed.day%29;
      *(piRN+3)=dateSeed.hours;
      *(piRN+4)=dateSeed.minutes%29;
      *(piRN+5)=dateSeed.seconds%29;
      *(piRN+6)=dateSeed.hundredths%29;
      *(piRN+7)=0;
      *piNumColumns=(*piMaxX)/10;
      *piNumRows=(*piMaxY)/10;
      *piMagnitudeDeltaX=(*piMaxX)/(*piNumColumns)/2;
      *piTwiceMagnitudeDeltaX
       =(*piMagnitudeDeltaX)+(*piMagnitudeDeltaX);
      *piMagnitudeDeltaY=(*piMaxY)/(*piNumRows)/2;
      *piTwiceMagnitudeDeltaY
       =(*piMagnitudeDeltaY)+(*piMagnitudeDeltaY);
      *piXMax=*piTwiceMagnitudeDeltaX*(*piNumColumns);
      *piYMax=*piTwiceMagnitudeDeltaY*(*piNumRows);
      while (*pprrRowHead != NULL)
        {
          free((*pprrRowHead)->pchRowPtr);
          prrPreviousPtr=*pprrRowHead;
          *pprrRowHead=(*pprrRowHead)->prrSuccessorPtr;
          free((char *) prrPreviousPtr);
        }
      if ((*pprrRowHead=(struct ROWREC *)
       malloc((unsigned) sizeof(struct ROWREC))) == NULL)
        *piFatalError=TRUE;
      else
        {
          (*pprrRowHead)->prrPredecessorPtr=NULL;
          (*pprrRowHead)->prrSuccessorPtr=NULL;
          *pprrRowTail=*pprrRowHead;
          if (((*pprrRowHead)->pchRowPtr=
           malloc((unsigned) 2*(*piNumColumns)+1)) == NULL)
            *piFatalError=TRUE;
          else
            {
              pchColumnPtr=(*pprrRowHead)->pchRowPtr;
              for (iColumnNum=0; iColumnNum < 2*(*piNumColumns)+1;
               iColumnNum++)
                {
                  *pchColumnPtr='W';
                  pchColumnPtr++;
                }
            }
        }
      iRowNum=1;
      while ((iRowNum <= *piNumRows) && (! *piFatalError))
        {
          if ((prrCurrentPtr=(struct ROWREC *)
           malloc((unsigned) sizeof(struct ROWREC))) == NULL)
            *piFatalError=TRUE;
          else
            {
              (*pprrRowTail)->prrSuccessorPtr=prrCurrentPtr;
              prrCurrentPtr->prrPredecessorPtr=*pprrRowTail;
              prrCurrentPtr->prrSuccessorPtr=NULL;
              *pprrRowTail=prrCurrentPtr;
              if ((prrCurrentPtr->pchRowPtr=
               malloc((unsigned) 2*(*piNumColumns)+1)) == NULL)
                *piFatalError=TRUE;
              else
                {
                  pchColumnPtr=prrCurrentPtr->pchRowPtr;
                  for (iColumnNum=0; iColumnNum < 2*(*piNumColumns)+1;
                   iColumnNum++)
                    {
                      *pchColumnPtr='W';
                      pchColumnPtr++;
                    }
                }
            }
          if ((prrCurrentPtr=(struct ROWREC *)
           malloc((unsigned) sizeof(struct ROWREC))) == NULL)
            *piFatalError=TRUE;
          else
            {
              (*pprrRowTail)->prrSuccessorPtr=prrCurrentPtr;
              prrCurrentPtr->prrPredecessorPtr=*pprrRowTail;
              prrCurrentPtr->prrSuccessorPtr=NULL;
              *pprrRowTail=prrCurrentPtr;
              if ((prrCurrentPtr->pchRowPtr=
               malloc((unsigned) 2*(*piNumColumns)+1)) == NULL)
                *piFatalError=TRUE;
              else
                {
                  pchColumnPtr=prrCurrentPtr->pchRowPtr;
                  for (iColumnNum=0; iColumnNum < 2*(*piNumColumns)+1;
                   iColumnNum++)
                    {
                      *pchColumnPtr='W';
                      pchColumnPtr++;
                    }
                }
            }
          iRowNum++;
        }
      iSum=0;
      for (iDigitNum=1; iDigitNum <= 3; iDigitNum++)
        {
          iDigit=*piRN;
          iRNIndex1=0;
          for (iRNIndex2=1; iRNIndex2 < 8; iRNIndex2++)
            {
              iTemInt=*(piRN+iRNIndex2);
              *(piRN+iRNIndex1)=iTemInt;
              iRNIndex1++;
              iDigit+=iTemInt;
              if (iDigit >= 29)
                iDigit-=29;
            }
          *(piRN+7)=iDigit;
          iSum=29*iSum+iDigit;
        }
      iX=2*(iSum%(*piNumColumns))+1;
      iSum=0;
      for (iDigitNum=1; iDigitNum <= 3; iDigitNum++)
        {
          iDigit=*piRN;
          iRNIndex1=0;
          for (iRNIndex2=1; iRNIndex2 < 8; iRNIndex2++)
            {
              iTemInt=*(piRN+iRNIndex2);
              *(piRN+iRNIndex1)=iTemInt;
              iRNIndex1++;
              iDigit+=iTemInt;
              if (iDigit >= 29)
                iDigit-=29;
            }
          *(piRN+7)=iDigit;
          iSum=29*iSum+iDigit;
        }
      iY=2*(iSum%(*piNumRows))+1;
      prrCurrentPtr=*pprrRowHead;
      for (iYOut=0; iYOut < iY; iYOut++)
        prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
      iFinished=FALSE;
      iRecurse=TRUE;
      ps2StackHead=NULL;
      while ((! iFinished) && (! *piFatalError))
        {
          if (iRecurse)
            {
              *((prrCurrentPtr->pchRowPtr)+iX)=' ';
              iDeltaIndex1=0;
              do
                {
                  iDeltaIndex2=*piRN;
                  iRNIndex1=0;
                  for (iRNIndex2=1; iRNIndex2 < 8; iRNIndex2++)
                    {
                      iTemInt=*(piRN+iRNIndex2);
                      *(piRN+iRNIndex1)=iTemInt;
                      iRNIndex1++;
                      iDeltaIndex2+=iTemInt;
                      if (iDeltaIndex2 >= 29)
                        iDeltaIndex2-=29;
                    }
                  *(piRN+7)=iDeltaIndex2;
                }
              while (iDeltaIndex2 >= 24);
              iRecurse=FALSE;
            }
            while ((iDeltaIndex1 < 4)
            &&     (! iRecurse)
            &&     (! *piFatalError))
              {
                iXNext
                 =iX+2*(*(piDeltaX+(24*iDeltaIndex1+iDeltaIndex2)));
                if ((iXNext <= 0) || (iXNext >= 2*(*piNumColumns)))
                  iDeltaIndex1++;
                else
                  {
                    switch (*(piDeltaY+(24*iDeltaIndex1+iDeltaIndex2)))
                      {
                        case -1:
                          prrNextPtr
                           =(prrCurrentPtr->prrPredecessorPtr)->
                           prrPredecessorPtr;
                          break;
                        case 1:
                          prrNextPtr=(prrCurrentPtr->prrSuccessorPtr)->
                           prrSuccessorPtr;
                          break;
                        default:
                          prrNextPtr=prrCurrentPtr;
                          break;
                      }
                    if (prrNextPtr == NULL)
                      iDeltaIndex1++;
                    else
                      {
                        if (*((prrNextPtr->pchRowPtr)+iXNext) == 'W')
                          {
                            if (iX == iXNext)
                              if ((*(piDeltaY
                               +(24*iDeltaIndex1+iDeltaIndex2)))
                               == 1)
                                *(((prrCurrentPtr->prrSuccessorPtr)
                                 ->pchRowPtr)+iX)=' ';
                              else
                                *(((prrCurrentPtr->prrPredecessorPtr)
                                 ->pchRowPtr)+iX)=' ';
                            else
                              {
                                iXOut=(iX+iXNext)/2;
                                *((prrCurrentPtr->pchRowPtr)+iXOut)=' ';
                              }
                            iX=iXNext;
                            prrCurrentPtr=prrNextPtr;
                            if ((ps2StackPtr=
                             (struct STACK2REC *) malloc(
                             (unsigned) sizeof(struct STACK2REC)))
                             == NULL)
                              *piFatalError=TRUE;
                            else
                              {
                                ps2StackPtr->ps2NextPtr=ps2StackHead;
                                ps2StackHead=ps2StackPtr;
                                ps2StackHead->chIndex1
                                 =(unsigned char) iDeltaIndex1;
                                ps2StackHead->chIndex2
                                 =(unsigned char) iDeltaIndex2;
                                iRecurse=TRUE;
                              }
                          }
                        else
                          iDeltaIndex1++;
                      }
                  }
              }
            if ((! iRecurse) && (! *piFatalError))
              {
                iDeltaIndex1=(int) ps2StackHead->chIndex1;
                iDeltaIndex2=(int) ps2StackHead->chIndex2;
                ps2StackPtr=ps2StackHead;
                ps2StackHead=ps2StackHead->ps2NextPtr;
                free((char *) ps2StackPtr);
                if (ps2StackHead == NULL)
                  iFinished=TRUE;
                else
                  switch (*(piDeltaY+(24*iDeltaIndex1+iDeltaIndex2)))
                    {
                      case -1:
                        prrCurrentPtr=(prrCurrentPtr->prrSuccessorPtr)->
                         prrSuccessorPtr;
                        break;
                      case 1:
                        prrCurrentPtr
                         =(prrCurrentPtr->prrPredecessorPtr)->
                         prrPredecessorPtr;
                        break;
                      default:
                        iX-=
                     (2*(*(piDeltaX+(24*iDeltaIndex1+iDeltaIndex2))));
                        break;
                    }
              }
        }
      if (! *piFatalError)
        {
          *(((*pprrRowHead)->pchRowPtr)+1)='S';
          *((((*pprrRowHead)->prrSuccessorPtr)->pchRowPtr)+1)='S';
          *(((*pprrRowTail)->pchRowPtr)+(2*(*piNumColumns)-1))=' ';
        }
      return;
    }

static void PaintMaze(pprrRowHead,piNumColumns,piMagnitudeDeltaX,
 piMagnitudeDeltaY,piTwiceMagnitudeDeltaX,piTwiceMagnitudeDeltaY,
 piXMax,piYMax,hPS,piFatalError)
  pROWREC *pprrRowHead;
  int         *piNumColumns;
  int         *piMagnitudeDeltaX;
  int         *piMagnitudeDeltaY;
  int         *piTwiceMagnitudeDeltaX;
  int         *piTwiceMagnitudeDeltaY;
  int         *piXMax;
  int         *piYMax;
  HPS         hPS;
  int         *piFatalError;
    {
      char    chPenColor;
      int     iColumnNum;
      int     iEven;
      char    *pchPixelPtr;
      pROWREC prrCurrentPtr;
      POINTL  ptlEndingPosition;
      POINTL  ptlStartingPosition;

      if (* piFatalError)
        {
          GpiSetColor(hPS,BACKTRACK_COLOR);
          ptlStartingPosition.x=0;
          ptlStartingPosition.y=0;
          ptlEndingPosition.x=*piXMax;
          ptlEndingPosition.y=*piYMax;
          GpiMove(hPS,&ptlStartingPosition);
          GpiBox(hPS,DRO_FILL,&ptlEndingPosition,(long) 0,(long) 0);
          DosBeep(60,333);
        }
      else
        {
          GpiSetColor(hPS,FLOOR_COLOR);
          ptlStartingPosition.x=0;
          ptlStartingPosition.y=0;
          ptlEndingPosition.x=*piXMax;
          ptlEndingPosition.y=*piYMax;
          GpiMove(hPS,&ptlStartingPosition);
          GpiBox(hPS,DRO_FILL,&ptlEndingPosition,(long) 0,(long) 0);
          prrCurrentPtr=*pprrRowHead;
          iEven=TRUE;
          ptlStartingPosition.y=0;
          ptlEndingPosition.y=0;
          while (prrCurrentPtr != NULL)
            {
              if (iEven)
                {
                  pchPixelPtr=(prrCurrentPtr->pchRowPtr)+1;
                  ptlStartingPosition.x=0;
                  ptlEndingPosition.x=(*piTwiceMagnitudeDeltaX);
                  GpiSetColor(hPS,WALL_COLOR);
                  chPenColor=' ';
                  for (iColumnNum=1; iColumnNum <= *piNumColumns;
                   iColumnNum++)
                    {
                      if (*pchPixelPtr == 'W')
                        {
                          if (chPenColor != 'W')
                            {
                              chPenColor='W';
                              GpiMove(hPS,&ptlStartingPosition);
                            }
                        }
                      else
                        {
                          if (chPenColor == 'W')
                            {
                              GpiLine(hPS,&ptlStartingPosition);
                              chPenColor=' ';
                            }
                        }
                      ptlStartingPosition.x=ptlEndingPosition.x;
                      ptlEndingPosition.x+=(*piTwiceMagnitudeDeltaX);
                      pchPixelPtr+=2;
                    }
                  if (chPenColor == 'W')
                    GpiLine(hPS,&ptlStartingPosition);
                  ptlStartingPosition.y+=(*piMagnitudeDeltaY);
                  ptlEndingPosition.y=ptlStartingPosition.y;
                }
              else
                {
                  pchPixelPtr=(prrCurrentPtr->pchRowPtr);
                  ptlStartingPosition.x=-(*piMagnitudeDeltaX);
                  ptlEndingPosition.x=(*piMagnitudeDeltaX);
                  chPenColor=' ';
                  for (iColumnNum=1; iColumnNum <= *piNumColumns;
                   iColumnNum++)
                    {
                      switch (*pchPixelPtr)
                        {
                          case 'A':
                            if (chPenColor != 'A')
                              {
                                if (chPenColor != ' ')
                                  GpiLine(hPS,&ptlStartingPosition);
                                chPenColor='A';
                                GpiSetColor(hPS,BACKTRACK_COLOR);
                                GpiMove(hPS,&ptlStartingPosition);
                              }
                            break;
                          case 'S':
                            if (chPenColor != 'S')
                              {
                                if (chPenColor != ' ')
                                  GpiLine(hPS,&ptlStartingPosition);
                                chPenColor='S';
                                GpiSetColor(hPS,SOLUTION_COLOR);
                                GpiMove(hPS,&ptlStartingPosition);
                              }
                            break;
                          default:
                            if (chPenColor != ' ')
                              {
                                GpiLine(hPS,&ptlStartingPosition);
                                chPenColor=' ';
                              }
                            break;
                        }
                      ptlStartingPosition.x=ptlEndingPosition.x;
                      ptlEndingPosition.x+=(*piTwiceMagnitudeDeltaX);
                      pchPixelPtr+=2;
                    }
                  if (chPenColor != ' ')
                    GpiLine(hPS,&ptlStartingPosition);
                  ptlStartingPosition.y+=(*piMagnitudeDeltaY);
                  ptlEndingPosition.y=ptlStartingPosition.y;
                }
              iEven=! iEven;
              prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
            }
          ptlStartingPosition.x=0;
          ptlEndingPosition.x=0;
          for (iColumnNum=1; iColumnNum <= *piNumColumns;
           iColumnNum++)
            {
              ptlStartingPosition.y=0;
              ptlEndingPosition.y=*piTwiceMagnitudeDeltaY;
              prrCurrentPtr=*pprrRowHead;
              iEven=TRUE;
              chPenColor=' ';
              GpiSetColor(hPS,WALL_COLOR);
              while (prrCurrentPtr != NULL)
                {
                  if (! iEven)
                    {
                      pchPixelPtr
                       =(prrCurrentPtr->pchRowPtr)+2*(iColumnNum-1);
                      if (*pchPixelPtr == 'W')
                        {
                          if (chPenColor != 'W')
                            {
                              chPenColor='W';
                              GpiMove(hPS,&ptlStartingPosition);
                            }
                        }
                      else
                        {
                          if (chPenColor == 'W')
                            {
                              GpiLine(hPS,&ptlStartingPosition);
                              chPenColor=' ';
                            }
                        }
                      ptlStartingPosition.y=ptlEndingPosition.y;
                      ptlEndingPosition.y+=(*piTwiceMagnitudeDeltaY);
                    }
                  iEven=! iEven;
                  prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
                }
              if (chPenColor == 'W')
                GpiLine(hPS,&ptlStartingPosition);
              ptlStartingPosition.x+=(*piMagnitudeDeltaX);
              ptlEndingPosition.x=ptlStartingPosition.x;
              ptlStartingPosition.y=0;
              ptlEndingPosition.y=(*piMagnitudeDeltaY);
              prrCurrentPtr=*pprrRowHead;
              iEven=TRUE;
              chPenColor=' ';
              while (prrCurrentPtr != NULL)
                {
                  if (iEven)
                    {
                      pchPixelPtr
                       =(prrCurrentPtr->pchRowPtr)+2*iColumnNum-1;
                      switch (*pchPixelPtr)
                        {
                          case 'A':
                            if (chPenColor != 'A')
                              {
                                if (chPenColor != ' ')
                                  GpiLine(hPS,&ptlStartingPosition);
                                chPenColor='A';
                                GpiSetColor(hPS,BACKTRACK_COLOR);
                                GpiMove(hPS,&ptlStartingPosition);
                              }
                            break;
                          case 'S':
                            if (chPenColor != 'S')
                              {
                                if (chPenColor != ' ')
                                  GpiLine(hPS,&ptlStartingPosition);
                                chPenColor='S';
                                GpiSetColor(hPS,SOLUTION_COLOR);
                                GpiMove(hPS,&ptlStartingPosition);
                              }
                            break;
                          default:
                            if (chPenColor != ' ')
                              {
                                GpiLine(hPS,&ptlStartingPosition);
                                chPenColor=' ';
                              }
                            break;
                        }
                      ptlStartingPosition.y=ptlEndingPosition.y;
                      ptlEndingPosition.y+=(*piTwiceMagnitudeDeltaX);
                      if (ptlEndingPosition.y > *piYMax)
                        ptlEndingPosition.y=*piYMax;
                    }
                  iEven=! iEven;
                  prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
                }
              if (chPenColor != ' ')
                GpiLine(hPS,&ptlStartingPosition);
              ptlStartingPosition.x+=(*piMagnitudeDeltaX);
              ptlEndingPosition.x=ptlStartingPosition.x;
            }
          ptlStartingPosition.x=*piXMax;
          ptlEndingPosition.x=*piXMax;
          ptlStartingPosition.y=0;
          ptlEndingPosition.y=*piYMax;
          GpiSetColor(hPS,WALL_COLOR);
          GpiMove(hPS,&ptlStartingPosition);
          GpiLine(hPS,&ptlEndingPosition);
        }
      return;
    }

static void DestroyMaze(pprrRowHead,pprrRowTail)
  pROWREC *pprrRowHead;
  pROWREC *pprrRowTail;
    {
      pROWREC prrPreviousPtr;

      while (*pprrRowHead != NULL)
        {
          free((*pprrRowHead)->pchRowPtr);
          prrPreviousPtr=*pprrRowHead;
          *pprrRowHead=(*pprrRowHead)->prrSuccessorPtr;
          free((char *) prrPreviousPtr);
        }
      *pprrRowTail=NULL;
      return;
    }

static void ClearPaths(pprrRowHead,piNumColumns)
  pROWREC *pprrRowHead;
  int     *piNumColumns;
    {
      int     iX;
      pROWREC prrCurrentPtr;

      prrCurrentPtr=*pprrRowHead;
      while (prrCurrentPtr != NULL)
        {
          for (iX=1; iX < 2*(*piNumColumns); iX++)
            if (*((prrCurrentPtr->pchRowPtr)+iX) != 'W')
              *((prrCurrentPtr->pchRowPtr)+iX)=' ';
          prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
        }
      *(((*pprrRowHead)->pchRowPtr)+1)='S';
      *((((*pprrRowHead)->prrSuccessorPtr)->pchRowPtr)+1)='S';
      return;
    }

static void OptionallyHaveComputerSolve(pprrRowHead,pprrRowTail,
 piDeltaX,piDeltaY,piNumColumns,piFatalError)
  pROWREC *pprrRowHead;
  pROWREC *pprrRowTail;
  int     *piDeltaX;
  int     *piDeltaY;
  int     *piNumColumns;
  int     *piFatalError;
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

      iX=1;
      prrCurrentPtr=(*pprrRowHead)->prrSuccessorPtr;
      prrNextPtr=prrCurrentPtr->prrSuccessorPtr;
      iFinished=FALSE;
      iRecurse=TRUE;
      ps1StackHead=NULL;
      while ((! iFinished) && (! *piFatalError))
        {
          if (iRecurse)
            {
              uchDeltaIndex1=0;
              iRecurse=FALSE;
            };
          while ((uchDeltaIndex1 < 4)
          &&     (! iFinished)
          &&     (! iRecurse)
          &&     (! *piFatalError))
            {
              switch (*(piDeltaY+(24*uchDeltaIndex1)))
                {
                  case -1:
                    iXNext=iX;
                    prrNextPtr=prrCurrentPtr->prrPredecessorPtr;
                    break;
                  case 1:
                    iXNext=iX;
                    prrNextPtr=prrCurrentPtr->prrSuccessorPtr;
                    break;
                  default:
                    prrNextPtr=prrCurrentPtr;
                    iXNext=iX+(*(piDeltaX+(24*uchDeltaIndex1)));
                    break;
                }
              if (*((prrNextPtr->pchRowPtr)+iXNext) == ' ')
                {
                  *((prrNextPtr->pchRowPtr)+iXNext)='S';
                  switch (*(piDeltaY+(24*uchDeltaIndex1)))
                    {
                      case -1:
                        prrNextPtr=prrNextPtr->prrPredecessorPtr;
                        break;
                      case 1:
                        prrNextPtr=prrNextPtr->prrSuccessorPtr;
                        break;
                      default:
                        iXNext+=(*(piDeltaX+(24*uchDeltaIndex1)));
                        break;
                    }
                  if (prrNextPtr != NULL)
                    {
                      *((prrNextPtr->pchRowPtr)+iXNext)='S';
                      iX=iXNext;
                      prrCurrentPtr=prrNextPtr;
                      if ((ps1StackPtr=(struct STACK1REC *) malloc(
                       (unsigned) sizeof(struct STACK1REC)))
                       == NULL)
                        *piFatalError=TRUE;
                      else
                        {
                          ps1StackPtr->ps1NextPtr=ps1StackHead;
                          ps1StackHead=ps1StackPtr;
                          ps1StackHead->chIndex1
                           =uchDeltaIndex1;
                          iRecurse=TRUE;
                        }
                    }
                  else
                    iFinished=TRUE;
                }
              else
                uchDeltaIndex1++;
            };
          if ((uchDeltaIndex1 >= 4) && (! *piFatalError))
            {
              *((prrCurrentPtr->pchRowPtr)+iX)=' ';
              iXNext=iX;
              prrNextPtr=prrCurrentPtr;
              uchDeltaIndex1=ps1StackHead->chIndex1;
              ps1StackPtr=ps1StackHead;
              ps1StackHead=ps1StackHead->ps1NextPtr;
              free((char *) ps1StackPtr);
              switch (*(piDeltaY+(24*uchDeltaIndex1)))
                {
                  case -1:
                    prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
                    *((prrCurrentPtr->pchRowPtr)+iX)=' ';
                    prrCurrentPtr=prrCurrentPtr->prrSuccessorPtr;
                    break;
                  case 1:
                    prrCurrentPtr=prrCurrentPtr->prrPredecessorPtr;
                    *((prrCurrentPtr->pchRowPtr)+iX)=' ';
                    prrCurrentPtr=prrCurrentPtr->prrPredecessorPtr;
                    break;
                  default:
                    iX-=(*(piDeltaX+(24*uchDeltaIndex1)));
                    *((prrCurrentPtr->pchRowPtr)+iX)=' ';
                    iX-=(*(piDeltaX+(24*uchDeltaIndex1)));
                    break;
                }
              uchDeltaIndex1++;
            }
        };
      if (! *piFatalError)
       *(((*pprrRowTail)->pchRowPtr)+(2*(*piNumColumns)-1))='S';
      while (ps1StackHead != NULL)
        {
          ps1StackPtr=ps1StackHead;
          ps1StackHead=ps1StackHead->ps1NextPtr;
          free((char *) ps1StackPtr);
        }
      return;
    }
