/*****************************************************************************
 * File: interface.h
 * Author: Lucas Berganton
 * Created: 04/01/2024
 * Description: Functions for some interfaces used in the program.
 *****************************************************************************/
#pragma once

#include <curses.h>
#include <stdarg.h>

#define MAIN_MENU_WIDTH 30
#define OPTIONS_MENU_WIDTH 38

void printCenterMessage(const char *msg, WINDOW *window);
void handleWithSmallTermSize(int y, int x);
int interfaceMenu(const char *title, const int n, ...);
void interfaceString(const char *title, const int n, ...);
void interfaceGetStringInput(char *msg, char *destiny, size_t lenght);
void optionsMenu(void);
void createProfile(void);
