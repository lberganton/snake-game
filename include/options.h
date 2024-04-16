#pragma once

#include <curses.h>

void paintOptionsMenu(WINDOW *header, WINDOW *info, WINDOW *options, WINDOW *save, WINDOW *load, WINDOW *back);
void optionsMenu(void);
