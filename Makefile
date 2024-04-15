# Project
TARGET = snake

# Files
INCLUDE = include
LIB = lib
SRC = src

# C Compile
CC = gcc
LINK = gcc
FLAGS = -Wall -Wextra -Wshadow -Wpedantic
CINCLUDE = -I $(INCLUDE)

CSRC = $(shell find $(SRC) -name '*.c')
COBJ = $(CSRC:.c=.o)

# Lib
ifeq ($(OS), Windows_NT)
	CURSESLIB = -l pdcurses
else
	CURSESLIB = -l ncursesw
endif

# Rules
%.o: %.c
	@echo "Compilando $<. . . "
	@$(CC) -c $(FLAGS) $(CINCLUDE) -o $@ $<
	@echo "Arquivo $@ gerado."

# Targets
all: compile link

compile: $(COBJ)

link:
	@echo "Linkando arquivos. . . "
	@$(LINK) -o $(TARGET) $(COBJ) $(CURSESLIB)
	@echo "Jogo montado."

.PHONY: clean
clean:
	@rm $(COBJ)
	@echo "Diretórios limpos."
