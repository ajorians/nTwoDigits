#ifndef GAME_H
#define GAME_H

#include <TwoDigitsLib/TwoDigitsLib.h>
#include <SDL/SDL.h>
#include "Background.h"
#include "Config.h"
#include "Metrics.h"
#include "Piece.h"
#include "SelectionInformation.h"
#include "Font.h"

#define GAME_QUIT      (0)
#define GAME_LOOPING   (1)
#define GAME_NEXTLEVEL (2)

struct Game
{
   TwoDigitsLib m_TwoDigits;
   int m_nLevelNum;
   struct Config* m_pConfig;//Does not own
   int m_bWon;
   struct Metrics* m_pMetrics;
   struct Piece* m_apPieces;
   struct SelecionInformation* m_pSelectionInformation;
   int m_bShouldQuit;
   struct SDL_Surface* m_pYouWinGraphic;
   struct SDL_Surface* m_pScreen;//Does not own
   struct Background* m_pBackground;
   Font* m_pFont;
   int m_bAnimating;
   int m_nAnimationStep;
};

void CreateGame(struct Game** ppGame, const char* pstrLevelData, int nLevelNum, struct Config* pConfig, struct SDL_Surface* pScreen);
void FreeGame(struct Game** ppGame);
int GameLoop(struct Game* pGame);
int GameShouldQuit(struct Game* pMenu);

#endif
