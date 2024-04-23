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
	@echo "Compilando $<. . . "
	@$(CC) -c $(CFLAGS) $(CINCLUDE) -o $@ $<
	@echo "Arquivo $@ gerado."

# Targets
all: compile link

compile: $(COBJ)

link:
	@echo "Linkando arquivos. . . "
	@$(LINK) -o $(TARGET) $(COBJ) $(CURSESLIB)
	@echo "Jogo montado."

compile-with-local-curses:
ifeq ($(OS), Windows_NT)
	@echo "Compilando jogo. . . "
	@$(CC) $(FLAGS) $(CINCLUDE) -I$(LIB)/PDCurses -o $(TARGET) $(CSRC) $(LIB)/PDCurses/wincon/pdcurses.a
	@echo "Jogo compilado."
else
	@echo "Isso só é possível em sistemas Windows."
endif

build-curses:
ifeq ($(OS), Windows_NT)
	@mkdir $(LIB)
	@echo "Diretório 'lib' criado."

	@echo "Clonando repositório. . ."
	@cd $(LIB) && git clone https://github.com/wmcbrine/PDCurses
	@echo "Repositório clonado."

	@echo "Compilando PDCurses. . . "
	@cd $(LIB)/PDCurses/wincon && mingw32-make.exe -s WIDE=Y UTF8=Y
	@echo "PDCurses compilado. . . "
else
	@echo "Isso só é possível em sistemas Windows."
endif

.PHONY: clean
clean:
	@rm $(COBJ)
	@echo "Diretórios limpos."
