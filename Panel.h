#pragma once

#include <SDL3/SDL.h>
#include "utils.h"

class Panel {
public:
	Panel();
	~Panel();

	void Render(SDL_Renderer* renderer);
	void UpdateScore(uint32_t score, uint32_t hiscore);

private:
	SDL_Texture* hiScoreTexture;
	Vec2<float> hiScoreSize;
	
	SDL_Texture* lastScoreTexture;
	Vec2<float> lastScoreSize;

	SDL_Texture* hintTexture;
	Vec2<float> hintSize;
};