#include <curses.h>
#include <curses.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "interface.h"
#include "game.h"
#include "config.h"

bool loadData(User *user);
bool saveData(User *user);
void initializeCurses(void);
void initializeColors(void);
void mainMenu(User *user);
void userMenu(User *user);

int main(void) {
  User user;

  setlocale(LC_CTYPE, "");

  initializeCurses();
  initializeColors();

  if (LINES < 24 || COLS < 80) {
    printCenterMessage("Janela muito pequena! Tamanho esperado: 80x24", stdscr);
    getch();
    endwin();
    return 1;
  }

  initializeGameElements();

  if (!loadData(&user)) {
    getUserName(&user);
    user.points = 0;
  }

  mainMenu(&user);

  if (!saveData(&user)) {
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
  init_color(COLOR_RED, 700, 0, 0);
  init_color(COLOR_GREEN, 0, 700, 0);
  init_color(COLOR_BLUE, 0, 1000, 1000);
  init_pair(GmBlack, COLOR_BLACK, COLOR_BLACK);
  init_pair(GmWhite, COLOR_WHITE, COLOR_BLACK);
  init_pair(GmRed, COLOR_RED, COLOR_BLACK);
  init_pair(GmGreen, COLOR_GREEN, COLOR_BLACK);
  init_pair(GmBlue, COLOR_BLUE, COLOR_BLACK);
}

bool loadData(User *user) {
  FILE *file = fopen("data", "rb");
  if (!file) {
    return false;
  }
  fgets(user->name, 31, file);
  user->name[strlen(user->name) - 1] = '\0';
  fscanf(file, "%" SCNu32, &user->points);
  fclose(file);
  return true;
}

bool saveData(User *user) {
  FILE *file = fopen("data", "wb");
  if (!file) {
    return false;
  }
  fprintf(file, "%s\n%" PRIu32, user->name, user->points);
  fclose(file);
  return true;
}

void mainMenu(User *user) {
  int choice;
  while (true) {
    choice = interfaceMenu("Snake", 3, "Iniciar", "Pontuação", "Sair");
    switch (choice) {
    case 1:
      startGame(user);
      break;
    case 2:
      userMenu(user);
      break;
    case 3:
      return;
    }
  }
}

void userMenu(User *user) {
  char buffer[40];
  sprintf(buffer, "%s: %" PRIu32 " Pontos", user->name, user->points);
  interfaceString("Pontuação", 1, buffer); 
}
