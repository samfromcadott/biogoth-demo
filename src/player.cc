#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "util.hh"

using namespace raylib;

void player_move() {
	auto view = registry.view<const Player, Velocity, const Collider, Facing>();

	for ( auto [entity, player, velocity, collider, facing] : view.each() ) {
		if (!player.can_move) continue;

		int dx = 0; // Input direction

		if ( IsKeyDown(KEY_RIGHT) )  dx = +1;
		if ( IsKeyDown(KEY_LEFT) ) dx = -1;
		if ( dx != 0 ) facing.direction = dx;

		float wish_speed = player.max_speed * dx;

		float acceleration = collider.on_floor ? player.ground_acceleration : player.air_acceleration;
		float deceleration = collider.on_floor ? player.ground_deceleration : player.air_deceleration;
		float turn_speed = collider.on_floor ? player.ground_turn_speed : player.air_turn_speed;
		float speed_change;

		if ( dx != 0 ) {
			// If the sign of dx and velocity.x are inequal the player is trying to change direction
			if ( sign(dx) != sign(velocity.value.x) ) speed_change = turn_speed;
			else speed_change = acceleration; // Continuing in same direction
		} else {
			speed_change = deceleration; // Decelerate if no input
		}

		// Move velocity towards target velocity
		velocity.value.x = move_towards( velocity.value.x, wish_speed, speed_change * GetFrameTime() );

		if ( IsKeyDown(KEY_SPACE) && collider.on_floor ) velocity.value.y -= 10.0;
		else if ( IsKeyDown(KEY_SPACE) && collider.wall_direction != 0 ) {
			velocity.value.y -= 1.0;
			velocity.value.x = collider.wall_direction;
			velocity.value = velocity.value.Normalize() * 10.0;
		}
	}
}

void player_attack() {
	auto view = registry.view<const Player, MeleeAttack, const Collider, const Position, const Facing>();

	for ( auto [entity, player, attack, collider, position, facing] : view.each() ) {
		if ( !IsKeyPressed(KEY_LEFT_CONTROL) ) continue;

		raylib::Vector2 ray_start = position.value + raylib::Vector2((collider.width/2+0.001)*facing.direction, -collider.height/2);
		raylib::Vector2 ray_end = ray_start + raylib::Vector2(attack.distance) * facing.direction;
		registry.emplace<RayCast>(entity, ray_start, ray_end);

		attack.active = true;
	}
}

void player_bite() {
	auto view = registry.view<Player, BiteAttack, const Collider, const Position, const Facing, Velocity>();

	for ( auto [entity, player, bite, collider, position, facing, velocity] : view.each() ) {
		if ( IsKeyPressed(KEY_V) ) {
			raylib::Vector2 ray_start =
				position.value + raylib::Vector2((collider.width/2+0.001)*facing.direction, -collider.height/2);
			raylib::Vector2 ray_end = ray_start + raylib::Vector2(bite.distance) * facing.direction;
			registry.emplace<RayCast>(entity, ray_start, ray_end);

			bite.active = true;
			player.can_move = false;
			velocity.value.x = 0.0;
		}

		if ( IsKeyReleased(KEY_V) ) {
			registry.remove<RayCast>(entity); // Delete the ray cast
			bite.active = false;
			player.can_move = true;
		}

	}
}
