#include "config.h"
#include <curses.h>
#include <inttypes.h>

extern Profile profile;

bool loadData(void) {
  FILE *file = fopen("data", "rb");
  if (!file) {
    return false;
  }
  fread(&profile, sizeof(Profile), 1, file);
  fclose(file);
  return true;
}

bool saveData(void) {
  FILE *file = fopen("data", "wb");
  if (!file) {
    return false;
  }
  fwrite(&profile, sizeof(Profile), 1, file);
  fclose(file);
  return true;
}
