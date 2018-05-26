CC = g++-5
CFLAGS = -std=c++14 -O3
EXEC_NAME = bin/game
LDFLAGS = -lSDL2 -lSDL2_image
BIN_DIR = obj/debug
TBIN_DIR = obj/debug


all: $(EXEC_NAME)

$(EXEC_NAME): $(BIN_DIR)/events_engine.o $(BIN_DIR)/user_events.o $(BIN_DIR)/game_engine.o $(BIN_DIR)/circle_dynamic_game_object.o $(BIN_DIR)/dynamic_game_object.o $(BIN_DIR)/game_object.o $(BIN_DIR)/objects_layer.o $(BIN_DIR)/static_game_object.o $(BIN_DIR)/world.o $(BIN_DIR)/animated_sprite.o $(BIN_DIR)/armature.o $(BIN_DIR)/bone.o $(BIN_DIR)/rigged_sprite.o $(BIN_DIR)/sprite_animation.o $(BIN_DIR)/camera.o $(BIN_DIR)/graphics_engine.o $(BIN_DIR)/sprite.o $(BIN_DIR)/texture_manager.o $(BIN_DIR)/body.o $(BIN_DIR)/circle_dynamic_body.o $(BIN_DIR)/dynamic_body.o $(BIN_DIR)/physics_engine.o $(BIN_DIR)/static_body.o $(BIN_DIR)/sound_engine.o $(TBIN_DIR)/box.o $(TBIN_DIR)/character.o $(TBIN_DIR)/ladder.o $(TBIN_DIR)/main.o $(TBIN_DIR)/rigged_test.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/%.o: src/events_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/%.o: src/game_types/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/%.o: src/graphics_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/%.o: src/graphics_engine/animation/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/%.o: src/physics_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/%.o: src/sound_engine/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

$(TBIN_DIR)/%.o: tests/%.cpp
	$(CC) $(FLAGS) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(TBIN_DIR)/*.o
