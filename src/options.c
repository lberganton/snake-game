#include "options.h"
#include "config.h"
#include "interface.h"
#include <curses.h>
#include <stdbool.h>
#include <string.h>

extern Profile profile;

void paintOptionsMenu(WINDOW *header, WINDOW *info, WINDOW *options, WINDOW *save, WINDOW *load, WINDOW *back) {
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

int optionsGetInput(WINDOW *window) {
  while (true) {
    switch (wgetch(window)) {
    case 'W':
    case 'w':
    case KEY_UP:
      return KEY_UP;
    case 'S':
    case 's':
    case KEY_DOWN:
      return KEY_DOWN;
    case 'A':
    case 'a':
    case KEY_LEFT:
      return KEY_LEFT;
    case 'D':
    case 'd':
    case KEY_RIGHT:
      return KEY_RIGHT;
    case KEY_RESIZE:
      return KEY_RESIZE;
    }
  }
}

static int optionsSelectedMono(WINDOW *window) {
  wmove(window, 1, 1);
  wchgat(window, OPTIONS_MENU_WIDTH - 2, A_REVERSE, 0, NULL);

  int input = optionsGetInput(window);
  
  wmove(window, 1, 1);
  wchgat(window, OPTIONS_MENU_WIDTH - 2, A_NORMAL, 0, NULL);
  
  return input;
}

void options(void) {
  int pos = 8;
  int input;
  bool resize;

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

    resize = false;

    while (pos > 0 && input != KEY_RESIZE) {
      switch (pos) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
        input = optionsSelectedMono(save);
        if (input == KEY_UP) {
          pos = 6;
        }
        if (input == KEY_DOWN) {
          pos = 8;
        }
        if (input == '\n') {
          saveData(profile.name);
          printCenterMessage("Perfil Salvo!", stdscr);
          
        }
        break;
      case 7:
        input = optionsSelectedMono(load);
        if (input == KEY_UP) {
          pos = 6;
        }
        if (input == KEY_DOWN) {
          pos = 8;
        }
        if (input == '\n') {
          //saveData(profile.name);
        }
        break;
      case 8:
        input = optionsSelectedMono(back);
        if (input == KEY_UP) {
          pos = 7;
        }
        if (input == KEY_DOWN) {
          pos = 1;
        }
        if (input == '\n') {
          pos = 0;
        }
      }
    }

    wclear(header);
    wclear(info);
    wclear(options);
    wclear(save);
    wclear(load);
    wclear(back);
    
    wrefresh(header);
    wrefresh(info);
    wrefresh(options);
    wrefresh(save);
    wrefresh(load);
    wrefresh(back);

    delwin(header);
    delwin(info);
    delwin(options);
    delwin(save);
    delwin(load);
    delwin(back);
  } while (resize);
}
