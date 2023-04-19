#pragma once

#include <raylib/raylib-cpp.hpp>

struct Player { // Tags an object as player
	float max_speed = 5.0;
	float ground_acceleration = 10.0;
	float air_acceleration = 5.0;
	float ground_deceleration = 15.0;
	float air_deceleration = 3.0;
	float ground_turn_speed = 13.0;
	float air_turn_speed = 5.0;
};
struct Gravity {}; // Makes an object affected by gravity

struct Position {
	raylib::Vector2 value;
};

struct Velocity {
	raylib::Vector2 value;
};

struct Collider {
	float width, height;
	bool on_floor;
	int wall_direction;

	raylib::Rectangle get_rectangle(raylib::Vector2 position) const {
		return raylib::Rectangle(
			position.x - width/2,
			position.y - height,
			width,
			height
		);
	}
};

struct Health {
	int now, max;
};

struct DebugColor {
	raylib::Color color;
};
