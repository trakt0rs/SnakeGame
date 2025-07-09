#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "utils.h"

class TextTextureBuilder {
public:
	static TextTextureBuilder& GetInstance() {
		if (!instance) {
			instance = new TextTextureBuilder();
		}
		return *instance;
	}

	void LoadFont(const char* path, uint8_t fontSize, SDL_Renderer* renderer);
	void BuildTexture(const char* text, Vec2<float>* textureSize, SDL_Texture** texture, const SDL_Color& color);

private:
	TextTextureBuilder(const TextTextureBuilder&) = delete;
	TextTextureBuilder& operator==(const TextTextureBuilder&) = delete;

	TextTextureBuilder();
	~TextTextureBuilder();

	static TextTextureBuilder* instance;

private:
	TTF_Font* font;
	SDL_Renderer* renderer;
};