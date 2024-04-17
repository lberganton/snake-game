#pragma once

#include <curses.h>
#include <stdbool.h>
#include "game.h"

typedef enum PlayerDirection {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} PlayerDirection;

typedef struct GameBody {
  uint16_t x, y;
  struct GameBody *next;
} GameBody;

typedef struct GamePlayer {
  uint32_t points;
  uint16_t size;
  uint16_t collected;
  uint32_t speed;
  uint16_t x, y;
  PlayerDirection direction;
  GameBody *start, *end;
} GamePlayer;

void initializePlayer(GamePlayer *player, GameMap *map);
bool updatePlayer(GameMap *map, GamePlayer *player, GameFood *food, int input);
void updateBody(GameMap *map, GamePlayer *player, uint16_t y, uint16_t x);
bool createBody(GameMap *map, GamePlayer *player);
void deletePlayer(GamePlayer *player);
