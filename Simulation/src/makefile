# C++ files which may be compiled
SIM = simulator.cpp

ANALYSIS = analysis.cpp

AUX = block.cpp ghost.cpp maze.cpp window.cpp eatable.cpp

# compiler specification
CC = g++

# suppress warnings
COMPILER_FLAGS = -w

# linking libraries
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

# executable name
SIM_NAME = simulator

ANL_NAME = anl

# this is the target that creates both executable files (both game and server)
sim : $(SIM) $(AUX)
	$(CC) $(SIM) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(SIM_NAME)

anly: $(ANALYSIS) $(AUX)
	$(CC) $(ANALYSIS) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(ANL_NAME)

clean:
	rm $(SIM_NAME) $(ANL_NAME)
