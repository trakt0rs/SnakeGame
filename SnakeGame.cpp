#include <print>
#include <cstdlib>

#include "SnakeGame.h"
#include "params.h"
#include "EventManager.h"
#include "TextTextureBuilder.h"
#include "scoreRW.h"

SnakeGame::SnakeGame() : grid(), snakeMoveTimePassed(0), moveDirection(MoveDirection::DOWN), gameOver(true), hiScore(0) {
	window = SDL_CreateWindow(Params::Window::title, Params::Window::width, Params::Window::height, SDL_WINDOW_HIDDEN);
	if (!window) {
		std::print("Failed to create SDL_Window: {}\n", SDL_GetError());
		return;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::print("Failed to create SDL_Rendere: {}", SDL_GetError());
		return;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	hiScore = scoreRW::getHighScoreFromFile();

	TextTextureBuilder::GetInstance().LoadFont("font.ttf", Params::PanelUI::fontSize, renderer);
	panel.UpdateScore(0, hiScore);

	SetStartingPositions();

	// Init was successful
	shouldQuit = false;
	SDL_ShowWindow(window);
}

SnakeGame::~SnakeGame() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void SnakeGame::Run() {
	uint64_t lastCounter = SDL_GetPerformanceCounter();

	const float targetFrameTime = 1.0f / Params::Window::targetFPS;

	while (!shouldQuit) {
		uint64_t nowCounter = SDL_GetPerformanceCounter();
		float deltaTime = static_cast<float>(nowCounter - lastCounter) / SDL_GetPerformanceFrequency();
		lastCounter = nowCounter;

		EventManager::GetInstance().PollEvents();

		Update(deltaTime);
		Render();

		// Delay to maintain target FPS
		float frameTime = static_cast<float>(SDL_GetPerformanceCounter() - nowCounter) / SDL_GetPerformanceFrequency();
		if (frameTime < targetFrameTime) {
			float delayTime = (targetFrameTime - frameTime) * 1000.0f;
			SDL_Delay(static_cast<uint32_t>(delayTime));
		}

		shouldQuit = EventManager::GetInstance().ShouldQuit();
	}
}

void SnakeGame::ListenKeys() {

	if (directionsPressed.size() > 2) return;

	EventManager& eventManager = EventManager::GetInstance();

	if (eventManager.IsKeyPressed(SDL_SCANCODE_W) && moveDirection != MoveDirection::DOWN) {
		if((!directionsPressed.empty() && directionsPressed.back() != MoveDirection::TOP) || directionsPressed.empty())
			directionsPressed.push(MoveDirection::TOP);
	}
	if (eventManager.IsKeyPressed(SDL_SCANCODE_S) && moveDirection != MoveDirection::TOP) {
		if ((!directionsPressed.empty() && directionsPressed.back() != MoveDirection::DOWN) || directionsPressed.empty())
			directionsPressed.push(MoveDirection::DOWN);
	}
	if (eventManager.IsKeyPressed(SDL_SCANCODE_D) && moveDirection != MoveDirection::LEFT) {
		if ((!directionsPressed.empty() && directionsPressed.back() != MoveDirection::RIGHT) || directionsPressed.empty())
			directionsPressed.push(MoveDirection::RIGHT);
	}
	if (eventManager.IsKeyPressed(SDL_SCANCODE_A) && moveDirection != MoveDirection::RIGHT) {
		if ((!directionsPressed.empty() && directionsPressed.back() != MoveDirection::LEFT) || directionsPressed.empty())
			directionsPressed.push(MoveDirection::LEFT);
	}
}

void SnakeGame::Update(float deltaTime) {
	if (!gameOver) {
		UpdateSnake(deltaTime);
		ListenKeys();
	}
	else {
		EventManager& event = EventManager::GetInstance();
		if (event.IsKeyPressed(SDL_SCANCODE_W) || event.IsKeyPressed(SDL_SCANCODE_A) || event.IsKeyPressed(SDL_SCANCODE_S) || event.IsKeyPressed(SDL_SCANCODE_D)) {
			gameOver = false;
			SetStartingPositions();
		}
	}
}

void SnakeGame::SetStartingPositions() {
	snakePositions.clear();
	grid.ClearGrid();

	fruitPos = GetFreeFruitPos();
	grid.SetCell(fruitPos, TileType::FRUIT);

	snakePositions.push_front({ Params::Game::gridSize / 2, Params::Game::gridSize / 2 });
	grid.SetCell(snakePositions.front(), TileType::SNAKE);

	while (!directionsPressed.empty()) directionsPressed.pop();
	directionsPressed.push(MoveDirection::DOWN);
}

void SnakeGame::UpdateSnake(float deltaTime) {
	snakeMoveTimePassed += deltaTime;

	if (snakeMoveTimePassed >= Params::Game::snakeSpeed) {
		snakeMoveTimePassed = 0.0f;

		// Get new direction
		if (!directionsPressed.empty()) {
			moveDirection = directionsPressed.front();
			directionsPressed.pop();
		}

		// Get new head position
		Vec2<int> snakePos = snakePositions.front();
		switch (moveDirection) {
		case MoveDirection::TOP:
			snakePos.y--;
			break;
		case MoveDirection::DOWN:
			snakePos.y++;
			break;
		case MoveDirection::RIGHT:
			snakePos.x++;
			break;
		case MoveDirection::LEFT:
			snakePos.x--;
		}

		gameOver = IsGameOver(snakePos);
		if (gameOver) return;

		snakePositions.emplace_front(snakePos);
		grid.SetCell(snakePos, TileType::SNAKE);

		grid.SetCell(*(snakePositions.end() - 1), TileType::EMPTY);
		Vec2<int> lastTilePos = *(snakePositions.end() - 1);
		snakePositions.pop_back();

		if (snakePos == fruitPos) {
			snakePositions.emplace_back(lastTilePos);
			fruitPos = GetFreeFruitPos();
			grid.SetCell(fruitPos, TileType::FRUIT);
		}
	}
}

void SnakeGame::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	grid.Render(renderer);
	if(gameOver)
		panel.Render(renderer);

	SDL_RenderPresent(renderer);
}

void SnakeGame::UpdateScore() {
	uint32_t score = static_cast<uint32_t>(snakePositions.size() - 1) * 10;
	if (score > hiScore) {
		hiScore = score;
		scoreRW::writeNewHighScore(score);
	}

	panel.UpdateScore(score, hiScore);
}

bool SnakeGame::IsGameOver(const Vec2<int>& newHeadPos) {
	bool over = false;
	// Check grid boundaries
	if (newHeadPos.x < 0 || newHeadPos.x > Params::Game::gridSize - 1 || newHeadPos.y < 0 || newHeadPos.y > Params::Game::gridSize - 1) {
		over = true;
		goto skip;
	}

	// Check other snake parts
	for (const Vec2<int> pos : snakePositions) {
		if (pos == newHeadPos) {
			over = true;
			break;
		}
	}

	skip: if (over) {
		UpdateScore();
	}
	return over;
}

Vec2<int> SnakeGame::GetFreeFruitPos() {
	std::vector<Vec2<int>> freePositions;

	for (uint8_t y = 0; y < Params::Game::gridSize; ++y) {
		for (uint8_t x = 0; x < Params::Game::gridSize; ++x) {
			Vec2<int> pos = { x, y };

			bool occupied = false;
			for (const auto& part : snakePositions) {
				if (part == pos) {
					occupied = true;
					break;
				}
			}

			if (!occupied) {
				freePositions.push_back(pos);
			}
		}
	}

	if(!freePositions.empty())
		return freePositions[rand() % freePositions.size()];

	return { -1, -1 };
}