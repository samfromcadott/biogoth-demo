#include <iostream>
#include <cmath>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"

// Checks if a collider is overlapping a non-empty tile
bool tile_overlap(const Position& position, const Collider& collider) {
	const float e = 0.0001; // Ensures next tile isn't counted

	int start_x = (position.value.x - collider.width/2) / tilemap.tile_size;
	int end_x = (position.value.x - e + collider.width/2) / tilemap.tile_size;
	int start_y = (position.value.y - collider.height) / tilemap.tile_size;
	int end_y = (position.value.y - e) / tilemap.tile_size;

	for (int x=start_x; x<=end_x; x++)
	for (int y=start_y; y<=end_y; y++) {
		if ( !tilemap.tile_in_map(x, y) ) continue; // If the coordinate is out of bounds go to the next one
		if ( tilemap(x, y) == empty_tile ) continue;
		if ( tilemap(x, y) != empty_tile ) return true;
	}

	return false; // Return false if no solid tiles were found
}

void move_collide() {
	auto view = registry.view<Position, const Velocity, const Collider>();
	for ( auto [entity, position, velocity, collider] : view.each() ) {
		// Move the entity
		position.value.y += velocity.value.y;

		// Check for collision
		if ( tile_overlap(position, collider) ) {
			// From above
			if ( velocity.value.y > 0 ) {
				position.value.y = floor(position.value.y / tilemap.tile_size) * tilemap.tile_size;
			}

			// From below
			else if ( velocity.value.y < 0 ) {
				position.value.y = ceil(position.value.y / tilemap.tile_size) * tilemap.tile_size;
			}
		}

		// Repeat for the other axis
		position.value.x += velocity.value.x;

		if ( tile_overlap(position, collider) ) {
			// From left
			if ( velocity.value.x > 0 ) {
				position.value.x = floor(position.value.x / tilemap.tile_size) * tilemap.tile_size;
			}

			// From right
			else if ( velocity.value.x < 0 ) {
				position.value.x = ceil(position.value.x / tilemap.tile_size) * tilemap.tile_size;
			}
		}

	}
}
