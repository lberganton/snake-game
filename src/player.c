#include "player.h"
#include "game.h"
#include "config.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

extern GmElement ElVoid;
extern GmElement ElSnakeHead;
extern GmElement ElSnakeBody;

void initializePlayer(GmPlayer *player, GmMap *map) {
  player->size = 1;
  player->collected = 0;
  player->points = 0;
  player->y = Y_PLAYER;
  player->x = X_PLAYER;
  player->direction = PlUp;
  player->start = NULL;
  player->end = NULL;

  wattrset(map->window, ElSnakeHead.attribute);
  mvwaddch(map->window, player->y, player->x, ElSnakeHead.graphic);
}

bool updatePlayer(GmMap *map, GmPlayer *player, GmFood *food, int input) {
  uint16_t oldY = player->y;
  uint16_t oldX = player->x;

  switch (input) {
  case 'w':
  case 'W':
  case KEY_UP:
    if (player->direction != PlDown) {
      player->direction = PlUp;
    }
    break;
  case 's':
  case 'S':
  case KEY_DOWN:
    if (player->direction != PlUp) {
      player->direction = PlDown;
    }
    break;
  case 'a':
  case 'A':
  case KEY_LEFT:
    if (player->direction != PlRight) {
      player->direction = PlLeft;
    }
    break;
  case 'd':
  case 'D':
  case KEY_RIGHT:
    if (player->direction != PlLeft) {
      player->direction = PlRight;
    }
  }

  switch (player->direction) { 
  case PlUp:
    if (player->y == 0) {
      return false;
    } 
    player->y--;
    break;
  case PlDown:
    if (player->y == Y_MAP - 1) {
      return false;
    } 
    player->y++;
    break;
  case PlLeft:
    if (player->x == 0) {
      return false;
    } 
    player->x--;
    break;
  case PlRight:
    if (player->x == X_MAP - 1) {
      return false;
    }
    player->x++;    
    break;
  }

  if (map->matrix[player->y][player->x].attribute == ElSnakeBody.attribute && map->matrix[player->y][player->x].graphic == ElSnakeBody.graphic) {
    wattrset(map->window, ElSnakeHead.attribute);
    mvwaddch(map->window, player->y, player->x, ElSnakeHead.graphic);
    return false;
  }

  map->matrix[oldY][oldX] = ElVoid;
  wattrset(map->window, ElVoid.attribute);
  mvwaddch(map->window, oldY, oldX, ElVoid.graphic);

  map->matrix[player->y][player->x] = ElSnakeHead;
  wattrset(map->window, ElSnakeHead.attribute);
  mvwaddch(map->window, player->y, player->x, ElSnakeHead.graphic);

  updateBody(map, player, oldY, oldX);

  if (player->y == food->y && player->x == food->x) {
    updateFood(food, map, player);
    createBody(map, player);
  }

  map->matrix[player->y][player->x] = ElSnakeHead;

  return true;
}

bool updateBody(GmMap *map, GmPlayer *player, uint16_t y, uint16_t x) {
  GmBody *body = player->start;
  uint16_t tempY, tempX;

  while (body) {
    map->matrix[body->y][body->x] = ElVoid;
    wattrset(map->window, ElVoid.attribute);
    mvwaddch(map->window, body->y, body->x, ElVoid.graphic);

    wattrset(map->window, ElSnakeBody.attribute);
    mvwaddch(map->window, y, x, ElSnakeBody.graphic);
    map->matrix[y][x] = ElSnakeBody;

    tempY = y;
    tempX = x;
    y = body->y;
    x = body->x;
    body->y = tempY;
    body->x = tempX;

    body = body->next;
  }

  return true;
}

bool createBody(GmMap *map, GmPlayer *player) {
  GmBody *new = (GmBody*) malloc(sizeof(GmBody));
  if (!new) {
    printCenterMessage(MSG_ALLOCATION_FAILED, map->window);
    return false;
  }

  new->next = NULL;

  if (!player->end) {
    player->start = new;
    player->end = new;
    new->previous = NULL;
    new->y = player->y;
    new->x = player->x;
  }
  else {
    new->y = player->end->y;
    new->x = player->end->x;
    new->previous = player->end;
    player->end->next = new;
    player->end = new;
  }

  map->matrix[new->y][new->x] = ElSnakeBody;

  return true;
}

void deletePlayer(GmPlayer *player) {
  GmBody *body = player->start;
  GmBody *next;
  while (body) {
    next = body->next;
    free(body);
    body = next;
  }
}
