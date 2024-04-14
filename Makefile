# Project
TARGET = snake

# Files
INCLUDE = include
LIB = lib
OUTPUT = output
SRC = src

# C Compile
CC = clang
FLAGS = -Wall -Wextra -Wshadow -pedantic
CINCLUDE = -I $(INCLUDE) -I $(LIB)
CLIB = -l ncursesw

# Sources
CSRC = $(shell find $(SRC) -name '*.c')

# Targets
all: compile runex

compile:
	@$(CC) $(FLAGS) $(CINCLUDE) -o $(TARGET) $(CSRC) $(CLIB)

runex:
	@gnome-terminal -- ./$(TARGET)

run:
	@./$(TARGET)
