#include <curses.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "config.h"
#include "interface.h"
#include "options.h"
#include "game.h"

Profile profile;

void initializeCurses(void);
void initializeColors(void);
void mainMenu(void);

int main(void) {
  setlocale(LC_CTYPE, "");

  initializeCurses();

  if (!has_colors()) {
    endwin();
    printf("Snake: Terminal não suporta cores.\n");
    return 1;
  }

  initializeColors();

  if (!loadData(DEFAULT_DATA_FILE_NAME)) {
    interfaceGetStringInput("Digite seu nome", profile.name, NAME_LEN);

    profile.bestScore = 0;
    profile.mostRecentScore = 0;

    profile.height = DEFAULT_HEIGHT;
    profile.width = DEFAULT_WIDTH;

    profile.colorFood = RED;
    profile.colorSnakeHead = GREEN;
    profile.colorSnakeBody = WHITE;
  }

  mainMenu();

  if (!saveData(DEFAULT_DATA_FILE_NAME)) {
    refresh();
    printCenterMessage("Falha no salvamento!", stdscr);
    getch();
  }

  endwin();

  return 0;
}

void initializeCurses(void) {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
  curs_set(false); 
}

void initializeColors(void) {
  start_color();

  if (can_change_color()) {
    init_color(COLOR_BLACK, 128, 128, 128);
    init_color(COLOR_RED, 700, 0, 0);
    init_color(COLOR_GREEN, 0, 700, 0);
    init_color(COLOR_BLUE, 0, 1000, 1000);
  }

  init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
  init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
  init_pair(RED, COLOR_RED, COLOR_BLACK);
  init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
}

void mainMenu(void) {
  int choice;

  while (true) {
    choice = interfaceMenu("Snake", 3, "Iniciar", "Opções", "Sair");
    
    switch (choice) {
    case 1:
      if (LINES < profile.height + 5 || COLS < profile.width + 2) {
        handleWithSmallTermSize(profile.height + 5, profile.width + 2);
      }
      startGame();
      break;
    case 2:
      optionsMenu();
      break;
    case 3:
      return;
    }
  }
}
