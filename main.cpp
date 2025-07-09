#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <print>
#include <ctime>
#include <cstdlib>

#include "SnakeGame.h"

int main() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::print("Failed to init SDL: {}", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (!TTF_Init()) {
		std::print("Failed to init SDL_ttf: {}", SDL_GetError());
		return EXIT_FAILURE;
	}

	srand(static_cast<unsigned>(time(nullptr)));

	SnakeGame game;
	game.Run();

	return EXIT_SUCCESS;
}