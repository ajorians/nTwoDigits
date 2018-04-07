#ifndef MAINMENU_H
#define MAINMENU_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "Background.h"
#include "Config.h"
#include "Font.h"

enum MenuChoice
{
   Play,
   Options,
   Help,
   Quit
};

struct MainMenu
{
   enum MenuChoice m_eChoice;
   Font *m_pFont;
   struct Config* m_pConfig;//Does not own
   struct SDL_Surface* m_pScreen;//Does not own
   struct SDL_Surface* m_pTitle;
};

void CreateMainMenu(struct MainMenu** ppMenu, struct Config* pConfig, struct SDL_Surface* pScreen);
void FreeMainMenu(struct MainMenu** ppMenu);
int MainMenuLoop(struct MainMenu* pMenu);
int MainMenuShouldQuit(struct MainMenu* pMenu);
int MainMenuShowOptions(struct MainMenu* pMenu);
int MainMenuShowHelp(struct MainMenu* pMenu);

#endif
