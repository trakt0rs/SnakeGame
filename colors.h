#pragma once

#include <array>
#include <SDL3/SDL_pixels.h>

namespace Colors {
	inline std::array<SDL_Color, 2> emptyTileColors = { {
		{ 18, 18, 18, 255 },
		{ 20, 20, 20, 255 }
	} };

	inline SDL_Color snake			= { 200, 200, 200, 255 };
	inline SDL_Color fruit			= { 200,   0,   0, 255 };
	inline SDL_Color panelColor		= {   0,   0,   0, 150 };
	inline SDL_Color fontColor		= { 255, 255, 255, 255 };
}