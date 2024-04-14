#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "interface.h"
#include "game.h"
#include "config.h"

GmElement ElVoid;
GmElement ElSnakeHead;
GmElement ElSnakeBody;
GmElement ElFood;

bool loadData(User *user);
bool saveData(User *user);
void initializeGameElements(void);
void initializeCurses(void);
void initializeColors(void);
void mainMenu(User *user);
void userMenu(User *user);

int main(void) {
  User user;

  setlocale(LC_CTYPE, "");

  initializeCurses();

  if (!has_colors()) {
    endwin();
    printf("Snake: Terminal não suporta cores.\n");
    return 1;
  }

  initializeColors();

  if (LINES < 24 || COLS < 80) {
    printCenterMessage("Janela muito pequena! Tamanho esperado: 80x24", stdscr);
    getch();
    endwin();
    return 1;
  }

  if (!loadData(&user)) {
    getUserName(&user);
    user.bestScore = 0;
    user.mostRecentScore = 0;
  }

  initializeGameElements();

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

  if (can_change_color()) {
    init_color(COLOR_BLACK, 128, 128, 128);
    init_color(COLOR_RED, 700, 0, 0);
    init_color(COLOR_GREEN, 0, 700, 0);
    init_color(COLOR_BLUE, 0, 1000, 1000);
  }

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
  fread(user->name, sizeof(char), 32, file);
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
  fwrite(user->name, sizeof(char), 32, file);
  fwrite(&user->mostRecentScore, sizeof(uint32_t), 1, file);
  fwrite(&user->bestScore, sizeof(uint32_t), 1, file);
  fclose(file);
  return true;
}

void initializeGameElements(void) {
  ElVoid.graphic = ' ';
  ElSnakeHead.graphic = ACS_BOARD;
  ElSnakeBody.graphic = ACS_BOARD;
  ElFood.graphic = ACS_DIAMOND;

  ElVoid.attribute = COLOR_PAIR(GmBlack);
  ElSnakeHead.attribute = COLOR_PAIR(GmBlue);
  ElSnakeBody.attribute = COLOR_PAIR(GmWhite);
  ElFood.attribute = COLOR_PAIR(GmRed);
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
  char mostRecentScore[30];
  char bestScore[30];
  snprintf(mostRecentScore, 30, "*Pontuação Recente: %" PRIu32, user->mostRecentScore);
  snprintf(bestScore, 30, "*Melhor Pontuação:  %" PRIu32, user->bestScore);
  interfaceString("Pontuação", 3, user->name, mostRecentScore, bestScore); 
}
