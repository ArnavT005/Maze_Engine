# C++ files which may be compiled
GAME = game.cpp

SRVR = server.cpp

AUX = block.cpp eatable.cpp ghost.cpp manager.cpp maze.cpp menu.cpp pacman.cpp portal.cpp scoreboard.cpp window.cpp

# compiler specification
CC = g++

# suppress warnings
COMPILER_FLAGS = -w

# linking libraries
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net

# executable name
GAME_NAME = game

SRVR_NAME = server

# this is the target that creates both executable files (both game and server)
all : $(GAME) $(SRVR) $(AUX)
	$(CC) $(GAME) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(GAME_NAME)
	$(CC) $(SRVR) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(SRVR_NAME)

# this target only creates game executable
game: $(GAME) $(AUX)
	$(CC) $(GAME) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(GAME_NAME)

# this target only creates server executable
server: $(SRVR)
	$(CC) $(SRVR) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(SRVR_NAME)

clean:
	rm $(GAME_NAME) $(SRVR_NAME)
