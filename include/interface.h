#pragma once

#include <curses.h>
#include <stdarg.h>

typedef struct User User;

void printCenterMessage(const char *msg, WINDOW *window);
void handleWithSmallTermSize(int y, int x);
int interfaceMenu(const char *title, const size_t n, ...);
void interfaceString(const char *title, const size_t n, ...);
void getUserName(void);
