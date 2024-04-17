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

void clearInputBuffer(WINDOW *window) {
  while (wgetch(window) != ERR);
}
