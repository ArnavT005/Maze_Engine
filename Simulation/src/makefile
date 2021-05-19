# C++ files which may be compiled
SIM = simulator.cpp

ANLYS = analysis.cpp

AUX = block.cpp ghost.cpp maze.cpp window.cpp eatable.cpp

# compiler specification
CC = g++

# suppress warnings
COMPILER_FLAGS = -w

# linking libraries
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

# executable name
SIM_NAME = simulator

ANLYS_NAME = analysis


# create both simulation and analysis executable
all: $(SIM) $(ANLYS) $(AUX)
	$(CC) $(SIM) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(SIM_NAME)
	$(CC) $(ANLYS) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(ANLYS_NAME)

# create only simulation executable
simulator : $(SIM) $(AUX)
	$(CC) $(SIM) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(SIM_NAME)

# creat only analysis executable
analysis: $(ANLYS) $(AUX)
	$(CC) $(ANLYS) $(AUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(ANLYS_NAME)

clean:
	rm $(SIM_NAME) $(ANLYS_NAME)
