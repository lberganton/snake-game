#include "config.h"
#include "interface.h"
#include <curses.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
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

void getUserName(User *user) {
  char buffer[NAME_LEN];
  int ch;
  int pos = 0;

  printCenterMessage("Digite seu nome", stdscr);

  attrset(A_NORMAL);
  move(LINES / 2 + 1, COLS / 2 - 8);

  curs_set(true);

  while ((ch = getch()) != '\n' || pos == 0) {
    if (ch == KEY_BACKSPACE && pos > 0) {
      move(getcury(stdscr), getcurx(stdscr) - 1);
      delch();
      pos--;
    }
    else if (isalpha(ch) && pos < NAME_LEN) {
      buffer[pos] = ch;
      addch(ch);
      pos++;
    }
  }

  strncpy(user->name, buffer, pos);
  user->name[pos] = '\0';
  curs_set(false);
  clear();
}
