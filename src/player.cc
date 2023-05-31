#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "util.hh"
#include "audio.hh"

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
	}
}

void player_jump() {
	auto view = registry.view<const Player, Position, Velocity, Collider, Gravity, Jump, Facing>();
	for ( auto [entity, player, position, velocity, collider, gravity, jump, facing] : view.each() ) {
		if( IsKeyPressed(KEY_SPACE) ) jump.wish_jump = true;

		// Floor jump
		if ( jump.wish_jump && collider.on_floor ) {
			velocity.value.y -= jump.speed;
			gravity.scale = jump.gravity_scale;
		}

		// Reset gravity scale when going down or on jump release
		if ( IsKeyReleased(KEY_SPACE) || velocity.value.y > 0.0 ) {
			gravity.scale = 1.0;
		}

		// Find if the player is touching a wall
		collider.wall_direction = 0;

		TileCoord left_side = tilemap.world_to_tile( {position.value.x - collider.width/2 - 1, position.value.y - 1} );
		TileCoord right_side = tilemap.world_to_tile( {position.value.x + collider.width/2 + 1, position.value.y - 1} );

		if ( tilemap(left_side) != 0 ) collider.wall_direction = -1;
		if ( tilemap(right_side) != 0 ) collider.wall_direction = +1;

		if ( collider.wall_direction != 0 && !collider.on_floor ) facing.direction = -collider.wall_direction;

		// Wall jump
		if ( jump.wish_jump && collider.wall_direction != 0 && !collider.on_floor ) {
			velocity.value.y = -1.0;
			velocity.value.x = -collider.wall_direction * 2.0;
			velocity.value = velocity.value.Normalize() * jump.speed;
			gravity.scale = jump.gravity_scale;
		}
	}
}

void jump_buffer() {
	auto view = registry.view<const Player, Position, Velocity, Collider, Gravity, Jump>();
	for ( auto [entity, player, position, velocity, collider, gravity, jump] : view.each() ) {
		jump.buffer_timer -= GetFrameTime();
		if (jump.buffer_timer <= 0) jump.wish_jump = false;

		// Check for key press
		if ( !IsKeyPressed(KEY_SPACE) ) continue;
		if ( collider.wall_direction != 0 || collider.on_floor ) continue; // Don't buffer when the player can jump
		jump.buffer_timer = jump.buffer_length; // Set the timer
		jump.wish_jump = true;
	}
}

void player_attack() {
	auto view = registry.view<const Player, MeleeAttack, const Collider, const Position, const Facing>();

	for ( auto [entity, player, attack, collider, position, facing] : view.each() ) {
		if ( !IsKeyPressed(KEY_LEFT_CONTROL) ) continue;
		if (attack.active) continue; // Don't attack if the player is already attacking

		raylib::Vector2 ray_start = position.value + raylib::Vector2((collider.width/2+0.001)*facing.direction, -collider.height/2);
		raylib::Vector2 ray_end = ray_start + raylib::Vector2(attack.distance) * facing.direction;
		registry.emplace_or_replace<RayCast>(entity, ray_start, ray_end);

		attack.active = true;
		attack.timer = attack.rate;

		play_sound("sword_swing", 0.6 + random_spread() * 0.1, 1.0 + random_spread() * 0.2);
	}
}

void player_bite() {
	auto view = registry.view<Player, BiteAttack, const Collider, const Position, const Facing, Velocity>();

	for ( auto [entity, player, bite, collider, position, facing, velocity] : view.each() ) {
		if ( IsKeyPressed(KEY_V) ) {
			raylib::Vector2 ray_start =
				position.value + raylib::Vector2((collider.width/2+0.001)*facing.direction, -collider.height/2);
			raylib::Vector2 ray_end = ray_start + raylib::Vector2(bite.distance) * facing.direction;
			registry.emplace_or_replace<RayCast>(entity, ray_start, ray_end);

			bite.active = true;
			player.can_move = false;
			velocity.value.x = 0.0;
		}

		if ( IsKeyReleased(KEY_V) ) {
			bite.active = false;
			player.can_move = true;
		}
	}
}

void player_animate() {
	auto view = registry.view<const Player, AnimationState, const Velocity, const Collider, const MeleeAttack, const BiteAttack>();

	for ( auto [entity, player, animation, velocity, collider, attack, bite] : view.each() ) {
		if (player_died) animation.set_state(DIE);
		else if ( attack.timer > 0.0 && !collider.on_floor ) animation.set_state(AIR_ATTACK);
		else if ( attack.timer > 0.0 ) animation.set_state(ATTACK);
		else if ( bite.active ) animation.set_state(BITE);
		else if ( collider.on_floor && velocity.value.x != 0 ) animation.set_state(WALK);
		else if ( collider.wall_direction != 0 && !collider.on_floor ) animation.set_state(WALL_SLIDE);
		else if ( !collider.on_floor ) animation.set_state(FALL);
		else animation.set_state(IDLE);
	}
}
