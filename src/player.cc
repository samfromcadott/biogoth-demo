#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "util.hh"

using namespace raylib;

void player_move() {
	float ground_acceleration = 10.0;
	float air_acceleration = 5.0;
	float ground_deceleration = 15.0;
	float air_deceleration = 3.0;
	float ground_turn_speed = 13.0;
	float air_turn_speed = 5.0;

	auto view = registry.view<const Player, Velocity, const Collider>();

	for ( auto [player, velocity, collider] : view.each() ) {
		int dx = 0; // Input direction
		const float max_speed = 5.0;

		if ( IsKeyDown(KEY_RIGHT) )  dx = +1;
		if ( IsKeyDown(KEY_LEFT) ) dx = -1;

		float wish_speed = max_speed * dx;

		float acceleration = collider.on_floor ? ground_acceleration : air_acceleration;
		float deceleration = collider.on_floor ? ground_deceleration : air_deceleration;
		float turn_speed = collider.on_floor ? ground_turn_speed : air_turn_speed;
		float speed_change;

		if ( dx != 0 ) {
			// If the sign of dx and velocity.x are inequal the player is trying to change direction
			if ( sign(dx) != sign(velocity.value.x) ) {
				speed_change = turn_speed;
			} else {
				speed_change = acceleration; // Continuing in same direction
			}
		} else {
			speed_change = deceleration; // Decelerate if no input
		}

		// Move velocity towards target velocity
		velocity.value.x = move_towards( velocity.value.x, wish_speed, speed_change * GetFrameTime() );

		if ( IsKeyDown(KEY_SPACE) && collider.on_floor ) velocity.value.y -= 10.0;
	}
}
