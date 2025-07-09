#include "TextTextureBuilder.h"

#include <print>

TextTextureBuilder* TextTextureBuilder::instance = nullptr;

TextTextureBuilder::TextTextureBuilder() : font(nullptr), renderer(nullptr) {}

TextTextureBuilder::~TextTextureBuilder() {
	TTF_CloseFont(font);
	TTF_Quit();
}

void TextTextureBuilder::LoadFont(const char* path, uint8_t fontSize, SDL_Renderer* renderer) {
	this->renderer = renderer;

	if (font)
		TTF_CloseFont(font);

	font = TTF_OpenFont(path, fontSize);

	if (!font) {
		std::print("Failed to open font: {}\n", SDL_GetError());
	}
}

void TextTextureBuilder::BuildTexture(const char* text, Vec2<float>* textureSize, SDL_Texture** texture, const SDL_Color& color) {
	if (texture && *texture)
		SDL_DestroyTexture(*texture);

	if (!renderer) {
		std::print("Font not initialized!\n");
		return;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, text, strlen(text), color);
	if (!surface) {
		std::print("Failed to create SDL_Surface for text: {}\n", SDL_GetError());
		return;
	}

	textureSize->x = static_cast<float>(surface->w);
	textureSize->y = static_cast<float>(surface->h);
	//std::print("Texture size {}, {}\n", surface->w, surface->h);

	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, surface);
	if (!t) {
		std::print("Failed to create Texture: {}\n", SDL_GetError());
	}

	if(texture) *texture = t;
	SDL_DestroySurface(surface);
}