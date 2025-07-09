#pragma once

#include <inttypes.h>
#include "utils.h"

namespace Params {
	namespace Window {
		inline const uint32_t width = 600;
		inline const uint32_t height = 600;
		inline const char* title = "Snake Game";
		inline const uint16_t targetFPS = 60;
	}

	namespace Game {
		inline const uint8_t gridSize = 14;
		inline const float snakeSpeed = 0.12f; // in seconds;
	}

	namespace PanelUI {
		inline Vec2<float> padding = { 40.0f, 40.0f };
		inline float spacing = 50.0f;
		inline uint8_t fontSize = 20;
	}
}