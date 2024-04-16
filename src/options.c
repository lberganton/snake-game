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
  mvwprintw(options, 1, OPTIONS_MENU_WIDTH - 5, "%3d", profile.width);
  mvwprintw(options, 2, 1, "Altura:");
  mvwprintw(options, 2, OPTIONS_MENU_WIDTH - 5, "%3d", profile.height);

  mvwprintw(options, 3, 1, "Comida:");
  wattrset(options, COLOR_PAIR(profile.colorFood));
  mvwaddch(options, 3, OPTIONS_MENU_WIDTH - 4, GRAPHIC_FOOD);
  wattrset(options, A_NORMAL);

  mvwprintw(options, 4, 1, "Cabeça:");
  wattrset(options, COLOR_PAIR(profile.colorSnakeHead));
  mvwaddch(options, 4, OPTIONS_MENU_WIDTH - 4, GRAPHIC_SNAKE_HEAD);
  wattrset(options, A_NORMAL);

  mvwprintw(options, 5, 1, "Corpo:");
  wattrset(options, COLOR_PAIR(profile.colorSnakeBody));
  mvwaddch(options, 5, OPTIONS_MENU_WIDTH - 4, GRAPHIC_SNAKE_BODY);
  wattrset(options, A_NORMAL);

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
    int input = wgetch(window);

    switch (input) {
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
    case '\n':
      return '\n';
    case KEY_RESIZE:
      return KEY_RESIZE;
    }
  }
}

static int optionsSelectedMono(WINDOW *window) {
  wmove(window, 1, 1);
  wchgat(window, OPTIONS_MENU_WIDTH - 2, A_REVERSE, 0, NULL);
  wrefresh(window);

  int input = optionsGetInput(window);
  
  wmove(window, 1, 1);
  wchgat(window, OPTIONS_MENU_WIDTH - 2, A_NORMAL, 0, NULL);
  wrefresh(window);
  
  return input;
}

static int optionsSelectedNumber(WINDOW *window, size_t y, int min, int max, int *value) {
  int input;

  do {
    wmove(window, y, 1);
    wchgat(window, OPTIONS_MENU_WIDTH - 6, A_REVERSE, 0, NULL);

    mvwprintw(window, y, OPTIONS_MENU_WIDTH - 5, "%3d", *value);

    wrefresh(window);

    input = optionsGetInput(window);

    if (input == KEY_LEFT || input == KEY_RIGHT) {
      int arrow;
      
      if (input == KEY_LEFT) {
        arrow = ACS_LARROW;
        if (*value == min) {
          *value = max;
        }
        else {
          (*value)--;
        }
      }

      if (input == KEY_RIGHT) {
        arrow = ACS_RARROW;
        if (*value == max) {
          *value = min; 
        }
        else {
          (*value)++;
        }
      }
      
      wmove(window, y, 1);
      wchgat(window, OPTIONS_MENU_WIDTH - 6, A_NORMAL, 0, NULL);

      mvwprintw(window, y, OPTIONS_MENU_WIDTH - 5, "%3d", *value);

      wattrset(window, A_NORMAL);
      mvwaddch(window, y, OPTIONS_MENU_WIDTH - 2, arrow);
      wrefresh(window);

      mvwaddch(window, y, OPTIONS_MENU_WIDTH - 2, ' ');
    }
  } while (input == KEY_LEFT || input == KEY_RIGHT);

  wmove(window, y, 1);
  wchgat(window, OPTIONS_MENU_WIDTH - 6, A_NORMAL, 0, NULL);
  wrefresh(window);

  return input;
}

static int optionsSelectedGameElement(WINDOW *window, size_t y, int graphic, int *color) {
  int input;

  do {
    wmove(window, y, 1);
    wchgat(window, OPTIONS_MENU_WIDTH - 6, A_REVERSE, 0, NULL);

    wattrset(window, COLOR_PAIR(*color));
    mvwaddch(window, y, OPTIONS_MENU_WIDTH - 4, graphic);
    wattrset(window, A_NORMAL);

    wrefresh(window);

    input = optionsGetInput(window);

    if (input == KEY_LEFT || input == KEY_RIGHT) {
      int arrow;
      
      if (input == KEY_LEFT) {
        arrow = ACS_LARROW;
        if (*color == BLACK + 1) {
          *color = WHITE;
        }
        else {
          (*color)--;
        }
      }

      if (input == KEY_RIGHT) {
        arrow = ACS_RARROW;
        if (*color == WHITE) {
          *color = BLACK + 1; 
        }
        else {
          (*color)++;
        }
      }
      
      wmove(window, y, 1);
      wchgat(window, OPTIONS_MENU_WIDTH - 6, A_NORMAL, 0, NULL);

      wattrset(window, COLOR_PAIR(*color));
      mvwaddch(window, y, OPTIONS_MENU_WIDTH - 4, graphic);

      wattrset(window, A_NORMAL);
      mvwaddch(window, y, OPTIONS_MENU_WIDTH - 2, arrow);
      wrefresh(window);

      mvwaddch(window, y, OPTIONS_MENU_WIDTH - 2, ' ');
    }
  } while (input == KEY_LEFT || input == KEY_RIGHT);

  wmove(window, y, 1);
  wchgat(window, OPTIONS_MENU_WIDTH - 6, A_NORMAL, 0, NULL);
  wrefresh(window);

  return input;
}

static bool optionsLoadProfile(void) {
  char buffer[NAME_LEN];
  interfaceGetStringInput("Digite o perfil", buffer, NAME_LEN);
  return loadData(buffer);
}

void optionsMenu(void) {
  int pos = 8;
  int input;
  bool keep;

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

    keypad(options, true);
    keypad(save, true);
    keypad(load, true);
    keypad(back, true);

    paintOptionsMenu(header, info, options, save, load, back);

    keep = true;

    do {
      switch (pos) {
      case 1:
        input = optionsSelectedNumber(options, 1, MIN_WIDTH, MAX_WIDTH, &profile.width);
        if (input == KEY_UP) {
          pos = 8;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        break;
      case 2:
        input = optionsSelectedNumber(options, 2, MIN_HEIGHT, MAX_HEIGHT, &profile.height);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        break;
      case 3:
        input = optionsSelectedGameElement(options, 3, GRAPHIC_FOOD, &profile.colorFood);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        break;
      case 4:
        input = optionsSelectedGameElement(options, 4, GRAPHIC_SNAKE_HEAD, &profile.colorSnakeHead);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        break;
      case 5:
        input = optionsSelectedGameElement(options, 5, GRAPHIC_SNAKE_BODY, &profile.colorSnakeBody);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        break;
      case 6:
        input = optionsSelectedMono(save);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        if (input == '\n') {
          saveData(profile.name);
          wclear(stdscr);
          printCenterMessage("Perfil Salvo!", stdscr);
          wgetch(stdscr);
        }
        break;
      case 7:
        input = optionsSelectedMono(load);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos++;
        }
        if (input == '\n') {
          wclear(stdscr);
          if (optionsLoadProfile()) {
            printCenterMessage("Perfil carregado!", stdscr);
          }
          else {
            printCenterMessage("Falha ao carregar perfil!", stdscr);
          }
          wgetch(stdscr);
        }
        break;
      case 8:
        input = optionsSelectedMono(back);
        if (input == KEY_UP) {
          pos--;
        }
        if (input == KEY_DOWN) {
          pos = 1;
        }
        if (input == '\n') {
          keep = false;
        }
      }
    } while (input != '\n' && input != KEY_RESIZE);

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
  } while (keep);
}
