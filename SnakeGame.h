#pragma once

#include <SDL3/SDL.h>
#include <queue>

#include "Grid.h"
#include "utils.h"
#include "Panel.h"

enum class MoveDirection {
	TOP,
	RIGHT,
	DOWN,
	LEFT
};

class SnakeGame {
public:
	SnakeGame();
	~SnakeGame();

	void Run();

private:
	void Update(float deltaTime);
	void Render();

	void UpdateSnake(float deltaTime);
	bool IsGameOver(const Vec2<int>& newHeadPos);
	Vec2<int> GetFreeFruitPos();

	void ListenKeys();

	void SetStartingPositions();

	void UpdateScore();

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	bool shouldQuit = true;

private:
	Grid grid;
	std::deque<Vec2<int>> snakePositions;
	float snakeMoveTimePassed;
	Vec2<int> fruitPos;

	std::queue<MoveDirection> directionsPressed;
	MoveDirection moveDirection;

	Panel panel;

	uint32_t hiScore;

	bool gameOver;
};