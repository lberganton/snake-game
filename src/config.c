#include "config.h"
#include <curses.h>
#include <inttypes.h>

bool loadData(User *user) {
  FILE *file = fopen("data", "rb");
  if (!file) {
    return false;
  }
  fread(user->name, sizeof(char), NAME_LEN, file);
  fread(&user->mostRecentScore, sizeof(uint32_t), 1, file);
  fread(&user->bestScore, sizeof(uint32_t), 1, file);
  fclose(file);
  return true;
}

bool saveData(User *user) {
  FILE *file = fopen("data", "wb");
  if (!file) {
    return false;
  }
  fwrite(user->name, sizeof(char), NAME_LEN, file);
  fwrite(&user->mostRecentScore, sizeof(uint32_t), 1, file);
  fwrite(&user->bestScore, sizeof(uint32_t), 1, file);
  fclose(file);
  return true;
}
