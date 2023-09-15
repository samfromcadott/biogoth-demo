#include <iostream>
#include <cmath>
#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "camera.hh"
#include "util.hh"

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

raylib::Vector2 CameraSystem::find_player() {
	return registry.get<Position>(player).value;
}

raylib::Vector2 CameraSystem::track_player() {
	return find_player() - base;
}

raylib::Vector2 CameraSystem::look_ahead() {
	raylib::Vector2 scale(96.0, 32.0);
	return registry.get<Velocity>(player).value * scale;
}

std::vector< raylib::Vector2 > CameraSystem::find_close_characters() {
	std::vector< raylib::Vector2 > character_list;

	auto view = registry.view<const Character, const Position>();
	for ( auto [entity, character, position] : view.each() ) {
		if ( position.value.Distance( find_player() ) < close_distance )
			character_list.push_back( position.value );
	}

	return character_list;
}

/// Find the average of all characters near the player
raylib::Vector2 CameraSystem::center_close_characters(const std::vector< raylib::Vector2 >& characters) {
	raylib::Vector2 sum;

	for ( const auto& v : characters ) sum += v;

	return ( sum / characters.size() ) - base;
}

/// Zooms to show all nearby characters
float CameraSystem::zoom_to_characters(const std::vector< raylib::Vector2 >& characters) {
	std::vector<float> values_x, values_y;

	for (auto p : characters) {
		values_x.push_back(p.x);
		values_y.push_back(p.y);
	}

	// Get the maximum distance of the characters
	float min_x = *std::min_element( values_x.begin(), values_x.end() );
	float min_y = *std::min_element( values_y.begin(), values_y.end() );

	float max_x = *std::max_element( values_x.begin(), values_x.end() );
	float max_y = *std::max_element( values_y.begin(), values_y.end() );

	float distance = Vector2Distance( {min_x, min_y}, {max_x, max_y} );

	float new_zoom = ease( distance / (close_distance * 2), max_zoom, min_zoom );
	return new_zoom - zoom;
}

void CameraSystem::clamp_camera() {
	const float map_width = tilemap.width * tilemap.tile_size;
	const float map_height = tilemap.height * tilemap.tile_size;

	float z = 1.0 / zoom;

	if (camera.target.x < camera.offset.x * z)
		camera.target.x = camera.offset.x * z; // Left edge
	else if ( camera.target.x > map_width - camera.offset.x * z )
		camera.target.x = map_width - camera.offset.x * z; // Right edge
	if (camera.target.y < camera.offset.y * z)
		camera.target.y = camera.offset.y * z; // Top edge
	else if ( camera.target.y > map_height - camera.offset.y * z )
		camera.target.y = map_height - camera.offset.y * z; // Bottom edge
}

void CameraSystem::init() {
	zoom = 1.0;
	max_zoom = 1.0;
	min_zoom = 0.8;
	close_distance = 800.0;

	camera = raylib::Camera2D( raylib::Vector2(screen_width/2, screen_height/2), {0.0, 0.0} );
	base = find_player();
	offset = raylib::Vector2(0, 0);
}

void CameraSystem::update() {
	const auto characters = find_close_characters();

	raylib::Vector2 delta = track_player() + look_ahead() + center_close_characters(characters);
	float delta_zoom = zoom_to_characters(characters);

	base += delta * GetFrameTime();
	zoom += delta_zoom * GetFrameTime();

	camera.target = base + offset;
	camera.zoom = zoom;
	clamp_camera();
}

raylib::Camera2D& CameraSystem::get_camera() {
	return camera;
}
