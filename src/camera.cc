#include <iostream>
#include <cmath>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"

void camera_update() {
	float map_width = tilemap.width * tilemap.tile_size;
	float map_height = tilemap.height * tilemap.tile_size;

	auto view = registry.view<const Player, const Position>();
	for ( auto [entity, player, position] : view.each() ) {
		camera.target.x += (position.value.x - camera.target.x) * 0.98 * GetFrameTime();
		camera.target.y += (position.value.y - camera.target.y) * 0.6 * GetFrameTime();

		// Restrict camera to map
		if (camera.target.x < camera.offset.x) camera.target.x = camera.offset.x; // Left edge
		if (camera.target.x > map_width - camera.offset.x) camera.target.x = map_width - camera.offset.x; // Right edge
		if (camera.target.y < camera.offset.y) camera.target.y = camera.offset.y; // Top edge
		if (camera.target.y > map_height - camera.offset.y) camera.target.y = map_width - camera.offset.y; // Bottom edge
	}
}
