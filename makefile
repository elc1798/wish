all: commander prompt dirman
	gcc -o wish -I ./utils wish.c utils/grunt.o utils/commander.o utils/prompt.o utils/dir_man.o

grunt: utils/grunt.c utils/grunt.h
	gcc -I ./utils -o utils/grunt.o -c utils/grunt.c

commander: grunt utils/commander.c utils/commander.h
	gcc -I ./utils -o utils/commander.o -c utils/commander.c

prompt: utils/prompt.c utils/prompt.h
	gcc -I ./utils -o utils/prompt.o -c utils/prompt.c

dirman: utils/dir_man.c utils/dir_man.h
	gcc -I ./utils -o utils/dir_man.o -c utils/dir_man.c

run: ./wish
	./wish

clean:
	rm -f wish
	rm -f utils/*.o
	rm -f *~
	rm -f \#*\#
