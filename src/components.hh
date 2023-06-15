#pragma once

#include <raylib/raylib-cpp.hpp>

#include "sprite.hh"
#include "weapon.hh"

struct Player { // Tags an object as player
	float max_speed = 10.0;
	float ground_acceleration = 9.0;
	float air_acceleration = 3.0;
	float ground_deceleration = 20.0;
	float air_deceleration = 0.5;
	float ground_turn_speed = 20.0;
	float air_turn_speed = 15.0;
	bool can_move = true;
};

struct Enemy {
	float aggro_range; // Distance they will charge player
	float attack_range; // Distance they will attack player
	float max_speed;
	bool active;
};

struct Gravity { // Makes an object affected by gravity
	float scale = 1.0;
};

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
	float rate; // Time between attacks
	float timer; // Time remaining on next attack
	bool active;
};

struct BiteAttack {
	float distance;
	float timer; // Time remianing to next drain
	int damage; // Health loss per second
	bool active; // Entity is currently biting the target
};

struct GunAttack {
	Gun gun;
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


struct Jump {
	float speed;
	float gravity_scale;
	float coyote_length; // Max time player can jump after leaving floor
	float buffer_length; // Max time for jump buffer
	float coyote_timer = 0; // Current coyote time
	float buffer_timer = 0; // Current buffer time
	bool can_jump = false;
	bool wish_jump = false;
};

struct AnimationState {
	Action state;
	Sprite* sprite;
	float timer;

	void set_state(Action new_state) {
		if (state == new_state) return;

		state = new_state;
		timer = 0;
	}
};
