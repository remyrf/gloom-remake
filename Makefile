main:
	cc src/player.c src/main.c src/platform.c -Wall -Wextra -Iinclude $$(pkg-config --libs --cflags raylib) -o gloom
