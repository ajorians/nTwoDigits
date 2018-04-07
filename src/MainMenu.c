#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#else
#endif
#include "MainMenu.h"
#include "Replacements.h"
#include "Levels.h"
#include "MenuGraphic.h"
#include "SDL/SDL_gfxPrimitives.h"

#define MENU_SELECTOR_LINE_WIDTH (2)

void CreateMainMenu(struct MainMenu** ppMenu, struct Config* pConfig, struct SDL_Surface* pScreen)
{
   *ppMenu = malloc(sizeof(struct MainMenu));
   struct MainMenu* pMenu = (*ppMenu);

   pMenu->m_pConfig = pConfig;
   pMenu->m_pScreen = pScreen;

   pMenu->m_eChoice = Play;

   pMenu->m_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 255/*R*/, 0/*G*/, 0/*B*/, 24);

#ifdef _TINSPIRE
   pMenu->m_pTitle = nSDL_LoadImage(image_KenKen);
#endif
}

void FreeMainMenu(struct MainMenu** ppMenu)
{
   struct MainMenu* pMenu = *ppMenu;

   FreeFont(pMenu->m_pFont);

   pMenu->m_pConfig = NULL;//Does not own
   pMenu->m_pScreen = NULL;//Does not own

#ifdef _TINSPIRE
   SDL_FreeSurface(pMenu->m_pTitle);
   pMenu->m_pTitle = NULL;
#endif

   free(*ppMenu);
   *ppMenu = NULL;
}

int PollEvents(struct MainMenu* pMenu)
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
               pMenu->m_eChoice = Quit;
               return 0;
               break;

            case SDLK_LEFT:
               if (pMenu->m_eChoice == Play) {
               }

               else if (pMenu->m_eChoice == Help) {
                  pMenu->m_eChoice = Options;
               }
               break;

            case SDLK_RIGHT:
               if (pMenu->m_eChoice == Play) {
               }

               else if (pMenu->m_eChoice == Options) {
                  pMenu->m_eChoice = Help;
               }
               break;

	       case SDLK_UP:
		  if( pMenu->m_eChoice == Options || pMenu->m_eChoice == Help) {
           pMenu->m_eChoice = Play;
		     }
		  break;

	       case SDLK_DOWN:
             if (pMenu->m_eChoice == Play ) {
                pMenu->m_eChoice = Options;
             }
		  break;

               case SDLK_SPACE:
               case SDLK_RETURN:
               case SDLK_LCTRL:
		  if( pMenu->m_eChoice == Play || pMenu->m_eChoice == Options || pMenu->m_eChoice == Help ) {
                     return 0;
		  }
                  break;

               default:
                  break;
            }
      }
   }

   return 1;
}

void UpdateDisplay(struct MainMenu* pMenu)
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

   DrawText(pMenu->m_pScreen, pMenu->m_pFont, 20, SCREEN_HEIGHT - 27, "Options", 0, 0, 0);
   DrawText(pMenu->m_pScreen, pMenu->m_pFont, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 27, "Help", 0, 0, 0);

   int r = 255, g = 0, b = 0, a = 200;

   int left = SCREEN_WIDTH / 2 - 62;
   int top = SCREEN_HEIGHT / 2 - 45;
   int right = left + 133;
   int bottom = top + 78;

   if (pMenu->m_eChoice == Options)
   {
      left = 7;
      top = SCREEN_HEIGHT - 32;
      right = left + 80;
      bottom = SCREEN_HEIGHT-5;
   }
   else if (pMenu->m_eChoice == Help)
   {
      left = SCREEN_WIDTH - 60;
      top = SCREEN_HEIGHT - 32;
      right = SCREEN_WIDTH-7;
      bottom = SCREEN_HEIGHT-5;
   }

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

int MainMenuLoop(struct MainMenu* pMenu)
{
   if( PollEvents(pMenu) == 0 )
      return 0;

   UpdateDisplay(pMenu);

   return 1;
}

int MainMenuShouldQuit(struct MainMenu* pMenu)
{
   return pMenu->m_eChoice == Quit;
}

int MainMenuShowOptions(struct MainMenu* pMenu)
{
   return pMenu->m_eChoice == Options;
}

int MainMenuShowHelp(struct MainMenu* pMenu)
{
   return pMenu->m_eChoice == Help;
}

