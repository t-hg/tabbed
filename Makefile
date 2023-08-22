default: build

build:
	gcc -lX11 -Wpedantic -o tabbed main.c
