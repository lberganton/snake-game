#pragma once

#include <curses.h>
#include <inttypes.h>
#include "config.h"

#define Y_MAP 19
#define X_MAP 78

#define Y_PLAYER (Y_MAP - 1)
#define X_PLAYER (X_MAP / 2)

typedef enum GmColors {
  GmWhite,
  GmBlack,
  GmRed,
  GmGreen,
  GmBlue
} GmColors;

typedef struct GmElement {
  int graphic;
  int attribute;
} GmElement;

typedef struct GmMap {
  WINDOW *window;
  GmElement *matrix[Y_MAP][X_MAP];
} GmMap;

typedef struct GmFood {
  uint16_t x, y;
} GmFood;

typedef struct GmScreen {
  WINDOW *border;
  WINDOW *info;
} GmScreen;

typedef struct GmPlayer GmPlayer;

void initializeMap(GmMap *map, GmScreen *screen);
void initializeFood(GmFood *food, GmMap *map);
void startGame(User *user);
void updateFood(GmFood *food, GmMap *map, GmPlayer *player);
void updateGameScreen(const GmPlayer *player, const GmMap *map, const GmScreen *screen);
