#include "Grid.h"

#include "colors.h"

Grid::Grid() : gridLayout() {
	tileSize = (float)Params::Window::width / (float)Params::Game::gridSize;
}

Grid::~Grid() {
}

void Grid::Render(SDL_Renderer* renderer) {
	for (uint8_t y = 0; y < Params::Game::gridSize; ++y) {
		for (uint8_t x = 0; x < Params::Game::gridSize; ++x) {
			SDL_FRect dst = {
				x * tileSize,
				y * tileSize,
				tileSize,
				tileSize
			};

			// Set color
			SDL_Color* color;
			switch (gridLayout[y][x]) {
			case TileType::EMPTY:
				color = &Colors::emptyTileColors[(x + y + 1) % 2];
				break;
			case TileType::SNAKE:
				color = &Colors::snake;
				break;
			default:
				color = &Colors::fruit;
			}

			SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
			SDL_RenderFillRect(renderer, &dst);
		}
	}
}

void Grid::ClearGrid() {
	for (auto& row : gridLayout) {
		row.fill(0);
	}
}

void Grid::SetCell(Vec2<int> gridPos, uint8_t tileType) {
	gridLayout[gridPos.y][gridPos.x] = tileType;
}