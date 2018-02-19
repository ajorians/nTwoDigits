#ifndef SELECTOR_H
#define SELECTOR_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include <TwoDigitsLib/TwoDigitsLib.h>

struct SelectionInformation
{
   int m_nCurrentX;
   int m_nCurrentY;
   TwoDigitsLib m_TwoDigits;//Does not own
};

void CreateSelectionInformation(struct SelectionInformation** ppSelectionInformation, TwoDigitsLib twodigits);
void FreeSelectionInformation(struct SelectionInformation** ppSelectionInformation);

enum Direction{Up, Down, Left, Right};
void Move(struct SelectionInformation* pSelectionInformation, enum Direction eDirection);

int GetCurrentX(struct SelectionInformation* pSelectionInformation);
int GetCurrentY(struct SelectionInformation* pSelectionInformation);
#endif

