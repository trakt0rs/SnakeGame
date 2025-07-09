#pragma once

#include <type_traits>

template<class T>
struct Vec2 {
	static_assert(std::is_same_v<T, int> || std::is_same_v<T, float>, "Vec2 can only be instantiated with int or float");

	T x = 0, y = 0;
	Vec2() = default;
	Vec2(T x, T y) : x(x), y(y) {}

	bool operator==(const Vec2<T>& other) const {
		return this->x == other.x && this->y == other.y;
	}
};