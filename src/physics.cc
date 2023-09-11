#include <iostream>
#include <cmath>
#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "util.hh"

void character_movement() {
	auto view = registry.view<Character, Movement, Collider, Velocity>();
	for ( auto [entity, character, movement, collider, velocity] : view.each() ) {
		if (!movement.can_move) continue;

		float wish_speed = movement.max_speed * movement.direction.x;

		float acceleration = collider.on_floor? movement.ground_acceleration : movement.air_acceleration;
		float deceleration = collider.on_floor? movement.ground_deceleration : movement.air_deceleration;
		float turn_speed = collider.on_floor? movement.ground_turn_speed : movement.air_turn_speed;

		float speed_change;

		if ( movement.direction.x != 0 )
			speed_change = sign(movement.direction.x) != sign(velocity.value.x)? turn_speed : acceleration;
		else
			speed_change = deceleration; // Decelerate if no input

		// Move velocity towards target velocity
		velocity.value.x = move_towards( velocity.value.x, wish_speed, speed_change * GetFrameTime() );
	}
}

// Finds the direction a collision comes from
raylib::Vector2 overlap_direction(const Position& position, const Collider& collider) {
	const float e = 0.1; // Ensures next tile isn't counted
	raylib::Vector2 direction(0, 0);

	int start_x = (position.value.x - collider.width/2) / tilemap.tile_size;
	int end_x = (position.value.x - e + collider.width/2) / tilemap.tile_size;
	int start_y = (position.value.y - collider.height) / tilemap.tile_size;
	int end_y = (position.value.y - e) / tilemap.tile_size;

	for (int x=start_x; x<=end_x; x++)
	for (int y=start_y; y<=end_y; y++) {
		if ( !tilemap.tile_in_map(x, y) ) continue; // If the coordinate is out of bounds go to the next one
		if ( tilemap(x, y) == empty_tile ) continue;

		if (x == start_x) direction.x = -1;
		else if (x == end_x) direction.x = +1;

		if (y == start_y) direction.y = -1;
		else if (y == end_y) direction.y = +1;
	}

	return direction; // Return false if no solid tiles were found
}

void move_collide() {
	auto view = registry.view<Position, Velocity, Collider>();
	for ( auto [entity, position, velocity, collider] : view.each() ) {
		raylib::Vector2 direction; // Direction the collision comes from

		collider.on_floor = false;
		collider.wall_direction = 0;

		// Round corners
		const float e = 0.1;

		TileCoord left_side = tilemap.world_to_tile(
			position.value.x - collider.width/2 + e, position.value.y - collider.height - e
		);
		TileCoord right_side = tilemap.world_to_tile(
			position.value.x + collider.width/2 - e, position.value.y - collider.height - e
		);

		// Upper corner collision
		bool left_collide = tilemap(left_side) != empty_tile;
		bool right_collide = tilemap(right_side) != empty_tile;

		// Slide around corner
		const float corner_push = 3;
		if ( left_collide && !right_collide && velocity.value.y < 0 && velocity.value.x >= 0) {
			position.value.x += corner_push;
		} else if ( right_collide && !left_collide && velocity.value.y < 0 && velocity.value.x <= 0) {
			position.value.x -= corner_push;
		}

		// Move the entity
		position.value.x += velocity.value.x;

		// Check for collision
		direction = overlap_direction(position, collider);
		if ( direction.x != 0 ) {
			// From left
			if ( direction.x == +1 ) {
				position.value.x = floor(position.value.x / tilemap.tile_size) * tilemap.tile_size;
			}

			// From right
			else if ( direction.x == -1 ) {
				position.value.x = ceil(position.value.x / tilemap.tile_size) * tilemap.tile_size;
			}

			velocity.value.x = 0.0;
		}

		// Repeat for the other axis
		position.value.y += velocity.value.y;

		direction = overlap_direction(position, collider);
		if ( direction.y != 0 ) {
			// From above
			if ( direction.y == +1 ) {
				position.value.y = floor(position.value.y / tilemap.tile_size) * tilemap.tile_size;
				collider.on_floor = true;
				velocity.value.y = 0.0;
			}

			// From below
			if ( direction.y == -1 ) {
				position.value.y = ceil(position.value.y / tilemap.tile_size) * tilemap.tile_size;

				// Only stop when not at corner
				if (left_collide && right_collide) velocity.value.y = 0.0;
			}
		}

	}
}

void gravity() {
	auto view = registry.view<Velocity, const Gravity>();
	for ( auto [entity, velocity, gravity] : view.each() ) {
		velocity.value.y += G * gravity.scale * GetFrameTime();
	}
}

void collider_overlap() {
	const float push_speed = 8.0;

	auto view = registry.view<Velocity, Position, const Collider>();
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
		if (position.value.x < other_position.value.x) position.value.x += -push_speed;
		else position.value.x += push_speed;
	}
}
