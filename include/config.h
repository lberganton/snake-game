#pragma once

#include <curses.h>
#include <inttypes.h>

#define FILE_DATA "data"
#define NAME_LEN 15

#define MSG_YOU_LOSE            "VOCÊ PERDEU!"
#define MSG_ALLOCATION_FAILED   "FALHA NA ALOCAÇÃO DE MEMÓRIA!"

#define GAME_MAX_DELAY 100000
#define GAME_MIN_DELAY 50000
#define GAME_SPEED_INCREASE 25

#define DEFAULT_HEIGHT 19
#define DEFAULT_WIDTH 78

#define MAX_HEIGHT 25
#define MAX_WIDTH 130

#define MENU_WIDTH 30

#define GRAPHIC_VOID ' '
#define GRAPHIC_FOOD ACS_DIAMOND
#define GRAPHIC_SNAKE_HEAD ACS_BOARD
#define GRAPHIC_SNAKE_BODY ACS_BOARD

typedef enum Colors {
  WHITE,
  BLACK,
  RED,
  GREEN,
  BLUE
} Colors;

typedef struct Profile {
  char name[NAME_LEN];
  uint32_t mostRecentScore;
  uint32_t bestScore;

  int width;
  int height;

  int attributesFood;
  int attributesSnakeHead;
  int attributesSnakeBody;
} Profile;

bool loadData(void);
bool saveData(void);
void options(void);
