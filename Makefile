CC = g++-5
CFLAGS = -Wall -std=c++14 -O3
EXEC_NAME = my_game_engine
INCLUDES = src/sound_engine/sound_engine.cpp
LDFLAGS = -lSDL2 -lSDL2_image
BIN_DIR = obj/Debug/src
TBIN_DIR = obj/Debug/tests

INSTALL_DIR = /usr/bin

all: game


game: $(BIN_DIR)/events_engine/events_engine.o $(BIN_DIR)/events_engine/user_events.o $(BIN_DIR)/game_engine.o $(BIN_DIR)/game_types/circle_dynamic_game_object.o $(BIN_DIR)/game_types/dynamic_game_object.o $(BIN_DIR)/game_types/game_object.o $(BIN_DIR)/game_types/objects_layer.o $(BIN_DIR)/game_types/static_game_object.o $(BIN_DIR)/game_types/world.o $(BIN_DIR)/graphics_engine/animation/animated_sprite.o $(BIN_DIR)/graphics_engine/animation/armature.o $(BIN_DIR)/graphics_engine/animation/bone.o $(BIN_DIR)/graphics_engine/animation/rigged_sprite.o $(BIN_DIR)/graphics_engine/animation/sprite_animation.o $(BIN_DIR)/graphics_engine/camera.o $(BIN_DIR)/graphics_engine/graphics_engine.o $(BIN_DIR)/graphics_engine/sprite.o $(BIN_DIR)/graphics_engine/texture_manager.o $(BIN_DIR)/physics_engine/body.o $(BIN_DIR)/physics_engine/circle_dynamic_body.o $(BIN_DIR)/physics_engine/dynamic_body.o $(BIN_DIR)/physics_engine/physics_engine.o $(BIN_DIR)/physics_engine/static_body.o $(BIN_DIR)/sound_engine/sound_engine.o $(TBIN_DIR)/box.o $(TBIN_DIR)/character.o $(TBIN_DIR)/ladder.o $(TBIN_DIR)/main.o $(TBIN_DIR)/rigged_test.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)


$(BIN_DIR)/events_engine/%.o: src/events_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/game_types/%.o: src/game_types/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/graphics_engine/%.o: src/graphics_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/graphics_engine/animation/%.o: src/graphics_engine/animation/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/physics_engine/%.o: src/physics_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/sound_engine/%.o: src/sound_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(TBIN_DIR)/%.o: tests/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)
