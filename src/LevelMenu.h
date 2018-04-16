#ifndef LEVELMENU_H
#define LEVELMENU_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "Background.h"
#include "Config.h"
#include "Font.h"
#include "StarDrawer.h"
#include <TwoDigitsLib/TwoDigitsLib.h>

struct LevelMenu
{
   int m_bCancel;
   int m_nCurrentLevel;
   Font *m_pFont;
   struct StarDrawer* m_pStarDrawer;
   struct Config* m_pConfig;//Does not own
   struct SDL_Surface* m_pScreen;//Does not own
   struct SDL_Surface* m_pTitle;
   struct SDL_Surface* m_pLevelSurface;
   int m_nScrollY;
   int m_nSrcScrollY;
};

void CreateLevelMenu(struct LevelMenu** ppMenu, int nLevelNum, struct Config* pConfig, struct SDL_Surface* pScreen);
void FreeLevelMenu(struct LevelMenu** ppMenu);
int LevelMenuLoop(struct LevelMenu* pMenu);
int LevelMenuShouldQuit(struct LevelMenu* pMenu);
int LevelMenuGetLevelNum(struct LevelMenu* pMenu);

#endif
