#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#endif
#include <SDL/SDL.h>
#include "Startup.h"
#include "MainMenu.h"
#include "LevelMenu.h"
#include "Game.h"
#include "Options.h"
#include "Help.h"
#include "Config.h"
#include "Levels.h"
#ifndef _TINSPIRE
#include <SDL/SDL_ttf.h>
#ifndef _WIN32
#include <sys/stat.h>
#endif
#endif
#include "Replacements.h"

#ifdef _TINSPIRE
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_FULLSCREEN|SDL_HWPALETTE)
#else
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_HWPALETTE)
#endif

#ifdef _WIN32
int WinMain(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
#ifdef _WIN32
   (argv);
#endif
#ifdef _TINSPIRE
   ArchiveSetCurrentDirectory( argv[0] );
   if( argc != 2 ) {
      if( !config_file_already_written() ) {
         write_config_file();
      }
   }
#endif

   printf("Initializing SDL.\n");

   /* Initialize the SDL library (starts the event loop) */
   if ( SDL_Init(SDL_INIT_VIDEO ) < 0 )
   {
#ifdef _TINSPIRE
      fprintf(stderr,
              "Couldn't initialize SDL: %s\n", SDL_GetError());
#endif
      exit(1);
   }

   printf("SDL initialized.\n");

   SDL_Surface* pScreen = NULL;
   pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SCREEN_VIDEO_MODE);

   //HSSetCurrentDirectory(argv[0]);

   if( pScreen == NULL )
   {
#ifdef _TINSPIRE
      fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SDL_GetError());
#endif
      exit(1);
   }
   else
   {
      /* Hides the cursor */
      SDL_ShowCursor(SDL_DISABLE);
   }

#ifndef _TINSPIRE
   //Initialize SDL_ttf
   if (TTF_Init() == -1)
   {
      exit(1);
   }
#endif

   int nLevelNumber = -1;
   char strLevelData[2048];
   struct Config* pConfig = NULL;
   CreateConfig(&pConfig);
   while(1) {
      int bShowHelp = 0, bShowOptions = 0;
      int bShouldQuit = 0;
      if( argc != 2 ) {
         nLevelNumber = GetLastLevel(pConfig);

         struct MainMenu* pMenu = NULL;
         CreateMainMenu(&pMenu, pConfig, pScreen);
         while(MainMenuLoop(pMenu)){}
         bShouldQuit = MainMenuShouldQuit(pMenu);
         bShowOptions = MainMenuShowOptions(pMenu);
         bShowHelp = MainMenuShowHelp(pMenu);
         FreeMainMenu(&pMenu);

         if( bShouldQuit )
            break;
      }
      else {
#ifndef _WIN32
         FILE *fp = fopen(argv[1], "r");
         if (!fp) { return 0; }
         struct stat filestat;
         if (stat(argv[1],&filestat) == -1) { fclose(fp); return 0; }

         fread(strLevelData, 1, filestat.st_size, fp);

         strLevelData[filestat.st_size] = 0;

         fclose(fp);
#endif
      }

      if( bShowOptions ) {
         struct Options* pOptions = NULL;
         CreateOptions(&pOptions, pConfig, pScreen);
         while(OptionsLoop(pOptions)){}
         FreeOptions(&pOptions);
         continue;
      }
      else if( bShowHelp ) {
         struct Help* pHelp = NULL;
         CreateHelp(&pHelp, pScreen);
         while(HelpLoop(pHelp)){}
         FreeHelp(&pHelp);
         continue;
      }
      else {

         while(1) {
            if (argc != 2) {
               //Show LevelSelectScreen

               struct LevelMenu *pLevelMenu = NULL;
               CreateLevelMenu(&pLevelMenu, nLevelNumber, pConfig, pScreen);
               while (LevelMenuLoop(pLevelMenu)) {}
               bShouldQuit = LevelMenuShouldQuit(pLevelMenu);

               if (bShouldQuit == 0) {
                  nLevelNumber = LevelMenuGetLevelNum(pLevelMenu);
                  printf("Loading level: %d\n", nLevelNumber);
                  LevelLoad(strLevelData, nLevelNumber);
               }

               FreeLevelMenu(&pLevelMenu);

               if (bShouldQuit)
                  break;
            }

            struct Game *pGame = NULL;

            while (1) {

               CreateGame(&pGame, strLevelData, nLevelNumber, pConfig, pScreen);
               int nLoop;
               while (1) {
                  nLoop = GameLoop(pGame);
                  if (nLoop != GAME_LOOPING)
                     break;
               }
               bShouldQuit = GameShouldQuit(pGame);
               FreeGame(&pGame);

               if (nLoop == GAME_QUIT)
                  break;
               if (bShouldQuit)
                  break;
               if (nLevelNumber == -1)
                  break;

               //Advance to next level if we can
               if (nLevelNumber < 249 && nLevelNumber != -1) {
                  nLevelNumber++;
                  LevelLoad(strLevelData, nLevelNumber);
               } else {
                  break;
               }
            }


             if (nLevelNumber == -1)
                 bShouldQuit=1;
            if (bShouldQuit)
               break;
         }
      }

      if( argc == 2 )
          break;
   }

   FreeConfig(&pConfig);

   printf("Quitting SDL.\n");

#ifndef _TINSPIRE
   //Quit SDL_ttf
   TTF_Quit();
#endif

   /* Shutdown all subsystems */
   SDL_Quit();

   printf("Quitting...\n");

   return 0;
}
