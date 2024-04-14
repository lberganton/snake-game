#pragma once

#include <curses.h>
#include <stdbool.h>
#include "game.h"

typedef enum PlDirections {
  PlLeft,
  PlRight,
  PlUp,
  PlDown
} PlDirections;

typedef struct GmBody {
  uint16_t x, y;
  struct GmBody *next;
  struct GmBody *previous;
} GmBody;

typedef struct GmPlayer {
  uint32_t points;
  uint16_t size;
  uint16_t collected;
  uint16_t x, y;
  PlDirections direction;
  GmBody *start, *end;
} GmPlayer;

void initializePlayer(GmPlayer *player, GmMap *map);
bool updatePlayer(GmMap *map, GmPlayer *player, GmFood *food, int input);
bool updateBody(GmMap *map, GmPlayer *player, uint16_t y, uint16_t x);
bool createBody(GmMap *map, GmPlayer *player);
void deletePlayer(GmPlayer *player);
