#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#endif
#include "Game.h"
#include "Replacements.h"
#include "YouWinGraphic.h"

#define ANIMATE_PIECES_INOUT_STEPS     (5)
#define DELAY_IN_ANIMATION_PIECES_STEP (10)

void CreateGame(struct Game** ppGame, const char* pstrLevelData, int nLevelNum, struct Config* pConfig, struct SDL_Surface* pScreen)
{
   *ppGame = malloc(sizeof(struct Game));
   struct Game* pGame = *ppGame;
   TwoDigitsLibCreate(&(pGame->m_TwoDigits), pstrLevelData);
   pGame->m_nLevelNum = nLevelNum;
   pGame->m_pConfig = pConfig;
   pGame->m_bWon = IsTwoDigitsGameOver(pGame->m_TwoDigits);
   pGame->m_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/, 24);

#ifdef _TINSPIRE
   pGame->m_pYouWinGraphic = nSDL_LoadImage(image_YouWin);
   SDL_SetColorKey(pGame->m_pYouWinGraphic, SDL_SRCCOLORKEY, SDL_MapRGB(pGame->m_pYouWinGraphic->format, 255, 255, 255));
#else
   pGame->m_pYouWinGraphic = NULL;
#endif

   pGame->m_pScreen = pScreen;
   CreateBackground(&(pGame->m_pBackground), pGame->m_pScreen, pGame->m_pConfig, 1);
   pGame->m_pMetrics = NULL;
   CreateMetrics(&pGame->m_pMetrics, pGame->m_TwoDigits, pGame->m_pConfig);

   pGame->m_pSelectionInformation = NULL;
   CreateSelectionInformation(&pGame->m_pSelectionInformation, pGame->m_TwoDigits);

   int nWidth = GetTwoDigitsWidth(pGame->m_TwoDigits);
   int nHeight = GetTwoDigitsHeight(pGame->m_TwoDigits);
   int nNumPtrs = nWidth * nHeight;
   pGame->m_apPieces = malloc(nNumPtrs*sizeof(struct Piece));
   for(int x=0; x<nWidth; x++) {
      for(int y=0; y<nHeight; y++) {
         struct Piece* pPiece = &pGame->m_apPieces[x+y*nWidth];
         CreatePiece(pPiece, x, y, pGame->m_TwoDigits, pGame->m_pMetrics, pGame->m_pSelectionInformation, pConfig);
      }
   }

   pGame->m_bShouldQuit = 0;
   pGame->m_bAnimating = 1;
   pGame->m_nAnimationStep = ANIMATE_PIECES_INOUT_STEPS;
}

void FreeGame(struct Game** ppGame)
{
   struct Game* pGame = *ppGame;

   int nWidth = GetTwoDigitsWidth(pGame->m_TwoDigits);
   int nHeight = GetTwoDigitsHeight(pGame->m_TwoDigits);
   for(int x=0; x<nWidth; x++) {
      for(int y=0; y<nHeight; y++) {
         struct Piece* pPiece = &pGame->m_apPieces[x+y*nWidth];
         FreePiece(pPiece);
      }
   }
   free(pGame->m_apPieces);

   if(pGame->m_pYouWinGraphic != NULL )
      SDL_FreeSurface(pGame->m_pYouWinGraphic);

   FreeSelectionInformation(&pGame->m_pSelectionInformation);
   FreeBackground(&pGame->m_pBackground);
   FreeMetrics(&pGame->m_pMetrics);

   FreeFont(pGame->m_pFont);
   pGame->m_pFont = NULL;

   pGame->m_pConfig = NULL;//Does not own
   pGame->m_pScreen = NULL;//Does not own

   free(pGame);
   *ppGame = NULL;
}

void DrawBoard(struct Game* pGame)
{
   SDL_FillRect(pGame->m_pScreen, NULL, SDL_MapRGB(pGame->m_pScreen->format, 255, 215, 139));

   DrawBackground(pGame->m_pBackground);

   int nWidth = GetTwoDigitsWidth(pGame->m_TwoDigits);
   int nHeight = GetTwoDigitsHeight(pGame->m_TwoDigits);

   //Draw pieces
   for(int x=0; x<nWidth; x++) {
      for(int y=0; y<nHeight; y++) {
         struct Piece* pPiece = &pGame->m_apPieces[x+y*nWidth];
         PieceDraw(pPiece, pGame->m_pScreen);
      }
   }

   char buffer[8];

   SDL_Rect rectTallyBox;
   rectTallyBox.x = 0;
   rectTallyBox.y = GetTallyBoxTop(pGame->m_pMetrics);
   rectTallyBox.w = GetTallyBoxWidth(pGame->m_pMetrics);
   rectTallyBox.h = GetTallyBoxHeight(pGame->m_pMetrics);
   SDL_FillRect(pGame->m_pScreen, &rectTallyBox, SDL_MapRGB(pGame->m_pScreen->format, 0, 255, 0));
   IntToA(buffer, sizeof(buffer), GetTwoDigitsLeftMarkedTotal(pGame->m_TwoDigits));
   DrawText(pGame->m_pScreen, pGame->m_pFont, rectTallyBox.x + rectTallyBox.w/2, rectTallyBox.y + rectTallyBox.h/2, buffer, 0, 0, 0);

   rectTallyBox.x = GetRightTallyBoxLeft(pGame->m_pMetrics);
   SDL_FillRect(pGame->m_pScreen, &rectTallyBox, SDL_MapRGB(pGame->m_pScreen->format, 0, 255, 0));
   IntToA(buffer, sizeof(buffer), GetTwoDigitsRightMarkedTotal(pGame->m_TwoDigits));
   DrawText(pGame->m_pScreen, pGame->m_pFont, rectTallyBox.x + rectTallyBox.w/2, rectTallyBox.y + rectTallyBox.h/2, buffer, 0, 0, 0);

   if( pGame->m_bWon == 1 && pGame->m_pYouWinGraphic != NULL ) {
      SDL_Rect rectYouWin;
      rectYouWin.x = (SCREEN_WIDTH - pGame->m_pYouWinGraphic->w)/2;
      rectYouWin.y = (SCREEN_HEIGHT - pGame->m_pYouWinGraphic->h)/2;
      rectYouWin.w = pGame->m_pYouWinGraphic->w;
      rectYouWin.h = pGame->m_pYouWinGraphic->h;
      SDL_BlitSurface(pGame->m_pYouWinGraphic, NULL, pGame->m_pScreen, &rectYouWin);
   }
   
   SDL_UpdateRect(pGame->m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void UpdateGameWon(struct Game* pGame)
{
   if (pGame->m_bWon != 1)
      return;

   if( pGame->m_nLevelNum > 0 && pGame->m_nLevelNum <= 250 ) {
#ifdef _TINSPIRE
      SetBeatLevel(pGame->m_pConfig, pGame->m_nLevelNum-1/*To 0-base*/, 1);
#endif
   }

   pGame->m_bAnimating = 1;
}

void Undo(struct Game* pGame)
{
   if( pGame->m_bWon )
      return;

   TwoDigitsUndo(pGame->m_TwoDigits);
}

void Redo(struct Game* pGame)
{
   if( pGame->m_bWon )
      return;

   TwoDigitsRedo(pGame->m_TwoDigits);
}

int GamePollEvents(struct Game* pGame)
{
   SDL_Event event;
   while( SDL_PollEvent( &event ) ) {
      switch( event.type ) {
         case SDL_KEYDOWN:
            switch( event.key.keysym.sym) {
              case SDLK_ESCAPE:
                  return 0;
                  break;

               case SDLK_UP:
		  if( pGame->m_bWon != 1 ) {
                     Move(pGame->m_pSelectionInformation, Up);
		  }
                  break;

	       case SDLK_DOWN:
		  if( pGame->m_bWon != 1 ) {
                     Move(pGame->m_pSelectionInformation, Down);
		  }
                  break;

               case SDLK_LEFT:
		  if( pGame->m_bWon != 1 ) {
                     Move(pGame->m_pSelectionInformation, Left);
		  }
                  break;

               case SDLK_RIGHT:
		  if( pGame->m_bWon != 1 ) {
                     Move(pGame->m_pSelectionInformation, Right);
		  }
		  break;
		case SDLK_PLUS:
		  Redo(pGame);
		  break;
		case SDLK_MINUS:
		  Undo(pGame);
                  break;
               case SDLK_RETURN:
                  ToggleTwoDigitsSpot(pGame->m_TwoDigits, GetCurrentX(pGame->m_pSelectionInformation), GetCurrentY(pGame->m_pSelectionInformation));
                  pGame->m_bWon = IsTwoDigitsGameOver(pGame->m_TwoDigits);
                  UpdateGameWon(pGame);
                  break;

               case SDLK_a:
                  DoSolveTwoDigits(pGame->m_TwoDigits);
                  break;

               default:
                  break;
            }
          default:
             break;
      }
   }
   return 1;
}

void HandleAnimations(struct Game* pGame)
{
   if (pGame->m_bAnimating == 0)
      return;

   if (pGame->m_nAnimationStep > 0)
   {
      SDL_Delay(DELAY_IN_ANIMATION_PIECES_STEP);
      pGame->m_nAnimationStep--;
      return;
   }
   pGame->m_nAnimationStep = ANIMATE_PIECES_INOUT_STEPS;

   int nWidth = GetTwoDigitsWidth(pGame->m_TwoDigits);
   int nHeight = GetTwoDigitsHeight(pGame->m_TwoDigits);

   int nMadeChange = 0;

   if (pGame->m_bWon)
   {
      for (int y = 0; y < nHeight; y++) {
         for (int x = 0; x < nWidth; x++) {
            struct Piece* pPiece = &pGame->m_apPieces[x + y*nWidth];
            if (IsDrawingPieceBlack(pPiece) == 0)
            {
               SetPieceBlack(pPiece, 1);
               nMadeChange = 1;
               return;
            }
         }
      }
   }
   else
   {
      for (int y = 0; y < nHeight; y++) {
         for (int x = 0; x < nWidth; x++) {
            struct Piece* pPiece = &pGame->m_apPieces[x + y*nWidth];
            if (IsDrawingPieceBlack(pPiece) == 1)
            {
               SetPieceBlack(pPiece, 0);
               nMadeChange = 1;
               return;
            }
         }
      }
   }

   if (nMadeChange == 0)
   {
      pGame->m_bAnimating = 0;
   }
}

int GameLoop(struct Game* pGame)
{
   if( GamePollEvents(pGame) == 0 )
      return GAME_QUIT;

   HandleAnimations(pGame);

   DrawBoard(pGame);

   SDL_Delay(30);

   if (pGame->m_bWon == 1 && pGame->m_bAnimating == 0)
      return GAME_NEXTLEVEL;

   return GAME_LOOPING;
}

int GameShouldQuit(struct Game* pGame)
{
   return pGame->m_bShouldQuit;
}

