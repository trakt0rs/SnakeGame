#pragma once

#include <array>
#include <SDL3/SDL_events.h>

class EventManager {
public:
	static EventManager& GetInstance() {
		if (instance == nullptr) {
			instance = new EventManager();
		}
		return *instance;
	}

	void PollEvents();

	inline bool IsKeyPressed(SDL_Scancode key) { return keysPressed[key] && !lastKeysPressed[key]; }
	inline const bool ShouldQuit() { return shouldQuit; }

private:
	EventManager();
	~EventManager() = default;

	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	static EventManager* instance;

private:
	std::array<bool, SDL_SCANCODE_COUNT> keysPressed;
	std::array<bool, SDL_SCANCODE_COUNT> lastKeysPressed;

	bool shouldQuit;
};