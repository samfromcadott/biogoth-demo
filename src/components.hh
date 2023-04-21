#pragma once

#include <raylib/raylib-cpp.hpp>

struct Player { // Tags an object as player
	float max_speed = 8.0;
	float ground_acceleration = 10.0;
	float air_acceleration = 5.0;
	float ground_deceleration = 15.0;
	float air_deceleration = 3.0;
	float ground_turn_speed = 13.0;
	float air_turn_speed = 5.0;
	bool can_move = true;
};

struct Enemy {
	float aggro_range; // Distance they will charge player
	float attack_range; // Distance they will attack player
	float max_speed;
};

struct Gravity {}; // Makes an object affected by gravity

struct Position {
	raylib::Vector2 value;
};

struct Velocity {
	raylib::Vector2 value;
};

struct Facing {
	int direction;
};

struct Collider {
	float width, height;
	bool on_floor;
	int wall_direction;
	bool enabled; // Disabled collision with other entities

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

struct MeleeAttack {
	float distance;
	int damage;
	bool active;
};

struct BiteAttack {
	float distance;
	float timer; // Time remianing to next drain
	int damage; // Health loss per second
	bool active; // Entity is currently biting the target
};

struct GunAttack {
	int damage; // Damage of each bullet
	int count; // Number of bullets fired at once
	float spread; // Spread when bullers are fired
	float speed; // Speed of each bullet
	float rate; // Time between shots
	float timer; // Time until another shot is possible
};

struct RayCast {
	raylib::Vector2 start, end;

	bool intersect(raylib::Rectangle collider) const {
		bool left_intersect = CheckCollisionLines(
			start, end,
			raylib::Vector2( collider.GetX(),  collider.GetY() ),
			raylib::Vector2( collider.GetX(),  collider.GetY()+collider.GetHeight() ),
			NULL
		);
		bool right_intersect = CheckCollisionLines(
			start, end,
			raylib::Vector2( collider.GetX()+collider.GetWidth(),  collider.GetY() ),
			raylib::Vector2( collider.GetX()+collider.GetWidth(),  collider.GetY()+collider.GetHeight() ),
			NULL
		);

		return left_intersect || right_intersect;
	}
};

struct Bullet {
	int damage;
};
