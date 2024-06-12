
CC      := clang
DEBUG   := -g3
OPTIM   := -O1
CFLAGS  := -Wall -std=c99
RELEASE := $(CFLAGS) $(OPTIM)
SANI    := $(CFLAGS) -fsanitize=undefined -fsanitize=address $(DEBUG)

chess: chess.c chess.h test.c
	$(CC) chess.c $(RELEASE) -o chess.out

chess_s: chess.c chess.h
	$(CC) chess.c $(SANI) -o chess_s.out

all: chess chess_s