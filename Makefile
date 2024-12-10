CXX = c++
CXXFLAGS = -std=c++20 -O2 -g -Wall -Wextra -Wpedantic -Wshadow -Walloc-zero -Wcast-align -Wconversion -Wctad-maybe-unsupported -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wduplicated-branches -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wformat-signedness -Wformat=2 -Winit-self -Wlogical-op -Wmismatched-tags -Wnoexcept -Wnon-virtual-dtor -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wredundant-tags -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wuseless-cast -Wvolatile -Wzero-as-null-pointer-constant -fmax-errors=2
LIBS = -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_audio -lallegro_acodec
SRC = src/main.cpp src/global_variables.cpp src/model/border/border.cpp src/model/vec2/vec2.cpp src/model/bounding_box/bounding_box.cpp src/model/bounceable/bounceable.cpp src/model/racket/racket.cpp  src/model/score/score.cpp src/model/game_board/game_board.cpp src/model/life/life.cpp src/model/ball/ball.cpp src/vue/display_game/display_game.cpp src/vue/color/colors.hpp src/controller/controller_game.cpp src/vue/canvas/canvas.cpp src/vue/figures/forme.cpp src/vue/figures/point.cpp src/vue/game_over/draw_game_over.cpp src/vue/game_win/draw_game_win.cpp src/vue/piece/ball.cpp src/vue/piece/brik.cpp src/vue/piece/racket.cpp src/vue/wall/wall_game.cpp
TARGET = arkanoid

# Règle par défaut
all: $(TARGET)

# Compilation
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: rmclean
rmclean: clean
	rm -f allegro