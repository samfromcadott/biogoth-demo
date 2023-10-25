#include <iostream>
#include <cmath>
#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "util.hh"
#include "audio.hh"

// Checks for line of sight between two tile coordinates
bool line_of_sight(const TileCoord a, const TileCoord b) {
	TileCoord c = a;

	while (c.x != b.x || c.y != b.y) {
		// Move towards the destination
		c.x += sign(b.x - c.x);
		c.y += sign(b.y - c.y);

		if (c.x == b.x && c.y == b.y) return true; // Return true if the destination is reached
		if ( !tilemap.tile_in_map(c) ) return false; // Return false if it gets to the edge of the map
		if ( tilemap(c) != empty_tile ) return false; // If a tile in the way isn't empty then there is no line of sight
	}

	return false;
}

Vector2 GuardBrain::find_player() {
	vec2 closest = {-1000000, -1000000};
	const vec2 owner_position = registry.try_get<Position>(owner)->value;
	const int owner_team = registry.try_get<Character>(owner)->team;
	float closest_dist = 1000000;

	auto enemy_view = registry.view<const Character, const Position, const Collider>();
	for ( auto [enemy, character, position, collider] : enemy_view.each() ) {
		if (enemy == owner) continue; // Skip self
		if (character.team == owner_team) continue; // Skip allies

		vec2 location = position.value;
		location.y -= collider.height; // Find the enemy's head

		// If this enemy is closer than the last closest set it as closest
		if ( location.Distance(owner_position) < closest_dist ) {
			closest = location;
			closest_dist = location.Distance(owner_position);
		}
	}

	return closest;
}

void GuardBrain::think() {
	auto& weapon_set = *registry.try_get<WeaponSet>(owner);
	auto& movement = *registry.try_get<Movement>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& position = *registry.try_get<Position>(owner);
	auto& velocity = *registry.try_get<Velocity>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	// Check if the entity is in the air
	if (!collider.on_floor) {
		weapon_set[0]->end();
		return;
	}

	// Check for line of sight to the player
	Vector2 player_position = find_player();
	TileCoord player_coord = tilemap.world_to_tile(player_position);
	TileCoord entity_coord = tilemap.world_to_tile(position.value.x, position.value.y-collider.height);
	if ( !line_of_sight(entity_coord, player_coord) ) {
		movement.direction.x = 0;
		return;
	}

	// Get the distance and direction of the player
	float distance = abs( player_position.x - position.value.x );
	int direction = sign( player_position.x - position.value.x );

	// If the player is in aggro_range, set facing and velocity to move toward them
	if ( distance > aggro_range ) return;

	movement.direction.x = direction;
	facing.direction = direction;

	// Check if the entity is on a ledge
	const TileCoord next_tile = tilemap.world_to_tile( position.value.x+(direction*(collider.width+3)/2), position.value.y+1 );

	// Don't walk off a ledge if the player is above
	if ( tilemap(next_tile) == empty_tile && player_position.y < position.value.y )
		movement.direction.x = 0;

	// If the player if in attack_range and the GunAttack timer <= 0, stop moving and attack them
	if ( distance > attack_range ) return;

	// Firing gun
	movement.direction.x = 0;
	if ( abs(velocity.value.x) > 2.0 ) return; // Wait until stopped to shoot

	if (weapon_set[0]->timer > 0.0) return;

	weapon_set[0]->fire();
}
