#pragma once

#include <raylib/raylib-cpp.hpp>

struct Player {}; // Tags an object as player
struct Gravity {}; // Makes an object affected by gravity

struct Position {
	raylib::Vector2 value;
};

struct Velocity {
	raylib::Vector2 value;
};

struct Collider {
	float width, height;

	raylib::Rectangle get_rectangle(raylib::Vector2 position) const {
		return raylib::Rectangle(
			position.x - width/2,
			position.y - height,
			width,
			height
		);
	}
};
