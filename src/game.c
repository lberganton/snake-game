#include "game.h"
#include "player.h"
#include "config.h"
#include "interface.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>

extern Profile profile;

void initializeGameScreen(GameMap *map, GameScreen *screen) {
  screen->border = newwin(Y_MAP + 2, X_MAP + 2, (LINES - (Y_MAP + 2 + 3)) / 2, (COLS - (X_MAP + 2)) / 2);
  box(screen->border, ACS_VLINE, ACS_HLINE);

  screen->info = newwin(3, getmaxx(screen->border), getbegy(screen->border) + getmaxy(screen->border), getbegx(screen->border));
  box(screen->info, ACS_VLINE, ACS_HLINE);

  map->window = newwin(Y_MAP, X_MAP, getbegy(screen->border) + 1, getbegx(screen->border) + 1);
  keypad(map->window, true);
  nodelay(map->window, true);
  
  wrefresh(screen->border);
  wrefresh(screen->info);
}

void deleteGameScreen(GameMap *map, GameScreen *screen) {
  wclear(map->window); 
  wclear(screen->border);
  wclear(screen->info);

  wrefresh(map->window);
  wrefresh(screen->border);
  wrefresh(screen->info);
  
  delwin(map->window);
  delwin(screen->border);
  delwin(screen->info);
}

void initializeMap(GameMap *map) {
  for (size_t i = 0; i < Y_MAP; i++) {
    for (size_t j = 0; j < X_MAP; j++) {
      map->matrix[i][j] = ELEMENT_VOID;
    }
  }
}

void startGame(void) {
  GamePlayer player;
  GameFood food;
  GameMap map;
  GameScreen screen;
  int input;

  initializeGameScreen(&map, &screen);
  initializeMap(&map);
  initializePlayer(&player, &map);
  createFood(&food, &map);
  paintMap(&map);

  while (true) {
    input = wgetch(map.window);
    while (wgetch(map.window) != ERR);

    if (input == KEY_RESIZE) {
      if (LINES < Y_MAP + 5 || COLS < X_MAP + 2) {
        handleWithSmallTermSize(Y_MAP + 5, X_MAP + 2);
      }
      
      deleteGameScreen(&map, &screen);
      initializeGameScreen(&map, &screen);
      paintMap(&map);
    }

    updateGameScreen(&player, &map, &screen);

    if (!updatePlayer(&map, &player, &food, input)) {
      printCenterMessage(MSG_YOU_LOSE, map.window);
      wrefresh(map.window);
      sleep(2);
      while (wgetch(map.window) != ERR);
      break;
    }
    
    usleep((double) GAME_MAX_DELAY - player.speed * ((GAME_MAX_DELAY - GAME_MIN_DELAY) / GAME_SPEED_INCREASE));
  }

  deletePlayer(&player);

  profile.mostRecentScore = player.points;
  profile.bestScore = player.points > profile.bestScore ? player.points : profile.bestScore;

  deleteGameScreen(&map, &screen);
}

void paintElement(GameMap *map, GameElement element, int y, int x) {
  int graphic, attribute;

  switch (element) {
  case ELEMENT_VOID:
    graphic = GRAPHIC_VOID;
    attribute = BLACK;
    break;
  case ELEMENT_FOOD:
    graphic = GRAPHIC_FOOD;
    attribute = profile.foodAttribute;
    break;
  case ELEMENT_SNAKE_HEAD:
    graphic = GRAPHIC_SNAKE_HEAD;
    attribute = profile.snakeHeadAttribute;
    break;
  case ELEMENT_SNAKE_BODY:
    graphic = GRAPHIC_SNAKE_BODY;
    attribute = profile.snakeBodyAttribute;
  }
  wattrset(map->window, attribute);
  mvwaddch(map->window, y, x, graphic);
}

void paintMap(GameMap *map) {
  for (size_t i = 0; i < Y_MAP; i++) {
    for (size_t j = 0; j < X_MAP; j++) {
      paintElement(map, map->matrix[i][j], i, j);
    }
  }
}

void createFood(GameFood *food, GameMap *map) {
  srand(time(NULL));

  do {
    food->y = rand() % (Y_MAP - 1);
    food->x = rand() % (X_MAP - 1);
  } while (map->matrix[food->y][food->x] != ELEMENT_VOID);

  map->matrix[food->y][food->x] = ELEMENT_FOOD;
}

static void printScreenInfo(GameScreen *screen, uint8_t y, uint8_t x, char *descriptor, uint32_t info) {
  wattrset(screen->info, A_REVERSE);
  mvwprintw(screen->info, y, x, "%s", descriptor);
  wattrset(screen->info, A_NORMAL);
  wprintw(screen->info, ": %" PRIu32, info);
}

void updateGameScreen(GamePlayer *player, GameMap *map, GameScreen *screen) { 
  wrefresh(map->window);

  uint8_t block = (getmaxx(screen->info) - 2) / 3;

  printScreenInfo(screen, 1, 1, "Pontuação do Jogo", player->points);
  printScreenInfo(screen, 1, block, "Comidas Coletadas", (uint32_t) player->collected);
  printScreenInfo(screen, 1, block * 2, "Melhor Pontuação", profile.bestScore);

  wrefresh(screen->info);
}
