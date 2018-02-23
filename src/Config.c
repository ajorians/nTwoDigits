#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "Config.h"
#include "Replacements.h"

void CreateConfig(struct Config** ppConfig)
{
   *ppConfig = malloc(sizeof(struct Config));

   struct Config* pConfig = (*ppConfig);
   for(int nLevel = 0; nLevel<(int)(sizeof(pConfig->m_nBeatLevels)/sizeof(pConfig->m_nBeatLevels[0])); nLevel++)
      pConfig->m_nBeatLevels[nLevel] = 0;

   pConfig->m_nDrawBackground = 1;
   pConfig->m_nLastLevel = 0;

#ifndef _WIN32
   pConfig->m_Archive = NULL;
   ArchiveCreate(&pConfig->m_Archive);

   OpenArchiveFile(pConfig->m_Archive, "TwoDigitsSettings.tns");
   int nSettings = GetNumberArchives(pConfig->m_Archive, "Settings");
   char strName[MAX_NAME_LENGTH];
   for(int i=0; i<nSettings; i++) {
      StringCopy(strName, MAX_NAME_LENGTH, GetName(pConfig->m_Archive, "Settings", i));

      char buffer[16];

      for(int nLevel = 0; nLevel<(int)(sizeof(pConfig->m_nBeatLevels)/sizeof(pConfig->m_nBeatLevels[0])); nLevel++) {
         StringFmt(buffer, sizeof(buffer), "Level%d", nLevel);

         if( strcmp(strName, buffer) == 0 ) {
            pConfig->m_nBeatLevels[nLevel] = atoi( GetValue(pConfig->m_Archive, "Settings", i) );
            break;
         }
      }

      StringCopy(buffer, sizeof(buffer), "DrawBkg");
      if( strcmp(strName, buffer) == 0 ) {
         pConfig->m_nDrawBackground = atoi(GetValue(pConfig->m_Archive, "Settings", i));
      }

      StringCopy(buffer, sizeof(buffer), "LastLevel");
      if( strcmp(strName, buffer) == 0 ) {
         pConfig->m_nLastLevel = atoi( GetValue(pConfig->m_Archive, "Settings", i) );
      }
   }
#endif
}

void FreeConfig(struct Config** ppConfig)
{
   char buffer[16];
   char bufferName[16];
   struct Config* pConfig = *ppConfig;
#ifndef _WIN32
   ArchiveSetBatchMode(pConfig->m_Archive, ARCHIVE_ENABLE_BATCH);
   for(int nLevel=0; nLevel<(int)(sizeof(pConfig->m_nBeatLevels)/sizeof(pConfig->m_nBeatLevels[0])); nLevel++) {
      sprintf(buffer, "%d", pConfig->m_nBeatLevels[nLevel]);
      sprintf(bufferName, "Level%d", nLevel);
      UpdateArchiveEntry(pConfig->m_Archive, "Settings", bufferName, buffer, NULL);
   }

   sprintf(buffer, "%d", pConfig->m_nDrawBackground);
   strcpy(bufferName, "DrawBkg");
   UpdateArchiveEntry(pConfig->m_Archive, "Settings", bufferName, buffer, NULL);

   sprintf(buffer, "%d", pConfig->m_nLastLevel);
   strcpy(bufferName, "LastLevel");
   UpdateArchiveEntry(pConfig->m_Archive, "Settings", bufferName, buffer, NULL);

   ArchiveSetBatchMode(pConfig->m_Archive, ARCHIVE_DISABLE_BATCH);

   ArchiveFree(&pConfig->m_Archive);
#endif

   free(*ppConfig);
   *ppConfig = NULL;
}

void SetBeatLevel(struct Config* pConfig, int nLevelNum, int nBeat)
{
   if( nLevelNum < 0 || nLevelNum >= (int)(sizeof(pConfig->m_nBeatLevels)/sizeof(pConfig->m_nBeatLevels[0])) )
      return;

   pConfig->m_nBeatLevels[nLevelNum] = nBeat; 
}

int GetBeatLevel(struct Config* pConfig, int nLevelNum)
{
   int nRet = 0;
   if( nLevelNum >= 0 && nLevelNum <(int)(sizeof(pConfig->m_nBeatLevels)/sizeof(pConfig->m_nBeatLevels[0])) )
      nRet = pConfig->m_nBeatLevels[nLevelNum];
   
   return nRet;
}

void SetLastLevel(struct Config* pConfig, int nLastLevelNum)
{
   pConfig->m_nLastLevel = nLastLevelNum;
}

int GetLastLevel(struct Config* pConfig)
{
   return pConfig->m_nLastLevel;
}

int GetDrawBackground(struct Config* pConfig)
{
   return pConfig->m_nDrawBackground;
}

void SetDrawBackground(struct Config* pConfig, int nOn)
{
   pConfig->m_nDrawBackground = nOn;
}

