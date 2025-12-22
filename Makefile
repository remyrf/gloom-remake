build:
	cc src/player.c src/main.c src/platform.c src/game.c src/coin.c src/shadow.c -Wall -Wextra -Iinclude $$(pkg-config --libs --cflags raylib) -o gloom

run: build
	./gloom
