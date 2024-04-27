/*****************************************************************************
 * File: data.c
 * Author: Lucas Berganton
 * Created: 04/01/2024
 * Description: Implements functions to manipulated the data files.
 *****************************************************************************/
#include "config.h"
#include <stdio.h>
#include <string.h>

extern Profile profile;

bool loadData(char *fileName) {
  char buffer[32];

  strncpy(buffer, fileName, strlen(fileName) + 1);
  strncat(buffer, DATA_FILE_EXTENSION, strlen(DATA_FILE_EXTENSION) + 1);

  FILE *file = fopen(buffer, "rb");
  if (!file) {
    return false;
  }

  fread(&profile, sizeof(Profile), 1, file);
  fclose(file);

  return true;
}

#include <curses.h>
#include <stdlib.h>

bool saveData(char *fileName) {
  char buffer[32];

  strncpy(buffer, fileName, strlen(fileName) + 1);
  strncat(buffer, DATA_FILE_EXTENSION, strlen(DATA_FILE_EXTENSION) + 1);
  
  FILE *file = fopen(buffer, "wb");
  if (!file) {
    return false;
  }

  fwrite(&profile, sizeof(Profile), 1, file);
  fclose(file);

  return true;
}
