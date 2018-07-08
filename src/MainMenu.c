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
   pMenu->m_pTitle = nSDL_LoadImage(image_TwoDigits);
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

                case SDLK_4:
            case SDLK_LEFT:
               if (pMenu->m_eChoice == Play) {
               }

               else if (pMenu->m_eChoice == Help) {
                  pMenu->m_eChoice = Options;
               }
               break;

                case SDLK_6:
            case SDLK_RIGHT:
               if (pMenu->m_eChoice == Play) {
               }

               else if (pMenu->m_eChoice == Options) {
                  pMenu->m_eChoice = Help;
               }
               break;

                case SDLK_8:
	       case SDLK_UP:
		  if( pMenu->m_eChoice == Options || pMenu->m_eChoice == Help) {
           pMenu->m_eChoice = Play;
		     }
		  break;

                case SDLK_2:
	       case SDLK_DOWN:
             if (pMenu->m_eChoice == Play ) {
                pMenu->m_eChoice = Options;
             }
		  break;

                case SDLK_5:
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
#ifdef _TINSPIRE
    if( !has_colors )
    {
        SDL_FillRect(pMenu->m_pScreen, NULL, SDL_MapRGB(pMenu->m_pScreen->format, 255, 255, 255));
    }
#else
   SDL_FillRect(pMenu->m_pScreen, NULL, SDL_MapRGB(pMenu->m_pScreen->format, 255, 215, 139));
#endif

   int rText = 0, gText = 0, bText = 0;

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

   if( has_colors ) {
   SDL_BlitSurface(pMenu->m_pTitle, &rectSrc, pMenu->m_pScreen, &rectDst);
   }

   if( !has_colors ) {
      rText = 255, gText = 255, bText = 255;
   }
#endif

   int left = SCREEN_WIDTH / 2 - 25;
   int top = SCREEN_HEIGHT / 2 - 45;
   int right = left + 50;
   int bottom = top + 27;

   SDL_Rect rectBK;
   rectBK.w = right - left;
   rectBK.h = bottom - top;
   rectBK.x = left;
   rectBK.y = top;
#ifdef _TINSPIRE
   if( has_colors )
#endif
   SDL_FillRect(pMenu->m_pScreen, &rectBK, SDL_MapRGB(pMenu->m_pScreen->format, 255, 255, 255));

   DrawText(pMenu->m_pScreen, pMenu->m_pFont, rectBK.x + 12, rectBK.y + 9, "Play", rText, gText, bText);

   rectBK.x = 7;
   rectBK.y = SCREEN_HEIGHT - 32;
   rectBK.w = 80;
#ifdef _TINSPIRE
   if( has_colors )
#endif
   SDL_FillRect(pMenu->m_pScreen, &rectBK, SDL_MapRGB(pMenu->m_pScreen->format, 255, 255, 255));
   DrawText(pMenu->m_pScreen, pMenu->m_pFont, rectBK.x + 12, rectBK.y + 9, "Options", rText, gText, bText);

   rectBK.x = SCREEN_WIDTH - 60;
   rectBK.w = 53;
#ifdef _TINSPIRE
   if( has_colors )
#endif
   SDL_FillRect(pMenu->m_pScreen, &rectBK, SDL_MapRGB(pMenu->m_pScreen->format, 255, 255, 255));
   DrawText(pMenu->m_pScreen, pMenu->m_pFont, rectBK.x + 12, rectBK.y + 9, "Help", rText, gText, bText);

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

