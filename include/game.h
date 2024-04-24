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

typedef struct GamePlayer GamePlayer;

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

void startGame(void);
void initializeGameScreen(GameMap *map, GameScreen *screen);
void deleteGameScreen(GameMap *map, GameScreen *screen);
void initializeMap(GameMap *map);
void createFood(GameFood *food, GameMap *map);
void paintElement(GameMap *map, GameElement element, int y, int x);
void paintMap(GameMap *map);
void updateGameScreen(GamePlayer *player, GameMap *map, GameScreen *screen);
