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
	auto view = registry.view<Position, Velocity, Collider>();
	for ( auto [entity, position, velocity, collider] : view.each() ) {
		collider.on_floor = false;
		collider.wall_direction = 0;

		// Move the entity
		position.value.y += velocity.value.y;

		// Check for collision
		if ( tile_overlap(position, collider) ) {
			// From above
			if ( velocity.value.y > 0 ) {
				position.value.y = floor(position.value.y / tilemap.tile_size) * tilemap.tile_size;
				collider.on_floor = true;
			}

			// From below
			else if ( velocity.value.y < 0 ) {
				position.value.y = ceil(position.value.y / tilemap.tile_size) * tilemap.tile_size;
			}

			velocity.value.y = 0.0;
		}

		// Repeat for the other axis
		position.value.x += velocity.value.x;

		if ( tile_overlap(position, collider) ) {
			// From left
			if ( velocity.value.x > 0 ) {
				position.value.x = floor(position.value.x / tilemap.tile_size) * tilemap.tile_size;
				collider.wall_direction = -1;
			}

			// From right
			else if ( velocity.value.x < 0 ) {
				position.value.x = ceil(position.value.x / tilemap.tile_size) * tilemap.tile_size;
				collider.wall_direction = +1;
			}

			velocity.value.x = 0.0;
		}

	}
}

void gravity() {
	auto view = registry.view<Velocity, const Gravity>();
	for ( auto [entity, velocity] : view.each() ) {
		velocity.value.y += G * GetFrameTime();
	}
}

void collider_overlap() {
	const float push_speed = 1.0;

	auto view = registry.view<Velocity, const Position, const Collider>();
	for ( auto entity : view )
	for ( auto other : view ) {
		if (entity == other) continue; // Skip self

		auto& position = view.get<Position>(entity);
		auto& collider = view.get<Collider>(entity);
		auto& velocity = view.get<Velocity>(entity);
		auto& other_position = view.get<Position>(other);
		auto& other_collider = view.get<Collider>(other);

		if ( !collider.enabled || !other_collider.enabled ) continue;

		// Check for collision
		bool collided = collider.get_rectangle(position.value).CheckCollision( other_collider.get_rectangle(other_position.value) );
		if ( !collided ) continue;

		// Push away if they overlap
		if (position.value.x < other_position.value.x) velocity.value.x = -push_speed;
		else velocity.value.x = push_speed;
	}
}
