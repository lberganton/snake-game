#include <curses.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

extern Profile profile;

void printCenterMessage(const char *msg, WINDOW *window) {
  wattrset(window, A_REVERSE);
  mvwprintw(window, getmaxy(window) / 2, (getmaxx(window) - strlen(msg)) / 2, "%s", msg);
}

void handleWithSmallTermSize(int y, int x) {
  char buffer[64];

  snprintf(buffer, 64, "Redimensione para: %dx%d", x, y);

  while (LINES < y || COLS < x) {
    clear();
    printCenterMessage(buffer, stdscr);
    while (getch() != KEY_RESIZE);
  }

  clear();
  refresh();
}

int interfaceMenu(const char *title, const int n, ...) {
  int pos, input;

  do {
    if (LINES < n + 4 || COLS < MAIN_MENU_WIDTH) {
      handleWithSmallTermSize(n + 4, MAIN_MENU_WIDTH);
    }

    va_list args;
    va_start(args, n);

    WINDOW* header = newwin(2, MAIN_MENU_WIDTH, (LINES - (n + 4)) / 2, (COLS - MAIN_MENU_WIDTH) / 2);
    WINDOW* options = newwin(n + 2, MAIN_MENU_WIDTH, (LINES - n) / 2, (COLS - MAIN_MENU_WIDTH) / 2);
    wborder(header, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_ULCORNER, ACS_URCORNER, ACS_VLINE, ACS_VLINE);
    wborder(options, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);

    wmove(header, 1, (MAIN_MENU_WIDTH - strlen(title)) / 2);
    wprintw(header, "%s", title);

    for (register int i = 1; i <= n; i++) {
      wmove(options, i, 1);
      wprintw(options, "[%d] - %s", i, va_arg(args, char*));
    }

    keypad(options, true);

    wrefresh(header);
    wrefresh(options);
    pos = 1;

    wmove(options, pos, 1);
    wchgat(options, MAIN_MENU_WIDTH - 2, A_REVERSE, 0, NULL);

    while ((input = wgetch(options)) != '\n' && input != KEY_RESIZE) {
      switch (input) {
      case 'w':
      case 'W':
      case KEY_UP:
        wchgat(options, MAIN_MENU_WIDTH - 2, A_NORMAL, 0, NULL);
        pos = (pos == 1) ? (int) n : pos - 1;
        wmove(options, pos, 1);
        wchgat(options, MAIN_MENU_WIDTH - 2, A_REVERSE, 0, NULL);
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
        wchgat(options, MAIN_MENU_WIDTH - 2, A_NORMAL, 0, NULL);
        pos = (pos == (int) n) ? 1 : pos + 1;
        wmove(options, pos, 1);
        wchgat(options, MAIN_MENU_WIDTH - 2, A_REVERSE, 0, NULL);
      }
    }

    wclear(header);
    wrefresh(header);
    delwin(header);
    wclear(options);
    wrefresh(options);
    delwin(options);

    va_end(args);
  } while (input == KEY_RESIZE);

  return pos;
}

void interfaceString(const char *title, const int n, ...) {
  int input;

  do {
    if (LINES < n + 4 || COLS < MAIN_MENU_WIDTH) {
      handleWithSmallTermSize(n + 4, MAIN_MENU_WIDTH);
    }

    va_list args;
    va_start(args, n);

    WINDOW* header = newwin(2, MAIN_MENU_WIDTH, (LINES - (n + 4)) / 2, (COLS - MAIN_MENU_WIDTH) / 2);
    WINDOW* options = newwin(n + 2, MAIN_MENU_WIDTH, (LINES - n) / 2, (COLS - MAIN_MENU_WIDTH) / 2);
    wborder(header, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_ULCORNER, ACS_URCORNER, ACS_VLINE, ACS_VLINE);
    wborder(options, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);

    wmove(header, 1, (MAIN_MENU_WIDTH - strlen(title)) / 2);
    wprintw(header, "%s", title);

    for (register int i = 1; i <= n; i++) {
      wmove(options, i, 1);
      wprintw(options, "%s", va_arg(args, char*));
    }

    wrefresh(header);
    wrefresh(options);

    input = wgetch(options);

    wclear(header);
    wrefresh(header);
    delwin(header);
    wclear(options);
    wrefresh(options);
    delwin(options);
  } while (input == KEY_RESIZE);
}

void interfaceGetStringInput(char *msg, char *destiny, size_t lenght) {
  size_t pos = 0;
  int ch;

  printCenterMessage(msg, stdscr);

  attrset(A_NORMAL);
  move(LINES / 2 + 1, COLS / 2 - 8);

  curs_set(true);

  while ((ch = getch()) != '\n' || pos == 0) {
    if (ch == KEY_BACKSPACE && pos > 0) {
      move(getcury(stdscr), getcurx(stdscr) - 1);
      delch();
      pos--;
    }
    else if (isalpha(ch) && pos < lenght) {
      destiny[pos] = ch;
      addch(ch);
      pos++;
    }
  }

  destiny[pos] = '\0';
  curs_set(false);
  clear();
}
