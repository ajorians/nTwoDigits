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

void CreateLevelMenu(struct LevelMenu** ppMenu, int nLevelNum, struct Config* pConfig, struct SDL_Surface* pScreen)
{
   *ppMenu = malloc(sizeof(struct LevelMenu));
   struct LevelMenu* pMenu = (*ppMenu);

   pMenu->m_pConfig = pConfig;
   pMenu->m_pScreen = pScreen;

   pMenu->m_bCancel = 0;
   pMenu->m_nCurrentLevel = nLevelNum >= 1 ? nLevelNum : 1;

   pMenu->m_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 255/*R*/, 0/*G*/, 0/*B*/, 24);

   CreateStarDrawer( &pMenu->m_pStarDrawer );

#ifdef _TINSPIRE
   pMenu->m_pTitle = nSDL_LoadImage(image_TwoDigits);
#endif
}

void FreeLevelMenu(struct LevelMenu** ppMenu)
{
   struct LevelMenu* pMenu = *ppMenu;

   FreeFont(pMenu->m_pFont);

   FreeStarDrawer( &pMenu->m_pStarDrawer );

   pMenu->m_pConfig = NULL;//Does not own
   pMenu->m_pScreen = NULL;//Does not own

#ifdef _TINSPIRE
   SDL_FreeSurface(pMenu->m_pTitle);
   pMenu->m_pTitle = NULL;
#endif

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
            switch( event.key.keysym.sym )
            {
            case SDLK_ESCAPE:
               printf("Hit Escape!n");
               pMenu->m_bCancel = 1;
               return 0;
               break;

            case SDLK_LEFT:
               if (pMenu->m_nCurrentLevel > 1) {
                  pMenu->m_nCurrentLevel--;
		            SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
               }
               break;

            case SDLK_RIGHT:
               if (pMenu->m_nCurrentLevel < 250) {
                  pMenu->m_nCurrentLevel++;
		            SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
               }

               break;

	       case SDLK_UP:
		  break;

	       case SDLK_DOWN:
		  break;

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

void LevelMenuUpdateDisplay(struct LevelMenu* pMenu)
{
#ifndef _TINSPIRE
   SDL_FillRect(pMenu->m_pScreen, NULL, SDL_MapRGB(pMenu->m_pScreen->format, 255, 215, 139));
#endif

#ifdef _TINSPIRE
   SDL_Rect rectSrc, rectDst;

   rectSrc.w = 320;
   rectSrc.h = 240;
   rectSrc.x = 0;
   rectSrc.y = 0;

   rectDst.w = 320;
   rectDst.h = 240;
   rectDst.x = 0;
   rectDst.y = 0;

   SDL_BlitSurface(pMenu->m_pTitle, &rectSrc, pMenu->m_pScreen, &rectDst);
#endif

   char levelNumBuffer[4];
   IntToA(levelNumBuffer, 4, pMenu->m_nCurrentLevel);

   char buffer[16];
   //StringCopy(buffer, 16, "");
   StringCopy(buffer, 16, "Level #");
   StringAppend(buffer, 16, levelNumBuffer);

   int left = SCREEN_WIDTH / 2 - 80;
   int top = SCREEN_HEIGHT / 2 - 45;
   int right = left + 160;
   int bottom = top + 53;

   SDL_Rect rectBK;
   rectBK.w = right - left;
   rectBK.h = bottom - top;
   rectBK.x = left;
   rectBK.y = top;
   SDL_FillRect(pMenu->m_pScreen, &rectBK, SDL_MapRGB(pMenu->m_pScreen->format, 255, 255, 255));
   DrawText(pMenu->m_pScreen, pMenu->m_pFont, rectBK.x + 12, rectBK.y + 9, buffer, 255, 255, 255);

   if( GetBeatLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel-1/*ToBase 0*/) == 1 )
      DrawStar(pMenu->m_pStarDrawer, pMenu->m_pScreen, rectBK.x+22, rectBK.y-5);

   int nDescriptX = rectBK.x + 12;
   int nDescriptY = rectBK.y + 29;

   int nSubDescriptX = nDescriptX;
   int nSubDescriptY = nDescriptY+12;

   if( pMenu->m_nCurrentLevel >= 1 && pMenu->m_nCurrentLevel <= 3)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "First Steps", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-30", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 4 && pMenu->m_nCurrentLevel <= 9)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Moving Up", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-50", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 10 && pMenu->m_nCurrentLevel <= 15)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Three Quarter", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-75", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 16 && pMenu->m_nCurrentLevel <= 45)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Full #1", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-99", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 46 && pMenu->m_nCurrentLevel <= 48)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Primary", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: Prime numbers", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 49 && pMenu->m_nCurrentLevel <= 60)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Ninety Percent", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-90", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 61 && pMenu->m_nCurrentLevel <= 90)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Full #2", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-99", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 91 && pMenu->m_nCurrentLevel <= 135)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Odd", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: Odd numbers", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 136 && pMenu->m_nCurrentLevel <= 180)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Two Digits Only", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 10-99", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 181 && pMenu->m_nCurrentLevel <= 240)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Full #3", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: 1-99", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel >= 241 && pMenu->m_nCurrentLevel <= 249)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Three", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: Multiples of 3", 255, 255, 255);
   }
   else if( pMenu->m_nCurrentLevel == 250)
   {
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nDescriptX, nDescriptY, "Ultimate", 255, 255, 255);
      DrawText(pMenu->m_pScreen, pMenu->m_pFont, nSubDescriptX, nSubDescriptY, "Range: ???", 255, 255, 255);
   }


   int r = 255, g = 0, b = 0, a = 200;

   //Top
   thickLineRGBA(pMenu->m_pScreen, (Sint16)left, (Sint16)top, (Sint16)right, (Sint16)top, MENU_SELECTOR_LINE_WIDTH, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
   //Left
   thickLineRGBA(pMenu->m_pScreen, (Sint16)left, (Sint16)top, (Sint16)left, (Sint16)bottom, MENU_SELECTOR_LINE_WIDTH, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
   //Bottom
   thickLineRGBA(pMenu->m_pScreen, (Sint16)left, (Sint16)bottom, (Sint16)right, (Sint16)bottom, MENU_SELECTOR_LINE_WIDTH, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
   //Right
   thickLineRGBA(pMenu->m_pScreen, (Sint16)right, (Sint16)top, (Sint16)right, (Sint16)bottom, MENU_SELECTOR_LINE_WIDTH, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);

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

