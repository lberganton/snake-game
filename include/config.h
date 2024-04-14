#pragma once

#include <curses.h>
#include <stdint.h>

#define FILE_DATA "data"
#define USER_NAME "user"
#define STRING_MAX_LEN 128

#define MSG_YOU_LOSE            "VOCÊ PERDEU!"
#define MSG_ALLOCATION_FAILED   "FALHA NA ALOCAÇÃO DE MEMÓRIA!"

#define GAME_MAX_DELAY 100000
#define GAME_MIN_DELAY 50000
#define GAME_SPEED_INCREASE 25

#define RANK_LENGTH 10

typedef struct User {
  char name[32];
  uint32_t mostRecentScore;
  uint32_t bestScore;
} User;

void printCenterMessage(const char *msg, WINDOW *window);
void getUserName(User *user);
