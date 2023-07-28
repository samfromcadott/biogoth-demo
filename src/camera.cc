#include <iostream>
#include <cmath>
#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"

void camera_update() {
	float map_width = tilemap.width * tilemap.tile_size;
	float map_height = tilemap.height * tilemap.tile_size;

	auto view = registry.view<const Player, const Position, const Velocity>();
	for ( auto [entity, player, position, velocity] : view.each() ) {
		// Scale of camera movement
		float sx = 0.8;
		float sy = 0.3;

		// Track the player faster when near top or bottom
		if (
			position.value.y < camera.target.y - screen_height/4 ||
			position.value.y > camera.target.y + screen_height/4
		) sy = 1.0;

		raylib::Vector2 look_ahead(96.0, 32.0);
		raylib::Vector2 target = position.value + (velocity.value * look_ahead); // Look ahead

		camera.target.x += (target.x - camera.target.x) * sx * GetFrameTime();
		camera.target.y += (target.y - camera.target.y) * sy * GetFrameTime();

		// Restrict camera to map
		if (camera.target.x < camera.offset.x) camera.target.x = camera.offset.x; // Left edge
		if (camera.target.x > map_width - camera.offset.x) camera.target.x = map_width - camera.offset.x; // Right edge
		if (camera.target.y < camera.offset.y) camera.target.y = camera.offset.y; // Top edge
		if (camera.target.y > map_height - camera.offset.y) camera.target.y = map_height - camera.offset.y; // Bottom edge
	}
}
