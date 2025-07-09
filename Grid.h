#pragma once

#include <array>
#include <SDL3/SDL.h>

#include "params.h"
#include "utils.h"

using GridLayout = std::array<std::array<uint8_t, Params::Game::gridSize>, Params::Game::gridSize>;

enum TileType {
	EMPTY,
	SNAKE,
	FRUIT
};

class Grid {
public:
	Grid();
	~Grid();

	void Render(SDL_Renderer* renderer);
	void ClearGrid();
	void SetCell(Vec2<int> gridPos, uint8_t tileType);

private:
	GridLayout gridLayout;
	float tileSize;
};