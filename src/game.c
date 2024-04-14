#include "game.h"
#include "player.h"
#include "config.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>

GmElement ElVoid;
GmElement ElSnakeHead;
GmElement ElSnakeBody;
GmElement ElFood;

void initializeGameElements(void) {
  ElVoid = (GmElement) {.graphic = ' ', .attribute = COLOR_PAIR(GmBlack)};
  ElSnakeHead = (GmElement) {.graphic = ACS_BOARD, .attribute = COLOR_PAIR(GmBlue)};
  ElSnakeBody = (GmElement) {.graphic = ACS_BOARD, .attribute = COLOR_PAIR(GmWhite)};
  ElFood = (GmElement) {.graphic = ACS_DIAMOND, .attribute = COLOR_PAIR(GmRed)};
}

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
      map->matrix[i][j] = ElVoid;
    }
  }
  map->matrix[Y_PLAYER][X_PLAYER] = ElSnakeHead;
}

void initializeFood(GmFood *food, GmMap *map) {
  srand(time(NULL));
  do {
    food->y = rand() % (Y_MAP - 1);
    food->x = rand() % (X_MAP - 1);
  } while (map->matrix[food->y][food->x].attribute != ElVoid.attribute && map->matrix[food->y][food->x].graphic != ElVoid.graphic);
  map->matrix[food->y][food->x] = ElFood;

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
    updateGameScreen(&player, &map, &screen);
    input = wgetch(map.window);
    while (wgetch(map.window) != ERR);
    if (!updatePlayer(&map, &player, &food, input)) {
      printCenterMessage(MSG_YOU_LOSE, map.window);
      wrefresh(map.window);
      sleep(2);
      while (wgetch(map.window) != ERR);
      break;
    }
    usleep(GAME_DELAY);
  }

  deletePlayer(&player);

  user->points += player.points;

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
  map->matrix[food->y][food->x] = ElVoid;
  do {
    food->y = rand() % (Y_MAP - 1);
    food->x = rand() % (X_MAP - 1);
  } while (map->matrix[food->y][food->x].attribute != ElVoid.attribute && map->matrix[food->y][food->x].graphic != ElVoid.graphic);
  map->matrix[food->y][food->x] = ElFood;

  wattrset(map->window, ElFood.attribute);
  mvwaddch(map->window, food->y, food->x, ElFood.graphic);
}

static void printScreenInfo(const GmScreen *screen, uint8_t y, uint8_t x, const char *descriptor, uint32_t info) {
  wattrset(screen->info, A_REVERSE);
  mvwprintw(screen->info, y, x, "%s", descriptor);
  wattrset(screen->info, A_NORMAL);
  wprintw(screen->info, ": %" PRIu32, info);
}

void updateGameScreen(const GmPlayer *player, const GmMap *map, const GmScreen *screen) { 
  wrefresh(map->window);
  printScreenInfo(screen, 1, (getmaxx(screen->info) - 2) / 3, "Comidas Coletadas", (uint32_t) player->collected);
  printScreenInfo(screen, 1, 1, "Pontuação do Jogo", player->points);
  printScreenInfo(screen, 1, ((getmaxx(screen->info) - 2) / 3) + ((getmaxx(screen->info) - 2) / 3), "Maior Pontuação", 0);
  wrefresh(screen->info);
}
