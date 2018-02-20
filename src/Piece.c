#include "Piece.h"

#include "Font.h"
#include "Replacements.h"
#include "SDL/SDL_gfxPrimitives.h"

#define PIECE_BORDER_SIZE	(4)

static Font* g_pFont = NULL;

void CreatePiece(struct Piece* pPiece, int x, int y, TwoDigitsLib twodigits, struct Metrics* pMetrics, struct SelectionInformation* pSelectionInformation, struct Config* pConfig)
{
   pPiece->m_nX = x;
   pPiece->m_nY = y;
   pPiece->m_TwoDigits = twodigits;
   pPiece->m_pMetrics = pMetrics;
   pPiece->m_pConfig = pConfig;
   pPiece->m_pSelectionInformation = pSelectionInformation;

   if( g_pFont == NULL ) {
      g_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/, 24);
   }
}

void FreePiece(struct Piece* pPiece)
{
   pPiece->m_TwoDigits = NULL;//Does not own
   pPiece->m_pMetrics = NULL;//Does not own
   pPiece->m_pConfig = NULL;//Does not own
   pPiece->m_pSelectionInformation = NULL;//Does not own

   if( g_pFont != NULL ) {
      FreeFont(g_pFont);
      g_pFont = NULL;
   }
}

void PieceDraw(struct Piece* pPiece, struct SDL_Surface* pScreen)
{
   int w = GetPieceWidth(pPiece->m_pMetrics, pPiece->m_nX, pPiece->m_nY);
   int h = GetPieceHeight(pPiece->m_pMetrics, pPiece->m_nX, pPiece->m_nY);
   int x = GetPieceLeft(pPiece->m_pMetrics, pPiece->m_nX, pPiece->m_nY) + w/2;
   int y = GetPieceTop(pPiece->m_pMetrics, pPiece->m_nX, pPiece->m_nY) + h/2;

   int radius = SDL_min(w/2, h/2);

   int r = 127, g = 127, b = 127;

   if( pPiece->m_nX == GetCurrentX(pPiece->m_pSelectionInformation) && pPiece->m_nY == GetCurrentY(pPiece->m_pSelectionInformation) )
   {
      r = 255, g = 0, b = 0;
   }

   filledEllipseRGBA(pScreen, x, y, radius, radius, r, g, b, 255);

   radius = radius - radius/8;

   r = 30, g = 144, b = 255;

   enum TwoDigitMarking eMarking;
   GetTwoDigitMarking(pPiece->m_TwoDigits, pPiece->m_nX, pPiece->m_nY, &eMarking);
   if( eMarking == LeftMarked ) {
      r = 0, g = 255, b = 0;
   }
   else if( eMarking == RightMarked)
   {
      r = 0, g = 0, b = 255;
   }
   filledEllipseRGBA(pScreen, x, y, radius, radius, r, g, b, 255);

   static char buffer[5];

   int nSpotValue = GetTwoDigitsSpotValue(pPiece->m_TwoDigits, pPiece->m_nX, pPiece->m_nY);
   if (nSpotValue > 0)
   {
      IntToA(buffer, 4, nSpotValue);

      int top = GetPieceValueTextTop(pPiece->m_pMetrics, pPiece->m_nX, pPiece->m_nY);
      int left = GetPieceValueTextLeft(pPiece->m_pMetrics, pPiece->m_nX, pPiece->m_nY);

      Font* pFont = g_pFont;
      int nR = 0, nG = 0, nB = 0;

      DrawText(pScreen, pFont, left, top, buffer, nR, nG, nB);
   }
}

 
