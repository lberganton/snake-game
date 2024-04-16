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

void getUserName(void) {
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

  strncpy(profile.name, buffer, pos);
  profile.name[pos] = '\0';
  curs_set(false);
  clear();
}

static void paintOptionsMenu(WINDOW *header, WINDOW *info, WINDOW *options, WINDOW *save, WINDOW *load, WINDOW *back) {
  char *title = "Opções";
  char *saveString = "Salvar Perfil";
  char *loadString = "Carregar Perfil";
  char *backString = "Voltar";

  wborder(header, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_ULCORNER, ACS_URCORNER, ACS_VLINE, ACS_VLINE);
  wborder(info, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  wborder(options, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  wborder(save, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  wborder(load, ACS_VLINE, ACS_VLINE, ACS_HLINE, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  wborder(back, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);

  wmove(header, 1, (OPTIONS_MENU_WIDTH - strlen(title)) / 2);
  wprintw(header, "%s", title);

  mvwprintw(info, 1, 1, "Perfil: %s", profile.name);
  mvwprintw(info, 2, 1, "   * Pontuação Recente: %d", profile.mostRecentScore);
  mvwprintw(info, 3, 1, "   * Melhor Pontuação: %d", profile.bestScore);

  mvwprintw(options, 1, 1, "Largura:");
  mvwprintw(options, 1, OPTIONS_MENU_WIDTH - 4, "%-3d", profile.width);
  mvwprintw(options, 2, 1, "Altura:");
  mvwprintw(options, 2, OPTIONS_MENU_WIDTH - 4, "%-3d", profile.height);
  mvwprintw(options, 3, 1, "Comida:");
  mvwprintw(options, 4, 1, "Cabeça:");
  mvwprintw(options, 5, 1, "Corpo:");

  wmove(save, 1, (OPTIONS_MENU_WIDTH - strlen(saveString)) / 2);
  wprintw(save, "%s", saveString);

  wmove(load, 1, (OPTIONS_MENU_WIDTH - strlen(loadString)) / 2);
  wprintw(load, "%s", loadString);

  wmove(back, 1, (OPTIONS_MENU_WIDTH - strlen(backString)) / 2);
  wprintw(back, "%s", backString);

  wrefresh(header);
  wrefresh(info);
  wrefresh(options);
  wrefresh(save);
  wrefresh(load);
  wrefresh(back);
}

void interfaceOptions(void) {
  int input;

  do {
    if (LINES < 19 || COLS < OPTIONS_MENU_WIDTH) {
      handleWithSmallTermSize(19, OPTIONS_MENU_WIDTH);
    }

    WINDOW *header = newwin(2, OPTIONS_MENU_WIDTH, (LINES - 19) / 2, (COLS - OPTIONS_MENU_WIDTH) / 2);
    WINDOW *info = newwin(4, OPTIONS_MENU_WIDTH, getbegy(header) + 2, (COLS - OPTIONS_MENU_WIDTH) / 2);
    WINDOW *options = newwin(6, OPTIONS_MENU_WIDTH, getbegy(info) + 4, (COLS - OPTIONS_MENU_WIDTH) / 2);
    WINDOW *save = newwin(2, OPTIONS_MENU_WIDTH, getbegy(options) + 6, (COLS - OPTIONS_MENU_WIDTH) / 2);
    WINDOW *load = newwin(2, OPTIONS_MENU_WIDTH, getbegy(save) + 2, (COLS - OPTIONS_MENU_WIDTH) / 2);
    WINDOW *back = newwin(3, OPTIONS_MENU_WIDTH, getbegy(load) + 2, (COLS - OPTIONS_MENU_WIDTH) / 2);

    paintOptionsMenu(header, info, options, save, load, back);

    int pos = 8;

    wgetch(options);
    pos = 2;
    if (pos >= 1 && pos <= 5) {
      wmove(options, pos, OPTIONS_MENU_WIDTH - 6);
      wchgat(options, 5, A_REVERSE, 0, NULL);
      //wchgat(back, OPTIONS_MENU_WIDTH - 2, A_REVERSE, 0, NULL);
    }

    wmove(back, 1, 1);
    wchgat(back, OPTIONS_MENU_WIDTH - 2, A_REVERSE, 0, NULL);

    while ((input = wgetch(options)) != '\n' && input != KEY_RESIZE) {
      if (pos >= 1 && pos <= 5) {
        wmove(options, pos, OPTIONS_MENU_WIDTH - 7);
        wchgat(options, 6, A_REVERSE, 0, NULL);
        //wchgat(back, OPTIONS_MENU_WIDTH - 2, A_REVERSE, 0, NULL);
      }
      switch (input) {
      case 'w':
      case 'W':
      case KEY_UP:
        
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
      }
    }
    
    wrefresh(header);
    wrefresh(info);
    wrefresh(options);
    wrefresh(save);
    wrefresh(load);
    wrefresh(back);
    wgetch(header);

    /*
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

    va_end(args);*/
  } while (input == KEY_RESIZE);
}
