#pragma once

#include "ncurses.h"
#include <stdarg.h>
#include <string.h>

int interfaceMenu(const char *title, const size_t n, ...) {
  static const size_t large = 30;

  va_list args;
  va_start(args, n);

  WINDOW* header = newwin(2, large, (LINES - (n + 4)) / 2, (COLS - large) / 2);
  WINDOW* options = newwin(n + 2, large, (LINES - n) / 2, (COLS - large) / 2);
  wborder(header, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_ULCORNER, ACS_URCORNER, ACS_VLINE, ACS_VLINE);
  wborder(options, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);

  wmove(header, 1, (large - strlen(title)) / 2);
  wprintw(header, "%s", title);

  for (register size_t i = 1; i <= n; i++) {
    wmove(options, i, 1);
    wprintw(options, "[%zu] - %s", i, va_arg(args, char*));
  }

  keypad(options, true);

  wrefresh(header);
  wrefresh(options);
  int pos = 1;
  int ch;

  wmove(options, pos, 1);
  wchgat(options, large - 2, A_REVERSE, 0, NULL);

  while ((ch = wgetch(options)) != '\n') {
    switch (ch) {
    case 'w':
    case 'W':
    case KEY_UP:
      wchgat(options, large - 2, A_NORMAL, 0, NULL);
      pos = (pos == 1) ? (int) n : pos - 1;
      wmove(options, pos, 1);
      wchgat(options, large - 2, A_REVERSE, 0, NULL);
      break;
    case 's':
    case 'S':
    case KEY_DOWN:
      wchgat(options, large - 2, A_NORMAL, 0, NULL);
      pos = (pos == (int) n) ? 1 : pos + 1;
      wmove(options, pos, 1);
      wchgat(options, large - 2, A_REVERSE, 0, NULL);
    }
  }

  wclear(header);
  wrefresh(header);
  delwin(header);
  wclear(options);
  wrefresh(options);
  delwin(options);

  return pos;
}

void interfaceString(const char *title, const size_t n, ...) {
  static const size_t large = 30;

  va_list args;
  va_start(args, n);

  WINDOW* header = newwin(2, large, (LINES - (n + 4)) / 2, (COLS - large) / 2);
  WINDOW* options = newwin(n + 2, large, (LINES - n) / 2, (COLS - large) / 2);
  wborder(header, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_ULCORNER, ACS_URCORNER, ACS_VLINE, ACS_VLINE);
  wborder(options, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);

  wmove(header, 1, (large - strlen(title)) / 2);
  wprintw(header, "%s", title);

  for (register size_t i = 1; i <= n; i++) {
    wmove(options, i, 1);
    wprintw(options, "%s", va_arg(args, char*));
  }

  wrefresh(header);
  wrefresh(options);

  wgetch(options);

  wclear(header);
  wrefresh(header);
  delwin(header);
  wclear(options);
  wrefresh(options);
  delwin(options);
}
