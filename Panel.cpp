#include "Panel.h"

#include <string>
#include <print>

#include "TextTextureBuilder.h"
#include "params.h"
#include "colors.h"

Panel::Panel() : hiScoreTexture(nullptr), lastScoreTexture(nullptr), hintTexture(nullptr) {

}

Panel::~Panel() {
	SDL_DestroyTexture(hiScoreTexture);
	SDL_DestroyTexture(lastScoreTexture);
}

void Panel::UpdateScore(uint32_t score, uint32_t hiscore) {
	std::string hiStr = "HI: " + std::to_string(hiscore);
	TextTextureBuilder::GetInstance().BuildTexture(hiStr.c_str(), &hiScoreSize, &hiScoreTexture, Colors::fontColor);

	std::string lastStr = "LAST: " + std::to_string(score);
	TextTextureBuilder::GetInstance().BuildTexture(lastStr.c_str(), &lastScoreSize, &lastScoreTexture, Colors::fontColor);

	TextTextureBuilder::GetInstance().BuildTexture("Press WASD to start", &hintSize, &hintTexture, Colors::fontColor);
}

void Panel::Render(SDL_Renderer* renderer) {

	// Draw panel
	const float maxWidth = std::max(hiScoreSize.x, std::max(lastScoreSize.x, hintSize.x)) + Params::PanelUI::padding.x * 2;
	const float totalHeight = hiScoreSize.y + lastScoreSize.y + hintSize.y + Params::PanelUI::padding.y * 2 + Params::PanelUI::spacing * 2;
	SDL_FRect panelDst = {
		(Params::Window::width - maxWidth) / 2.0f,
		(Params::Window::height - totalHeight) / 2.0f,
		maxWidth,
		totalHeight
	};

	SDL_SetRenderDrawColor(renderer, Colors::panelColor.r, Colors::panelColor.g, Colors::panelColor.b, Colors::panelColor.a);
	SDL_RenderFillRect(renderer, &panelDst);

	// Draw Highscore text
	SDL_FRect hiDst = {
		panelDst.x + Params::PanelUI::padding.x,
		panelDst.y + Params::PanelUI::padding.y,
		hiScoreSize.x,
		hiScoreSize.y
	};
	
	SDL_RenderTexture(renderer, hiScoreTexture, NULL, &hiDst);

	// Draw last score text
	SDL_FRect lastDst = {
		hiDst.x,
		hiDst.y + Params::PanelUI::spacing + hiDst.h,
		lastScoreSize.x,
		lastScoreSize.y
	};

	SDL_RenderTexture(renderer, lastScoreTexture, NULL, &lastDst);

	// Draw hint text
	SDL_FRect hintDst = {
		hiDst.x,
		lastDst.y + Params::PanelUI::spacing + lastDst.h,
		hintSize.x,
		hintSize.y
	};

	SDL_RenderTexture(renderer, hintTexture, NULL, &hintDst);
}