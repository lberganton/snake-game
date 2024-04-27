# Project
TARGET = snake

# Files
INCLUDE = include
LIB = lib
SRC = src

# C Compiler
CC = gcc
LINK = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow
CINCLUDE = -I $(INCLUDE)

CSRC = $(wildcard $(SRC)/*.c)
COBJ = $(CSRC:.c=.o)

# Lib
CURSESLIB = -l ncursesw

# Rules
%.o: %.c
	@echo "Compiling $<. . . "
	@$(CC) -c $(CFLAGS) $(CINCLUDE) -o $@ $<
	@echo "File $@ generated."

# Targets
all: compile link

compile: $(COBJ)

link:
	@echo "Linking files. . . "
	@$(LINK) -o $(TARGET) $(COBJ) $(CURSESLIB)
	@echo "Game builded."

compile-with-local-curses:
ifeq ($(OS), Windows_NT)
	@echo "Compiling game. . . "
	@$(CC) $(FLAGS) $(CINCLUDE) -I$(LIB)/PDCurses -o $(TARGET) $(CSRC) $(LIB)/PDCurses/wincon/pdcurses.a
	@echo "Game compiled."
else
	@echo "This is only possible at Windows systems."
endif

build-curses:
ifeq ($(OS), Windows_NT)
	@mkdir $(LIB)
	@echo "'lib' directory created."

	@echo "Cloning repository. . ."
	@cd $(LIB) && git clone https://github.com/wmcbrine/PDCurses
	@echo "Repository cloned."

	@echo "Building PDCurses. . . "
	@cd $(LIB)/PDCurses/wincon && mingw32-make.exe -s WIDE=Y UTF8=Y
	@echo "PDCurses builded. . . "
else
	@echo "This is only possible at Windows systems."
endif

.PHONY: clean
clean:
	@rm $(wildcard $(SRC)/*.o)
	@echo "Cleaned directories."
