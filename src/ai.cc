#include <iostream>
#include <cmath>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "util.hh"

void enemy_think() {
	raylib::Vector2 player_position;

	// Find player position
	auto player_view = registry.view<const Player, const Position>();
	for ( auto [entity, player, position] : player_view.each() ) {
		player_position = position.value;
		break;
	}

	auto view = registry.view<const Enemy, Velocity, const Position, Facing>();
	for ( auto [entity, enemy, velocity, position, facing] : view.each() ) {
		// Get the distance and direction of the player
		float distance = position.value.Distance(player_position);
		int direction = sign( player_position.x - position.value.x );

		// If the player is in aggro_range, set facing and velocity to move toward them
		if ( distance < enemy.aggro_range ) {
			velocity.value.x = enemy.max_speed * direction;
			facing.direction = direction;
		}

		// If the player if in attack_range and the GunAttack timer <= 0, stop moving and attack them
	}
}
