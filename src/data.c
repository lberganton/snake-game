/*****************************************************************************
 * File: data.c
 * Author: Lucas Berganton
 * Created: 04/01/2024
 * Description: Implements functions to manipulated the data files.
 *****************************************************************************/
#include "config.h"
#include <stdio.h>

extern Profile profile;

bool loadData(char *fileName) {
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    return false;
  }
  fread(&profile, sizeof(Profile), 1, file);
  fclose(file);
  return true;
}

bool saveData(char *fileName) {
  FILE *file = fopen(fileName, "wb");
  if (!file) {
    return false;
  }
  fwrite(&profile, sizeof(Profile), 1, file);
  fclose(file);
  return true;
}
