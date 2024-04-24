/*****************************************************************************
 * File: game.h
 * Author: Lucas Berganton
 * Created: 04/01/2024
 * Description: Definition of the structs and function for the game.
 *****************************************************************************/
#pragma once

#include "config.h"
#include <curses.h>
#include <inttypes.h>
#include <stdbool.h>

typedef enum GameElement {
  ELEMENT_VOID,
  ELEMENT_FOOD,
  ELEMENT_SNAKE_HEAD,
  ELEMENT_SNAKE_BODY
} GameElement;

typedef struct GameMap {
  WINDOW *window;
  GameElement matrix[MAX_HEIGHT][MAX_WIDTH];
} GameMap;

typedef struct GameFood {
  uint16_t x, y;
} GameFood;

typedef struct GameScreen {
  WINDOW *border;
  WINDOW *info;
} GameScreen;

typedef enum Direction {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} Direction;

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
  Direction direction;
  GameBody *start, *end;
} GamePlayer;

void startGame(void);
void initializeGameScreen(GameMap *map, GameScreen *screen);
void deleteGameScreen(GameMap *map, GameScreen *screen);
void initializeMap(GameMap *map);
void createFood(GameFood *food, GameMap *map);
void paintElement(GameMap *map, GameElement element, int y, int x);
void paintMap(GameMap *map);
void updateGameScreen(GamePlayer *player, GameMap *map, GameScreen *screen);

void initializePlayer(GamePlayer *player, GameMap *map);
bool updatePlayer(GameMap *map, GamePlayer *player, GameFood *food, int input);
bool createBody(GameMap *map, GamePlayer *player);
void updateBody(GameMap *map, GameBody *body, uint16_t y, uint16_t x);
void deletePlayer(GamePlayer *player);
