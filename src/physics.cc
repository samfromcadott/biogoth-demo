#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"

void move_collide() {
	raylib::Rectangle floor_rect(200, 200, 128, 64);

	auto view = registry.view<Position, const Velocity, const Collider>();
	for ( auto [entity, position, velocity, collider] : view.each() ) {
		// Move the entity
		position.value.y += velocity.value.y;

		// Check for collision
		if ( collider.get_rectangle(position.value).CheckCollision(floor_rect) ) {
			// From above
			if ( velocity.value.y > 0 ) {
				position.value.y = floor_rect.GetY();
			}

			// From below
			else if ( velocity.value.y < 0 ) {
				position.value.y = floor_rect.GetY() + floor_rect.GetHeight() + collider.height;
			}
		}

		// Repeat for the other axis
		position.value.x += velocity.value.x;

		if ( collider.get_rectangle(position.value).CheckCollision(floor_rect) ) {
			// From left
			if ( velocity.value.x > 0 ) {
				position.value.x = floor_rect.GetX() - collider.width/2;
			}

			// From right
			else if ( velocity.value.x < 0 ) {
				position.value.x = floor_rect.GetX() + floor_rect.GetWidth() + collider.width/2;
			}
		}

	}
}
