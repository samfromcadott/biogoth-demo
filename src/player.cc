#include <iostream>
#include <raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "util.hh"
#include "audio.hh"
#include "controls.hh"

using namespace raylib;

void player_move() {
	auto view = registry.view<const Player, Movement, const Collider, Facing>();

	for ( auto [entity, player, movement, collider, facing] : view.each() ) {
		if (!player.can_move) continue;

		int dx = 0; // Input direction
		movement.direction.x = 0;

		if ( command_down(COMMAND_RIGHT) )  dx = +1;
		if ( command_down(COMMAND_LEFT) ) dx = -1;
		if ( dx != 0 ) facing.direction = dx;

		movement.direction.x = dx;
	}
}

void player_jump() {
	auto view = registry.view<const Player, Position, Velocity, Collider, Gravity, Jump, Facing>();
	for ( auto [entity, player, position, velocity, collider, gravity, jump, facing] : view.each() ) {
		if( command_pressed(COMMAND_JUMP) && player.can_move ) jump.wish_jump = true;

		// Floor jump
		if ( jump.wish_jump && collider.on_floor ) {
			velocity.value.y -= jump.speed;
			gravity.scale = jump.gravity_scale;
		}

		// Reset gravity scale when going down or on jump release
		if ( command_released(COMMAND_JUMP) || velocity.value.y > 0.0 ) {
			gravity.scale = 1.0;
		}

		// Find if the player is touching a wall
		collider.wall_direction = 0;

		TileCoord left_side = tilemap.world_to_tile( position.value.x - collider.width/2 - 4, position.value.y - 1 );
		TileCoord right_side = tilemap.world_to_tile( position.value.x + collider.width/2 + 4, position.value.y - 1 );

		if ( tilemap(left_side) != empty_tile ) collider.wall_direction = -1;
		if ( tilemap(right_side) != empty_tile ) collider.wall_direction = +1;

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
		if ( !command_pressed(COMMAND_JUMP) ) continue;
		if ( collider.wall_direction != 0 || collider.on_floor ) continue; // Don't buffer when the player can jump
		jump.buffer_timer = jump.buffer_length; // Set the timer
		jump.wish_jump = true;
	}
}

void player_attack() {
	auto view = registry.view<const Player, WeaponSet, const Collider, const Position, const Facing>();

	for ( auto [entity, player, weapon_set, collider, position, facing] : view.each() ) {
		if ( !command_down(COMMAND_ATTACK) ) continue;
		if (weapon_set[0]->active) continue; // Don't attack if the player is already attacking
		if (!player.can_move) continue;

		weapon_set[0]->fire();
	}
}

void player_bite() {
	auto view = registry.view<Player, WeaponSet, const Collider, const Position, const Facing, Velocity>();

	for ( auto [entity, player, weapon_set, collider, position, facing, velocity] : view.each() ) {
		if ( command_pressed(COMMAND_BITE) )
			weapon_set[1]->fire();

		if ( command_released(COMMAND_BITE) )
			weapon_set[1]->end();
	}
}
