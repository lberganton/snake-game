#include "game.h"
#include "player.h"
#include "config.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>

extern GmElement ElVoid;
extern GmElement ElSnakeHead;
extern GmElement ElSnakeBody;
extern GmElement ElFood;

void initializeGameScreen(GmScreen *screen) {
  screen->border = newwin(Y_MAP + 2, X_MAP + 2, (LINES - (Y_MAP + 2 + 3)) / 2, (COLS - (X_MAP + 2)) / 2);
  box(screen->border, ACS_VLINE, ACS_HLINE);

  screen->info = newwin(3, getmaxx(screen->border), getbegy(screen->border) + getmaxy(screen->border), getbegx(screen->border));
  box(screen->info, ACS_VLINE, ACS_HLINE);
  
  wrefresh(screen->border);
  wrefresh(screen->info);
}

void initializeMap(GmMap *map, GmScreen *screen) {
  map->window = newwin(Y_MAP, X_MAP, getbegy(screen->border) + 1, getbegx(screen->border) + 1);
  keypad(map->window, true);
  
  for (size_t i = 0; i < Y_MAP; i++) {
    for (size_t j = 0; j < X_MAP; j++) {
      map->matrix[i][j] = &ElVoid;
    }
  }

  map->matrix[Y_PLAYER][X_PLAYER] = &ElSnakeHead;
}

void initializeFood(GmFood *food, GmMap *map) {
  srand(time(NULL));

  do {
    food->y = rand() % (Y_MAP - 1);
    food->x = rand() % (X_MAP - 1);
  } while (map->matrix[food->y][food->x] != &ElVoid);
  map->matrix[food->y][food->x] = &ElFood;

  wattrset(map->window, ElFood.attribute);
  mvwaddch(map->window, food->y, food->x, ElFood.graphic);
}

void startGame(User *user) {
  GmPlayer player;
  GmFood food;
  GmMap map;
  GmScreen screen;
  int input;

  initializeGameScreen(&screen);
  initializeMap(&map, &screen);
  initializePlayer(&player, &map);
  initializeFood(&food, &map);

  nodelay(map.window, true);

  while (true) {
    updateGameScreen(user, &player, &map, &screen);

    input = wgetch(map.window);
    while (wgetch(map.window) != ERR);

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

  user->mostRecentScore = player.points;
  user->bestScore = player.points > user->bestScore ? player.points : user->bestScore;

  wclear(map.window); 
  wrefresh(map.window);
  delwin(map.window);
  wclear(screen.border);
  wrefresh(screen.border);
  delwin(screen.border);
  wclear(screen.info);
  wrefresh(screen.info);
  delwin(screen.info);
}

void updateFood(GmFood *food, GmMap *map, GmPlayer *player) {
  player->collected++;
  player->points += 10;
  map->matrix[food->y][food->x] = &ElVoid;

  do {
    food->y = rand() % (Y_MAP - 1);
    food->x = rand() % (X_MAP - 1);
  } while (map->matrix[food->y][food->x] != &ElVoid);

  map->matrix[food->y][food->x] = &ElFood;

  wattrset(map->window, ElFood.attribute);
  mvwaddch(map->window, food->y, food->x, ElFood.graphic);
}

static void printScreenInfo(const GmScreen *screen, uint8_t y, uint8_t x, const char *descriptor, uint32_t info) {
  wattrset(screen->info, A_REVERSE);
  mvwprintw(screen->info, y, x, "%s", descriptor);
  wattrset(screen->info, A_NORMAL);
  wprintw(screen->info, ": %" PRIu32, info);
}

void updateGameScreen(const User *user, const GmPlayer *player, const GmMap *map, const GmScreen *screen) { 
  wrefresh(map->window);

  uint8_t sector = (getmaxx(screen->info) - 2) / 3;

  printScreenInfo(screen, 1, 1, "Pontuação do Jogo", player->points);
  printScreenInfo(screen, 1, sector, "Comidas Coletadas", (uint32_t) player->collected);
  printScreenInfo(screen, 1, sector * 2, "Melhor Pontuação", user->bestScore);

  wrefresh(screen->info);
}
