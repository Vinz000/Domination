compiler=gcc
panel =-lpanel
curses=-lncurses

srcFiles = 	src/io.o \
		src/game.o \
		src/main.o \
		src/curses.o \
		src/print.o \

domination: $(srcFiles)
	$(compiler) -o domination $(srcFiles) $(panel) $(curses)  
	@rm src/*.o

clean:
	rm domination
