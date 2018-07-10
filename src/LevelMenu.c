#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#else
#endif
#include "LevelMenu.h"
#include "Replacements.h"
#include "Levels.h"
#include "MenuGraphic.h"
#include "SDL/SDL_gfxPrimitives.h"

#define MENU_SELECTOR_LINE_WIDTH (2)
#define TITLE_HEIGHT (12)
#define SUBTITLE_HEIGHT (12)

const int g_LevelPieceWidth = 40;
const int g_LevelPieceHeight = 40;

const int g_arrLevelSections[] = {3, 6, 6, 30, 3, 12, 30, 45, 45, 60, 9, 1};//Number of levels in group

Uint16 get_pixel16( SDL_Surface *surface, int x, int y )
{
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp; //Get the requested pixel
   return *(Uint16 *)p;
}

void put_pixel16( SDL_Surface *surface, int x, int y, Uint16 pixel )
{
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
   *(Uint16 *)p = pixel;
}

void RotateSurface(SDL_Surface* pSurface, SDL_Surface* pSurfaceFlipped)
{
   //If the surface must be locked
   if( SDL_MUSTLOCK( pSurface ) )
   {
      //Lock the surface
      SDL_LockSurface( pSurface );
   }
   if( SDL_MUSTLOCK( pSurfaceFlipped ) )
   {
      //Lock the surface
      SDL_LockSurface( pSurfaceFlipped );
   }

   //Go through columns
   for( int x = 0; x < pSurface->w; x++ )
   {
      //Go through rows
      for( int y = 0; y < pSurface->h; y++ )
      {
         //Get pixel
         Uint16 pixel = get_pixel16( pSurface, x, y );
         int nDestX = pSurface->h - y - 1;
         if( nDestX < 0)
         {
            int a = 0;
            a++;
         }
         put_pixel16( pSurfaceFlipped, nDestX, x, pixel );
      }
   }

   //Unlock surface
   if( SDL_MUSTLOCK( pSurface ) )
   {
      SDL_UnlockSurface( pSurface );
   }
   if( SDL_MUSTLOCK( pSurfaceFlipped ) )
   {
      SDL_UnlockSurface( pSurfaceFlipped );
   }
}

void CreateLevelMenu(struct LevelMenu** ppMenu, int nLevelNum, struct Config* pConfig, struct SDL_Surface* pScreen)
{
   *ppMenu = malloc(sizeof(struct LevelMenu));
   struct LevelMenu* pMenu = (*ppMenu);

   pMenu->m_pConfig = pConfig;
   pMenu->m_pScreen = pScreen;

   pMenu->m_bCancel = 0;
   pMenu->m_nCurrentLevel = nLevelNum >= 1 ? nLevelNum : 1;

   const int nSurfaceWidth = 150;
   int nSurfaceHeight = 0;
   for(int i=0; i<sizeof(g_arrLevelSections)/sizeof(g_arrLevelSections[0]); i++)
   {
      nSurfaceHeight+=TITLE_HEIGHT;
      nSurfaceHeight+=SUBTITLE_HEIGHT;

      int nRows = (g_arrLevelSections[i]+2)/3;
      nSurfaceHeight += nRows * g_LevelPieceHeight;
   }
   nSurfaceHeight += 1000;
   pMenu->m_pLevelSurface = SDL_CreateRGBSurface(0, nSurfaceWidth, nSurfaceHeight, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
   pMenu->m_nScrollY = -1;
   pMenu->m_nSrcScrollY = -1;

   pMenu->m_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 255/*R*/, 0/*G*/, 0/*B*/, 24);

   {
      pMenu->m_pSelect = SDL_CreateRGBSurface(0, 30, 70, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
      struct SDL_Surface *pTempSurface = SDL_CreateRGBSurface(0, pMenu->m_pSelect->h, pMenu->m_pSelect->w, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
      SDL_FillRect(pTempSurface, NULL, SDL_MapRGB(pTempSurface->format, 255, 215, 139));
      DrawText(pTempSurface, pMenu->m_pFont, 0, 0, "Select", 255, 0, 0);
      RotateSurface(pTempSurface, pMenu->m_pSelect);
      SDL_FreeSurface(pTempSurface);
   }

   {
      pMenu->m_pLevel = SDL_CreateRGBSurface(0, 30, 70, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
      struct SDL_Surface *pTempSurface = SDL_CreateRGBSurface(0, pMenu->m_pLevel->h, pMenu->m_pLevel->w, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
      SDL_FillRect(pTempSurface, NULL, SDL_MapRGB(pTempSurface->format, 255, 215, 139));
      DrawText(pTempSurface, pMenu->m_pFont, 0, 0, "Level", 255, 0, 0);
      RotateSurface(pTempSurface, pMenu->m_pLevel);
      SDL_FreeSurface(pTempSurface);
   }

   CreateStarDrawer( &pMenu->m_pStarDrawer );
}

void FreeLevelMenu(struct LevelMenu** ppMenu)
{
   struct LevelMenu* pMenu = *ppMenu;

   FreeFont(pMenu->m_pFont);

   FreeStarDrawer( &pMenu->m_pStarDrawer );

   pMenu->m_pConfig = NULL;//Does not own
   pMenu->m_pScreen = NULL;//Does not own

   SDL_FreeSurface(pMenu->m_pLevelSurface);
   pMenu->m_pLevelSurface = NULL;

   SDL_FreeSurface(pMenu->m_pSelect);
   pMenu->m_pSelect = NULL;
   SDL_FreeSurface(pMenu->m_pLevel);
   pMenu->m_pLevel = NULL;

   free(*ppMenu);
   *ppMenu = NULL;
}

int LevelMenuPollEvents(struct LevelMenu* pMenu)
{
   SDL_Event event;
   while( SDL_PollEvent( &event ) )
   {
      switch( event.type )
      {
         case SDL_KEYDOWN:
            printf( "Key press detected\n" );
            switch( event.key.keysym.sym ) {
               case SDLK_ESCAPE:
                  printf("Hit Escape!n");
                  pMenu->m_bCancel = 1;
                  return 0;
                  break;
               case SDLK_4:
               case SDLK_LEFT:
                  if (pMenu->m_nCurrentLevel > 1) {
                     pMenu->m_nCurrentLevel--;
                     SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
                  }
                  break;

                case SDLK_6:
               case SDLK_RIGHT:
                  if (pMenu->m_nCurrentLevel < 250) {
                     pMenu->m_nCurrentLevel++;
                     SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
                  }
                  break;
                case SDLK_8:
               case SDLK_UP:
                  if (pMenu->m_nCurrentLevel >= 4) {
                     pMenu->m_nCurrentLevel -= 3;
                     SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
                  }
                  break;

                case SDLK_2:
               case SDLK_DOWN:
                  if (pMenu->m_nCurrentLevel <= (250 - 3)) {
                     pMenu->m_nCurrentLevel += 3;
                     SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
                  }
                  break;

                case SDLK_5:
               case SDLK_SPACE:
               case SDLK_RETURN:
               case SDLK_LCTRL:
                     return 0;
                  break;

               default:
                  break;
            }
      }
   }

   return 1;
}

void DrawLevelPiece(int x, int y, int nLevelNumber, int bSelected, Font* pFont, struct SDL_Surface* pScreen, int nBeatLevel, struct StarDrawer* pStarDrawer)
{
   int w = g_LevelPieceWidth;
   int h = g_LevelPieceHeight;

   int radius = SDL_min(w/2, h/2);

   int r = 127, g = 127, b = 127, a = 255;

   if( bSelected == 1 )
   {
      r = 255, g = 0, b = 0;
   }

   filledEllipseRGBA(pScreen, (Sint16)x, (Sint16)y, (Sint16)radius, (Sint16)radius, (Uint8)r, (Uint8)g, (Uint8)b, a);

   int nPortionRim = 5;
#ifdef _TINSPIRE
   if( !has_colors )
   {
      if( bSelected == 1)
          nPortionRim = 9;
   }
#endif
   radius = radius - radius/nPortionRim;

#ifdef _TINSPIRE
    if( has_colors ) {
#endif
        r = 30, g = 144, b = 255;
#ifdef _TINSPIRE
    } else
    {
        r = 255, g = 255, b = 255, a = 255;
    }
#endif
   filledEllipseRGBA(pScreen, (Sint16)x, (Sint16)y, (Sint16)radius, (Sint16)radius, (Uint8)r, (Uint8)g, (Uint8)b, a);

   static char buffer[5];

   IntToA(buffer, 4, nLevelNumber);

   int top = y;
   int left = x - 3;
   if( nLevelNumber >= 10 )
      left -= 3;
   if( nLevelNumber >= 100 )
      left -= 3;

   int nR = 0, nG = 0, nB = 0;
   DrawText(pScreen, pFont, left, top, buffer, nR, nG, nB);

   if( nBeatLevel == 1)
      DrawStar(pStarDrawer, pScreen, left-w/2+3, top-h/2);
}

void DrawLevelSurface( struct LevelMenu* pMenu, struct SDL_Surface* pSurface, int* pnCurrentLevelY)
{
   SDL_FillRect(pSurface, NULL, SDL_MapRGB(pSurface->format, 255, 215, 139));

   char* arrTitles[] = {
           "First Steps",
           "Moving Up",
           "Three Quarter",
           "Full #1",
           "Primary",
           "Ninety Percent",
           "Full #2",
           "Odd",
           "Two Digits Only",
           "Full #3",
           "Three",
           "Ultimate"

   };

   char* arrSubTitles[] = {
           "Range: 1-30",
           "Range: 1-50",
           "Range: 1-75",
           "Range: 1-99",
           "Range: Prime numbers",
           "Range: 1-90",
           "Range: 1-99",
           "Range: Odd numbers",
           "Range: 10-99",
           "Range: 1-99",
           "Range: Multiples of 3",
           "Range: ???"
   };

   const int nLeft = 0;

   int x=nLeft, y=0;
   int nCurrentLevel = 1;
   for(int nCurrentSection=0; nCurrentSection<sizeof(g_arrLevelSections)/sizeof(g_arrLevelSections[0]); nCurrentSection++)
   {
      if( y > 0 || y < pSurface->h ){
         x = nLeft;
         DrawText(pSurface, pMenu->m_pFont, x, y, arrTitles[nCurrentSection], 0, 0, 0);
         y+= TITLE_HEIGHT;
         DrawText(pSurface, pMenu->m_pFont, x, y, arrSubTitles[nCurrentSection], 0, 0, 0);
         y+= SUBTITLE_HEIGHT;
      }

      y += (g_LevelPieceHeight/2);//Needs spacing since drawing ellipse is at center
      x = nLeft + (g_LevelPieceWidth/2);//Needs spacing since drawing ellipse is at center

      int nGroupLevelStart = nCurrentLevel;
      int nPos = 0;
      for( int nLevelWithinGroup = 0; nLevelWithinGroup < g_arrLevelSections[nCurrentSection]; nLevelWithinGroup++)
      {
         int level = nGroupLevelStart + nLevelWithinGroup;
         int selected = level == pMenu->m_nCurrentLevel ? 1 : 0;

         if( selected == 1 )
         {
            if( nLevelWithinGroup == 0 )
            {
               *pnCurrentLevelY = y - g_LevelPieceHeight / 2 - TITLE_HEIGHT - SUBTITLE_HEIGHT;

            } else {
               *pnCurrentLevelY = y - g_LevelPieceHeight / 2 - 2/*Little extra*/;
            }
         }

         nCurrentLevel++;

         if( y < 0 || y > pSurface->h ) {
            continue;
         }

         DrawLevelPiece(x, y, level, selected, pMenu->m_pFont, pSurface, GetBeatLevel(pMenu->m_pConfig, level-1/*To 0-based*/), pMenu->m_pStarDrawer);
         x+= g_LevelPieceWidth + 5;
         nPos++;
         if( nPos >= 3 )
         {
            x = nLeft + (g_LevelPieceWidth/2);
            y += g_LevelPieceHeight;
            nPos = 0;
         }
      }
      if( nPos > 0 ) {
         y += g_LevelPieceHeight;
      }
   }
}

void LevelMenuUpdateDisplay(struct LevelMenu* pMenu)
{
   SDL_FillRect(pMenu->m_pScreen, NULL, SDL_MapRGB(pMenu->m_pScreen->format, 255, 215, 139));

   int nCurrentLevelY = 0;
   DrawLevelSurface( pMenu, pMenu->m_pLevelSurface, &nCurrentLevelY );

   if( pMenu->m_nScrollY == -1 ) {
      int n = nCurrentLevelY- g_LevelPieceHeight;
      if( n < 0 )
         n = 0;
      if( n >= (pMenu->m_pLevelSurface->h - g_LevelPieceHeight) )
         n = pMenu->m_pLevelSurface->h - g_LevelPieceHeight;
      pMenu->m_nScrollY = pMenu->m_nSrcScrollY = n;
   }

   const int nDestinationTop = 10;
   const int nDestinationHeight = SCREEN_HEIGHT - nDestinationTop;

   SDL_Rect src;
   src.w = pMenu->m_pLevelSurface->w;
   src.h = nDestinationHeight;
   src.x = 0;
   src.y = pMenu->m_nSrcScrollY;

   SDL_Rect dst;
   dst.w = src.w;
   dst.h = nDestinationHeight;
   dst.x = SCREEN_WIDTH/2 - src.w/2;
   dst.y = nDestinationTop;
   SDL_BlitSurface(pMenu->m_pLevelSurface, &src, pMenu->m_pScreen, &dst);

   if( pMenu->m_nSrcScrollY < pMenu->m_nScrollY )
   {
      int nAmount = 3;
      if( (pMenu->m_nScrollY - pMenu->m_nSrcScrollY) > (4*g_LevelPieceHeight) )
         nAmount = 18;
      else if( (pMenu->m_nScrollY - pMenu->m_nSrcScrollY) > (3*g_LevelPieceHeight) )
         nAmount = 8;
      else if( (pMenu->m_nScrollY - pMenu->m_nSrcScrollY) > (2*g_LevelPieceHeight) )
         nAmount = 6;
      pMenu->m_nSrcScrollY+= nAmount;
   }
   if( pMenu->m_nSrcScrollY > pMenu->m_nScrollY )
   {
      int nAmount = 3;
      if( (pMenu->m_nSrcScrollY - pMenu->m_nScrollY) > (4*g_LevelPieceHeight) )
         nAmount = 18;
      else if( (pMenu->m_nSrcScrollY - pMenu->m_nScrollY) > (3*g_LevelPieceHeight) )
         nAmount = 8;
      else if( (pMenu->m_nSrcScrollY - pMenu->m_nScrollY) > (2*g_LevelPieceHeight) )
         nAmount = 6;
      pMenu->m_nSrcScrollY-=nAmount;
   }

   //Check if needs to scroll
   int nCurrentLevelAboveScreen = pMenu->m_nScrollY > nCurrentLevelY ? 1 : 0;
   int nCurrentLevelBelowScreen = (pMenu->m_nScrollY + nDestinationHeight - (g_LevelPieceHeight + 15/*Little extra*/) - nCurrentLevelY) < 0 ? 1 : 0;
   if( nCurrentLevelAboveScreen == 1 || nCurrentLevelBelowScreen == 1 )
   {
      if( nCurrentLevelAboveScreen == 1 ){
         pMenu->m_nScrollY-=(g_LevelPieceHeight*1.5);
         if( pMenu->m_nScrollY < 0)
            pMenu->m_nScrollY=0;
      }
      else if( nCurrentLevelBelowScreen == 1 ){
         pMenu->m_nScrollY+=(g_LevelPieceHeight*1.5);
         if( pMenu->m_nScrollY > (pMenu->m_pLevelSurface->h - nDestinationHeight) )
            pMenu->m_nScrollY=pMenu->m_pLevelSurface->h - nDestinationHeight;
      }
   }

   SDL_Rect selectDst;
   selectDst.w = pMenu->m_pSelect->w;
   selectDst.h = pMenu->m_pSelect->h;
   selectDst.x = 15;
   selectDst.y = (SCREEN_HEIGHT - selectDst.h)/2 + 10;
   SDL_BlitSurface(pMenu->m_pSelect, NULL, pMenu->m_pScreen, &selectDst);

   SDL_Rect levelDst;
   levelDst.w = pMenu->m_pLevel->w;
   levelDst.h = pMenu->m_pLevel->h;
   levelDst.x = SCREEN_WIDTH - levelDst.w-45;
   levelDst.y = (SCREEN_HEIGHT - levelDst.h)/2 + 10;
   SDL_BlitSurface(pMenu->m_pLevel, NULL, pMenu->m_pScreen, &levelDst);

   SDL_UpdateRect(pMenu->m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

int LevelMenuLoop(struct LevelMenu* pMenu)
{
   if( LevelMenuPollEvents(pMenu) == 0 )
      return 0;

   LevelMenuUpdateDisplay(pMenu);

   return 1;
}

int LevelMenuShouldQuit(struct LevelMenu* pMenu)
{
   return pMenu->m_bCancel;
}

int LevelMenuGetLevelNum(struct LevelMenu* pMenu)
{
   return pMenu->m_nCurrentLevel;
}

