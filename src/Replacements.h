#ifndef REPLACEMENTS_H
#define REPLACEMENTS_H

#ifndef _TINSPIRE
#define SCREEN_WIDTH	(320)
#define SCREEN_HEIGHT	(240)
#endif

#define SCREEN_BIT_DEPTH        (16)

void IntToA(char* pstrBuffer, int bufferSize, int n);

void StringCopy(char* pstrOutput, int bufferSize, char* pstrInput);

void StringAppend(char* pstrBuffer, int nSize, char* pstrAddition);

void StringFmt(char* pstrBuffer, int nSize, char* pstrFormat, ...);

#endif
