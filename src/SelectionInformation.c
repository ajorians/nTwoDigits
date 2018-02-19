#include "SelectionInformation.h"
#ifndef _TINSPIRE
#include <stdlib.h>
#endif

#define SELECTOR_LINE_WIDTH     (4)

void CreateSelectionInformation(struct SelectionInformation** ppSelectionInformation, TwoDigitsLib twodigits)
{
   *ppSelectionInformation = malloc(sizeof(struct SelectionInformation));
   struct SelectionInformation* pSelectionInformation = *ppSelectionInformation;
   pSelectionInformation->m_nCurrentX = pSelectionInformation->m_nCurrentY = 0;
   pSelectionInformation->m_TwoDigits = twodigits;
}

void FreeSelectionInformation(struct SelectionInformation** ppSelectionInformation)
{
   struct SelectionInformation* pSelectionInformation = *ppSelectionInformation;
   pSelectionInformation->m_TwoDigits = NULL;//Does not own

   free(pSelectionInformation);
}

void Move(struct SelectionInformation* pSelectionInformation, enum Direction eDirection)
{
   switch( eDirection ) {
      default:
      case Up:
         if( pSelectionInformation->m_nCurrentY > 0 ) pSelectionInformation->m_nCurrentY--;
         break;
      case Down:
         if( pSelectionInformation->m_nCurrentY < (GetTwoDigitsHeight(pSelectionInformation->m_TwoDigits)-1) ) pSelectionInformation->m_nCurrentY++;
         break;
      case Left:
         if( pSelectionInformation->m_nCurrentX > 0 ) pSelectionInformation->m_nCurrentX--;
         break;
      case Right:
	 if( pSelectionInformation->m_nCurrentX < (GetTwoDigitsWidth(pSelectionInformation->m_TwoDigits)-1) ) pSelectionInformation->m_nCurrentX++;
         break;
   }
}

int GetCurrentX(struct SelectionInformation* pSelectionInformation)
{
   return pSelectionInformation->m_nCurrentX;
}

int GetCurrentY(struct SelectionInformation* pSelectionInformation)
{
   return pSelectionInformation->m_nCurrentY;
}

