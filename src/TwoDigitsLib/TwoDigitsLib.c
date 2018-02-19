//Public domain :)

#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif
#include "TwoDigitsLib/TwoDigitsLib.h"
#include "Defines.h"

#define TWODIGITSLIB_RUN_SAFE

//#define DEBUG_DETAILS

#define MAX_CELL_VALUES	(10)

struct Cell
{
   int m_nValue;
   enum TwoDigitMarking m_eMarking;
};

struct TwoDigitsBoard
{
   int m_nWidth;
   int m_nHeight;
   struct Cell* m_pItems;
};

struct Cell* GetAt(struct TwoDigitsBoard* pBoard, int nX, int nY)
{
#ifdef TWODIGITSLIB_RUN_SAFE
   if( nX < 0 || nY < 0 || (nX >= pBoard->m_nWidth) || (nY >= pBoard->m_nHeight)) {
      printf("Accessing non-existing element %d,%d\n", nX, nY);
      return NULL;
   }
#endif

   struct Cell* pItem = pBoard->m_pItems + (pBoard->m_nWidth * nY + nX);

   return pItem;
}

struct TwoDigitAction
{
   int x;
   int y;
   enum TwoDigitMarking eMarking;
   struct TwoDigitAction* m_pNext;
};

struct TwoDigits
{
   int m_nLastError;
   struct TwoDigitsBoard* m_pBoard;
   struct TwoDigitAction* m_pUndoActions;
   struct TwoDigitAction* m_pRedoActions;
};

void Cleanup(struct TwoDigits** ppT)
{
   if( *ppT != NULL ) {
      struct TwoDigits* pT = *ppT;
      if(pT->m_pBoard != NULL ) {
         if(pT->m_pBoard->m_pItems != NULL ) {
            free(pT->m_pBoard->m_pItems);
            pT->m_pBoard->m_pItems = NULL;
         }
         free(pT->m_pBoard);
         pT->m_pBoard = NULL;
      }
   }
}

int TwoDigitsLibCreate(TwoDigitsLib* api, const char* pstrFile)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = malloc(sizeof(struct TwoDigits));
   if( pT == NULL ){//Out of memory
      return TWODIGITSLIB_OUT_OF_MEMORY;
   }
   pT->m_pBoard = NULL;

   pT->m_pBoard = malloc(sizeof(struct TwoDigitsBoard));
   if( pT->m_pBoard == NULL ){//Out of memory
      Cleanup(&pT);
      return TWODIGITSLIB_OUT_OF_MEMORY;
   }

   pT->m_pBoard->m_pItems = NULL;

   if( strstr(pstrFile, "TwoDigits ") != pstrFile ) {//TwoDigits file version check
      printf("Not TwoDigits data\n");
      Cleanup(&pT);
      return TWODIGITSLIB_NOT_PUZZLE_FILE;
   }

   if( strstr(pstrFile, "1 ") != (pstrFile + strlen("TwoDigits ")) ) {//Version check
      printf("Unable to read TwoDigits data\n");
      Cleanup(&pT);
      return TWODIGITSLIB_NOT_PUZZLE_FILE;
   }

   char* pstr = pstrFile + strlen("TwoDigits 1 ");

   char buffer[16];
   int nSpotInBuffer = 0;

   int nWidth = -1, nHeight = -1;
   while(pstr != '\0') {
      char ch = *pstr; pstr++;
      if( isdigit(ch) ) {
         buffer[nSpotInBuffer++] = ch;
      }
      else {
         if( ch != ' ' && ch != '\n' && ch != '\r' )
            break;
         if( nSpotInBuffer == 0 )
            continue;
         buffer[nSpotInBuffer] = '\0';
         nSpotInBuffer = 0;
         int nValue = atoi(buffer);

#ifdef DEBUG_DETAILS
         printf("Value %d\n", nValue);
#endif

         if( nWidth < 0 ) {
#ifdef TWODIGITSLIB_RUN_SAFE
            if( nValue < 0 || nValue > 20 ) {
               printf("Bad value for width of value %d\n", nValue);
            }
#endif
            nWidth = nValue;
            pT->m_pBoard->m_nWidth = nWidth;
         }
         else if( nHeight < 0 ) {
            int x, y;

#ifdef TWODIGITSLIB_RUN_SAFE
            if( nValue < 0 || nValue > 20 ) {
               printf("Bad value for height of value %d\n", nValue);
            }
#endif

            nHeight = nValue;
            pT->m_pBoard->m_nHeight = nHeight;

            pT->m_pBoard->m_pItems = malloc(nWidth*nHeight*sizeof(struct Cell));

            if( pT->m_pBoard->m_pItems == NULL ) {//Out of memory
               Cleanup(&pT);
               return TWODIGITSLIB_OUT_OF_MEMORY;
            }

            for(x=0; x<nWidth; x++) {
               for(y=0; y<nHeight; y++) {
                  struct Cell* pCell = GetAt(pT->m_pBoard, x, y);
                  pCell->m_nValue = 0;
                  pCell->m_eMarking = NotMarked;
               }
            }

            for(y=0; y<nHeight; y++) {
               for (x = 0; x<nWidth; x++) {
                  struct Cell* pCell = GetAt(pT->m_pBoard, x, y);

                  nSpotInBuffer = 0;
                  while(pstr != '\0') {
                     ch = *pstr; pstr++;
                     if( isdigit(ch) ) {
                        buffer[nSpotInBuffer++] = ch;
                     }
                     else {
                        if (ch != ' ' && ch != '\n' && ch != '\r' ) {
                           pstr--;
                           break;
                        }
                        if( nSpotInBuffer == 0 )
                           continue;

                        break;
                     }
                  }

                  buffer[nSpotInBuffer] = '\0';
                  nSpotInBuffer = 0;
                  nValue = atoi(buffer);

                  //printf("Value for cell: %d\n", nValue);
                  pCell->m_nValue = nValue;

              }
           }
         }
      }
   }

   pT->m_pUndoActions = NULL;
   pT->m_pRedoActions = NULL;

   pT->m_nLastError = TWODIGITSLIB_OK;
   *api = pT;

   return TWODIGITSLIB_OK;
}

int TwoDigitsLibFree(TwoDigitsLib* api)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = *api;

   ClearUndos(*api);
   ClearRedos(*api);

   if( pT->m_pBoard != NULL ) {
      free(pT->m_pBoard);
      pT->m_pBoard = NULL;
   }

   free(pT);
   *api = NULL;
   return TWODIGITSLIB_OK;
}

int GetTwoDigitsLibError(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;
   return pT->m_nLastError;
}

void ClearTwoDigitsLibError(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;
   pT->m_nLastError = TWODIGITSLIB_OK;
}

//TwoDigitsLib related functions
int GetTwoDigitsWidth(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;
   return pT->m_pBoard->m_nWidth;
}

int GetTwoDigitsHeight(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;
   return pT->m_pBoard->m_nHeight;
}

int IsTwoDigitsGameOver(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   int nLeft, nRight;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;

   nLeft = GetTwoDigitsLeftMarkedTotal(api);
   nRight = GetTwoDigitsRightMarkedTotal(api);

   if( nLeft != nRight )
      return TWODIGITSLIB_NOT_GAMEOVER;

   if( nLeft == 0 || nRight == 0 )
      return TWODIGITSLIB_NOT_GAMEOVER;

   //printf("Game over\n");
   return TWODIGITSLIB_GAMEOVER;
}

void ClearUndos(TwoDigitsLib api)
{
   DEBUG_FUNC_NAME;

   struct TwoDigits* pT = (struct TwoDigits*)api;

   struct TwoDigitAction* pCurrent = pT->m_pUndoActions;
   while(pCurrent != NULL) {
      struct TwoDigitAction* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pT->m_pUndoActions = pCurrent;
   }
}

void ClearRedos(TwoDigitsLib api)
{
   DEBUG_FUNC_NAME;

   struct TwoDigits* pT = (struct TwoDigits*)api;

   struct TwoDigitAction* pCurrent = pT->m_pRedoActions;
   while(pCurrent != NULL) {
      struct TwoDigitAction* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pT->m_pRedoActions = pCurrent;
   }
}

void AddUndo(TwoDigitsLib api, int x, int y, enum TwoDigitMarking eMarking)
{
   DEBUG_FUNC_NAME;

   struct TwoDigits* pT = (struct TwoDigits*)api;

   struct TwoDigitAction* pAction = malloc(sizeof(struct TwoDigitAction));
   if( pAction == NULL ) {
      DEBUG_MSG("Out of memory: AddUndo\n");
   //Assume allocated
   }

   pAction->x = x;
   pAction->y = y;
   //pAction->eMarking = eMarking;

   struct TwoDigitAction* pRoot = pT->m_pUndoActions;
   pAction->m_pNext = pRoot;
   pT->m_pUndoActions = pAction;
}

void AddRedo(TwoDigitsLib api, int x, int y, int value)
{
   DEBUG_FUNC_NAME;

   struct TwoDigits* pT = (struct TwoDigits*)api;

   struct TwoDigitAction* pAction = malloc(sizeof(struct TwoDigitAction));
   if( pAction == NULL ) {
      DEBUG_MSG("Out of memory: AddRedo\n");
      //Assume allocated
   }

   pAction->x = x;
   pAction->y = y;
   //pAction->value = value;

   struct TwoDigitAction* pRoot = pT->m_pRedoActions;
   pAction->m_pNext = pRoot;
   pT->m_pRedoActions = pAction;
}

int TwoDigitsUndo(TwoDigitsLib api)
{
   DEBUG_FUNC_NAME;

   struct TwoDigits* pT = (struct TwoDigits*)api;

   struct TwoDigitAction* pRoot = pT->m_pUndoActions;
   if( pRoot == NULL )
      return TWODIGITSLIB_CANNOT_UNDO;

   pT->m_pUndoActions = pRoot->m_pNext;
   AddRedo(api, pRoot->x, pRoot->y, GetTwoDigitsSpotValue(api, pRoot->x, pRoot->y));
   //GetAt(pT->m_pBoard, pRoot->x, pRoot->y)->m_nValue = pRoot->value;

   free(pRoot);
   pRoot = NULL;

   return TWODIGITSLIB_OK;
}

int TwoDigitsRedo(TwoDigitsLib api)
{
   DEBUG_FUNC_NAME;

   struct TwoDigits* pT = (struct TwoDigits*)api;

   struct TwoDigitAction* pRoot = pT->m_pRedoActions;
   if( pRoot == NULL )
      return TWODIGITSLIB_CANNOT_REDO;

   pT->m_pRedoActions = pRoot->m_pNext;
   AddUndo(api, pRoot->x, pRoot->y, GetTwoDigitsSpotValue(api, pRoot->x, pRoot->y));
   //GetAt(pT->m_pBoard, pRoot->x, pRoot->y)->m_nValue = pRoot->value;

   free(pRoot);
   pRoot = NULL;

   return TWODIGITSLIB_OK;
}

int GetTwoDigitsSpotValue(TwoDigitsLib api, int x, int y)
{
   struct TwoDigits* pT;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;

   if (x < 0 || x >= pT->m_pBoard->m_nWidth || y < 0 || y >= pT->m_pBoard->m_nHeight)
      return TWODIGITSLIB_BADARGUMENT;

   return GetAt(pT->m_pBoard, x, y)->m_nValue;
}

int ToggleTwoDigitsSpot(TwoDigitsLib api, int x, int y)
{
   struct TwoDigits* pT;
   struct Cell* pCell;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;

   if (x < 0 || x >= pT->m_pBoard->m_nWidth || y < 0 || y >= pT->m_pBoard->m_nHeight)
      return TWODIGITSLIB_BADARGUMENT;

   ClearRedos(api);

   pCell = GetAt(pT->m_pBoard, x, y);
   enum TwoDigitMarking eOldMarking = pCell->m_eMarking;
   AddUndo(api, x, y, eOldMarking);
   if( eOldMarking == NotMarked ) {
      pCell->m_eMarking = LeftMarked;
   }
   else if( eOldMarking == LeftMarked )
   {
      pCell->m_eMarking = RightMarked;
   }
   else if( eOldMarking == RightMarked)
   {
      pCell->m_eMarking = NotMarked;
   }

   return TWODIGITSLIB_OK;
}

int GetTwoDigitMarking(TwoDigitsLib api, int x, int y, enum TwoDigitMarking* pMarking)
{
   struct TwoDigits* pT;
   struct Cell* pCell;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;

   if (x < 0 || x >= pT->m_pBoard->m_nWidth || y < 0 || y >= pT->m_pBoard->m_nHeight)
      return TWODIGITSLIB_BADARGUMENT;

   pCell = GetAt(pT->m_pBoard, x, y);

   *pMarking = pCell->m_eMarking;

   return TWODIGITSLIB_OK;
}

int GetTwoDigitsLeftMarkedTotal(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   int nTotal;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;
   nTotal = 0;

   for(int x=0; x<GetTwoDigitsWidth(api); x++)
   {
      for(int y=0; y<GetTwoDigitsHeight(api); y++)
      {
         if( GetAt(pT->m_pBoard, x, y)->m_eMarking == LeftMarked )
            nTotal += GetTwoDigitsSpotValue(api, x, y);
      }
   }

   return nTotal;
}

int GetTwoDigitsRightMarkedTotal(TwoDigitsLib api)
{
   struct TwoDigits* pT;
   int nTotal;
   DEBUG_FUNC_NAME;

   pT = (struct TwoDigits*)api;
   nTotal = 0;

   for(int x=0; x<GetTwoDigitsWidth(api); x++)
   {
      for(int y=0; y<GetTwoDigitsHeight(api); y++)
      {
         if( GetAt(pT->m_pBoard, x, y)->m_eMarking == RightMarked )
            nTotal += GetTwoDigitsSpotValue(api, x, y);
      }
   }

   return nTotal;
}


