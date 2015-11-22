all:
	gcc -o wish -I ./utils wish.c utils/commander.c utils/prompt.c

run:
	./wish

clean:
	rm wish

