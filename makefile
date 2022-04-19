

SRC_DIR := ~/Downloads/impfiles2/cop-game-1835/cop-game
# SRC_DIR := .

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -I/usr/include/SDL2/ -L/usr/include/SDL2/
 
 
play : $(OBJ_FILES)
	g++ $(LDFLAGS) $^ -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o $@
