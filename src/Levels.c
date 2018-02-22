#include "Levels.h"
#include "Replacements.h"

#define LEVEL(x) case x: StringCopy(pstr, 2048, LEVEL_ ## x ); break;

void LevelLoad(char* pstr, int nLevelNum)
{
   switch( nLevelNum ) {
      LEVEL(1);
      LEVEL(2);
      LEVEL(3);
      LEVEL(4);
      LEVEL(5);
      LEVEL(6);
      LEVEL(7);
      LEVEL(8);
      LEVEL(9);
      LEVEL(10);
      LEVEL(11);
      LEVEL(12);
      LEVEL(13);
      LEVEL(14);
      LEVEL(15);
      LEVEL(16);
      LEVEL(17);
      LEVEL(18);
      LEVEL(19);
   }
}

