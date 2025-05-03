CC = g++
CFLAGS = -std=c++17 -Wall -I./include
# Add -lSDL2_test to link the SDL test library
LDFLAGS = -L./lib -lmingw32 -lSDL2main -lSDL2 -lstdc++fs -lgdi32 -luser32 -lkernel32 -lwinmm -limm32 -lole32 -loleaut32 -luuid -lversion -lsetupapi -mwindows -lSDL2_test

# Source files
SRC_DIR = src
# Only include the files we're using now
SRC_FILES = src/gui.cpp src/main.cpp src/Task.cpp

# Object files
OBJ_DIR = build/obj
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable (add .exe for Windows)
TARGET = bin/TO_DO_APP.exe

# Make sure build directories exist (Windows compatible)
$(OBJ_DIR):
	if not exist build mkdir build
	if not exist build\obj mkdir build\obj

bin:
	if not exist bin mkdir bin

# Default target
all: bin $(OBJ_DIR) $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile source files to object files (use forward slashes for Windows compatibility)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files (Target already includes .exe now)
clean:
	echo Cleaning build files...
	if exist build rmdir /s /q build
	REM Use backslashes for Windows del command
	if exist bin\TO_DO_APP.exe del /q bin\TO_DO_APP.exe

# Run the application (use notdir to get filename)
run: $(TARGET)
	echo Running $(TARGET)...
	cd bin && $(notdir $(TARGET))

.PHONY: all clean run