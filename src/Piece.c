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
   pPiece->m_bDrawBlack = 1;

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

   int r = 128, g = 128, b = 128, a = 255;

   if( pPiece->m_nX == GetCurrentX(pPiece->m_pSelectionInformation) 
      && pPiece->m_nY == GetCurrentY(pPiece->m_pSelectionInformation) 
      && pPiece->m_bDrawBlack == 0 )
   {
      r = 255, g = 0, b = 0;
#ifdef _TINSPIRE
      if( !has_colors )
      {
        r = 169, g = 169, b = 169;
      }
#endif
   }

   filledEllipseRGBA(pScreen, (Sint16)x, (Sint16)y, (Sint16)radius, (Sint16)radius, (Uint8)r, (Uint8)g, (Uint8)b, a);

   int nPortionRim = 5;
#ifdef _TINSPIRE
   if( !has_colors ) {
       if( pPiece->m_nX == GetCurrentX(pPiece->m_pSelectionInformation)
      && pPiece->m_nY == GetCurrentY(pPiece->m_pSelectionInformation)
      && pPiece->m_bDrawBlack == 0 )
           nPortionRim = 9;
   }
#endif
   radius = radius - radius/nPortionRim;

   if (pPiece->m_bDrawBlack)
   {
      r = g = b = 0;
      filledEllipseRGBA(pScreen, (Sint16)x, (Sint16)y, (Sint16)radius, (Sint16)radius, (Uint8)r, (Uint8)g, (Uint8)b, a);
      return;
   }
   else
   {
      r = 30, g = 144, b = 255;
#ifdef _TINSPIRE
      if( !has_colors )
      {
         r = 192, g = 192, b = 192, a = 255;
      }
#endif

      enum TwoDigitMarking eMarking;
      GetTwoDigitMarking(pPiece->m_TwoDigits, pPiece->m_nX, pPiece->m_nY, &eMarking);
      if (eMarking == LeftMarked) {
         r = 0, g = 255, b = 0;
#ifdef _TINSPIRE
      if( !has_colors )
      {
         r = 169, g = 169, b = 169, a = 255;
      }
#endif
      }
      else if (eMarking == RightMarked)
      {
         r = 0, g = 0, b = 255;
#ifdef _TINSPIRE
      if( !has_colors )
      {
         r = 128, g = 128, b = 128, a = 255;
      }
#endif
      }
      filledEllipseRGBA(pScreen, (Sint16)x, (Sint16)y, (Sint16)radius, (Sint16)radius, (Uint8)r, (Uint8)g, (Uint8)b, a);

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
}

int IsDrawingPieceBlack(struct Piece* pPiece)
{
   return pPiece->m_bDrawBlack;
}

void SetPieceBlack(struct Piece* pPiece, int nNewState)
{
   pPiece->m_bDrawBlack = nNewState;
}


