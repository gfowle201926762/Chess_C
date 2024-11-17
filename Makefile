
CC      := clang
DEBUG   := -g3
OPTIM   := -O3
CFLAGS  := -std=c99
RELEASE := $(CFLAGS) $(OPTIM)
SANI    := $(CFLAGS) -fsanitize=undefined -fsanitize=address $(DEBUG)
INCLUDE	:= -I"/opt/homebrew/Cellar/openjdk@17/17.0.13/libexec/openjdk.jdk/Contents/Home/include/" -I"/opt/homebrew/Cellar/openjdk@17/17.0.13/libexec/openjdk.jdk/Contents/Home/include/darwin/"

clean_chess:
	rm -f chess.out

clean_chess_s:
	rm -f chess_s.out

clean: clean_chess clean_chess_s

make_chess: chess.c chess.h test.c
	$(CC) chess.c $(RELEASE) -o chess.out

make_chess_java: chess.c chess.h hashkeys.h com_chess_application_services_NativeEngineService.h test.c
	$(CC) $(INCLUDE) chess.c $(RELEASE) -o libchess.dylib -arch x86_64

make_chess_java_s: chess.c chess.h hashkeys.h com_chess_application_services_NativeEngineService.h test.c
	$(CC) $(INCLUDE) chess.c $(SANI) -o libchess.dylib -arch x86_64

make_chess_s: chess.c chess.h test.c
	$(CC) chess.c $(SANI) -o chess_s.out

chess: clean_chess make_chess

chess_s: clean_chess_s make_chess_s

all: clean make_chess make_chess_s