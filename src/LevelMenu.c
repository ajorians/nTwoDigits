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

void UpdateDimensionAndOperations(struct LevelMenu* pMenu, int nLevelNum);

void CreateLevelMenu(struct LevelMenu** ppMenu, int nLevelNum, struct Config* pConfig, struct SDL_Surface* pScreen)
{
   *ppMenu = malloc(sizeof(struct LevelMenu));
   struct LevelMenu* pMenu = (*ppMenu);

   pMenu->m_pConfig = pConfig;
   pMenu->m_pScreen = pScreen;

   pMenu->m_bCancel = 0;
   pMenu->m_nCurrentLevel = nLevelNum >= 1 ? nLevelNum : 1;

   UpdateDimensionAndOperations(pMenu, pMenu->m_nCurrentLevel);

   pMenu->m_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 255/*R*/, 0/*G*/, 0/*B*/, 24);

   CreateStarDrawer( &pMenu->m_pStarDrawer );

#ifdef _TINSPIRE
   pMenu->m_pTitle = nSDL_LoadImage(image_KenKen);
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
                  UpdateDimensionAndOperations(pMenu, pMenu->m_nCurrentLevel);
               }
               break;

            case SDLK_RIGHT:
               if (pMenu->m_nCurrentLevel < 249) {
                  pMenu->m_nCurrentLevel++;
		            SetLastLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel);
                  UpdateDimensionAndOperations(pMenu, pMenu->m_nCurrentLevel);
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

void UpdateDimensionAndOperations(struct LevelMenu* pMenu, int nLevelNum)
{
   (pMenu);
   (nLevelNum);
   /*char strLevelData[2048];
   LevelLoad(strLevelData, nLevelNum);
   KenKenLib kenken;
   KenKenLibCreate(&kenken, strLevelData);

   pMenu->m_nDimension = GetKenKenWidth(kenken);
   pMenu->m_eOperations = GetKenKenOperations(kenken);

   KenKenLibFree(&kenken);*/
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

   int x = SCREEN_WIDTH/2+15;
   int y = SCREEN_HEIGHT/2 - 34;

   DrawText(pMenu->m_pScreen, pMenu->m_pFont, x, y, buffer, 255, 255, 255);

   if( GetBeatLevel(pMenu->m_pConfig, pMenu->m_nCurrentLevel-1/*ToBase 0*/) == 1 )
      DrawStar(pMenu->m_pStarDrawer, pMenu->m_pScreen, x+22, y-5);

   /*IntToA(levelNumBuffer, 4, pMenu->m_nDimension);
   StringCopy(buffer, 16, "");
   StringAppend(buffer, 16, levelNumBuffer);
   StringAppend(buffer, 16, " x ");
   StringAppend(buffer, 16, levelNumBuffer);
   x += 0;
   y += 16;
   DrawText(pMenu->m_pScreen, pMenu->m_pFont, x, y, buffer, 0, 0, 0);

   StringCopy(buffer, 16, "");
   if( (pMenu->m_eOperations & AddOperation) == AddOperation) {
      StringAppend(buffer, 16, "+ ");
   }
   if ((pMenu->m_eOperations & SubtractOperation) == SubtractOperation) {
      StringAppend(buffer, 16, "- ");
   }
   if ((pMenu->m_eOperations & MultiplyOperation) == MultiplyOperation){
      StringAppend(buffer, 16, "x ");
   }
   if ((pMenu->m_eOperations & DivideOperation) == DivideOperation) {
      StringAppend(buffer, 16, "/");
   }

   x -= 0;
   y += 14;
   DrawText(pMenu->m_pScreen, pMenu->m_pFont, x, y, buffer, 0, 0, 0);*/

   int r = 255, g = 0, b = 0, a = 200;

   int left = SCREEN_WIDTH / 2 - 62;
   int top = SCREEN_HEIGHT / 2 - 45;
   int right = left + 133;
   int bottom = top + 78;

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

