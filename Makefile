build: wordle.c
	gcc -g wordle.c -o wordle -Wall -lncurses
run: wordle
	./wordle
clean:
	rm wordle
