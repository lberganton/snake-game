#include "config.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>

void printCenterMessage(const char *msg, WINDOW *window) {
  wattrset(window, A_REVERSE);
  mvwprintw(window, getmaxy(window) / 2, (getmaxx(window) - strlen(msg)) / 2, "%s", msg);
}

void getUserName(User *user) {
  char buffer[32];
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
    else if (isalpha(ch) && pos < 32) {
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
