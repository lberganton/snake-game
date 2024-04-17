#include "player.h"
#include "game.h"
#include "config.h"
#include "interface.h"
#include <curses.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

extern Profile profile;

void initializePlayer(GamePlayer *player, GameMap *map) {
  player->size = 1;
  player->collected = 0;
  player->points = 0;
  player->speed = 0;
  player->y = profile.height - 1;
  player->x = profile.width / 2;
  player->direction = DIRECTION_UP;
  player->start = NULL;
  player->end = NULL;
  
  map->matrix[player->y][player->x] = ELEMENT_SNAKE_HEAD;
}

bool updatePlayer(GameMap *map, GamePlayer *player, GameFood *food, int input) {
  uint16_t oldY = player->y;
  uint16_t oldX = player->x;

  switch (input) {
  case 'w':
  case 'W':
  case KEY_UP:
    if (player->direction != DIRECTION_DOWN) {
      player->direction = DIRECTION_UP;
    }
    break;
  case 's':
  case 'S':
  case KEY_DOWN:
    if (player->direction != DIRECTION_UP) {
      player->direction = DIRECTION_DOWN;
    }
    break;
  case 'a':
  case 'A':
  case KEY_LEFT:
    if (player->direction != DIRECTION_RIGHT) {
      player->direction = DIRECTION_LEFT;
    }
    break;
  case 'd':
  case 'D':
  case KEY_RIGHT:
    if (player->direction != DIRECTION_LEFT) {
      player->direction = DIRECTION_RIGHT;
    }
  }

  switch (player->direction) { 
  case DIRECTION_UP:
    if (player->y == 0) {
      return false;
    } 
    player->y--;
    break;
  case DIRECTION_DOWN:
    if (player->y == profile.height - 1) {
      return false;
    } 
    player->y++;
    break;
  case DIRECTION_LEFT:
    if (player->x == 0) {
      return false;
    } 
    player->x--;
    break;
  case DIRECTION_RIGHT:
    if (player->x == profile.width - 1) {
      return false;
    }
    player->x++;
  }

  if (map->matrix[player->y][player->x] == ELEMENT_SNAKE_BODY) {
    paintElement(map, ELEMENT_SNAKE_HEAD, player->y, player->x);
    return false;
  }

  map->matrix[oldY][oldX] = ELEMENT_VOID;
  paintElement(map, ELEMENT_VOID, oldY, oldX);

  map->matrix[player->y][player->x] = ELEMENT_SNAKE_HEAD;
  paintElement(map, ELEMENT_SNAKE_HEAD, player->y, player->x);

  updateBody(map, player->start, oldY, oldX);

  if (player->y == food->y && player->x == food->x) {
    player->collected++;
    player->points += 10;

    if (player->speed < GAME_SPEED_INCREASE) {
      player->speed++;
    }

    map->matrix[food->y][food->x] = ELEMENT_VOID;
    createFood(food, map);
    paintElement(map, ELEMENT_FOOD, food->y, food->x);
    createBody(map, player);
  }

  map->matrix[player->y][player->x] = ELEMENT_SNAKE_HEAD;

  return true;
}

void updateBody(GameMap *map, GameBody *body, uint16_t gotoY, uint16_t gotoX) {
  if (body) {
    map->matrix[body->y][body->x] = ELEMENT_VOID;
    paintElement(map, ELEMENT_VOID, body->y, body->x);

    paintElement(map, ELEMENT_SNAKE_BODY, gotoY, gotoX);
    map->matrix[gotoY][gotoX] = ELEMENT_SNAKE_BODY;

    uint16_t swapY = body->y;
    uint16_t swapX = body->x;
    body->y = gotoY;
    body->x = gotoX;
    gotoY = swapY;
    gotoX = swapX;

    updateBody(map, body->next, gotoY, gotoX);
  }
}

bool createBody(GameMap *map, GamePlayer *player) {
  GameBody *new = (GameBody*) malloc(sizeof(GameBody));
  if (!new) {
    printCenterMessage(MSG_ALLOCATION_FAILED, map->window);
    return false;
  }

  new->next = NULL;

  if (!player->end) {
    player->start = new;
    new->y = player->y;
    new->x = player->x;
  }
  else {
    new->y = player->end->y;
    new->x = player->end->x;
    player->end->next = new;
  }
  player->end = new;

  map->matrix[new->y][new->x] = ELEMENT_SNAKE_BODY;

  return true;
}

void deletePlayer(GamePlayer *player) {
  GameBody *body = player->start;
  GameBody *next;
  while (body) {
    next = body->next;
    free(body);
    body = next;
  }
}
