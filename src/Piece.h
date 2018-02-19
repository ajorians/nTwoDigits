#ifndef PIECE_H
#define PIECE_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include <TwoDigitsLib/TwoDigitsLib.h>
#include "Metrics.h"
#include "Config.h"
#include "SelectionInformation.h"

struct Piece
{
   int m_nX;
   int m_nY;
   TwoDigitsLib m_TwoDigits;//Does not own
   struct Metrics* m_pMetrics;//Does not own
   struct Config* m_pConfig;//Does not own
   struct SelectionInformation* m_pSelectionInformation;//Does not own
};

void CreatePiece(struct Piece* pPiece, int x, int y, TwoDigitsLib twodigits, struct Metrics* pMetrics, struct SelectionInformation* pSelectionInformation, struct Config* pConfig);
void FreePiece(struct Piece* pPiece);
void PieceDraw(struct Piece* pPiece, struct SDL_Surface* pScreen);

#endif

