# Cross-platform Makefile for To-Do App (Windows & Linux)

# Detect OS
ifeq ($(OS),Windows_NT)
	IS_WINDOWS := 1
else
	IS_WINDOWS := 0
endif

# Compiler and flags
CC = g++
CFLAGS = -std=c++17 -Wall -I./include
SRC_DIR = src
SRC_FILES = src/gui.cpp src/main.cpp src/Task.cpp
OBJ_DIR = build/obj
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

ifeq ($(IS_WINDOWS),1)
	TARGET = bin/TO_DO_APP.exe
	LDFLAGS = -L./lib -lmingw32 -lSDL2main -lSDL2 -lstdc++fs -lgdi32 -luser32 -lkernel32 -lwinmm -limm32 -lole32 -loleaut32 -luuid -lversion -lsetupapi -mwindows -lSDL2_test
	MKDIR = if not exist $(subst /,\\,$(1)) mkdir $(subst /,\\,$(1))
	RM = if exist $(subst /,\\,$(1)) del /q $(subst /,\\,$(1))
	RMDIR = if exist $(subst /,\\,$(1)) rmdir /s /q $(subst /,\\,$(1))
else
	TARGET = bin/TO_DO_APP
	LDFLAGS = -L./lib -lSDL2 -lSDL2_test -lstdc++fs
	MKDIR = mkdir -p $(1)
	RM = rm -f $(1)
	RMDIR = rm -rf $(1)
endif

# Ensure build directories exist
$(OBJ_DIR):
	$(call MKDIR,build)
	$(call MKDIR,build/obj)

bin:
	$(call MKDIR,bin)

all: bin $(OBJ_DIR) $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Cleaning build files...
	$(call RMDIR,build)
	$(call RM,$(TARGET))

run: $(TARGET)
ifeq ($(IS_WINDOWS),1)
	cd bin && TO_DO_APP.exe
else
	cd bin && ./TO_DO_APP
endif

.PHONY: all clean run