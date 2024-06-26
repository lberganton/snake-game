/*****************************************************************************
 * File: config.h
 * Author: Lucas Berganton
 * Created: 04/01/2024
 * Description: Provides the definition of 'profile' struct and some useful
 * macros that configures some of the technical elements.
 *****************************************************************************/
#pragma once

#include <curses.h>
#include <inttypes.h>

#define DEFAULT_DATA_FILE_NAME "data"
#define DATA_FILE_EXTENSION ".data"
#define NAME_LEN 15

#define GAME_MAX_DELAY 100000
#define GAME_MIN_DELAY 50000
#define GAME_SPEED_INCREASE 25

#define DEFAULT_HEIGHT 19
#define DEFAULT_WIDTH 78

#define MAX_HEIGHT 25
#define MAX_WIDTH 130
#define MIN_HEIGHT 10
#define MIN_WIDTH 77

#define GRAPHIC_VOID ' '
#define GRAPHIC_FOOD ACS_DIAMOND
#define GRAPHIC_SNAKE_HEAD ACS_BOARD
#define GRAPHIC_SNAKE_BODY ACS_BOARD

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

typedef struct Profile {
  char name[NAME_LEN];
  uint32_t mostRecentScore;
  uint32_t bestScore;

  int width;
  int height;

  int colorFood;
  int colorSnakeHead;
  int colorSnakeBody;
} Profile;

bool loadData(char *fileName);
bool saveData(char *fileName);
