#include "Font.h"
#include "Replacements.h"

Font* LoadFont(char* pstrName, int nID, int r, int g, int b, int size)
{
#ifdef _TINSPIRE
   return nSDL_LoadFont(nID, r, g, b);
#else
   Font* pFont = TTF_OpenFont(pstrName, size);
#ifndef _WIN32
   if( pFont == NULL )
   {
      char buffer[256];
      strcpy(buffer, "../../");
      strcat(buffer, pstrName);
      pFont = TTF_OpenFont(buffer, size);
      if( pFont == NULL )
      {
         strcpy(buffer, "../../src/");
         strcat(buffer, pstrName);
         pFont = TTF_OpenFont(buffer, size);
      }
   }
#endif
   return pFont;
#endif
}

void FreeFont(Font* pFont)
{
#ifdef _TINSPIRE
   nSDL_FreeFont(pFont);
#else
   TTF_CloseFont(pFont);
#endif
}

void DrawText(SDL_Surface* pSurface, Font* pFont, int x, int y, char* pstrBuffer, int r, int g, int b)
{
#ifdef _TINSPIRE
   nSDL_DrawString(pSurface, pFont, x, y, pstrBuffer);
#else
   SDL_Surface *message = NULL;
   SDL_Color textColor = { r, g, b };
   message = TTF_RenderText_Solid(pFont, pstrBuffer, textColor);
   SDL_Rect rectSrc, rectDst;
   rectSrc.w = SCREEN_WIDTH- x;
   rectSrc.h = 24;
   rectSrc.x = 0;
   rectSrc.y = 0;

   rectDst.w = 150;
   rectDst.h = 24;
   rectDst.x = x;
   rectDst.y = y;

   SDL_BlitSurface(message, &rectSrc, pSurface, &rectDst);
   SDL_FreeSurface(message);
#endif
}

