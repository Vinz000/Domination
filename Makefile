compiler=gcc
panel =-lpanel
curses=-lncurses
srcDirectory=srcFiles

srcFiles = 	$(srcDirectory)/io.o \
		$(srcDirectory)/game.o \
		$(srcDirectory)/main.o \
		$(srcDirectory)/curses.o \
$(srcDirectory)/print.o \

domination: $(srcFiles)
	$(compiler) -o domination $(srcFiles) $(panel) $(curses)  
	@rm $(srcDirectory)/*.o

clean:
	rm domination
