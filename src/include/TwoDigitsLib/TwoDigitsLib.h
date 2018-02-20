#ifndef TWODIGITSLIB_H_INCLUDED
#define TWODIGITSLIB_H_INCLUDED

typedef void* TwoDigitsLib;

#define TWODIGITSLIB_OK			(0)
#define TWODIGITSLIB_BADARGUMENT		(-1)
#define TWODIGITSLIB_OUT_OF_MEMORY		(-2)
#define TWODIGITSLIB_NOT_PUZZLE_FILE	(-3)
#define TWODIGITSLIB_UNABLE			(-4)

#define TWODIGITSLIB_GAMEOVER		(1)
#define TWODIGITSLIB_NOT_GAMEOVER		(0)

#define TWODIGITSLIB_CANNOT_UNDO (1)
#define TWODIGITSLIB_CANNOT_REDO (1)

enum TwoDigitMarking
{
   NotMarked,
   LeftMarked,
   RightMarked
};

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int TwoDigitsLibCreate(TwoDigitsLib* api, const char* pstrFile);
int TwoDigitsLibFree(TwoDigitsLib* api);

int GetTwoDigitsLibError(TwoDigitsLib api);
void ClearTwoDigitsLibError(TwoDigitsLib api);

//////////////////////////////////////////////
//TwoDigitsLib related functions
//////////////////////////////////////////////
int GetTwoDigitsWidth(TwoDigitsLib api);
int GetTwoDigitsHeight(TwoDigitsLib api);
int IsTwoDigitsGameOver(TwoDigitsLib api);

int TwoDigitsUndo(TwoDigitsLib api);
int TwoDigitsRedo(TwoDigitsLib api);

int GetTwoDigitsSpotValue(TwoDigitsLib api, int x, int y);
int ToggleTwoDigitsSpot(TwoDigitsLib api, int x, int y);
int GetTwoDigitMarking(TwoDigitsLib api, int x, int y, enum TwoDigitMarking* pMarking);

int GetTwoDigitsLeftMarkedTotal(TwoDigitsLib api);
int GetTwoDigitsRightMarkedTotal(TwoDigitsLib api);

int DoSolveTwoDigits(TwoDigitsLib api);

#endif
