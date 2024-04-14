#pragma once

#include <ncurses.h>
#include <stdint.h>

#define FILE_DATA "data"
#define USER_NAME "user"
#define STRING_MAX_LEN 128

#define MSG_YOU_LOSE            "VOCÊ PERDEU!"
#define MSG_ALLOCATION_FAILED   "FALHA NA ALOCAÇÃO DE MEMÓRIA!"

#define GAME_DELAY 100000
#define RANK_LENGTH 10

typedef struct User {
  char name[32];
  uint32_t points;
} User;

void printCenterMessage(const char *msg, WINDOW *window);
void getUserName(User *user);
