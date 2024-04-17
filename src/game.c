#include "game.h"
#include "config.h"
#include "interface.h"
#include "player.h"
#include <curses.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern Profile profile;

void initializeGameScreen(GameMap *map, GameScreen *screen) {
  screen->border = newwin(profile.height + 2, profile.width + 2,
                          (LINES - (profile.height + 2 + 3)) / 2,
                          (COLS - (profile.width + 2)) / 2);
  box(screen->border, ACS_VLINE, ACS_HLINE);

  screen->info =
      newwin(3, MIN_WIDTH, getbegy(screen->border) + getmaxy(screen->border),
             (COLS - MIN_WIDTH) / 2);
  box(screen->info, ACS_VLINE, ACS_HLINE);

  map->window =
      newwin(profile.height, profile.width, getbegy(screen->border) + 1,
             getbegx(screen->border) + 1);
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
  for (int i = 0; i < profile.height; i++) {
    for (int j = 0; j < profile.width; j++) {
      map->matrix[i][j] = ELEMENT_VOID;
    }
  }
}

void startGame(void) {
  GamePlayer player;
  GameFood food;
  GameMap map;
  GameScreen screen;

  initializeGameScreen(&map, &screen);
  initializeMap(&map);
  initializePlayer(&player, &map);
  createFood(&food, &map);
  paintMap(&map);

  double increase = (GAME_MAX_DELAY - GAME_MIN_DELAY) / GAME_SPEED_INCREASE;

  while (true) {
    int input = wgetch(map.window);
    clearInputBuffer(map.window);

    if (input == KEY_RESIZE) {
      if (LINES < profile.height + 5 || COLS < profile.width + 2) {
        handleWithSmallTermSize(profile.height + 5, profile.width + 2);
      }

      deleteGameScreen(&map, &screen);
      initializeGameScreen(&map, &screen);
      paintMap(&map);
    }

    updateGameScreen(&player, &map, &screen);

    if (!updatePlayer(&map, &player, &food, input)) {
      printCenterMessage("VOCÊ PERDEU!", map.window);
      wrefresh(map.window);
      sleep(2);
      clearInputBuffer(map.window);
      break;
    }

    usleep(GAME_MAX_DELAY - player.speed * increase);
  }

  deleteGameScreen(&map, &screen);
  deletePlayer(&player);

  profile.mostRecentScore = player.points;
  profile.bestScore =
      player.points > profile.bestScore ? player.points : profile.bestScore;

  saveData(DEFAULT_DATA_FILE_NAME);
}

void paintElement(GameMap *map, GameElement element, int y, int x) {
  int graphic, attribute;

  switch (element) {
  case ELEMENT_VOID:
    graphic = GRAPHIC_VOID;
    attribute = COLOR_PAIR(BLACK);
    break;
  case ELEMENT_FOOD:
    graphic = GRAPHIC_FOOD;
    attribute = COLOR_PAIR(profile.colorFood);
    break;
  case ELEMENT_SNAKE_HEAD:
    graphic = GRAPHIC_SNAKE_HEAD;
    attribute = COLOR_PAIR(profile.colorSnakeHead);
    break;
  case ELEMENT_SNAKE_BODY:
    graphic = GRAPHIC_SNAKE_BODY;
    attribute = COLOR_PAIR(profile.colorSnakeBody);
  }

  mvwaddch(map->window, y, x, graphic | attribute);
}

void paintMap(GameMap *map) {
  for (int i = 0; i < profile.height; i++) {
    for (int j = 0; j < profile.width; j++) {
      paintElement(map, map->matrix[i][j], i, j);
    }
  }
}

void createFood(GameFood *food, GameMap *map) {
  srand(time(NULL));

  do {
    food->y = rand() % (profile.height - 1);
    food->x = rand() % (profile.width - 1);
  } while (map->matrix[food->y][food->x] != ELEMENT_VOID);

  map->matrix[food->y][food->x] = ELEMENT_FOOD;
}

static void printScreenInfo(GameScreen *screen, uint8_t x, char *descriptor,
                            uint32_t info) {
  wattrset(screen->info, A_REVERSE);
  mvwprintw(screen->info, 1, x, "%s", descriptor);
  wattrset(screen->info, A_NORMAL);
  wprintw(screen->info, ": %-5" PRIu32 " ", info);
}

void updateGameScreen(GamePlayer *player, GameMap *map, GameScreen *screen) {
  wrefresh(map->window);

  uint8_t block = (getmaxx(screen->info) - 2) / 3;

  printScreenInfo(screen, 1, "Pontuação do Jogo", player->points);
  printScreenInfo(screen, block, "Comidas Coletadas", player->collected);
  printScreenInfo(screen, block * 2, "Melhor Pontuação", profile.bestScore);

  wrefresh(screen->info);
}
